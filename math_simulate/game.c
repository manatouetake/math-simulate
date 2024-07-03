#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "winlib_db.h"
#include "kbhit.h"
#include "kadai2.h"
#include <unistd.h>

void title(void); //title screen

void home(void);      //home(selection) screen
int kbmove_home(int); //home画面での"->"の動き
int home_n = 0;       //-> position

void simulator(void);                    //simulator画面
void dot_move(int *x, int *y);           //dot moving rule
void draw_simulator(int, int, int, int); //simulator画面の描画
void simulate_speed(int *speed);         //change処理速度
struct xy_coordinate                     //p,q の構造体
{
    int x;
    int y;
};
struct xy_coordinate p;
struct xy_coordinate q;
char black_win_text[100];
char red_win_text[100];
char probability_text_black[100];
char probability_text_red[100];
int fin = 0;
int speed = 100000;

int getrandom(int, int); //min~maxの乱数

void next(void); //enterで次の画面
void back(void); //bで前の画面

void option(void);                         //option画面
void option_draw(int, int);                //option画面の描画
int kbmove_option(int);                    //option画面での"->"の動き
int kbmove_option_dot(int *stop_n);        //option画面での"->"の動き(止まるか止まらないか)
int kbmove_option_start(int *x0, int *y0); //option画面での"->"の動き(初期座標)
int option_n = 0;
int stop = 0;
int px0 = 95, py0 = 95;   //pの初期座標
int qx0 = 195, qy0 = 195; //qの初期座標
int point_E = 0;          //交点eのフラグ

int main()
{
    initwin();
    title();
    while (1)
    {
        home();
        switch (home_n)
        {
        case 0:
            simulator();
            break;
        case 1:
            option();
            break;
        case 2:
            ball_game();
            break;
        case 3:
            exit(0);
        }
    }
    return 0;
}

void title(void) //title screen
{
    g_clear();
    text(100, 100, "2020 NU Math(Science) Q.4 Simulation");
    text(100, 120, "+Extra Game");
    int QR[37][37] = {
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1}, //1
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1}, //2
        {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1}, //3
        {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1}, //4
        {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1}, //5
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, //6
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1}, //7
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //8
        {1, 1, 1, 1, 9, 9, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1}, //9
        {0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0}, //10
        {0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0}, //11
        {0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1}, //12
        {0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1}, //13
        {1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1}, //14
        {0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0}, //15
        {1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1}, //16
        {0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0}, //17
        {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1}, //18
        {1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1}, //19
        {0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0}, //20
        {1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0}, //21
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, //22
        {0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0}, //23
        {0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0}, //24
        {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1}, //25
        {0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1}, //26
        {0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0}, //27
        {1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, //28
        {0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //29
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1}, //30
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1}, //31
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0}, //32
        {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0}, //33
        {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1}, //34
        {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0}, //35
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, //36
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, //37
    };
    int i, j;
    for (j = 0; j < 37; j++) //draw QR code
    {
        for (i = 0; i < 37; i++)
        {
            if (QR[i][j] == 1)
            {
                g_fill(300 + 4 * i, 300 + 4 * j, 4, 4);
            }
        }
    }
    text(280, 295, "The Original Question Sentence");
    text(80, 370, "Please Read the QR Code. ->");
    display_update();
    while (1) //press enterの色替え
    {
        switch (time(NULL) % 6)
        {
        case 0:
            g_rgb(60000, 0, 0); //red
            break;
        case 1:
            g_rgb(60000, 0, 60000); //purple
            break;
        case 2:
            g_rgb(10000, 0, 50000); //blue
            break;
        case 3:
            g_rgb(0, 40000, 60000); //sky blue
            break;
        case 4:
            g_rgb(0, 60000, 0); //green
            break;
        case 5:
            g_rgb(60000, 40000, 0); //yellow
            break;
        }
        text(100, 180, "Press Enter");
        display_update();
        if (kbhit())
        {
            break;
        }
    }
    next();
}

void home(void) //home(selection) screen
{
    int n = 0;
    while (1)
    {
        g_clear();
        text(20, 50, "2020 NU Math(Science) Q.4 Simulation");
        text(50, 100, "Simulate!");
        text(50, 120, "Option");
        text(50, 140, "Ball Getting Game");
        text(50, 160, "Finish");
        text(20, 100 + 20 * n, "->");
        text(100, 300, "B: Back to Title");
        display_update();
        n = kbmove_home(n); //move "->" up or down
        if (n == 4)
        {
            break;
        }
        home_n = n;
    }
    return;
}

