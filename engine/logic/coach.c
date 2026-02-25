#include "coach.h"
#include "core/constants.h"
#include "entities/ball.h"
#include "entities/team.h"
#include "game/scene.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// Set to false to let the other team use their own logic (if you implement it)
// Set to true to test your logic on both teams
bool coach_both_teams = true;


// Used for handling the "freeze" problem players having for tackle
// if the player's tackle counter reaches over a certain number,
// his state will become IDLE
int tacke_counters[2][PLAYER_COUNT] = {0};


// 1 if player just shot.
int player_shot[2][PLAYER_COUNT] = {0};


#define FORWARD_SHOOTING_RANGE 80.0f
#define FORWARD_INTERCEPTING_RANGE 200.0f


/* -------------------------------------------------------------------------
 * Logic Functions
 *  TODO 1: You must implement the following functions in Phase 2.
 *        Each player in each team has its own functions.
 *        You can add new functions, but are NOT ALLOWED to remove
 *        the existing functions or change their structure.
 * ------------------------------------------------------------------------- 
 * ⚠️ STUDENT RULES FOR PHASE 2:
 * You are restricted to modifying ONLY specific variables in each function:
 *
 * 1. MOVEMENT FUNCTIONS (movement_logic_X_Y):
 * Allowed: player->velocity
 * Goal:    Determine the direction and speed of movement.
 *
 * 2. SHOOTING FUNCTIONS (shooting_logic_X_Y):
 * Allowed: ball->velocity
 * Goal:    Determine the direction and power of the kick/pass.
 *
 * 3. CHANGE STATE FUNCTIONS (change_state_logic_X_Y):
 * Allowed: player->state
 * Goal:    Switch between IDLE, MOVING, SHOOTING, or INTERCEPTING.
 *
 * NOTE: Directly modifying any other attributes will be flagged as a violation.
 * Thank you for your attention to this matter!
 * ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------
 * helper functions prototype
 * ------------------------------------------------------------------------- */

static Vec2 make_velocity_vector(Vec2 start, Vec2 target, float velocity);
static int is_colliding(const struct Player* p1, const struct Player* p2);
static int is_ball_colliding(const struct Player* p, const struct Ball* b) ;
static bool is_kickoff(struct Scene *scene);
static bool is_out(struct Scene *scene);
static Vec2 get_opponent_goal(struct Player* player);


/* -------------------------------------------------------------------------
 * movement logic helper functions prototype
 * ------------------------------------------------------------------------- */

static void verify_movement(struct Player *player);
static bool is_player_out(struct Player *player);
static void move_player_back_to_field(struct Player *player);
static bool verify_position(struct Player *player,  struct Scene *scene);
static bool verify_collision(struct Player *player,  struct Scene *scene);


/* -------------------------------------------------------------------------
 * movement logic functions prototype
 * ------------------------------------------------------------------------- */

static void forward_movement_logic(struct Player *self,  struct Scene *scene);
static void defender_movement_logic(struct Player *self,  struct Scene *scene);
static void gk_movement_logic(struct Player *self,  struct Scene *scene);


/* -------------------------------------------------------------------------
 * shooting logic helper functions prototype
 * ------------------------------------------------------------------------- */

static struct Player* nearest_teammate(const struct Player* origin,  struct Scene *scene);
static void verify_shoot(struct Ball *ball);
static void player_shot_fill_zero();
static void pass(struct Ball* ball, struct Player *target, float velocity);
static void shoot(struct Ball* ball, struct Vec2 target, float velocity);


/* -------------------------------------------------------------------------
 * shooting logic functions prototype
 * ------------------------------------------------------------------------- */

static void forward_shooting_logic(struct Player *player,  struct Scene *scene);
static void defender_shooting_logic(struct Player *player,  struct Scene *scene);
static void gk_shooting_logic(struct Player *player,  struct Scene *scene);


/* -------------------------------------------------------------------------
 * change state logic functions prototype
 * ------------------------------------------------------------------------- */

static void forward_change_state_logic(struct Player *player,  struct Scene *scene);
static void defender_change_state_logic(struct Player *player,  struct Scene *scene);
static void gk_change_state_logic(struct Player *player,  struct Scene *scene);



