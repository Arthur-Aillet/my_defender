/*
** EPITECH PROJECT, 2022
** My_Defender
** File description:
** enemy
*/

#ifndef ENEMY_H_
    #define ENEMY_H_

    #include "structs.h"
    #define TILE_SIZE 60
    #define SIGN(x) (x > 0 ? 1 : -1)
    #define nextph env->c_game.map[(int) pos.y / 60][(int) pos.x / 60].next_path
    #define nextph_type env->c_game.map[(int) pos.y / 60][(int) pos.x / 60].type

enemy *create_null_enemy(void);
void evolve_enemy(env_t *env, enemy *mob);
void display_enemies(sfRenderWindow *window, env_t *env);
void evolve_all_enemies(env_t *env);
enemy *create_enemies_type(void);
enemy *remove_next_enemy_if_needed(enemy *precedent);
enemy *get_oldest(env_t *env, turret_t *turret);
enemy *get_nearest(env_t *env, turret_t *turret);
wave_t wave_create(env_t *env, enemy *enemies_type);
void clone_enemy(env_t *env, enemy to_clone);
wave_t wave_manage(env_t *env, enemy *enemies_type, wave_t current_wave);
enemy *last_e_link(enemy *first);
enemy create_enemy_from_file(char *titre);
enemy *create_null_enemy(void);

#endif