int kbmove_home(int n) //(home) move "->" up or down
{

    if (kbhit())
    {
        int c = 0;
        c = getchar();
        if (c == 'w') //上に動かす
        {
            switch (n)
            {
            case 0:
                n = 3;
                break;
            case 1:
                n = 0;
                break;
            case 2:
                n = 1;
                break;
            case 3:
                n = 2;
                break;
            }
        }
        if (c == 's') //下に動かす
        {
            switch (n)
            {
            case 0:
                n = 1;
                break;
            case 1:
                n = 2;
                break;
            case 2:
                n = 3;
                break;
            case 3:
                n = 0;
                break;
            }
        }
        if (c == '\n') //次に進む
        {
            return 4;
        }
        if (c == 'b') //title画面に戻る
        {
            title();
            return 0;
        }
    }
    return n;
}

void simulator(void) //シミュレーション page
{
    p.x = px0, p.y = py0;
    q.x = qx0, q.y = qy0;
    int black_win_n = 0;
    int red_win_n = 0;
    sprintf(black_win_text, "Black Won:%d", black_win_n);
    sprintf(red_win_text, "Red Won  :%d", red_win_n);
    speed = 250000;
    fin = 0;
    while (1)
    {
        g_clear();
        draw_simulator(p.x, p.y, q.x, q.y);
        while (1)
        {
            dot_move(&p.x, &p.y);
            simulate_speed(&speed);
            usleep(speed);
            g_clear();
            draw_simulator(p.x, p.y, q.x, q.y);
            if (p.x == q.x && p.y == q.y)
            {
                black_win_n++;
                sprintf(black_win_text, "Black Won:%d", black_win_n);
                sprintf(probability_text_black, "Probability of Black Win:%f%%", (float)100 * black_win_n / (black_win_n + red_win_n));
                sprintf(probability_text_red, "Probability of Red Win  :%f%%", (float)100 * red_win_n / (black_win_n + red_win_n));
                g_clear();
                text(100, 250, "Black Win!!");
                draw_simulator(p.x, p.y, q.x, q.y);
                p.x = px0, p.y = py0;
                q.x = qx0, q.y = qy0;
                usleep(4 * speed);
                break;
            }
            dot_move(&q.x, &q.y);
            simulate_speed(&speed);
            usleep(speed);
            g_clear();
            draw_simulator(p.x, p.y, q.x, q.y);
            if (p.x == q.x && p.y == q.y)
            {
                red_win_n++;
                sprintf(red_win_text, "Red Won  :%d", red_win_n);
                sprintf(probability_text_black, "Probability of Black Win:%f%%", (float)100 * black_win_n / (black_win_n + red_win_n));
                sprintf(probability_text_red, "Probability of Red Win  :%f%%", (float)100 * red_win_n / (black_win_n + red_win_n));
                g_clear();
                text(100, 250, "Red Win!!");
                draw_simulator(p.x, p.y, q.x, q.y);
                p.x = px0, p.y = py0;
                q.x = qx0, q.y = qy0;
                usleep(4 * speed);
                break;
            }
        }
        if (kbhit())
        {
            int c = 0;
            c = getchar();
            if (c == 'f' || fin == 1)
            {
                text(85, 380, "B: Back to Home");
                display_update();
                back();
                return;
            }
            if (c == 'n') //処理速度標準化
            {
                speed = 250000;
            }
            if (c == 'q') //処理速度高速化
            {
                speed = 0;
            }
        }
    }
    return;
}

void draw_simulator(int px, int py, int qx, int qy) //シミュレーション画面の描画
{
    text(10, 10, "2020 NU Math(Science) Q.4 Simulation");
    int n = time(NULL) % 4;
    switch (n) //nowの後ろの.が毎秒変わり、動作中を示す
    {
    case 0:
        text(10, 30, "Simulating now");
        break;
    case 1:
        text(10, 30, "Simulating now.");
        break;
    case 2:
        text(10, 30, "Simulating now..");
        break;
    case 3:
        text(10, 30, "Simulating now...");
        break;
    }
    text(85, 85, "A");
    text(85, 205, "B");
    text(205, 205, "C");
    text(205, 85, "D");
    text(85, 300, "Q: Speed MAX!");
    text(85, 320, "N: Speed Normal");
    text(85, 360, "F: Finish Simulating");
    text(250, 100, black_win_text);
    text(250, 120, red_win_text);
    text(250, 140, probability_text_black);
    text(250, 160, probability_text_red);
    g_box(95, 95, 100, 100);
    if (point_E == 1)
    {
        g_line(95, 95, 195, 195);
        g_line(95, 195, 195, 95);
        text(144, 139, "E");
    }
    g_fill(px - 5, py - 5, 10, 10);
    g_rgb(65535, 0, 0);
    g_fill(qx - 5, qy - 5, 10, 10);
    g_rgb(0, 0, 0);
    display_update();
    return;
}

