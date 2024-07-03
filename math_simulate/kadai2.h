#include <stdio.h>
#include <pthread.h>
#include "winlib_db.h"
#include "kbhit.h"
#include <time.h>


void draw();                              //描画する関数
void *func(void *arg); //ball処理&取得判定関連スレッド
void ball_game();