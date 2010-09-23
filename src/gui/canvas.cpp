#include "gui/sdl_canvas.h"

#include "gui/canvas.h"

Clip::Clip (int x, int y, int w, int h, int rx, int ry) :
    off_x (x), off_y (y), width (w), height (h), rel_x (rx), rel_y (ry) {

}

Clip::~Clip () {
}

Canvas::Canvas () :
    width (0), height (0) {

}

Canvas::~Canvas () {
}

Canvas* Canvas::create_canvas () {
    return new SDLCanvas ();
}
