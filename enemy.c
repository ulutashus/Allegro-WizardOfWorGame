/*
*   enemy_main(), enemy_print(), enemy_rotation(), enemy_move(), create_enemy(),
*   random_position(), touch_player(), find_enemy_bitmap()
*
*   Fonksiyonlarinin implementasyonlarini iceren C dosyasi
*/

#include "wizard.h"
#include "globals.h"
#include "game_dat.h"

/*
*   Canavarlarin her seyini ayarlayan ana fonksiyon.
*/
void enemy_main()
{
    int i, luck;

    /* Canavarlarin yon degisikligini ayarlayan kisim */
    if(frame % 5 == 0)
    for(i=0 ; i < MAX_ENEMY ; ++i)
    {
        luck = rand() % 2;
        if(enemies[i].blocked || (luck == 0 && (enemies[i].x - 130) % 50 == 0 && (enemies[i].y - 85) % 53 == 0) )
            enemy_rotation(&enemies[i]);
    }

    /* Canavarlarin hareketini ayarlayan kisim */
    enemy_move();

    /* Canavarlarin player'a dokunarak oldurmesi */
    if(touch_player())
    {
        play_sample(dat[S_PLAYERDEAD].dat,volume,127,950,0);
        plive=-400;
    }

    while(speed_random > 0)
    {
        for(i=0 ; i < MAX_ENEMY ; ++i)
        {
        /* Canavarlarin rastgele ates etmesi */
            if(enemies[i].live == 1 && enemies[i].type != worluk)
            {
                luck = rand() % (enemies[i].type + 1);
                if (luck == 0)
                    fire_position(&(enemies[i].e_fire), enemies[i].x, enemies[i].y, enemies[i].dir, 'e');
            }
        /* Garwor ve Thorwor'larin rast gele gorunmez olmasi */
            if(enemies[i].live == 1 && (enemies[i].type == garwor || enemies[i].type == thorwor) )
            {
                luck = rand() % (enemies[i].type + 10);
                if (luck == 0)
                    enemies[i].hide = 1;
            }
        /* Wizard of Wor'un rastgele isinlanmasi */
            if( enemies[i].type == wizardofwor && enemies[i].live == 1 )
            {
                luck = rand() % (enemies[i].type + 5);
                if (luck == 0)
                    random_position(&enemies[i].x, &enemies[i].y);
            }
            --speed_random;
        }
    }

    /* Gorunmez olan canavarin tekrar gorunur hale gelmesi */
    for(i=0 ; frame == 1 && i < MAX_ENEMY ; ++i)
        if( enemies[i].hide == 1) enemies[i].hide = 0;
}

/*
*   Canavarlari ekrana bastiran fonksiyon
*/
void enemy_print()
{
    int i;
    for(i=0 ; i < MAX_ENEMY ; ++i)
    {
        /* Canavar yasiyorsa ve gornmez deil ise ekrana basiyor */
        if(enemies[i].live > 0 && enemies[i].hide == 0)
            masked_blit(enemies[i].shape, second_buffer, 0, 0, enemies[i].x, enemies[i].y, P_SIZE, P_SIZE);
        /* Canavar yasamiyorsa veya gorunmez ise hicbir sey yapmiyor */
        else if (enemies[i].live == 0 || enemies[i].hide == 1);
        /* Cavarin patlamasini ekrana basan kisim */
        else
        {
            if(enemies[i].live >= -400 && enemies[i].live < -300)
                masked_blit(dat[BOM1].dat, second_buffer, 0, 0, enemies[i].x, enemies[i].y, P_SIZE, P_SIZE);
            else if (enemies[i].live >= -300 && enemies[i].live < -200)
                masked_blit(dat[BOM2].dat, second_buffer, 0, 0, enemies[i].x, enemies[i].y, P_SIZE, P_SIZE);
            else if (enemies[i].live >= -200 && enemies[i].live < -100)
                masked_blit(dat[BOM3].dat, second_buffer, 0, 0, enemies[i].x, enemies[i].y, P_SIZE, P_SIZE);
            else
                masked_blit(dat[BOM4].dat, second_buffer, 0, 0, enemies[i].x, enemies[i].y, P_SIZE, P_SIZE);
            enemies[i].live += 1;
        }
    }
}

