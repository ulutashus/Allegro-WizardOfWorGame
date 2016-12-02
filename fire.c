/*
*   fire_position(), enemy_fire_print(), player_fire_print(), fire_move(),
*   hit_player(), hit_enemy(), hit_border()
*
*   Fonksiyonlarinin implementasyonlarini iceren C dosyasi
*/

#include "wizard.h"
#include "globals.h"
#include "game_dat.h"

/*
*   Bu fonksiyon sirasiyla fire_t* (ayarlanacak atesin adresi), int (ates edenin
*   x kordinati), int (ates edenin y kordinati), char (ates edenin yonu),
*   char (ates edenin canavar mi yoksa player mi oldudugunu belirten degisken)
*   verilerini alir ve yeni bir ates olusturur.
*/
void fire_position(fire_t *thefire, int x, int y, char direction, char type)
{
    /* Ates olusturma sarti, mevcut bir atesi olmamasi */
    if ( !(thefire->fire) )
    {
        if(type == 'p') /* Player atesi ise ona gore ses caliniyor */
        {
            stop_sample(dat[S_PFIRE].dat);
            play_sample(dat[S_PFIRE].dat,volume,127,950,0);
        }
        else            /* Canavar atesi ise ona gore ses caliniyor */
        {
            stop_sample(dat[S_EFIRE].dat);
            play_sample(dat[S_EFIRE].dat,volume,127,950,0);
        }

        thefire->fire = 1;  /* Ates aktiflestiriliyor */

    /* Ates edenin yonune gore atesin pozisyonu ve yonu ayarlaniyor */
        if(direction == 'r')
        {
            thefire->fx = x + 37;
            thefire->fy = y + 15;
            thefire->f_dir = 'r';
        }
        if(direction == 'l')
        {
            thefire->fx = x - 15;
            thefire->fy = y + 15;
            thefire->f_dir = 'l';
        }
        if(direction == 'u' || direction == 'U')
        {
            thefire->fx = x + 15;
            thefire->fy = y - 15;
            thefire->f_dir = 'u';
        }
        if(direction == 'd' || direction == 'D')
        {
            thefire->fx = x + 15;
            thefire->fy = y + 37;
            thefire->f_dir = 'd';
        }
    }
}

/*
*   Fonksiyona yollanan canavarin, atesinin ekrana basimini yapan fonksiyon
*/
void enemy_fire_print(fire_t *thefire)
{
    /* Ates duvara carptiysa kapatiliyor */
    if(hit_border(*thefire)) thefire->fire = 0;
    /* Player'in vurulma durumu s*/
    else if ( hit_player(*thefire) )
    {
        play_sample(dat[S_PLAYERDEAD].dat,volume,127,950,0);
        plive = -400;
        thefire->fire = 0;
    }
    /* Atesin ekrana basilma durumu */
    else
    {
       switch(thefire->f_dir)
        {
            case 'r':
            case 'l':
            masked_blit(dat[FIRE_H].dat, second_buffer, 0, 0, thefire->fx, thefire->fy, 12, 4);
            break;

            case 'u':
            case 'd':
            masked_blit(dat[FIRE_V].dat, second_buffer, 0, 0, thefire->fx, thefire->fy, 4, 12);
            break;
        }
    }
}

/*
*   Player atesinin ekrana basimini yapna fonksiyon
*/
void player_fire_print()
{
    int i;
    i=hit_enemy();  /* Canavar vurulup vurulmadigina bakiyor */

    /* Ates duvara carptiysa kapatiliyor */
    if(hit_border(p_fire))
    {
        p_fire.fire = 0;
        score -= 5;
    }
    /* Canavar vurulma durumu */
    else if ( i != -1 )
    {

        play_sample(dat[S_DEAD].dat,volume,127,950,0);
        p_fire.fire = 0;
        ++num_hit;

        if(now_double)  score += enemy_points[i] * 2;
        else            score += enemy_points[i];

        if(i == worluk || i == wizardofwor) next_double = 1;

        set_level(i);
    }
    /* Atesin ekrana basilma durumu */
    else
    {
       switch(p_fire.f_dir)
        {
            case 'r':
            case 'l':
            rectfill(second_buffer,p_fire.fx,p_fire.fy,p_fire.fx+12,p_fire.fy+1,FIRE);
            break;

            case 'u':
            case 'd':
            rectfill(second_buffer,p_fire.fx,p_fire.fy,p_fire.fx+1,p_fire.fy+12,FIRE);
            break;
        }
    }
}

