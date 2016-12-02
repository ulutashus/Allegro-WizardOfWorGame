/*
*   Summer_HW_101044020
*
*   YAZAN   :   Huseyin Ulutas
*   TARIH   :   05/08/2011
*
*   TANIM   :   Wizard of Wor oyununun driver kismini iceren C dosyasi.
*
*   NOT     :   Bu oyun, Code Blocks derleyicisi ile Windows 7 isletim sisteminde yazilmistir.
*/

#include "wizard.h"
#include "set_globals.h"
#include "game_dat.h"

/* DRIVER */
int main(void)
{
    FILE *inp, *outp;

    /* Yuksek skorlar dosyadan okunuyor */
    inp = fopen("highscores.db", "rb");
    fread(highscores, sizeof(int), 5, inp);
    fclose(inp);

    initialize();
    menu();

    /* Oyun kapanirken yuksek skorlari kaydediyor */
    outp = fopen("highscores.db", "wb");
    fwrite(highscores, sizeof(int), 5, outp);
    fclose(outp);

    unload_datafile(dat);
    return 0;
}
END_OF_MAIN();

/*
*   Oyun menusunu olusturan fonksiyon
*/
void menu()
{
    int ok=0, press_down=0, press_up=0;

    while(1)
    {
        clear_bitmap(second_buffer);
        /* Menu secenekleri */
        textout_centre_ex(second_buffer, dat[F_MENU].dat, "PLAY GAME", 400, 200, BLUE, -1);
        textout_centre_ex(second_buffer, dat[F_MENU].dat, "HIGH SCORES", 400, 250, BLUE, -1);
        textout_centre_ex(second_buffer, dat[F_MENU].dat, "INSTRUCTIONS", 400, 300, BLUE, -1);
        textout_centre_ex(second_buffer, dat[F_MENU].dat, "EXIT", 400, 350, BLUE, -1);

        /* Yukari assagi hareket */
        if(!(key[KEY_DOWN]))    press_down=0;
        if(!(key[KEY_UP]))      press_up=0;
        if(ok < 3 && press_down == 0 && key[KEY_DOWN]) { ++ok; press_down=1; }
        if(ok > 0 && press_up == 0 && key[KEY_UP])     { --ok; press_up=1;   }

        /* Aktif secenegin ayarlandigi kisim */
        switch(ok)
        {
            case 0:
            textout_centre_ex(second_buffer, dat[F_MENU].dat, "PLAY GAME", 400, 200, FIRE, -1);
            break;
            case 1:
            textout_centre_ex(second_buffer, dat[F_MENU].dat, "HIGH SCORES", 400, 250, FIRE, -1);
            break;
            case 2:
            textout_centre_ex(second_buffer, dat[F_MENU].dat, "INSTRUCTIONS", 400, 300, FIRE, -1);
            break;
            case 3:
            textout_centre_ex(second_buffer, dat[F_MENU].dat, "EXIT", 400, 350, FIRE, -1);
            break;
        }

        /* Secme islemi */
        if(key[KEY_ENTER])
        {
            if(ok == 0)
            {
                reset_speed_counter();
                stop_sample(dat[S_GAMEOVER].dat);
                play_game();
            }
            else if(ok == 1)    print_highscores();
            else if(ok == 2)    print_instructions();
            else if(ok == 3)    break;
        }

        blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
    }
}

/*
*   Oyunu oynamayi saglayan ana fonksiyon
*/
void play_game()
{
    int i;

    while(1)
    {
        /* Tum ekrana basim islemlerini gerceklestiriyor */
        print_screen();

        /* Level baslangicini ayarliyor */
        if(create) set_level(0);
        /* Oyun bittigi takdirde tebrik mesaji veriyor */
        if(level == 15)
        {
            congrats();
            break;
        }

        /* Oyuncunun dogma islemini gerceklestiriyor */
        player_born();

        /* Oyunun sonlanma durumunu gerceklestiriyor */
        if( game_over() )   break;

        /* Kullanici kontrolleri */
        if( control() )     break;

        /* Kullanici ve Canavar Ateslerinin Hareketleri */
        while(speed_fire > 0)
        {
            if(p_fire.fire)
                fire_move(&p_fire);
            for(i=0 ; i < MAX_ENEMY ; ++i)
                if(enemies[i].e_fire.fire)
                    fire_move( &(enemies[i].e_fire) );
            --speed_fire;
        }

        /* Canavarlar */
        enemy_main();

        /* Radar */
        if(frame % 10 == 0) radar();

        ++frame;
        if(frame>2000) frame = 0;
    }
}

/*
*   Yuksek skorlari ekrana basan fonksiyon
*/
void print_highscores()
{
    int i, j;

    clear_keybuf();
    clear_bitmap(second_buffer);
    textout_centre_ex(second_buffer, dat[F_MENU].dat, "HIGH SCORES", 400, 100, BLUE, -1);

    for(i=0, j=200; i < 5 ; ++i, j+=50)
    {
        if(score == highscores[i])
            textprintf_centre_ex(second_buffer, dat[F_SCORE].dat, 400, j, FIRE, -1, "%d . %d", i+1, highscores[i]);
        else
            textprintf_centre_ex(second_buffer, dat[F_SCORE].dat, 400, j, BLUE, -1, "%d . %d", i+1, highscores[i]);
    }


    blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
    while(1) if(keypressed()) break;
}

/*
*  Oyun bilgilerini ekrana basan fonksiyon
*/
void print_instructions()
{
    clear_keybuf();
    clear_bitmap(second_buffer);
    textout_centre_ex(second_buffer, dat[F_MENU].dat, "POINTS", 400, 50, BLUE, -1);
    blit(dat[POINTS].dat, second_buffer, 0,0,190,100,400,189);

    textout_centre_ex(second_buffer, dat[F_MENU].dat, "CONTROLS", 400, 320, BLUE, -1);
    textout_centre_ex(second_buffer, dat[F_LEVEL].dat, "Arrow Keys to Walk", 400, 370, YELLOW, -1);
    textout_centre_ex(second_buffer, dat[F_LEVEL].dat, "Press Space to Fire", 400, 390, YELLOW, -1);

    blit(second_buffer, screen, 0, 0, 0, 0, 800, 600);
    while(1) if(keypressed()) break;
}
