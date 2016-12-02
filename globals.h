/*
*   Global degiskenleri iceren Header File
*/

#ifndef GLOBALS_H
#define GLOBALS_H

    extern volatile long speed_player, speed_enemy, speed_fire, speed_worluk, speed_random;

    extern DATAFILE *dat;

    extern BITMAP  *second_buffer, *player, *levels[7], *radar_bitmap;

    extern int enemy_points[5];

    extern int score, frame, num_hit, volume, create, level, active_worluk;
    extern int px, py, plive, plife, borning, next_double, now_double;
    extern char dir;
    extern int highscores[5];

    extern enemy_t enemies[MAX_ENEMY];
    extern fire_t p_fire;

#endif
