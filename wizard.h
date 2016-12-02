/*
*   Gerekli includes, defines, typedefs ve function prototypes iceren Header File
*/

#ifndef WIZARD_H
#define WIZARD_H

/******************* INCLUDES *******************/
    #include <allegro.h>
    #include <time.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>

/******************* DEFINES *******************/
    #define P_SIZE 34       /* Oyuncu ve Canavar bitmaplerinin boyutu */
    #define MAX_ENEMY 13

    /* Renkler */
    #define PLAYER makecol(254,254,105)
    #define BLUE makecol(66,66,230)
    #define FIRE makecol(255,0,0)
    #define BLACK makecol(0,0,0)
    #define YELLOW makecol(255,255,66)
    #define BURWOR makecol(163,150,255)
    #define GARWOR makecol(254,254,128)
    #define THORWOR makecol(255,165,165)
    #define FONTCOLOR makecol(230,66,66)

/******************* TYPEDEFS *******************/

    typedef enum {wizardofwor=0,worluk=1,thorwor=2,garwor=3,burwor=4}monster_t;

    typedef struct
    {
        int fire;
        int fx, fy;
        char f_dir;
    }fire_t;

    typedef struct
    {
        monster_t type;
        int x, y;
        char dir;
        int blocked;
        int live;
        int hide;
        fire_t e_fire;
        BITMAP *shape;
    }enemy_t;

/******************* PROTOTYPES *******************/
    /* main.c */
    void play_game();
    void menu();
    void print_instructions();
    void print_highscores();

    /* other.c */
    void initialize();
    void radar();
    void save_high_score();
    void increment_speed_player();
    void increment_speed_enemy();
    void increment_speed_fire();
    void increment_speed_worluk();
    void increment_speed_random();
    void reset_speed_counter();
    void wait(double);

    /* print.c */
    void print_screen();
    void get_ready();
    void double_score();
    void congrats();
    int game_over();

    /* level.c */
    void set_level(monster_t);
    void reset_game();
    void new_level();

    /* player.c */
    int control();
    void player_print();
    void player_born();
    void move_left();
    void move_right();
    void move_up();
    void move_down();
    int left_avaliable(int, int);
    int right_avaliable(int, int);
    int up_avaliable(int, int);
    int down_avaliable(int, int);

    /* fire.c */
    void player_fire_print();
    void enemy_fire_print(fire_t*);
    void fire_position(fire_t*, int, int, char, char);
    void fire_move(fire_t*);
    int hit_border(fire_t);
    int hit_enemy();
    int hit_player(fire_t);

    /* enemy.c */
    void enemy_main();
    void create_enemy(monster_t);
    void enemy_rotation(enemy_t *);
    void enemy_move();
    void random_position(int*, int*);
    void enemy_print();
    int touch_player();
    BITMAP* find_enemy_bitmap(monster_t, char);
    int enemy_left_avaliable(int, int);
    int enemy_right_avaliable(int, int);
    int enemy_up_avaliable(int, int);
    int enemy_down_avaliable(int, int);

#endif
