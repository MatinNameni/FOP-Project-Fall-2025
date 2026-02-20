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

/* Team 1 movement logic */
void movement_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 movement logic */
void movement_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 1 shooting logic */
void shooting_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 shooting logic */
void shooting_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 1 change_state logic */
void change_state_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 change_state logic */
void change_state_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void change_state_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

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
 *  TODO 2: Replace these default values with your desired skill points.
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Talents team1_talents[6] = {
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
};

/* Team 2 */
static struct Talents team2_talents[6] = {
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
    {5, 5, 5, 5},
};

struct Talents get_talents(int team, int kit) {
    if (coach_both_teams) return team1_talents[kit];
    return (team == 1) ? team1_talents[kit] : team2_talents[kit];
}


/* -------------------------------------------------------------------------
 * Positioning
 *  TODO 3: Decide players positions at kick-off.
 *        Players must stay on their half, outside the center circle.
 *        Keep in mind that the kick-off team's first player will automatically
 *             be placed at the center of the pitch.
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Vec2 team1_positions[6] = {
    {300, CENTER_Y},
    {250, CENTER_Y-150},
    {200, CENTER_Y-75},
    {150, CENTER_Y},
    {200, CENTER_Y+75},
    {250, CENTER_Y+150},
};

/* Team 2 */
static struct Vec2 team2_positions[6] = {
    {750, CENTER_Y},
    {800, CENTER_Y-150},
    {850, CENTER_Y-75},
    {900, CENTER_Y},
    {850, CENTER_Y+75},
    {800, CENTER_Y+150},
};

struct Vec2 get_positions(int team, int kit) {
    return (team == 1) ? team1_positions[kit] : team2_positions[kit];
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
    float max_speed = MAX_BALL_VELOCITY * player->talents.shooting;
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

    if ((top_point    > CENTER_Y + PITCH_H / 2) ||
        (bottom_point < CENTER_Y - PITCH_H / 2) ||
        (right_point  < CENTER_X - PITCH_W / 2) ||
        (left_point   > CENTER_X + PITCH_H / 2))
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

    float max_speed = MAX_PLAYER_VELOCITY * (float)player->talents.agility;

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

            player->velocity.y = (float) random_speed * -1;
        }
    }

    if(past_right)
    {
        if(player->velocity.x)
            player->velocity.x *= (player->velocity.x < 0) ? 1 : -1;

        else{
            srand((unsigned int)time(NULL));
            int random_speed = rand() % (int) floor(max_speed);

            player->velocity.y = (float) random_speed * -1;
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
static void verify_position(struct Player *player, const struct Scene *scene) {
    struct Player* ball_possessor = scene->ball->possessor;

    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        struct Player* p1 = scene->first_team->players[i];
        struct Player* p2 = scene->second_team->players[i];
        
        // p1 out of boundaries check
        if(is_player_out(p1))
            move_player_back_to_field(p1);

        // p2 out of boundaries check
        if(is_player_out(p2))
            move_player_back_to_field(p2);
    }
    
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
 * @brief Verifies a player collision with another.
 *
 * This function ensures that players don't overlap each other 
 * and if they collide it modifies their velocity vector.
 *
 * @param player Pointer to the player whose collision is being verified.
 * @param scene Pointer to the current game scene.
 */
static void verify_collision(struct Player *player, const struct Scene *scene) {
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        struct Player* p1 = scene->first_team->players[i];
        struct Player* p2 = scene->second_team->players[i];

        // check if player collides with p1
        if(is_colliding(player, p1) && player != p1){
            // TODO: player bounce back to the direction that connects centers of two players.
        }

        // check if player collides with p2
        if(is_colliding(player, p2) && player != p2){
            // TODO: the same as colliding p1.
        }
    }
    
}