#include <math.h>
//#include <iostream>

#include "app.h"

#include "main.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.141f
#endif

float icos[angles];
float isin[angles];

//static GameInfo gameinfo;
//static GameSetting gameset;

//proc neni cisarsky rez jako zakusek

static void calc_angles () {
    for (int a = 0; a < angles; a++) {
        icos[a] = floor (cos(M_PI * 2  * a / angles) * digits) / digits;
        isin[a] = floor (sin(M_PI * 2  * a / angles) * digits) / digits;
    }
}

/*static void print_sdl_matrix (const SDLPango_Matrix& mat) {
    for (int c = 0; c < 4; c++) {
        for (int i = 0; i < 4; i++) {
            cout << int (mat.m[i][c]) << " ";
        }
        cout << '\n';
    }
}*/

int main (int argc, char *argv[]) {
    calc_angles ();

/*    cout << "black back\n";
    print_sdl_matrix (*MATRIX_BLACK_BACK);
    cout << "white back\n";
    print_sdl_matrix (*MATRIX_WHITE_BACK);
    cout << "trans black letter\n";
    print_sdl_matrix (*MATRIX_TRANSPARENT_BACK_BLACK_LETTER);
    cout << "trans white letter\n";
    print_sdl_matrix (*MATRIX_TRANSPARENT_BACK_WHITE_LETTER);
    cout << "trans tran letter\n";
    print_sdl_matrix (*MATRIX_TRANSPARENT_BACK_TRANSPARENT_LETTER);*/


    App::initialize ();
    App::run ();
    App::uninitialize ();
    
    return 0;
}
