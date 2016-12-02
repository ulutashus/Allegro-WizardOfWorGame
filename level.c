#include "wizard.h"
#include "globals.h"
#include "game_dat.h"

/*
*   Bu fonksiyon level ozelliklerine gore canavar olusumunu saglar, ayriyetten
*   kacinci level oldugunu ekrana yazar.
*/
void set_level(monster_t type)
{
    int i, temp_level;

    if(level > 6) temp_level = 6;   /* Bir defada dogan canavar sayisinin 6 dan */
    else temp_level = level;        /* fazla olmamasi icin */

    /* 6 tane Burwor olusturuluyor */
    if( num_hit == 0 )
    {
        /* Canavar hizi, ates etme, yok olma ve isinlanma hizi ayarlaniyor */
        install_int_ex(increment_speed_enemy, BPS_TO_TIMER(level * 5 + 95));
        install_int_ex(increment_speed_random, MSEC_TO_TIMER(105 - (level * 3)));

        get_ready();
        if(now_double) double_score();

        for(i=0 ; i < MAX_ENEMY ; ++i)  enemies[i].live = 0;
        for(i=0 ; i < 6 ; ++i)          create_enemy(burwor);
        create=0;
    }
    else if(num_hit >= (7 - temp_level))
    {
        if(type == burwor)
            create_enemy(garwor);
        else if(type == garwor)
            create_enemy(thorwor);
        else if( num_hit == 6 + (temp_level * 2) && level > 1 )
        {
            play_sample(dat[S_WORLUK].dat,volume,127,950,1);
            create_enemy(worluk);
            active_worluk=1;
            speed_worluk=0;
        }
        else if( level % 3 == 0 && num_hit == 7 + (temp_level * 2))
        {
            stop_sample(dat[S_WORLUK].dat);
            active_worluk=0;
            create_enemy(wizardofwor);
        }
        /* Level Sonlandirilmasi */
        else if( (level == 1 && num_hit == temp_level * 2 + 6) ||
                 (level % 3 == 0 && num_hit == temp_level * 2 + 8) ||
                 (level > 1 && num_hit == temp_level * 2 + 7) )
        new_level();
    }
}

/*
* Level gecislerini ayarlayan fonksiyon.
*/
void new_level()
{
    print_screen();
    stop_sample(dat[S_WORLUK].dat);
    stop_sample(dat[S_DEAD].dat);
    stop_sample(dat[S_PFIRE].dat);
    play_sample(dat[S_LASTHIT].dat,volume,127,950,0);
    active_worluk=0;

    now_double = next_double;
    next_double = 0;
    num_hit=0;
    plive=0;    /* Player olmedigi halde yeni levelda yeniden dogmasi icin olduruyoruz */
    ++plife;    /* Bu yuzden canindan gitmesin diye bir arttiriyoruz */
    if(level % 2 == 0) ++plife; /* her 2 levelda, bir can hediye */
    px=633;     /* Player pozisyonunu yeniden dogus haline getiriyoruz */
    py=410;
    ++level;
    create=1;
    wait(2.5);
}

/*
*   Yeni oyuna baslanmasi icin oyun ayarlarini resetleyen fonksiyon. Tum ayarlar
*   baslangic haline getiriliyor.
*/
void reset_game()
{
    int i;
    /* Reset */
    score=0; frame=0; num_hit=0; create=1; level=1;
    px=633; py=410; plive=0; plife=3; borning=0; dir='l';
    p_fire.fire=0; active_worluk=0; next_double=0; now_double=0;
    speed_player=0; speed_enemy=0; speed_fire=0; speed_worluk=0; speed_random=0;
    for(i=0 ; i < MAX_ENEMY ; ++i)  enemies[i].live = 0;
}