/*
*   Fonksiyona yollanan atesin hareketini saglayan fonksiyon
*/
void fire_move(fire_t *thefire)
{
    if(thefire->f_dir == 'r')  thefire->fx += 1;
    if(thefire->f_dir == 'l')  thefire->fx -= 1;
    if(thefire->f_dir == 'u')  thefire->fy -= 1;
    if(thefire->f_dir == 'd')  thefire->fy += 1;
}

/*
*   Fonksiyona yollanan atesin player'i vurup vurmadigini kontrol eden fonksiyon.
*   Vurduysa 1, vurmadiysa 0 return eder.
*/
int hit_player(fire_t thefire)
{
    switch(thefire.f_dir)
    {
        case 'r':
            if(plive == 1 && is_inside_bitmap(player, thefire.fx+12 - px , thefire.fy - py, 0))
                return 1;
        break;

        case 'l':
        case 'u':
            if(plive == 1 && is_inside_bitmap(player, thefire.fx - px, thefire.fy - py, 0))
                return 1;
        break;

        case 'd':
            if(plive  == 1 && is_inside_bitmap(player, thefire.fx - px, thefire.fy+12 - py, 0))
                return 1;
        break;
    }

    return 0;
}

/*
*   Player atesinin herhangi bir canavar vurup vurmadigini kontrol eden fonksiyon.
*   Vurdugu takdirde vurdugu canavarin turunu temsil eden sayi return eder.
*   Vurmadigi takdirde -1 return eder.
*/
int hit_enemy()
{
    int i;
    switch(p_fire.f_dir)
    {
        case 'r':
        for(i = 0 ; i < MAX_ENEMY ; ++i)
        {
            if(enemies[i].live == 1 && is_inside_bitmap(enemies[i].shape, p_fire.fx+12 - enemies[i].x , p_fire.fy - enemies[i].y, 0))
            {
                enemies[i].hide = 0;
                enemies[i].live = -400;
                return (enemies[i].type);
            }
        }
        break;

        case 'l':
        case 'u':
        for(i = 0 ; i < MAX_ENEMY ; ++i)
        {
            if(enemies[i].live == 1 && is_inside_bitmap(enemies[i].shape, p_fire.fx - enemies[i].x, p_fire.fy - enemies[i].y, 0))
            {
                enemies[i].hide = 0;
                enemies[i].live = -400;
                return (enemies[i].type);
            }
        }
        break;

        case 'd':
        for(i = 0 ; i < MAX_ENEMY ; ++i)
        {
            if(enemies[i].live == 1 && is_inside_bitmap(enemies[i].shape, p_fire.fx - enemies[i].x, p_fire.fy+12 - enemies[i].y, 0))
            {
                enemies[i].hide = 0;
                enemies[i].live = -400;
                return (enemies[i].type);
            }
        }
        break;
    }
    return -1;
}

/*
*   Fonkisyona yollanan atesin duvara carpip carpmadigini kontrol eden fonksiyon.
*   Duvara carptiysa 1, carpmadiysa 0 return eder.
*/
int hit_border(fire_t thefire)
{
    int i;
    if(thefire.f_dir == 'r' || thefire.f_dir == 'l')
    {
        for(i = thefire.fx ; i <= thefire.fx+12 ; ++i)
            if(getpixel(second_buffer, i, thefire.fy) == BLUE || i > 678 || i < 119)
                return 1;
    }
    else
    {
       for(i = thefire.fy ; i <= thefire.fy+12 ; ++i)
            if(getpixel(second_buffer, thefire.fx, i) == BLUE || i > 395 || i < 84)
                return 1;
    }

    return 0;
}
