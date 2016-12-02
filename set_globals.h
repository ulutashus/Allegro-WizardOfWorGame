/*
*   Global degiskenlerin ilk degerlerini ayarlayan Header File
*/

#ifndef SET_GLOBALS_H
#define SET_GLOBALS_H

    volatile long speed_player=0, speed_enemy=0, speed_fire=0, speed_worluk=0, speed_random=0;

    DATAFILE *dat;

    BITMAP  *second_buffer, *player, *levels[7], *radar_bitmap;

    int enemy_points[5]={2500,1000,500,200,100};

    int score=0, frame=0, num_hit=0, volume=255, create=1, level=1, active_worluk=0;
    int px=633, py=410, plive=0, plife=3, borning=0, next_double=0, now_double=0;
    char dir='l';
    int highscores[5];

    enemy_t enemies[MAX_ENEMY];
    fire_t p_fire;

#endif