/*
*   Fonkisyona yollanan canavarin donme zamani geldiginde veya duvara carptiginda
*   yon degistirmesini saglayan fonksiyon
*/
void enemy_rotation(enemy_t *enemy)
{
    int again, i=0;
    char temp;

    if(enemy->live == 1)
    {
        again=0;
        while(again == 0)
        {
            ++i;
            switch(rand() % 4)
            {
                case 0:
                    again = enemy_right_avaliable(enemy->x, enemy->y);
                    if(again) temp = 'r';
                    break;
                case 1:
                    again = enemy_left_avaliable(enemy->x, enemy->y);
                    if(again) temp = 'l';
                    break;
                case 2:
                    again = enemy_up_avaliable(enemy->x, enemy->y);
                    if(again) temp = 'u';
                    break;
                case 3:
                    again = enemy_down_avaliable(enemy->x, enemy->y);
                    if(again) temp = 'd';
                    break;
            }

            if(i < 20)
            {
                if      (enemy->dir=='r'&&temp=='l') again=0;
                else if (enemy->dir=='l'&&temp=='r') again=0;
                else if (enemy->dir=='d'&&temp=='u') again=0;
                else if (enemy->dir=='u'&&temp=='d') again=0;
            }
        }
        enemy->blocked = 0;
        enemy->dir = temp;
    }
}

/*
*   Canavarlarin hareketini saglayan fonksiyon
*/
void enemy_move()
{
    int i;

    /* Bu while dongusu canavarin hizini ayarliyor */
    while( (active_worluk == 0 && speed_enemy > 0) || (active_worluk == 1 && speed_worluk > 0) )
    {
    for(i=0 ; i < MAX_ENEMY ; ++i)
    {
        if(enemies[i].live == 1)
        {
            switch(enemies[i].dir)
            {
            /* SAGA HAREKET */
                case 'r':
                /* Sag kapidan girip sol kapidan cikma durumu */
                if(enemies[i].x > 643 && ( enemies[i].y > 189 && enemies[i].y < 199 ) )
                {
                    if(enemies[i].type == worluk)
                    {
                        ++num_hit;
                        enemies[i].live = 0;
                        set_level(0);
                    }
                    play_sample(dat[S_GATE].dat,volume,127,950,0);
                    enemies[i].x = 130;
                    enemies[i].y = 191;
                }
                /* Yol musaitse ilerliyor */
                else if(enemy_right_avaliable(enemies[i].x, enemies[i].y))
                    enemies[i].x +=1;
                /* Yol musait degilse, carptigini blocked degiskeniyle belirtiyor */
                else    enemies[i].blocked=1;
                break;

            /* SOLA HAREKET */
                case 'l':
                if(enemies[i].x < 124 && ( enemies[i].y > 189 && enemies[i].y < 199 ) )
                {
                    if(enemies[i].type == worluk)
                    {
                        ++num_hit;
                        enemies[i].live = 0;
                        set_level(0);
                    }
                    play_sample(dat[S_GATE].dat,volume,127,950,0);
                    enemies[i].x = 630;
                    enemies[i].y = 191;
                }
                else if(enemy_left_avaliable(enemies[i].x, enemies[i].y))
                    enemies[i].x -= 1;
                else    enemies[i].blocked=1;
                break;

            /* YUKARIYA HAREKET */
                case 'u':
                if(enemy_up_avaliable(enemies[i].x, enemies[i].y))
                    enemies[i].y -= 1;
                else    enemies[i].blocked=1;
                break;

            /* ASSAGIYA HAREKET */
                case 'd':
                if(enemy_down_avaliable(enemies[i].x, enemies[i].y))
                    enemies[i].y += 1;
                else    enemies[i].blocked=1;
                break;
            }

            /* Canavarin seklini yukluyor */
            enemies[i].shape = find_enemy_bitmap(enemies[i].type, enemies[i].dir);
        }
    }
    if(active_worluk == 1)  --speed_worluk;
    else                    --speed_enemy;
    }
}

