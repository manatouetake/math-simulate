#include <stdio.h>
#include <pthread.h>
#include "winlib_db.h"
#include "kbhit.h"
#include <time.h>
#include "kadai2.h"
int bar_x = 100;    //初期座標
int bar_high = 10;  //バーの高さ(y軸)
int bar_width = 30; //バーの長さ(x軸)

int ball_x[10];
int ball_y[10]; //要素数=ballの数

int red[10];
int green[10];
int blue[10];

int score = 0;
char score_str[100];

int add_time = 0;
int add_size = 0;

int start_time, left_time = 1;           //time関連
int longbar_time = 5, getred_time = 0;   //red取得time関連
int bigball_time = 5, getgreen_time = 0; //green取得time関連
int bluetext_time = 5, getblue_time = 0;
int bar_x_position1 = 0, bar_x_position2 = 0, bar_x_position3 = 0;

int finish_game = 0;

int GetRandom(int min, int max) //min～maxの間の変数
{
    static int flag;
    if (flag == 0)
    {
        srand((unsigned int)time(NULL));
        flag = 1;
    }
    return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void draw() //描画する関数
{
    int i;
    g_clear();
    text(10, 10, score_str);
    text(30, 350, "B:Back to Home");
    g_fill(bar_x, 250, bar_width, bar_high);
    for (i = 0; i < 10; i++)
    {
        g_rgb(red[i], green[i], blue[i]);
        g_fill(ball_x[i], ball_y[i], 10 + add_size, 10 + add_size); //ball描画
    }
    g_rgb(0, 0, 0);
}

void *func(void *arg) //ball処理&取得判定関連スレッド
{
    while (left_time > 0) // (30+add_time)second実行
    {
        int i;
        for (i = 0; i < 10; i++)
        {

            ball_y[i] += 15;
            if ((bar_x <= ball_x[i] + 10 + add_size && ball_x[i] <= bar_x + bar_width) &&
                (250 <= ball_y[i] + 10 + add_size && ball_y[i] <= 250 + bar_high)) //取得判定
            {
                score++;
                ball_y[i] = 0;
                ball_x[i] = GetRandom(0, 450);
                if (red[i] == 65535 && bar_width < 80)
                {
                    bar_width += 50;
                    getred_time = time(NULL); //取得ballが赤ならその時間を測定
                    bar_x_position1 = bar_x;
                }
                if (green[i] == 65535 && add_size < 30)
                {
                    add_size += 30;
                    getgreen_time = time(NULL); ////取得ballが緑ならその時間を測定
                    bar_x_position2 = bar_x;
                }
                if (blue[i] == 65535)
                {
                    add_time += 5;
                    getblue_time = time(NULL);
                    bar_x_position3 = bar_x;
                }
            }

            if (ball_y[i] >= 300) //とれずにballが落ちたとき
            {
                ball_y[i] = 0;
                ball_x[i] = GetRandom(0, 450);
            }

            if (ball_y[i] == 0) //ballが降り始める瞬間
            {
                int color = GetRandom(0, 9);
                switch (color)
                {
                case 0: //red
                case 1:
                    red[i] = 65535;
                    green[i] = 0;
                    blue[i] = 0;
                    break;

                case 2: //green
                case 3:
                    red[i] = 0;
                    green[i] = 65535;
                    blue[i] = 0;
                    break;

                case 4: //blue
                case 5:
                    red[i] = 0;
                    green[i] = 0;
                    blue[i] = 65535;
                    break;

                default: //black
                    red[i] = 0;
                    green[i] = 0;
                    blue[i] = 0;
                    break;
                }
            }
            if (longbar_time == time(NULL) - getred_time && bar_width > 30) //赤を取って5秒経過&barが初期長さよりも長いとき
            {
                bar_width = 30;
            }
            if (bigball_time == time(NULL) - getgreen_time && add_size >= 0) //緑を取って5秒経過&ballが初期大きさよりも大きいとき
            {
                add_size = 0;
            }
            left_time = 30 + add_time - difftime(time(NULL), start_time); //残り時間
            sprintf(score_str, "score %d     %d seconds left", score, left_time);
        }
        draw();
        if (longbar_time > time(NULL) - getred_time && bar_width > 30) //赤を取って5秒経過&barが初期長さよりも長いとき
        {
            text(bar_x_position1, 250 - 8 * (time(NULL) - getred_time), "Longer!! (5sec)");
        }
        if (bigball_time > time(NULL) - getgreen_time && add_size >= 0) //緑を取って5秒経過&ballが初期大きさよりも大きいとき
        {
            text(bar_x_position2, 250 - 8 * (time(NULL) - getgreen_time), "Bigger!! (5sec)");
        }
        if (bluetext_time > time(NULL) - getblue_time && add_time > 0)
        {
            text(bar_x_position3, 250 - 8 * (time(NULL) - getblue_time), "Left time +5seconds!!");
        }
        display_update();
        if (finish_game == 1)
        {
            return 0;
        }
    }
    return 0;
}

void ball_game()
{
    int i;
    srand(time(NULL));
    for (i = 0; i < 10; i++) //ball生成
    {
        ball_x[i] = GetRandom(0, 450);
        ball_y[i] = -300 + 40 * i;
        red[i] = 0;
        green[i] = 0;
        blue[i] = 0;
    }
    start_time = time(NULL);
    pthread_t pthread;
    pthread_create(&pthread, NULL, &func, NULL);
    while (left_time > 0) //do 30sec
    {
        if (kbhit())
        {
            int c;
            c = getchar();
            if (c == 'd' && bar_x + bar_width <= 500)
            {
                bar_x = bar_x + 10;
            }
            if (c == 'a' && bar_x >= 0)
            {
                bar_x = bar_x - 10;
            }
            if (c == 'b')
            {
                finish_game = 1;
                //初期化
                pthread_join(pthread, NULL); //ballスレッドの終了
                finish_game = 0;
                bar_x = 100;    //初期座標
                bar_width = 30; //バーの長さ(x軸)
                score = 0;
                score_str[0] = '\0';
                add_time = 0;
                add_size = 0;
                left_time = 1; //time関連
                longbar_time = 5;
                getred_time = 0;   //red取得time関連
                getgreen_time = 0; //green取得time関連
                getblue_time = 0;
                return;
            }
        }
    }
    pthread_join(pthread, NULL); //ballスレッドの終了
    g_clear();
    char str[100];
    sprintf(str, "Your score is %d.", score);
    text(100, 150, str);
    text(100, 200, "Press Enter to Finish.");
    display_update();
    int a;
    if ((a = getchar()) == '\n')
    {
        //初期化
        bar_x = 100;    //初期座標
        bar_width = 30; //バーの長さ(x軸)
        score = 0;
        score_str[0] = '\0';
        add_time = 0;
        add_size = 0;
        left_time = 1; //time関連
        longbar_time = 5;
        getred_time = 0;   //red取得time関連
        getgreen_time = 0; //green取得time関連
        getblue_time = 0;
        return;
    }
}
