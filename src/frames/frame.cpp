#include "frame.h"
#include "engine/render.h"

/*Frame::Frame (Control* par, float x, float y, float w, float h, const ustring& name) :
Control (par,
        Render::get_width () * x,
        Render::get_height () * y,
        Render::get_width () * w,
        Render::get_height () * h,
        name) {
}*/

Frame* Frame::create_frame (_Control* par, const ControlParameters* parms) {
    Frame* result = new Frame ();
    result->init_control (par, parms);
    return result;
}