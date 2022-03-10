/*
** EPITECH PROJECT, 2021
** mydefender
** File description:
** main.c
*/

#include "menu.h"
#include "enemy.h"
#include "csfml.h"
#include "lib.h"
#include "structs.h"
#include "mouse.h"
#include "button.h"
#include "events.h"
#include "maths.h"
#include "map.h"
#include "hud.h"

void update_player_data(env_t *env, sfClock *clock)
{
    if (sfTime_asSeconds(sfClock_getElapsedTime(clock)) > 1) {
        env->c_game.player_stats.energy.x += env->c_game.player_stats.energy_income.x;
        env->c_game.player_stats.energy.y += env->c_game.player_stats.energy_income.y;
        env->c_game.player_stats.steel.x += env->c_game.player_stats.steel_income.x;
        env->c_game.player_stats.steel.y += env->c_game.player_stats.steel_income.y;
        if (env->c_game.player_stats.energy.x + env->c_game.player_stats.energy.y * 0.1 > 0) {
            while (env->c_game.player_stats.energy.y > 9) {
                env->c_game.player_stats.energy.y -= 10;
                env->c_game.player_stats.energy.x += 1;
            }
            while (env->c_game.player_stats.energy.y < 0) {
                env->c_game.player_stats.energy.y += 10;
                env->c_game.player_stats.energy.x -= 1;
            }
        } else {
            while (env->c_game.player_stats.energy.y < -9) {
                env->c_game.player_stats.energy.y = 0;
                env->c_game.player_stats.energy.x -= 1;
            }
            while (env->c_game.player_stats.energy.y > 0) {
                env->c_game.player_stats.energy.y -= 10;
                env->c_game.player_stats.energy.x += 1;
            }
        }
        if (env->c_game.player_stats.steel.x + env->c_game.player_stats.steel.y * 0.1 > 0) {
            while (env->c_game.player_stats.steel.y > 9 ) {
                env->c_game.player_stats.steel.y -= 10;
                env->c_game.player_stats.steel.x += 1;
            }
            while (env->c_game.player_stats.steel.y < 0) {
                env->c_game.player_stats.steel.y += 10;
                env->c_game.player_stats.steel.x -= 1;
            }
        } else {
            while (env->c_game.player_stats.steel.y < -9) {
                env->c_game.player_stats.steel.y = 0;
                env->c_game.player_stats.steel.x -= 1;
            }
            while (env->c_game.player_stats.steel.y > 0) {
                env->c_game.player_stats.steel.y -= 10;
                env->c_game.player_stats.steel.x += 1;
            }
        }
        sfClock_restart(clock);
    }
}

pop_button *create_turret_button_ui(int nbr)
{
    pop_button *button = malloc(sizeof(pop_button) * (nbr + 1));

    for (int i = 0; i < nbr; i++) {
        button[i].titre = setup_text("Base Turret", "font/Xero.ttf", 15);
        button[i].onglet = create_object("img/onglet.png", VC{i * 180, 820}, VC{2, 2});
        button[i].icon = create_object("img/turret1_1.png", VC{i * 180 + 90, 900}, VC{0.2, 0.2});
        sfText_setPosition(button[i].titre.text, VC{i * 180 + 12, 840});
        sfSprite_setOrigin(button[i].icon.sprite, VC{150, 250});
        sfSprite_setRotation(button[i].icon.sprite, 90);
    }
    button[nbr].onglet.sprite = NULL;
    return button;
}

void display_turret_icon(pop_button *buttons, sfRenderWindow *window, int i, sfVector2f mouse_pos)
{
    if (pos_in_square(mouse_pos, sfSprite_getGlobalBounds(buttons[i].icon.sprite))) {
        sfSprite_setColor(buttons[i].icon.sprite, sfColor_fromRGBA(220, 220, 220, 150));
        sfRenderWindow_drawSprite(window, buttons[i].icon.sprite, NULL);
        sfSprite_setColor(buttons[i].icon.sprite, sfColor_fromRGBA(255, 255, 255, 255));
    } else {
        sfRenderWindow_drawSprite(window, buttons[i].icon.sprite, NULL);
    }
}

