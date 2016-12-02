/*
*   player_print(), player_born(), move_left(), move_right(), move_up(),
*   move_down(), left_avaliable(), right_avaliable(), up_avaliable(),
*   down_avaliable()
*
*   Fonksiyonlarinin implementasyonlarini iceren C dosyasi
*/

#include "wizard.h"
#include "globals.h"
#include "game_dat.h"

/*
*   Kullanici tus kontrollerini saglayan fonksiyon. ESC ye basildigi takdirde
*   1, basilmadiginda 0 return eder.
*/
int control()
{
    static int press_space, press_m;

    /* Kullanici hareketlerinin yapildigi kisim */
    while(speed_player > 0)
    {
        if (key[KEY_LEFT] && plive==1)
            move_left();
        if (key[KEY_RIGHT] && plive==1)
            move_right();
        if (key[KEY_UP] && plive==1)
            move_up();
        if (key[KEY_DOWN] && plive==1)
            move_down();
        --speed_player;
    }

    /* Kullanici atesinin yapildigi kisim */
    if (!(key[KEY_SPACE]))  press_space=0;
    if (key[KEY_SPACE] && press_space == 0 && plive==1 && p_fire.fire == 0)
    {
        fire_position(&p_fire, px, py, dir, 'p');
        press_space=1;
    }

    /* Ses ayarinin yapildigi kisim */
    if (!(key[KEY_M]))  press_m=0;
    if ((key[KEY_M]) && press_m == 0 )
    {
        if(volume==255) volume = 0;
        else            volume = 255;
        press_m = 1;
    }

    /* Menuye donusu ayarlayan kisim */
    if (key[KEY_ESC])   return 1;
    else                return 0;
}

/*
*   Player'i ekrana basan fonksiyon
*/
void player_print()
{
    /* Yasiyorken veya doguyorken ekrana basiyor */
    if(plive > 0 || borning)
        masked_blit(player, second_buffer, 0, 0, px, py, P_SIZE, P_SIZE);
    /* Patlama anini ekrana basiyor */
    else if(plive != 0)
    {
        if(plive >= -400 && plive < -300)
            masked_blit(dat[BOM1].dat, second_buffer, 0, 0, px, py, P_SIZE, P_SIZE);
        else if (plive >= -300 && plive < -200)
            masked_blit(dat[BOM2].dat, second_buffer, 0, 0, px, py, P_SIZE, P_SIZE);
        else if (plive>= -200 && plive< -100)
            masked_blit(dat[BOM3].dat, second_buffer, 0, 0, px, py, P_SIZE, P_SIZE);
        else
            masked_blit(dat[BOM4].dat, second_buffer, 0, 0, px, py, P_SIZE, P_SIZE);
        plive += 1;
    }
    /* Oldugu zaman pozisyonunu doguma uygun hale getiriyor */
    if (!borning && plive == 0) {px=633; py=410;}
}

/*
*   Player'in dogumunu gerceklestiren fonksiyon
*/
void player_born()
{
    if(plive == 0)
    {
        player = (BITMAP*)dat[P_LEFT].dat;
        dir='l';
        if(key[KEY_UP] && !borning)
        {
            play_sample(dat[S_BORN].dat,volume,127,950,0);
            borning=1;
        }
        if(speed_player > 0 && borning == 1 && plife != 0)
        {
            if ( py > 350 )     --py;   /* Dogum ani */
            else                { --plife; plive=1; borning=0;} /* Dogumun sonlanmasi */
            --speed_player;
        }
    }
}

/*
* Player'in sola hareketini saglayan fonksiyon.
*/
void move_left()
{
    /* Gecis kapisina gelme durumu ve diger kapiya isinlanma */
    if(px < 124 && ( py > 189 && py < 199 ) )
    {
        play_sample(dat[S_GATE].dat,volume,127,950,0);
        px = 642;
        py = 192;
    }
    else if(!left_avaliable(px, py))
    {
        if((frame % 200) < 100)
            player = (BITMAP*)dat[P_LEFT].dat;
        else
            player = (BITMAP*)dat[P_LEFT_W].dat;
        --px;
        dir = 'l';
    }
}

/*
* Player'in saga hareketini saglayan fonksiyon.
*/
void move_right()
{
    /* Gecis kapisina gelme durumu ve diger kapiya isinlanma */
    if(px > 643 && ( py > 189 && py < 199 ) )
    {
        play_sample(dat[S_GATE].dat,volume,127,950,0);
        px = 125;
        py = 192;
    }
    else if(!right_avaliable(px, py))
    {
        if((frame % 200) < 100)
            player = (BITMAP*)dat[P_RIGHT].dat;
        else
            player = (BITMAP*)dat[P_RIGHT_W].dat;
        ++px;
        dir = 'r';
    }
}

/*
* Player'in yukari hareketini saglayan fonksiyon.
*/
void move_up()
{
    if(!up_avaliable(px, py))
    {
        if(dir == 'l' || dir == 'u' || dir == 'D')
        {
            if((frame % 200) < 100)
                player = (BITMAP*)dat[P_UP_L].dat;
            else
                player = (BITMAP*)dat[P_UP_L_W].dat;
            dir = 'u';
        }
        else if(dir == 'r' || dir == 'U' || dir == 'd')
        {
            if((frame % 200) < 100)
                player = (BITMAP*)dat[P_UP_R].dat;
            else
                player = (BITMAP*)dat[P_UP_R_W].dat;
            dir = 'U';
        }
        --py;
    }
}

/*
* Player'in assagi hareketini saglayan fonksiyon.
*/
void move_down()
{
    if(!down_avaliable(px, py))
    {
        if(dir == 'l' || dir == 'd' || dir == 'U')
        {
            if((frame % 200) < 100)
                player = (BITMAP*)dat[P_DOWN_L].dat;
            else
                player = (BITMAP*)dat[P_DOWN_L_W].dat;
            dir = 'd';
        }
        else if(dir == 'r' || dir == 'D' || dir == 'u' )
        {
            if((frame % 200) < 100)
                player = (BITMAP*)dat[P_DOWN_R].dat;
            else
                player = (BITMAP*)dat[P_DOWN_R_W].dat;
            dir = 'D';
        }
        ++py;
    }
}

/*
* Sol tarafin musaitligini kontrol eder, müsait ise 0 degil ise 1 return eder.
*/
int left_avaliable(int x, int y)
{
    int i;
    for(i=y+P_SIZE+1 ; i >= y-1 ; --i)
        if(getpixel(second_buffer, x - 2, i) == BLUE)
            return 1;
    return 0;
}

/*
* Sag tarafin musaitligini kontrol eder, müsait ise 0 degil ise 1 return eder.
*/
int right_avaliable(int x, int y)
{
    int i;
    for(i=y+P_SIZE+1 ; i >= y-1 ; --i)
        if(getpixel(second_buffer, x + P_SIZE + 2, i) == BLUE)
            return 1;
    return 0;
}

/*
* Yukarinin musaitligini kontrol eder, müsait ise 0 degil ise 1 return eder.
*/
int up_avaliable(int x, int y)
{
    int i;
    for(i=x+P_SIZE+1 ; i >= x-1 ; --i)
        if(getpixel(second_buffer, i, y - 2) == BLUE)
            return 1;
    return 0;
}

/*
* Assaginin musaitligini kontrol eder, müsait ise 0 degil ise 1 return eder.
*/
int down_avaliable(int x, int y)
{
    int i;
    for(i=x+P_SIZE+1 ; i >= x-1 ; --i)
        if(getpixel(second_buffer, i, y + P_SIZE + 2) == BLUE)
            return 1;
    return 0;
}