/* Team 1 movement logic */
void movement_logic_1_0(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_1_1(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_1_2(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_1_3(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_1_4(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_1_5(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }

/* Team 2 movement logic */
void movement_logic_2_0(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_2_1(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_2_2(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_2_3(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_2_4(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }
void movement_logic_2_5(struct Player *self, struct Scene *scene) { forward_movement_logic(self, scene); }

/* Team 1 shooting logic */
void shooting_logic_1_0(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_1_1(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_1_2(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_1_3(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_1_4(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_1_5(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }

/* Team 2 shooting logic */
void shooting_logic_2_0(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_2_1(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_2_2(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_2_3(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_2_4(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }
void shooting_logic_2_5(struct Player *self, struct Scene *scene) { forward_shooting_logic(self, scene); }

/* Team 1 change_state logic */
void change_state_logic_1_0(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_1_1(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_1_2(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_1_3(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_1_4(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_1_5(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }

/* Team 2 change_state logic */
void change_state_logic_2_0(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_2_1(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_2_2(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_2_3(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_2_4(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }
void change_state_logic_2_5(struct Player *self, struct Scene *scene) { forward_change_state_logic(self, scene); }

/* -------------------------------------------------------------------------
 * Lookup tables for factory
 * ------------------------------------------------------------------------- */
static PlayerLogicFn team1_movement[6] = {
    movement_logic_1_0, movement_logic_1_1, movement_logic_1_2,
    movement_logic_1_3, movement_logic_1_4, movement_logic_1_5
};

static PlayerLogicFn team2_movement[6] = {
    movement_logic_2_0, movement_logic_2_1, movement_logic_2_2,
    movement_logic_2_3, movement_logic_2_4, movement_logic_2_5
};

static PlayerLogicFn team1_shooting[6] = {
    shooting_logic_1_0, shooting_logic_1_1, shooting_logic_1_2,
    shooting_logic_1_3, shooting_logic_1_4, shooting_logic_1_5
};

static PlayerLogicFn team2_shooting[6] = {
    shooting_logic_2_0, shooting_logic_2_1, shooting_logic_2_2,
    shooting_logic_2_3, shooting_logic_2_4, shooting_logic_2_5
};

static PlayerLogicFn team1_change_state[6] = {
    change_state_logic_1_0, change_state_logic_1_1, change_state_logic_1_2,
    change_state_logic_1_3, change_state_logic_1_4, change_state_logic_1_5
};

static PlayerLogicFn team2_change_state[6] = {
    change_state_logic_2_0, change_state_logic_2_1, change_state_logic_2_2,
    change_state_logic_2_3, change_state_logic_2_4, change_state_logic_2_5
};

/* -------------------------------------------------------------------------
 * Factory functions
 * ------------------------------------------------------------------------- */
PlayerLogicFn get_movement_logic(int team, int kit) {
    if (coach_both_teams) return team1_movement[kit];
    return (team == 1) ? team1_movement[kit] : team2_movement[kit];
}

PlayerLogicFn get_shooting_logic(int team, int kit) {
    if (coach_both_teams) return team1_shooting[kit];
    return (team == 1) ? team1_shooting[kit] : team2_shooting[kit];
}

PlayerLogicFn get_change_state_logic(int team, int kit) {
    if (coach_both_teams) return team1_change_state[kit];
    return (team == 1) ? team1_change_state[kit] : team2_change_state[kit];
}

/* -------------------------------------------------------------------------
 * TALENTS
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Talents team1_talents[6] = {
    {3, 6, 4, 7},   // forward 1   
    {3, 7, 3, 7},   // forward 2
    {7, 4, 3, 6},   // defender 1
    {8, 3, 2, 7},   // gk
    {8, 4, 3, 5},   // defender 2
    {4, 5, 4, 7},   // forward 3
};

/* Team 2 */
static struct Talents team2_talents[6] = {
    {2, 6, 4, 8},   // forward 1
    {4, 5, 4, 7},   // forward 2
    {7, 5, 3, 5},   // defender 1
    {8, 3, 2, 7},   // gk
    {8, 4, 2, 6},   // defender 2
    {3, 7, 3, 7},   // forward 3
};

struct Talents get_talents(int team, int kit) {
    if (coach_both_teams) return team1_talents[kit];
    return (team == 1) ? team1_talents[kit] : team2_talents[kit];
}


/* -------------------------------------------------------------------------
 * Positioning
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Vec2 team1_positions[6] = { 
    {350, CENTER_Y},        // forward 1
    {300, CENTER_Y-150},    // forward 2
    {200, CENTER_Y-75},     // defender 1
    {40, CENTER_Y},         // gk
    {200, CENTER_Y+75},     // defender 2
    {300, CENTER_Y+150},    // forward 3
};

/* Team 2 */
static struct Vec2 team2_positions[6] = { 
    {750, CENTER_Y},        // forward 1
    {700, CENTER_Y-150},    // forward 2
    {800, CENTER_Y-75},     // defender 1
    {960, CENTER_Y},        // gk
    {800, CENTER_Y+75},     // defender 2
    {700, CENTER_Y+150},    // forward 3
};

struct Vec2 get_positions(int team, int kit) {
    return (team == 1) ? team1_positions[kit] : team2_positions[kit];
}


/* -------------------------------------------------------------------------
 * Preferred Positions
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Vec2 team1_preferred_positions[6] = { 
    {600, CENTER_Y},        // forward 1
    {750, CENTER_Y-200},    // forward 2
    {200, CENTER_Y-75},     // defender 1
    {40, CENTER_Y},         // gk
    {200, CENTER_Y+75},     // defender 2
    {750, CENTER_Y+200},    // forward 3
};

/* Team 2 */
static struct Vec2 team2_preferred_positions[6] = { 
    {400, CENTER_Y},        // forward 1
    {300, CENTER_Y-200},    // forward 2
    {800, CENTER_Y-75},     // defender 1
    {960, CENTER_Y},        // gk
    {800, CENTER_Y+75},     // defender 2
    {300, CENTER_Y+200},    // forward 3
};

struct Vec2 get_preferred_positions(int team, int kit) {
    return (team == 1) ? team1_preferred_positions[kit] : team2_preferred_positions[kit];
}


/* -------------------------------------------------------------------------
 * helper functions
 * ------------------------------------------------------------------------- */

 /**
 * @brief Creates a velocity vector towards the position given
 */
static Vec2 make_velocity_vector(Vec2 start, Vec2 target, float velocity){
    float x_distance = target.x - start.x;
    float y_distance = target.y - start.y;
    float distance = hypotf(x_distance, y_distance);

    if (distance < 0.1f) return (Vec2){0, 0};

    Vec2 new_vel = {
        .x =  (x_distance / distance) * velocity,
        .y = (y_distance / distance) * velocity
    };

    return new_vel;
}

 /**
 * @brief Internal helper to check if a player's circular hitbox overlaps with one of the other players.
 * @return 1 if colliding, 0 otherwise.
 */
static int is_colliding(const struct Player* p1, const struct Player* p2) {
    // Standard Circle-to-Circle collision math: (dist^2 <= combined_radius^2)
    float dx = p1->position.x - p2->position.x;
    float dy = p1->position.y - p2->position.y;
    float dist_sq = dx * dx + dy * dy;
    float radius_sum = p1->radius + p2->radius;
    return dist_sq <= radius_sum * radius_sum;
}


 /**
 * @brief Internal helper to check if a player's circular hitbox overlaps the ball's.
 * @return 1 if colliding, 0 otherwise.
 */
static int is_ball_colliding(const struct Player* p, const struct Ball* b) {
    // Standard Circle-to-Circle collision math: (dist^2 <= combined_radius^2)
    float dx = p->position.x - b->position.x;
    float dy = p->position.y - b->position.y;
    float dist_sq = dx * dx + dy * dy;
    float radius_sum = p->radius + b->radius;
    return dist_sq <= radius_sum * radius_sum;
}


 /**
 * @brief Internal helper to check if scene state is at kickoff.
 * @return 1 if kickoff, 0 otherwise.
 */
static bool is_kickoff(struct Scene *scene){
    if((scene->ball->position.x != CENTER_X) || 
        (scene->ball->position.y != CENTER_Y))
    {
        return false;
    }

    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        struct Player* p1 = scene->first_team->players[i];
        struct Player* p2 = scene->second_team->players[i];

        if ((p1->velocity.x != 0 || p1->velocity.y != 0) ||
            (p2->velocity.x != 0 || p2->velocity.y != 0)) 
        {
            return false;
        }
    }

    return true;
}


 /**
 * @brief Internal helper to check if someone is going to take a throw-in.
 * @return 1 if kickoff, 0 otherwise.
 */
static bool is_out(struct Scene *scene){
    if((scene->ball->velocity.x != 0) || 
        (scene->ball->velocity.y != 0))
    {
        return false;
    }

    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        struct Player* p1 = scene->first_team->players[i];
        struct Player* p2 = scene->second_team->players[i];

        if ((p1->velocity.x != 0 || p1->velocity.y != 0) ||
            (p2->velocity.x != 0 || p2->velocity.y != 0)) 
        {
            return false;
        }
    }

    return true;
}


 /**
 * @brief Internal helper to find opponent goal.
 * @return The position of the center of opponent's goal line.
 */
static Vec2 get_opponent_goal(struct Player* player){
    Vec2 team1_goal = {
        .x = CENTER_X - PITCH_W / 2,
        .y = CENTER_Y
    };

    Vec2 team2_goal = {
        .x = CENTER_X + PITCH_W / 2,
        .y = CENTER_Y
    };

    Vec2 opponent_goal = (player->team == 1) ? team2_goal : team1_goal;

    return opponent_goal;
}


/* -------------------------------------------------------------------------
 * movement logic helper functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Verifies a player's position
 *
 * This function ensures that a player's velocity does not exceed
 * the maximum allowed speed derived from their agility talent.
 *
 * @param player Pointer to the player whose movement is being verified.
 */
static void verify_movement(struct Player *player) {
    float max_speed = MAX_PLAYER_VELOCITY * player->talents.agility / (float)MAX_TALENT_PER_SKILL;
    float abs_velocity_x = (player->velocity.x > 0) ? player->velocity.x : (player->velocity.x * -1.0f);
    float abs_velocity_y = (player->velocity.y > 0) ? player->velocity.y : (player->velocity.y * -1.0f);
    
    // checking x velocity
    if(abs_velocity_x > max_speed)
    {
        player->velocity.x = (player->velocity.x > 0) ? max_speed : (max_speed * -1.0f);
    }

    // checking y velocity
    if(abs_velocity_y > max_speed)
    {
        player->velocity.y = (player->velocity.y > 0) ? max_speed : (max_speed * -1.0f);
    }
}


/**
 * @brief Checks whether the player is out of bounds.
 *
 * The player is considered out only when its *entire area* lies outside
 * the pitch boundaries. Partial overlap with the pitch does NOT count as out.
 * 
 * 
 * @return true if the player is fully out of bounds, false otherwise.
 */
static bool is_player_out(struct Player *player){
    float top_point = player->position.y - player->radius;
    float bottom_point = player->position.y + player->radius;
    float right_point = player->position.x + player->radius;
    float left_point = player->position.x - player->radius;

    if ((top_point    > PITCH_Y + PITCH_H) ||
        (bottom_point < PITCH_Y) ||
        (left_point   > PITCH_X + PITCH_W) ||
        (right_point  < PITCH_X))
        {
            return true;
        }
    
    return false;
}


/**
 * @brief Moves the player back to the field if he's out.
 */
static void move_player_back_to_field(struct Player *player){
    float left_line   = PITCH_X;
    float right_line  = (PITCH_X + PITCH_W);
    float top_line    = PITCH_Y;
    float bottom_line = (PITCH_Y + PITCH_H);

    float top_point = player->position.y - player->radius;
    float bottom_point = player->position.y + player->radius;
    float right_point = player->position.x + player->radius;
    float left_point = player->position.x - player->radius;

    bool past_top = (bottom_point < top_line);
    bool past_bottom = (top_point > bottom_line);
    bool past_left = (right_point < left_line);
    bool past_right = (left_point > right_line);

    float max_speed = MAX_PLAYER_VELOCITY * (float)player->talents.agility / (float)MAX_TALENT_PER_SKILL;

    if(past_top)
    {
        if(player->velocity.y)
            player->velocity.y *= (player->velocity.y > 0) ? 1 : -1;

        else{
            srand((unsigned int)time(NULL));
            int random_speed = rand() % (int) floor(max_speed);

            player->velocity.y = (float) random_speed;
        }
    }

    if(past_bottom)
    {
        if(player->velocity.y)
            player->velocity.y *= (player->velocity.y < 0) ? 1 : -1;

        else{
            srand((unsigned int)time(NULL));
            int random_speed = rand() % (int) floor(max_speed);

            player->velocity.y = (float) random_speed * -1;
        }
    }

    if(past_left)
    {
        if(player->velocity.x)
            player->velocity.x *= (player->velocity.x > 0) ? 1 : -1;

        else{
            srand((unsigned int)time(NULL));
            int random_speed = rand() % (int) floor(max_speed);

            player->velocity.x = (float) random_speed * -1;
        }
    }

    if(past_right)
    {
        if(player->velocity.x)
            player->velocity.x *= (player->velocity.x < 0) ? 1 : -1;

        else{
            srand((unsigned int)time(NULL));
            int random_speed = rand() % (int) floor(max_speed);

            player->velocity.x = (float) random_speed * -1;
        }
    }
}


/**
 * @brief Verifies and limits a player's movement speed.
 *
 * This function ensures that a player's position is not out of bounds
 * except in the cases where player is taking a throw-in or a corner
 *
 * @param player Pointer to the player whose position is being verified.
 * @param scene Pointer to the current game scene.
 */
static bool verify_position(struct Player *player,  struct Scene *scene) {
    if(is_player_out(player))
    {
        if (!scene->ball->possessor){
            move_player_back_to_field(player);
            return true;
        }

        if ((is_out(scene) && scene->ball->possessor != player) || !is_out(scene)){
            move_player_back_to_field(player);
            return true;
        }
    }
    
    return false;
}


/**
 * @brief Verifies a player collision with another.
 *
 * This function ensures that players don't overlap each other 
 * and if they collide it modifies their velocity vector.
 *
 * @param player Pointer to the player whose collision is being verified.
 * @param scene Pointer to the current game scene.
 */
static bool verify_collision(struct Player *player,  struct Scene *scene) {
    float max_speed = MAX_PLAYER_VELOCITY * player->talents.agility / (float)MAX_TALENT_PER_SKILL;
    bool collision = false;

    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        struct Player* p1 = scene->first_team->players[i];
        struct Player* p2 = scene->second_team->players[i];

        // check if player collides with p1
        if(is_colliding(player, p1) && player != p1){
            Vec2 new_vel = make_velocity_vector(p1->position, player->position, max_speed);
            
            player->velocity.x += new_vel.x;
            player->velocity.y += new_vel.y;

            verify_movement(player);

            collision = true;
        }

        // check if player collides with p2
        if(is_colliding(player, p2) && player != p2){
            Vec2 new_vel = make_velocity_vector(p2->position, player->position, max_speed);
            
            player->velocity.x += new_vel.x;
            player->velocity.y += new_vel.y;

            verify_movement(player);

            collision = true;
        }
    }
    
    return collision;
}


/* -------------------------------------------------------------------------
 * movement logic functions
 * ------------------------------------------------------------------------- */

/**
 * @brief movement logic for forwards
 */
static void forward_movement_logic(struct Player *player,  struct Scene *scene){
    verify_movement(player);
    if(verify_position(player, scene)) return;
    if(verify_collision(player, scene)) return;

    struct Ball* ball = scene->ball;
    float max_speed = MAX_PLAYER_VELOCITY * player->talents.agility / (float)MAX_TALENT_PER_SKILL;

    float x_distance = ball->position.x - player->position.x;
    float y_distance = ball->position.y - player->position.y;
    float distance = hypotf(x_distance, y_distance);

    Vec2 opponent_goal = get_opponent_goal(player);

    // if he has the ball he rans toward the opponent goal
    if(ball->possessor == player)
    {
        player->velocity = make_velocity_vector(player->position, opponent_goal, max_speed);
    }

    else
    {
        // if no one has the ball and it's in a defined range he tries to catch it
        if(!ball->possessor){
            if(distance <= FORWARD_INTERCEPTING_RANGE && !player_shot[(player->team) - 1][player->kit]){
                player->velocity = make_velocity_vector(player->position, ball->position, max_speed);

                return;
            }
        }

        //if opponent's gk has the ball, the player goes to his preferred position
        struct Player *opponent_gk = (player->team == 1) ? scene->second_team->players[3] : scene->first_team->players[3];
        if(ball->possessor == opponent_gk){
            player->velocity = make_velocity_vector(player->position, get_preferred_positions(player->team, player->kit), max_speed);

            return;
        }

        // if ball is in his team's control he goes back to the opponent's half
        if(ball->last_team == player->team)
        {
            player->velocity = make_velocity_vector(player->position, get_preferred_positions(player->team, player->kit), max_speed);
        }

        else
        {
            // get the ball if opponent is in a defined range
            if (distance <= 100.0f){
                player->velocity = make_velocity_vector(player->position, ball->position, max_speed);

                return;
            }

            // if the ball be in the opponent's half, he tries to catch the ball
            // and if not he goes back to the opponent's half
            if ((player->team == 1 && ball->position.x > CENTER_X) ||
                (player->team == 2 && ball->position.x < CENTER_X))
            {
                player->velocity = make_velocity_vector(player->position, ball->position, max_speed);
            }

            else
            {
                player->velocity = make_velocity_vector(player->position, get_preferred_positions(player->team, player->kit), max_speed);
            }
        }
    }
}


/**
 * @brief movement logic for defenders
 */
static void defender_movement_logic(struct Player *player,  struct Scene *scene){
    verify_movement(player);
    if(verify_position(player, scene)) return;
    if(verify_collision(player, scene)) return;
}


/**
 * @brief movement logic for the goalkeeper
 */
static void gk_movement_logic(struct Player *player,  struct Scene *scene){
    verify_movement(player);
    if(verify_position(player, scene)) return;
    if(verify_collision(player, scene)) return;
}


/* -------------------------------------------------------------------------
 * shooting logic helper functions
 * ------------------------------------------------------------------------- */

/**
 * @brief This function finds the nearest teammate related to an origin.
 */
static struct Player* nearest_teammate(const struct Player* origin,  struct Scene *scene){
    struct Player* nearest = NULL;
    float min_dist = 999999.0f;
    struct Player* p = NULL;

    struct Player** teammates = (origin->team == 1) ? scene->first_team->players : scene->second_team->players;
    
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        if(teammates[i] == origin) continue;

        p = teammates[i];
        float d = hypotf(p->position.x - scene->ball->position.x, p->position.y - scene->ball->position.y);
        if (d < min_dist) { min_dist = d; nearest = p; }
    }
    
    if (!nearest) return NULL;

    return nearest;
}

 /**
 * @brief Verifies the validity of a ball shot.
 *
 * This function ensures that the ball's velocity after a shot does not
 * exceed the maximum allowed speed derived from the shooter's talent.
 *
 * @param ball    Pointer to the ball being shot.
 * @param kickoff True if the shot occurs during kickoff.
 */
static void verify_shoot(struct Ball *ball) {
    struct Player* player = ball->possessor;
    float max_speed = MAX_BALL_VELOCITY * player->talents.shooting / (float)MAX_TALENT_PER_SKILL;
    float abs_velocity_x = (ball->velocity.x > 0) ? ball->velocity.x : (ball->velocity.x * -1.0f);
    float abs_velocity_y = (ball->velocity.y > 0) ? ball->velocity.y : (ball->velocity.y * -1.0f);
    
    // checking x velocity
    if(abs_velocity_x > max_speed)
        ball->velocity.x = (ball->velocity.x > 0) ? max_speed : (max_speed * -1.0f);

    // checking y velocity
    if(abs_velocity_y > max_speed)
        ball->velocity.y = (ball->velocity.y > 0) ? max_speed : (max_speed * -1.0f);
}


/**
 * @brief Changes every player_shot elements to zero
 */
static void player_shot_fill_zero(){
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < PLAYER_COUNT; j++)
        {
            player_shot[i][j] = 0;
        }
        
    }
 
}


 /**
 * @brief Changes ball velocity vector towards the target player
 */
static void pass(struct Ball* ball, struct Player *target, float velocity){
    float x_distance = target->position.x - ball->position.x;
    float y_distance = target->position.y - ball->position.y;
    float distance = hypotf(x_distance, y_distance);

    Vec2 new_vel = {
        .x =  (x_distance / distance) * velocity,
        .y = (y_distance / distance) * velocity
    };

    ball->velocity.x = new_vel.x;
    ball->velocity.y = new_vel.y;

    // setting player player_shot to 1
    player_shot_fill_zero();
    int t_idx = ball->possessor->team - 1;
    int p_idx = ball->possessor->kit;
    player_shot[t_idx][p_idx] = 1;
}


 /**
 * @brief Changes ball velocity vector towards opponent's goal
 */
static void shoot(struct Ball* ball, struct Vec2 target, float velocity){
    float x_distance = target.x - ball->position.x;
    float y_distance = target.y - ball->position.y;
    float distance = hypotf(x_distance, y_distance);

    Vec2 new_vel = {
        .x =  (x_distance / distance) * velocity,
        .y = (y_distance / distance) * velocity
    };

    ball->velocity.x = new_vel.x;
    ball->velocity.y = new_vel.y;

    // setting player player_shot to 1
    player_shot_fill_zero();
    int t_idx = ball->possessor->team - 1;
    int p_idx = ball->possessor->kit;
    player_shot[t_idx][p_idx] = 1;
}


/* -------------------------------------------------------------------------
 * shooting logic functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Shooting logic for forward
 */
static void forward_shooting_logic(struct Player *player,  struct Scene *scene){
    struct Ball* ball = scene->ball;
    float max_velocity = MAX_BALL_VELOCITY * player->talents.shooting / (float)MAX_TALENT_PER_SKILL;

    verify_shoot(ball);

    // if the game is restarting and he has the ball then he passes it to the nearest teammate
    if (is_kickoff(scene)){
        if (ball->possessor == player){
                struct Player* nearest = nearest_teammate(player, scene);
                pass(ball, nearest, max_velocity);
            }
    }


    // if the player is going to take a throw-in he passes it to the nearest teammate
    else if(is_out(scene) && ball->possessor == player){
        struct Player* nearest = nearest_teammate(player, scene);
        pass(ball, nearest, max_velocity);
    }   


    else if(scene->state == STATE_RUNNING){
        Vec2 opponent_goal = get_opponent_goal(player);
        
        float max_velocity = MAX_BALL_VELOCITY * player->talents.shooting / (float)MAX_TALENT_PER_SKILL;

        float top_line    = opponent_goal.y - GOAL_HEIGHT / 2;
        float bottom_line = opponent_goal.y + GOAL_HEIGHT / 2;
        float goal_line   = opponent_goal.x;
        float other_line  = (player->team == 1) ? goal_line - FORWARD_SHOOTING_RANGE : goal_line + FORWARD_SHOOTING_RANGE;

        // if the player is near the opponent's goal, he shoots the ball towards the goal
        if ((player->team) == 1 &&
            (player->position.x >= other_line) &&
            (player->position.x <= goal_line) &&
            (player->position.y >= top_line) &&
            (player->position.y <= bottom_line) &&
            (ball->possessor && ball->possessor == player))
        {
            srand((unsigned int)time(NULL));
            int random_y = (rand() % (int) (GOAL_HEIGHT - 10)) + top_line +  5.0f;
            int target_x = goal_line;
            
            Vec2 target = {
                .x = target_x,
                .y = random_y
            };

            shoot(ball, target, max_velocity);
        }

        else if ((player->team) == 2 &&
            (player->position.x <= other_line) &&
            (player->position.x >= goal_line) &&
            (player->position.y >= top_line) &&
            (player->position.y <= bottom_line) &&
            (ball->possessor && ball->possessor == player))
        {
            srand((unsigned int)time(NULL));
            int random_y = (rand() % (int) (GOAL_HEIGHT - 10)) + top_line + 5.0f;
            int target_x = goal_line;
            
            Vec2 target = {
                .x = target_x,
                .y = random_y
            };

            shoot(ball, target, max_velocity);
        }
    } 
}

/**
 * @brief Shooting logic for denfender
 */
static void defender_shooting_logic(struct Player *player,  struct Scene *scene){ (void)scene; }


/**
 * @brief Shooting logic for goalkeeper
 */
static void gk_shooting_logic(struct Player *player,  struct Scene *scene){ (void)scene; }


/* -------------------------------------------------------------------------
 * change state logic functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Change state logic for forward
 */
static void forward_change_state_logic(struct Player *player,  struct Scene *scene){ 
    struct Ball* ball = scene->ball;

    // the player should not shoot while he doesn't have the ball
    if(ball->possessor != player && player->state == SHOOTING){
        player->state = IDLE;
        return;
    }

    // the player should not have the ball and be in INTERCEPTING state
    if(ball->possessor == player && player->state == INTERCEPTING){
        player->state = MOVING;
        return;
    }

    // if a player can't catch the ball before reaching over the tackle counter,
    // his state will be set to IDLE
    if(player->state == INTERCEPTING){
        if(tacke_counters[(player->team) - 1][player->kit] >= 40){
            player->state = IDLE;
            tacke_counters[(player->team) - 1][player->kit] = 0;
            return;
        }

        else{
            tacke_counters[(player->team) - 1][player->kit]++;
            return;
        }
    }

    // player tries to get the ball if it hits him
    if(is_ball_colliding(player, ball) && !player_shot[(player->team) - 1][player->kit] && ball->possessor != player){
        player->state = INTERCEPTING;
        return;
    }

    //if opponent's gk has the ball, the state will be set to MOVING
    struct Player *opponent_gk = (player->team == 1) ? scene->second_team->players[3] : scene->first_team->players[3];
    if(ball->possessor == opponent_gk){
        player->state = MOVING;
    }

    // if it's kickoff and the player has the ball, his state will be set to SHOOTING
    if (is_kickoff(scene)){
        if (ball->possessor == player){
            player->state = SHOOTING;
        }
            
        else player->state = IDLE;
    }


    // if the ball is out and the player is going to take a throw-in
    // his state will be set to SHOOTING
    else if(is_out(scene)){
        if(ball->possessor == player) player->state = SHOOTING;
    }


    else if(scene->state == STATE_RUNNING){
        Vec2 opponent_goal = get_opponent_goal(player);
        
        float top_line      = opponent_goal.y - GOAL_HEIGHT / 2;
        float bottom_line   = opponent_goal.y + GOAL_HEIGHT / 2;
        float goal_line     = opponent_goal.x;
        float other_line    = (player->team == 1) ? goal_line - FORWARD_SHOOTING_RANGE : goal_line + FORWARD_SHOOTING_RANGE;

        // if the player is near the opponent's goal, his state wil be set to SHOOTING
        if ((player->team == 1) &&
            (player->position.x >= other_line) &&
            (player->position.x <= goal_line) &&
            (player->position.y >= top_line) &&
            (player->position.y <= bottom_line) &&
            (ball->possessor && ball->possessor == player))
        {
            player->state = SHOOTING;
            return;
        }

        else if ((player->team == 2) &&
                (player->position.x <= other_line) &&
                (player->position.x >= goal_line) &&
                (player->position.y >= top_line) &&
                (player->position.y <= bottom_line) &&
                (ball->possessor && ball->possessor == player))
        {
            player->state = SHOOTING;
            return;
        }

        // if the player is at the preferred position and he has nothing else to do,
        // his state will be set to IDLE
        else{
            float pp_x_distance = get_preferred_positions(player->team, player->kit).x - player->position.x;
            float pp_y_distance = get_preferred_positions(player->team, player->kit).y - player->position.y;
            float pp_distance = hypotf(pp_x_distance, pp_y_distance);

            float b_x_distance = ball->position.x - player->position.x;
            float b_y_distance = ball->position.y - player->position.y;
            float b_distance = hypotf(b_x_distance, b_y_distance);

            if(pp_distance < 0.1f){
                if(ball->last_team == player->team) player->state = IDLE;

                else if(b_distance <= FORWARD_INTERCEPTING_RANGE) player->state = MOVING;
                
                else if((ball->last_team != player->team) &&
                        (   (player->team == 1 && ball->position.x < CENTER_X) ||
                            (player->team == 2 && ball->position.x > CENTER_X)))
                {
                    player->state = IDLE;
                }

                else player->state = MOVING;
            }

            else player->state = MOVING;
        }

    }
}

/**
 * @brief Change state logic for defender
 */
static void defender_change_state_logic(struct Player *player,  struct Scene *scene){ (void)scene; }


/**
 * @brief Change state logic for goalkeeper
 */
static void gk_change_state_logic(struct Player *player,  struct Scene *scene){ (void)scene; }