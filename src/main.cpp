#include <cmath>
#include <clocale>
#include <libintl.h>
#include <glibmm/init.h>
#include <giomm/init.h>
#include <glibmm/thread.h>
#include <SDL.h>
#include <iostream>

#include "app.h"
#include "basic_defs.h"

#include "main.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.141f
#endif

float icos[ANGLES];
float isin[ANGLES];

//proc neni cisarsky rez jako zakusek

static void calc_angles () {
    for (int a = 0; a < ANGLES; a++) {
        icos[a] = floor (cos (M_PI * 2 * a / ANGLES) * DIGITS) / DIGITS;
        isin[a] = floor (sin (M_PI * 2 * a / ANGLES) * DIGITS) / DIGITS;
    }
}

int main (int argc, char *argv[]) {
    setlocale (LC_ALL, "");

    Glib::init ();
    Glib::thread_init ();
    Gio::init ();
    if (SDL_Init (0))
        return 1;

    calc_angles ();

    textdomain ("nervici");
    cout << _("cosi") << '\n';

    app.initialize ();
    app.run ();
    app.uninitialize ();

    SDL_Quit ();

    return 0;
}
