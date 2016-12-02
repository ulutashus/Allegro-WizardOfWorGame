#include "wizard.h"
#include "globals.h"
#include "game_dat.h"

/*
*   Tum ekrana basim islemlerinin yapildigi fonksiyondur.
*/
void print_screen()
{
    int i,j;

    /* Level haritasini second_buffer'a yukluyor */
    if(level < 8)       blit(levels[level - 1], second_buffer, 0,0,0,0,800,600);
    else if(level < 14) blit(levels[(level % 7) - 1], second_buffer, 0,0,0,0,800,600);
    else                blit(levels[6], second_buffer, 0,0,0,0,800,600);

    /* Radari second_buffer'a yukluyor */
    blit(radar_bitmap, second_buffer, 0,0,310,420,180,102);

    /* Oyuncu canlarini second_buffer'a yukluyor */
    for(i=0, j=452; i < plife ; ++i, j-=42)
    {
        if(i == 0) masked_blit(dat[P_LEFT].dat, second_buffer, 0, 0, 633, 410, P_SIZE, P_SIZE);
        else       masked_blit(dat[P_LEFT].dat, second_buffer, 0, 0, 689, j, P_SIZE, P_SIZE);
    }

    if(p_fire.fire)
        player_fire_print();

    /* Canavar ateslerinin ekrana bastirilmasi */
    for(i=0 ; i < MAX_ENEMY ; ++i)
        if(enemies[i].e_fire.fire)
            enemy_fire_print( &(enemies[i].e_fire) );

    /* Canavarlarin ekrana bastirilmasi */
    enemy_print();

     /* Kullaniciyi ve skoru second_buffer'a yukluyor ve ekrana basiyor */
    player_print();
    textprintf_centre_ex(second_buffer, dat[F_SCORE].dat, 645, 470, YELLOW, -1, "%d", score);

    if(volume==0)
        masked_blit(dat[MUTE].dat, second_buffer, 0, 0, 749, 15, 36, 36);

    blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
}

/*
*   Her level baslangicinda kullaniciya hazir ol mesaji verir ve bekler.
*/
void get_ready()
{
    play_sample(dat[S_GAME].dat,volume,127,950,0);

    if(level < 8)
    {
        textprintf_centre_ex(levels[level-1], dat[F_LEVEL].dat, 399, 397, FONTCOLOR, BLACK, "DUNGEON %d", level);
        textout_centre_ex(levels[level-1], dat[F_LEVEL].dat, "ESC to Menu  /  Arrow Keys to Walk  /  Space to Fire  /  M to Mute", 400, 565, BLUE, -1);
    }
    else if(level < 14)
        textprintf_centre_ex(levels[(level % 7) - 1], dat[F_LEVEL].dat, 399, 397, FONTCOLOR, BLACK, "DUNGEON %d", level);
    else
        textout_centre_ex(levels[6], dat[F_LEVEL].dat, "LAST DUNGEON", 399, 397, FONTCOLOR, BLACK);

    if(level < 8)       blit(levels[level - 1], second_buffer, 0,0,0,0,800,600);
    else if(level < 14) blit(levels[(level % 7) - 1], second_buffer, 0,0,0,0,800,600);
    else                blit(levels[6], second_buffer, 0,0,0,0,800,600);

    masked_blit(dat[GETREADY].dat, second_buffer, 0,0,124,166,550,79);
    blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
    wait(2.5);
    masked_blit(dat[GO].dat, second_buffer, 0,0,340,255,118,78);
    blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
    wait(1);
}

/*
*   7. ve 14. levellardan once 'Double Score Dungeon' mesajini verir. Bu mesajin
*   verildigi levellarda puanlar 2 kati kadar kazanilir.
*/
void double_score()
{
    play_sample(dat[S_DOUBLESCORE].dat,volume,127,950,0);
    clear_bitmap(screen);
    masked_blit(dat[DOUBLESCORE].dat, screen, 0,0,125,127,550,346);
    wait(2);
}

/*
*   14 level sonunda Tebrik mesaji verir.
*/
void congrats()
{
    play_sample(dat[S_GAMEOVER].dat,volume,127,950,0);
    masked_blit(dat[CONGRATS].dat, second_buffer, 0,0,148,175,500,125);
    blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
    reset_game();
    wait(7);
}

/*
*   Kullaniciya oyunu kaybettigi mesajini verir. Oyun bittigi takdirde 1, devam
*   ettiginde 0 return eder.
*/
int game_over()
{
    if(!plive && !plife)
    {
        stop_sample(dat[S_WORLUK].dat);
        stop_sample(dat[S_PLAYERDEAD].dat);
        play_sample(dat[S_GAMEOVER].dat,volume,127,950,0);

        /* 'GAME OVER' ekrana basimi */
        if(level < 8)       blit(levels[level - 1], second_buffer, 0,0,0,0,800,600);
        else if(level < 14) blit(levels[(level % 7) - 1], second_buffer, 0,0,0,0,800,600);
        else                blit(levels[6], second_buffer, 0,0,0,0,800,600);
        masked_blit(dat[GAMEOVER].dat, second_buffer, 0,0,128,166,550,120);
        textprintf_centre_ex(second_buffer, dat[F_SCORE].dat, 645, 470, YELLOW, -1, "%d", score);
        blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
        save_high_score();
        wait(3.5);
        print_highscores(); /* Yuksek skorlar ekrana basiliyor */
        reset_game();   /* Yeni oyuna baslangic icin oyun resetleniyor */
        wait(3.5);
        return 1;
    }
    return 0;
}
