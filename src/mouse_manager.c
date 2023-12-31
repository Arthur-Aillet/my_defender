/*
** EPITECH PROJECT, 2022
** My_Defender
** File description:
** mouse_manager c
*/

#include "mouse.h"
#include <math.h>

object setup_mouse(char *filename, sfVector2f scale)
{
    object mouse;
    sfIntRect rect = create_rect(0, 0, 24, 24);

    mouse.sprite = sfSprite_create();
    mouse.texture = sfTexture_createFromFile(filename, NULL);
    sfSprite_setTextureRect(mouse.sprite, rect);
    sfSprite_setScale(mouse.sprite, scale);
    sfSprite_setTexture(mouse.sprite, mouse.texture, sfFalse);
    return (mouse);
}

sfVector2f get_true_mouse_pos(sfRenderWindow *window)
{
    sfVector2f mouse_pos = itofv2(sfMouse_getPositionRenderWindow(window));
    sfVector2u screen_size = sfRenderWindow_getSize(window);

    mouse_pos.x = mouse_pos.x / screen_size.x * WINDOW_WIDTH;
    mouse_pos.y = mouse_pos.y / screen_size.y * WINDOW_HEIGHT;
    return (mouse_pos);
}

void update_mouse_cursor(sfRenderWindow *window, object mouse, sfClock *clock)
{
    sfVector2f mouse_pos = get_true_mouse_pos(window);
    sfIntRect rect;
    sfIntRect rect_current = sfSprite_getTextureRect(mouse.sprite);
    static float cooldown = 0;
    float sec = fmod(sfTime_asSeconds(sfClock_getElapsedTime(clock)), 1);

    if (sec < cooldown)
        cooldown = 0;
    if (sec - cooldown > .05) {
        cooldown = sec;
        if (rect_current.left < 466 - 24)
            rect = create_rect(rect_current.left + 24, 0, 24, 24);
        else
            rect = create_rect(0, 0, 24, 24);
        sfSprite_setTextureRect(mouse.sprite, rect);
    }
    sfSprite_setPosition(mouse.sprite, mouse_pos);
    sfRenderWindow_drawSprite(window, mouse.sprite, NULL);
}