void display_turrets_button_ui(pop_button *buttons, sfRenderWindow *window, int pickedup)
{
    sfVector2f mouse_pos = get_true_mouse_pos(window);
    sfFloatRect rect;

    for (int i = 0; buttons[i].onglet.sprite != NULL; i++) {
        rect = sfSprite_getGlobalBounds(buttons[i].onglet.sprite);
        if (pos_in_square(mouse_pos, rect) == sfTrue && sfSprite_getPosition(buttons[i].onglet.sprite).y > 800) {
            sfSprite_move(buttons[i].onglet.sprite, VC{0, -1});
            sfSprite_move(buttons[i].icon.sprite, VC{0, -1});
            sfText_move(buttons[i].titre.text, VC{0, -1});
        }
        if (pos_in_square(mouse_pos, rect) == sfFalse && sfSprite_getPosition(buttons[i].onglet.sprite).y < 820) {
            sfSprite_move(buttons[i].onglet.sprite, VC{0, 1});
            sfSprite_move(buttons[i].icon.sprite, VC{0, 1});
            sfText_move(buttons[i].titre.text, VC{0, 1});
        }
        sfRenderWindow_drawSprite(window, buttons[i].onglet.sprite, NULL);
        sfRenderWindow_drawText(window, buttons[i].titre.text, NULL);
        if (pickedup != i)
            display_turret_icon(buttons, window, i, mouse_pos);
    }
}

int pickup_turrets(pop_button *but, sfVector2f mouse_pos, int pick, int *keys, env_t *env)
{
    if (pick == -1) {
        for (int i = 0; but[i].onglet.sprite != NULL; i++) {
            sfFloatRect rect = sfSprite_getGlobalBounds(but[i].icon.sprite);

            if (pos_in_square(mouse_pos, rect) && keys[leftMouse] == 1) {
                sfSprite_setPosition(but[i].icon.sprite, mouse_pos);
                return i;
            }
        }
    } else {
        if (keys[leftMouse] == 2 || keys[leftMouse] == 1) {
            sfVector2i coo = get_case_coords(mouse_pos);

            if (coo.x > 0 && coo.y > 0 && coo.x < 32 && coo.y < 18 && env->c_game.map[coo.y][coo.x].type == 0)
                sfSprite_setPosition(but[pick].icon.sprite, VC{coo.x * 60 + 30, coo.y * 60 + 30});
            else
                sfSprite_setPosition(but[pick].icon.sprite, mouse_pos);
            return pick;
        }
        if (keys[leftMouse] == 3 || keys[leftMouse] == 0) {
            //if (coo.x < 32 && coo.y < 18 && env->c_game.map[coo.y][coo.x].type == 0)                                             //                     HERE TO CREATE TURETTE
            //    create_turret(coo.x, coo.y, but[pick].type.cequetuveuxdelastructturret);                                  //
            sfSprite_setPosition(but[pick].icon.sprite, VC{pick * 180 + 90, sfSprite_getPosition(but[pick].onglet.sprite).y + 80});
            return -1;
        }
    }
    return -1;
}

void display_picked_turret(int pickedup, pop_button *buttons, sfRenderWindow *w)
{
    if (pickedup != -1) {
        sfColor darken = sfColor_fromRGBA(220, 220, 220, 150);
        sfColor normal = sfColor_fromRGBA(255, 255, 255, 255);

        sfSprite_setColor(buttons[pickedup].icon.sprite, darken);
        sfRenderWindow_drawSprite(w, buttons[pickedup].icon.sprite, NULL);
        sfSprite_setColor(buttons[pickedup].icon.sprite, normal);
    }
}

void create_game(env_t *env)
{
    env->c_game.player_stats.health = 100;
    env->c_game.player_stats.energy.x = -200;
    env->c_game.player_stats.energy.y = 0;
    env->c_game.player_stats.steel.x = -10;
    env->c_game.player_stats.steel.y = -5;
    env->c_game.player_stats.energy_income.x = 2;
    env->c_game.player_stats.energy_income.y = 5;
    env->c_game.player_stats.steel_income.x = 0;
    env->c_game.player_stats.steel_income.y = 1;
    env->c_game.player_stats.wave = 1;
    env->c_game.enemies = NULL;
    env->c_game.clock = sfClock_create();
    if (read_map(env, "map") == 1)
        my_errorstr("Error on map\n");
}