void dot_move(int *x, int *y) //dotP&Q move rules
{
    int n = getrandom(0 + stop, 2 + point_E);
    if (*x == 95 && *y == 95) //A
    {
        switch (n)
        {
        case 0:
            break; //don't move
        case 1:
            *x = 195; //D
            break;
        case 2:
            *y = 195; //B
            break;
        case 3:
            *x = 145;
            *y = 145; //E
            break;
        }
        return;
    }
    if (*x == 195 && *y == 95) //D
    {
        switch (n)
        {
        case 0:
            break; //dont move
        case 1:
            *x = 95; //A
            break;
        case 2:
            *y = 195; //C
            break;
        case 3:
            *x = 145;
            *y = 145; //E
            break;
        }
        return;
    }
    if (*x == 95 && *y == 195) //B
    {
        switch (n)
        {
        case 0:
            break; //dont move
        case 1:
            *x = 195; //C
            break;
        case 2:
            *y = 95; //A
            break;
        case 3:
            *x = 145;
            *y = 145; //E
            break;
        }
        return;
    }
    if (*x == 195 && *y == 195) //C
    {
        switch (n)
        {
        case 0:
            break; //dont move
        case 1:
            *x = 95; //C
            break;
        case 2:
            *y = 95; //D
            break;
        case 3:
            *x = 145;
            *y = 145; //E
            break;
        }
        return;
    }
    if (*x == 145 && *y == 145) //E
    {
        int n = getrandom(0 + stop, 4);
        switch (n)
        {
        case 0:
            break; //dont move
        case 1:
            *x = 95;
            *y = 95; //A
            break;
        case 2:
            *x = 95;
            *y = 195; //B
            break;
        case 3:
            *x = 195;
            *y = 195; //C
            break;
        case 4:
            *x = 195;
            *y = 95; //D
            break;
        }
        return;
    }
}

void simulate_speed(int *speed) //処理速度変更
{
    if (kbhit())
    {
        int c = 0;
        c = getchar();
        if (c == 'f')
        {
            fin = 1;
        }
        if (c == 'n')
        {
            *speed = 250000;
        }
        if (c == 'q')
        {
            *speed = 0;
        }
    }
    return;
}

void option(void) //option page
{
    int n = 0;
    while (1)
    {
        option_draw(n, 100);
        n = kbmove_option(n); //move "->" up or down
        if (n == 4)
        {
            n = option_n;
            switch (option_n)
            {
            case 0:
                while (1)
                {
                    int a = 0;
                    option_draw(n, option_n);
                    a = kbmove_option_dot(&stop);
                    if (a == 1)
                    {
                        break;
                    }
                }
                break;
            case 1:
                while (1)
                {
                    int a = 0;
                    option_draw(n, option_n);
                    a = kbmove_option_start(&px0, &py0);
                    if (a == 1)
                    {
                        break;
                    }
                }
                break;
            case 2:
                while (1)
                {
                    int a = 0;
                    option_draw(n, option_n);
                    a = kbmove_option_start(&qx0, &qy0);
                    if (a == 1)
                    {
                        break;
                    }
                }
            case 3:
                while (1)
                {
                    int a = 0;
                    option_draw(n, option_n);
                    a = kbmove_option_dot(&point_E);
                    if (a == 1)
                    {
                        break;
                    }
                }
                break;
            }
        }
        option_n = n;
        if (n == 5)
        {
            break;
        }
    }
    home_n = 0;
    return;
}

