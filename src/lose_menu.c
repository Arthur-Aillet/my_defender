/*
** EPITECH PROJECT, 2022
** My_Defender
** File description:
** pause_menu
*/

#include "csfml.h"
#include "lib.h"
#include "menu.h"
#include "structs.h"
#include "mouse.h"
#include "button.h"
#include "events.h"

button *create_buttons_lose(env_t *env)
{
    button *buttons = malloc(sizeof(button) * 2);
    sfIntRect square = create_rect(0, 0, 6065 / 3, 833);

    buttons[0] = create_button(VC{0.2, 0.2}, VC{258, WINDOW_HEIGHT / 2 + 50}, sfTrue);
    buttons[1] = create_button(VC{0.2, 0.2}, VC{258, WINDOW_HEIGHT / 2 + 250}, sfTrue);
    for (int i = 0; i < 2; i++) {
        setup_button_texture(&(buttons[i]), &square, "img/Blue_button.png");
        setup_button_sounds(&(buttons[i]), "sounds/click.ogg", "sounds/hover.ogg", env);
    }
    if (env->langue[0] == 'E') {
        setup_button_text(&(buttons[0]), "Main menu", "font/Xero.ttf", 40);
        setup_button_text(&(buttons[1]), "Quit", "font/Xero.ttf", 40);
    } else {
        setup_button_text(&(buttons[0]), "Menu principal", "font/Xero.ttf", 25);
        setup_button_text(&(buttons[1]), "Quitter", "font/Xero.ttf", 40);
    }
    for (int i = 0; i < 2; i++)
        center_button_text(&(buttons[i]));
    return (buttons);
}

void lose_menu(sfRenderWindow *window, object mouse, int *keys, env_t *env)
{
    int open = 1;
    button *buttons = create_buttons_lose(env);
    text texte;
    object backfr = create_object("img/pause_fr.jpg", VC{0, 0}, VC{1, 1});
    object backen = create_object("img/pause_en.jpg", VC{0, 0}, VC{1, 1});

    if (env->langue[0] == 'E')
        texte = setup_text("   Lose", "font/o_driftbold.ttf", 150);
    else
        texte = setup_text(" Defaite", "font/o_driftbold.ttf", 150);
    sfText_setPosition(texte.text, VC{135, 120});
    while (sfRenderWindow_isOpen(window) && open) {
        sfRenderWindow_clear(window, sfBlack);
        get_events(window, keys);
        if (keys[sfKeyEscape] == 3)
            open = 0;
        if (is_pressed(buttons[0], window, keys))
            open = 0;
        if (is_pressed(buttons[1], window, keys))
            sfRenderWindow_close(window);
        display_background(window, backfr, backen, env);
        for (int i = 0; i < 2; i++)
            display_button(window, &(buttons[i]), keys);
        sfRenderWindow_drawText(window, texte.text, NULL);
        update_mouse_cursor(window, mouse, env->tempo);
        sfRenderWindow_display(window);
    }
    for (int i = 0; i < 2; i++)
        destroy_button(buttons[i]);
    keys[sfKeyEscape] = 0;
}