#include "wizard.h"
#include "globals.h"
#include "game_dat.h"

/*
*   Gerekli Allegro initilalize fonksiyonlari cagrilir. Speed Counter'lar burada
*   ayarlanir, dat load edilir, gerekli bitmap'ler hazirlanir.
*/
void initialize()
{
    allegro_init();
    install_keyboard();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    set_window_title("Wizard of Wor by HUSEYIN ULUTAS");

    dat = load_datafile("game.dat");
    if(!dat)
    {
        allegro_message("Loading data file error");
        exit(1);
    }

    /* BITMAPS */
    second_buffer = create_bitmap(800,600);
    radar_bitmap  = create_bitmap(180,102);
    player        = (BITMAP*) dat[P_LEFT].dat;
    levels[0]     = (BITMAP*) dat[LEVEL1].dat;
    levels[1]     = (BITMAP*) dat[LEVEL2].dat;
    levels[2]     = (BITMAP*) dat[LEVEL3].dat;
    levels[3]     = (BITMAP*) dat[LEVEL4].dat;
    levels[4]     = (BITMAP*) dat[LEVEL5].dat;
    levels[5]     = (BITMAP*) dat[LEVEL6].dat;
    levels[6]     = (BITMAP*) dat[LEVEL7].dat;

    /* Hiz Ayarlari */
    LOCK_VARIABLE(speed_player);
    LOCK_FUNCTION(increment_speed_player);
    LOCK_VARIABLE(speed_enemy);
    LOCK_FUNCTION(increment_speed_enemy);
    LOCK_VARIABLE(speed_fire);
    LOCK_FUNCTION(increment_speed_fire);
    LOCK_VARIABLE(speed_worluk);
    LOCK_FUNCTION(increment_speed_worluk);
    LOCK_VARIABLE(speed_random);
    LOCK_FUNCTION(increment_speed_random);
    install_int_ex(increment_speed_player, BPS_TO_TIMER(150));
    install_int_ex(increment_speed_fire, BPS_TO_TIMER(600));
    install_int_ex(increment_speed_worluk, BPS_TO_TIMER(400));

    srand(time(NULL));
}

/*
*   Radar olusumunu saglayan fonksiyon.
*/
void radar()
{
    int i,j,a,color;

    clear_bitmap(radar_bitmap);
    for(a=0 ; a < MAX_ENEMY ; ++a)
    {
        if(enemies[a].live == 1 && enemies[a].type != worluk)
        {
            i = (enemies[a].x - 129) / 50;
            j = (enemies[a].y - 84) / 52;

            if(enemies[a].type == burwor)       color = BURWOR;
            else if(enemies[a].type == garwor)  color = GARWOR;
            else if(enemies[a].type == thorwor) color = THORWOR;
            else if(enemies[a].type == wizardofwor) color = BURWOR;

            rectfill(radar_bitmap,i * 16, j * 16,i * 16 + 16,j * 16 + 16,color);
        }
    }
}

/*
*   Yuksek skorlari kayit eden fonksiyon
*/
void save_high_score()
{
    int i,j;

    for(i=0 ; i < 5 ; ++i)
    {
        if(score > highscores[i])
        {
            for(j=3 ; j >= i ; --j) highscores[j+1] = highscores[j];
            highscores[i] = score;
            break;
        }
    }
}

/*
*   Fonksiyona yollanan double saniye kadar bekler.
*/
void wait(double second)
{
    clock_t endwait;
    endwait = clock () + second * CLOCKS_PER_SEC ;
    while (clock() < endwait) if(key[KEY_ESC]) break;
    reset_speed_counter();
}

/*
*   Player hizini ayarlayan fonksiyon
*/
void increment_speed_player()
{
  ++speed_player;
}
END_OF_FUNCTION(increment_speed_player);

/*
*   Canavarlarin hizini ayarlayan fonksiyon
*/
void increment_speed_enemy()
{
  ++speed_enemy;
}
END_OF_FUNCTION(increment_speed_enemy);

/*
*   Atesin hizini ayarlayan fonksiyon
*/
void increment_speed_fire()
{
  ++speed_fire;
}
END_OF_FUNCTION(increment_speed_fire);

/*
*   Worluk hizini ayarlayan fonksiyon
*/
void increment_speed_worluk()
{
  ++speed_worluk;
}
END_OF_FUNCTION(increment_speed_worluk);

/*
*   Canavar ateslerinin, yok olmalarin, ve isinlanmalarin tekrarlanma hizini
*   ayarlayan fonksiyon
*/
void increment_speed_random()
{
  ++speed_random;
}
END_OF_FUNCTION(increment_speed_random);

void reset_speed_counter()
{
    speed_player=0; speed_enemy=0; speed_fire=0; speed_worluk=0; speed_random=0;
}