void option_draw(int n, int option_n) //option画面の描画
{
    g_clear();
    text(20, 50, "2020 NU Math(Science) Q.4 Simulation");
    text(20, 70, "Option");
    switch (stop)
    {
    case 0:
        g_fill(237, 102, 30, 2);
        break;
    case 1:
        g_fill(297, 102, 70, 2);
        break;
    }
    if (px0 == 95 && py0 == 95) //Ap
    {
        g_fill(234, 122, 10, 2);
    }
    if (px0 == 95 && py0 == 195) //Bp
    {
        g_fill(257, 122, 10, 2);
    }
    if (px0 == 195 && py0 == 195) //Cp
    {
        g_fill(281, 122, 10, 2);
    }
    if (px0 == 195 && py0 == 95) //Dp
    {
        g_fill(305, 122, 10, 2);
    }
    if (qx0 == 95 && qy0 == 95) //Aq
    {
        g_fill(234, 142, 10, 2);
    }
    if (qx0 == 95 && qy0 == 195) //Bq
    {
        g_fill(257, 142, 10, 2);
    }
    if (qx0 == 195 && qy0 == 195) //Cq
    {
        g_fill(281, 142, 10, 2);
    }
    if (qx0 == 195 && qy0 == 95) //Dq
    {
        g_fill(305, 142, 10, 2);
    }
    switch (point_E)
    {
    case 0:
        g_fill(288, 162, 14, 2); //No
        break;
    case 1:
        g_fill(253, 162, 17, 2); //Yes
        break;
    }
    switch (option_n)
    {
    case 0:
        g_fill(46, 102, 29, 1);
        break;
    case 1:
        g_fill(46, 122, 130, 1);
        break;
    case 2:
        g_fill(46, 142, 130, 1);
        break;
    case 3:
        g_fill(46, 162, 150, 1);
        break;
    }
    text(50, 100, "Dots                           Stop       Don't Stop");
    text(50, 120, "Black First Position           A   B   C   D");
    text(50, 140, "Red   First Position           A   B   C   D");
    text(50, 160, "Diagonal Intersection'E'          Yes   No       ");
    text(20, 100 + 20 * n, "->");
    text(85, 360, "B: Back to Home");
    display_update();
}

int kbmove_option(int n) //(option) move -> up and down
{

    if (kbhit())
    {
        int c = 0;
        c = getchar();
        if (c == 'w')
        {
            switch (n)
            {
            case 0:
                n = 3;
                break;
            case 1:
                n = 0;
                break;
            case 2:
                n = 1;
                break;
            case 3:
                n = 2;
                break;
            }
        }
        if (c == 's')
        {
            switch (n)
            {
            case 0:
                n = 1;
                break;
            case 1:
                n = 2;
                break;
            case 2:
                n = 3;
                break;
            case 3:
                n = 0;
                break;
            }
        }
        if (c == '\n')
        {
            return 4;
        }
        if (c == 'b')
        {
            return 5;
        }
    }
    return n;
}

int kbmove_option_dot(int *stop_n) //stop_nの変更
{
    if (kbhit())
    {
        int c = 0;
        c = getchar();
        if (c == 'a')
        {
            switch (*stop_n)
            {
            case 0:
                *stop_n = 1;
                break;
            case 1:
                *stop_n = 0;
                break;
            }
        }
        if (c == 'd')
        {
            switch (*stop_n)
            {
            case 0:
                *stop_n = 1;
                break;
            case 1:
                *stop_n = 0;
                break;
            }
        }
        if (c == '\n')
        {
            return 1;
        }
    }
    return 0;
}

int kbmove_option_start(int *x0, int *y0)
{
    if (kbhit())
    {
        int c = 0;
        c = getchar();
        if (c == 'a')
        {
            if (*x0 == 95 && *y0 == 95) //A
            {
                *x0 = 195;
                *y0 = 95; //D
                return 0;
            }
            if (*x0 == 95 && *y0 == 195) //B
            {
                *x0 = 95;
                *y0 = 95; //A
                return 0;
            }
            if (*x0 == 195 && *y0 == 195) //C
            {
                *x0 = 95;
                *y0 = 195; //B
                return 0;
            }
            if (*x0 == 195 && *y0 == 95) //D
            {
                *x0 = 195;
                *y0 = 195; //C
                return 0;
            }
        }
        if (c == 'd')
        {
            if (*x0 == 95 && *y0 == 95) //A
            {
                *x0 = 95;
                *y0 = 195; //B
                return 0;
            }
            if (*x0 == 95 && *y0 == 195) //B
            {
                *x0 = 195;
                *y0 = 195; //C
                return 0;
            }
            if (*x0 == 195 && *y0 == 195) //C
            {
                *x0 = 195;
                *y0 = 95; //D
                return 0;
            }
            if (*x0 == 195 && *y0 == 95) //D
            {
                *x0 = 95;
                *y0 = 95; //A
                return 0;
            }
        }
        if (c == '\n')
        {
            return 1;
        }
    }
    return 0;
}

int getrandom(int min, int max) //min～maxの間の変数
{
    static int flag;
    if (flag == 0)
    {
        srand((unsigned int)time(NULL));
        flag = 1;
    }
    return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void next() //次のページに進む
{
    while (1)
    {
        if (kbhit())
        {
            int c = 0;
            c = getchar();
            if (c == '\n')
            {
                return;
            }
        }
    }
}

void back() //前のページに戻る
{
    while (1)
    {
        if (kbhit())
        {
            int c = 0;
            c = getchar();
            if (c == 'b')
            {
                return;
            }
        }
    }
}