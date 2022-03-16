/*
** EPITECH PROJECT, 2021
** mydefender
** File description:
** bullets.c
*/

#include "csfml.h"
#include "structs.h"
#include "map.h"
#include "maths.h"

#define DIST_COLLISION 25

int bullet_collision(bullet_t *bullet, env_t *env)
{
    enemy *actual = env->c_game.enemies;
    sfVector2f pos = sfSprite_getPosition(bullet->sprite);

    while (actual != NULL) {
        if (actual->type == 0) {
            actual = actual->next;
            continue;
        }
        if (dist_two_points(pos,
        sfSprite_getPosition(actual->sprite)) < DIST_COLLISION) {
            actual->health -= bullet->damage;
            return 1;
        }
        actual = actual->next;
    }
    return 0;
}

bullet_t *null_bullet(void)
{
    bullet_t *output = malloc(sizeof(bullet_t));

    output->is_null = 1;
    output->type = 0;
    output->speed = 0;
    output->damage = 0;
    output->shooter = NULL;
    output->direction = VC{0, 0};
    output->sprite = NULL;
    output->next = NULL;
    return output;
}

bullet_t *create_bullet(enemy *target, turret_t *shooter, env_t *env)
{
    bullet_t *output = malloc(sizeof(bullet_t));
    sfVector2f target_pos = sfSprite_getPosition(target->sprite);
    sfVector2f shooter_pos = sfSprite_getPosition(shooter->sprite);
    float angle = A_regarde_B(shooter_pos, target_pos);
    sfVector2f direction = {ATOV(angle)};

    output->direction = direction;
    output->shooter = shooter;
    output->is_null = 0;
    output->type = 1;
    output->speed = 49;
    output->damage = 5;
    output->sprite = sfSprite_create();
    sfSprite_setTexture(output->sprite, env->data.bullet_texture, sfFalse);
    sfSprite_setOrigin(output->sprite, VC{200, 200});
    sfSprite_setPosition(output->sprite, shooter_pos);
    sfSprite_setScale(output->sprite, VC{.1, .1});
    sfSprite_setRotation(output->sprite, angle);
    output->next = NULL;
    return output;
}

void new_bullet(env_t *env, enemy *target, turret_t *shooter)
{
    bullet_t *actual = env->c_game.bullets;

    if (target == NULL)
        return;
    if (actual == NULL) {
        env->c_game.bullets = null_bullet();
        actual = env->c_game.bullets;
    }
    while (actual->next != NULL) {
        actual = actual->next;
    }
    actual->next = create_bullet(target, shooter, env);
}