/*
*   Canavarin ilerleyecegi sol istikametin musait olup olmadigini kontrol eder.
*   Musaitse 1 degilse 0 return eder.
*/
int enemy_left_avaliable(int x, int y)
{
    int i,j;

    if( (x-130) % 50 == 0 )
    {
        for(i=x ; i >= x - 50 ; --i)
        {
            for(j=y ; j <= y + P_SIZE ; ++j)
                if(getpixel(second_buffer, i, j) == BLUE)
                    return 0;
        }
    }
    return 1;
}

/*
*   Canavarin ilerleyecegi sag istikametin musait olup olmadigini kontrol eder.
*   Musaitse 1 degilse 0 return eder.
*/
int enemy_right_avaliable(int x, int y)
{
    int i,j;

    if( (x-130) % 50 == 0 )
    {
        for(i=x ; i <= x + 50 ; ++i)
        {
            for(j=y ; j <= y + P_SIZE ; ++j)
                if(getpixel(second_buffer, i, j) == BLUE)
                    return 0;
        }
    }
    return 1;
}

/*
*   Canavarin ilerleyecegi yukari istikametin musait olup olmadigini kontrol eder.
*   Musaitse 1 degilse 0 return eder.
*/
int enemy_up_avaliable(int x, int y)
{
    int i,j;

    if( (y-85) % 53 == 0 )
    {
        for(j=y ; j >= y - 53 ; --j)
        {
            for(i=x ; i <= x + P_SIZE ; ++i)
                if(getpixel(second_buffer, i, j) == BLUE)
                    return 0;
        }
    }
    return 1;
}

/*
*   Canavarin ilerleyecegi assagi istikametin musait olup olmadigini kontrol eder.
*   Musaitse 1 degilse 0 return eder.
*/
int enemy_down_avaliable(int x, int y)
{
    int i,j;

    if( (y-85) % 53 == 0 )
    {
        for(j=y ; j <= y + 53 ; ++j)
        {
            for(i=x ; i <= x + P_SIZE ; ++i)
                if(getpixel(second_buffer, i, j) == BLUE)
                    return 0;
        }
    }
    return 1;
}

/*
*   Fonksiyona yollanan turde bir canavar olusturan fonksiyon
*/
void create_enemy(monster_t type)
{
    int i=0,again;
    stop_sample(dat[S_ENEMYBORN].dat);
    play_sample(dat[S_ENEMYBORN].dat,volume,127,950,0);
    create=0;
    do
    {
        again=0;
        if(enemies[i].live == 0)
        {
            enemies[i].type = type;
            random_position(&enemies[i].x, &enemies[i].y);
            enemies[i].live = 1;
            enemies[i].hide = 0;
            enemies[i].dir = 'r';
            enemies[i].shape = find_enemy_bitmap(enemies[i].type, enemies[i].dir);
        }
        else
        {
            again=1;
            ++i;
        }
    }while(again);
}

/*
*   Canavara dogmasi icin rastgele bir yer ayarlayan ve bu kordinatlari *x ve *y
*   ile return eden fonksiyon
*/
void random_position(int *x, int *y)
{
    int i, j, pi, pj, again=0;

    pi = (px - 129) / 50;   /* Player'in x kordinati */
    pj = (py - 84) / 52;    /* Player'in y kordinati */

    do
    {
        again=0;
        i = rand() % 11;
        j = rand() % 4;
        /* Player'a cok yakin biyerden dogmasini engelleyen kisim */
        if( i > pi - 2 && i < pi + 2 && j > pj - 2 && j < pj + 2 )  again=1;
    }while(again);

    *x = i * 50 + 130;
    *y = j * 53 + 85;
}

/*
*   Canavarin player'a degip degmedigine bakan fonksiyon. Her hangi bir canavar
*   player'a degiyorsa 1 return eder, hicbir canavar player'a degmiyorsa 0
*   return eder.
*/
int touch_player()
{
    int x,i,j;

    for(x=0 ; x < MAX_ENEMY ; ++x)
    {
        if(enemies[x].live == 1)
            for(i=enemies[x].x ; i <= enemies[x].x + P_SIZE ; ++i)
                for(j=enemies[x].y ; j <= enemies[x].y + P_SIZE ; ++j)
                    if(plive==1 && is_inside_bitmap(player, i - px , j - py, 0))
                        return 1;
    }

    return 0;
}