void game(sfRenderWindow *window, object mouse, int *keys, env_t *env)
{
    int open = 1;
    create_game(env);
    int pick = -1; // -1 == rien, sinon numéro du popup button
    sfClock *clock = sfClock_create();
    hud hud_player = create_hud();
    object background = create_object("img/background.jpg", VC{0, 0}, VC{1, 1});
    object worm_hole = create_object("img/icon.png", VC{env->c_game.starting_square.x * 60 , env->c_game.starting_square.y * 60 - 58}, VC{.3, 1});
    //turret tourelle = create_turret_1(5, 5);
    pop_button *buttons = create_turret_button_ui(7);

    create_test_enemy(env, 100);
    create_test_enemy(env, 50);
    create_test_enemy(env, 75);
    setmap_opacity(env);
    sfClock_restart(env->c_game.clock);
    while (sfRenderWindow_isOpen(window) && open) {

        /* Act */

        sfRenderWindow_clear(window, sfBlack);
        get_events(window, keys);
        //sfSprite_setRotation(tourelle.sprite, A_regarde_B(tourelle.position, sfSprite_getPosition(env->c_game.enemies->next->next->sprite)));
        pick = pickup_turrets(buttons, get_true_mouse_pos(window), pick, keys, env);
        update_player_data(env, clock);
        update_hud(hud_player, env);

        /* Display */

        sfRenderWindow_drawSprite(window, background.sprite, NULL);
        display_map(env, window);
        //sfRenderWindow_drawSprite(window, tourelle.sprite, NULL);
        display_hud(hud_player, env, window);
        sfRenderWindow_drawSprite(window, worm_hole.sprite, NULL);
        display_turrets_button_ui(buttons, window, pick);
        display_enemies(window, env);
        display_picked_turret(pick, buttons, window);
        update_mouse_cursor(window, mouse);
        sfRenderWindow_display(window);
        evolve_all_enemies(env);

        if (keys[sfKeyEscape] == 3) {
            if (pause_menu(window, mouse, keys, env) == 1)
                open = 0;
            sfClock_restart(env->c_game.clock);
        }

        if (sfTime_asMilliseconds(sfClock_getElapsedTime(env->c_game.clock)) > 16)
            sfClock_restart(env->c_game.clock);
    }
    sfClock_destroy(clock);
    keys[sfKeyEscape] = 0;
}

env_t *create_env(void)
{
    env_t *env = malloc(sizeof(env_t));

    env->fps = 75;
    env->screen_type = 0;
    env->vsync = 1;
    env->resolution = 1920;
    env->data.music = sfMusic_createFromFile("sounds/uncharted-worlds.ogg");
    sfMusic_play(env->data.music);
    sfMusic_setLoop(env->data.music, sfTrue);
    env->volume = 100;
    sfMusic_setVolume(env->data.music, env->volume / 2);
    env->langue = my_strdup("FR");
    env->data.ground_texture =
        sfTexture_createFromFile("img/grass.png", NULL);
    env->data.path_texture = sfTexture_createFromFile("img/dirt.png", NULL);
    return env;
}

int main(void)
{
    int *keys = init_keys();
    sfVideoMode main_w = {1920, 1080, 24};
    sfRenderWindow *window;
    object mouse = setup_mouse("img/cursor.png", VC{1.7, 1.7});
    env_t *env = create_env();
    sfImage *icon = sfImage_createFromFile("img/Elecricity.png");
    sfVector2u size = sfImage_getSize(icon);

    window = sfRenderWindow_create(main_w, "My defender", sfDefaultStyle, NULL);
    sfRenderWindow_setFramerateLimit(window, env->fps);
    sfRenderWindow_setMouseCursorVisible(window, sfFalse);
    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setIcon(window, size.x, size.y,  sfImage_getPixelsPtr(icon));
    main_menu(window, mouse, keys, env);
    sfMusic_destroy(env->data.music);
    destroy_object(mouse);
    return (0);
}

// Coding syle
// Lose screen
// change size in options

// # annimation cursor fps indépendant
