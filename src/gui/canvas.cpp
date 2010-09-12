#include "gui/sdlcanvas.h"

#include "gui/canvas.h"

Canvas::Canvas () :
    width (0), height (0) {

}

Canvas::~Canvas () {
}

Canvas* Canvas::create_canvas () {
    return new SDLCanvas ();
}