/*
*   Fonksiyona yollanan canavar turune ve yonune gore bitmap adresini bulur,
*   ve bu adresi return eder.
*/
BITMAP* find_enemy_bitmap(monster_t type, char d)
{
    BITMAP *index;

    switch(type)
    {
        case burwor:
        if((frame % 200) < 100)
        {
            if(d == 'r')        index = (BITMAP*)dat[BURWOR_R].dat;
            else if(d== 'l')    index = (BITMAP*)dat[BURWOR_L].dat;
            else if(d== 'u')    index = (BITMAP*)dat[BURWOR_U].dat;
            else if(d== 'd')    index = (BITMAP*)dat[BURWOR_D].dat;
        }
        else
        {
            if(d == 'r')        index = (BITMAP*)dat[BURWOR_R_W].dat;
            else if(d== 'l')    index = (BITMAP*)dat[BURWOR_L_W].dat;
            else if(d== 'u')    index = (BITMAP*)dat[BURWOR_U_W].dat;
            else if(d== 'd')    index = (BITMAP*)dat[BURWOR_D_W].dat;
        }
        break;

        case garwor:
        if((frame % 200) < 100)
        {
            if(d == 'r')        index = (BITMAP*)dat[GARWOR_R].dat;
            else if(d== 'l')    index = (BITMAP*)dat[GARWOR_L].dat;
            else if(d== 'u')    index = (BITMAP*)dat[GARWOR_U].dat;
            else if(d== 'd')    index = (BITMAP*)dat[GARWOR_D].dat;
        }
        else
        {
            if(d == 'r')        index = (BITMAP*)dat[GARWOR_R_W].dat;
            else if(d== 'l')    index = (BITMAP*)dat[GARWOR_L_W].dat;
            else if(d== 'u')    index = (BITMAP*)dat[GARWOR_U_W].dat;
            else if(d== 'd')    index = (BITMAP*)dat[GARWOR_D_W].dat;
        }
        break;

        case thorwor:
        if((frame % 200) < 100)
        {
            if(d == 'r')        index = (BITMAP*)dat[THORWOR_R].dat;
            else if(d== 'l')    index = (BITMAP*)dat[THORWOR_L].dat;
            else if(d== 'u')    index = (BITMAP*)dat[THORWOR_U].dat;
            else if(d== 'd')    index = (BITMAP*)dat[THORWOR_D].dat;
        }
        else
        {
            if(d == 'r')        index = (BITMAP*)dat[THORWOR_R_W].dat;
            else if(d== 'l')    index = (BITMAP*)dat[THORWOR_L_W].dat;
            else if(d== 'u')    index = (BITMAP*)dat[THORWOR_U_W].dat;
            else if(d== 'd')    index = (BITMAP*)dat[THORWOR_D_W].dat;
        }
        break;

        case worluk:
        if((frame % 200) < 100) index = (BITMAP*)dat[WORLUK].dat;
        else                    index = (BITMAP*)dat[WORLUK_W].dat;
        break;

        case wizardofwor:
        if((frame % 200) < 100)
        {
            if(d == 'r')        index = (BITMAP*)dat[WIZARDOFWOR_R].dat;
            else if(d== 'l')    index = (BITMAP*)dat[WIZARDOFWOR_L].dat;
            else if(d== 'u')    index = (BITMAP*)dat[WIZARDOFWOR_U].dat;
            else if(d== 'd')    index = (BITMAP*)dat[WIZARDOFWOR_D].dat;
        }
        else
        {
            if(d == 'r')        index = (BITMAP*)dat[WIZARDOFWOR_R_W].dat;
            else if(d== 'l')    index = (BITMAP*)dat[WIZARDOFWOR_L_W].dat;
            else if(d== 'u')    index = (BITMAP*)dat[WIZARDOFWOR_U_W].dat;
            else if(d== 'd')    index = (BITMAP*)dat[WIZARDOFWOR_D_W].dat;
        }
        break;
    }

    return index;
}
