/*
** EPITECH PROJECT, 2021
** mydefender
** File description:
** map_reader.c
*/

#include "csfml.h"
#include "lib.h"
#include "structs.h"
#include "mouse.h"
#include "button.h"
#include "events.h"

#define REAL_MAP_LEN 193 // je compte 192 pourtant, étrange
#define MAP_LEN 32
#define SPRITE_SCALE (sfVector2f) {3.75, 3.75}
#define DATABLOCK 6

sfSprite *create_sprite(env_t *env, int type, sfVector2f pos)
{
    sfSprite *sprite = sfSprite_create();

    if (type == 0)
        sfSprite_setTexture(sprite, env->data.ground_texture, sfFalse);
    else
        sfSprite_setTexture(sprite, env->data.path_texture, sfFalse);
    sfSprite_setPosition(sprite, pos);
    sfSprite_setScale(sprite, SPRITE_SCALE);
    return sprite;
}

int setup_block(env_t *env, map_block *line, char *buf, sfVector2f pos)
{
    char *temp_str = NULL;

    if (strlen(buf) < DATABLOCK || buf[0] - '0' < 0 || buf[0] - '0' > 1)
        return 1;
    line->type = buf[0] - '0';

    if (line->type == 1) {
        temp_str = max_strdup(&(buf[1]), 2);
        line->next_path.x = my_get_nbr(temp_str);
        free(temp_str);
        temp_str = max_strdup(&(buf[3]), 2);
        line->next_path.y = my_get_nbr(temp_str);
        free(temp_str);
    }
    line->turret = NULL;
    line->sprite = create_sprite(env, line->type, pos);
    return 0;
}

sfVector2f len_to_xy(int len, int max_x)
{
    sfVector2f vector = {0, 0};

    vector.y = len / max_x;
    vector.x = len % max_x;
    return vector;
}

map_block *map_line(env_t *env, char *buffer)
{
    map_block *line = malloc(sizeof(map_block) * (MAP_LEN));

    if (my_strlen(buffer) != REAL_MAP_LEN)
        return NULL;
    for (int i = 0; i < MAP_LEN; i++) {
        if (setup_block(env, &line[i], buffer + i * DATABLOCK,
            len_to_xy(i, MAP_LEN)) == 1)
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
        size = getline(&buffer, &size, fd);
        if (size < 0) {
            free(buffer);
            return 1;
        }
        final_tab[i] = map_line(game, buffer);
        if (final_tab[i] == NULL)
            return 1;
    }
    game->map = final_tab;
    return 0;
}
