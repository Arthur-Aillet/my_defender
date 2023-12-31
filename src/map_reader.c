/*
** EPITECH PROJECT, 2021
** mydefender
** File description:
** map_reader c
*/

#include "csfml.h"
#include "lib.h"
#include "structs.h"
#include "mouse.h"
#include "button.h"
#include "events.h"
#include "map.h"

sfSprite *create_sprite(env_t *env, int type, sfVector2f pos)
{
    sfSprite *sprite = sfSprite_create();

    if (type == 0 || type == 3)
        sfSprite_setTexture(sprite, env->data.ground_texture, sfFalse);
    if (type == 1)
        sfSprite_setTexture(sprite, env->data.path_texture, sfFalse);
    if (type == 2) {
        sfSprite_setTexture(sprite, env->data.path_texture, sfFalse);
        env->c_game.starting_square = pos;
    }
    pos.x *= CASE_SIZE;
    pos.y *= CASE_SIZE;
    sfSprite_setPosition(sprite, pos);
    sfSprite_setScale(sprite, SPRITE_SCALE);
    return sprite;
}

int setup_block(env_t *env, map_block *bloc, char *buf, sfVector2f pos)
{
    char *temp_str = NULL;

    if (my_strlen(buf) < DATABLOCK)
        return 1;
    bloc->type = buf[0] - '0';

    if (bloc->type == 1 || bloc->type == 2) {
        temp_str = max_strdup(&(buf[1]), 2);
        bloc->next_path.x = my_get_nbr(temp_str);
        free(temp_str);
        temp_str = max_strdup(&(buf[3]), 2);
        bloc->next_path.y = my_get_nbr(temp_str);
        free(temp_str);
    }
    bloc->turret = NULL;
    bloc->sprite = create_sprite(env, bloc->type, pos);
    return 0;
}

map_block *map_line(env_t *env, char *buffer, int nb)
{
    map_block *line = malloc(sizeof(map_block) * (MAP_LEN));

    if (my_strlen(buffer) != REAL_MAP_LEN)
        return NULL;
    for (int i = 0; i < MAP_LEN; i++) {
        if (setup_block(env, &line[i], buffer + i * DATABLOCK, VC{i, nb}))
            return NULL;
    }
    return line;
}

int read_map(env_t *game, char *path)
{
    FILE *fd = fopen(path, "r");
    size_t size = 0;
    char *buffer = NULL;
    map_block **final_tab = malloc(sizeof(map_block *) * 19);

    final_tab[18] = NULL;
    if (fd == NULL)
        return 1;
    for (int i = 0; i < 18; i++) {
        if (getline(&buffer, &size, fd) < 0) {
            free(buffer);
            return 1;
        }
        final_tab[i] = map_line(game, buffer, i);
        if (final_tab[i] == NULL)
            return 1;
    }
    game->c_game.map = final_tab;
    return 0;
}
