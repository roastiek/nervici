#include "button.h"

Button::Button (Control* par, int x, int y, int w, int h, const ustring& text, const ustring& name) :
Control (par, x, y, w, h, name) {

    set_text (text);
}
