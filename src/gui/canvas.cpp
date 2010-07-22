#include <SDL.h>
#include <SDL_Pango.h>
#include <SDL_gfxPrimitives.h>

#include "utils.h"
#include "gui/implementor.h"

#include "gui/canvas.h"

SDL_Surface *make_surface (int width, int height) {
    return SDL_CreateRGBSurface (SDL_SWSURFACE, width, height, 32, 0xff, 0xff00, 0xff0000, 0x00000000);
}

Canvas::Canvas () :
width (0),
height (0) {
    impl = new Implementor ();
}

Canvas::~Canvas () {
    delete impl;
}

void Canvas::set_width (int value) {
    if (width != value) {
        if (impl->surface != NULL) {
            SDL_FreeSurface (impl->surface);
            impl->surface = NULL;
        }

        width = value;

        if (width > 0 && height > 0) {
            impl->surface = make_surface (width, height);
        }
    }
}

void Canvas::set_height (int value) {
    if (height != value) {
        if (impl->surface != NULL) {
            SDL_FreeSurface (impl->surface);
            impl->surface = NULL;
        }

        height = value;

        if (width > 0 && height > 0) {
            impl->surface = make_surface (width, height);
        }
    }
}

void Canvas::set_font (const ustring& value) {
    SDLPango_FreeContext (impl->pango_context);
    font.name = value;
    ustring desc = value + " " + to_string<int>(font.size);
    impl->pango_context = SDLPango_CreateContext_GivenFontDesc (desc.c_str ());
    SDLPango_SetDefaultColor (impl->pango_context, &impl->font_color);
}

void Canvas::set_font_color (Uint32 value) {
    font.color = value;
    impl->font_color.m[0][1] = 0xff & (value >> 24);
    impl->font_color.m[1][1] = 0xff & (value >> 16);
    impl->font_color.m[2][1] = 0xff & (value >> 8);
    impl->font_color.m[3][1] = 0xff;
    impl->font_color.m[0][0] = 0xff & (value >> 24);
    impl->font_color.m[1][0] = 0xff & (value >> 16);
    impl->font_color.m[2][0] = 0xff & (value >> 8);
    impl->font_color.m[3][0] = 0x00;
    SDLPango_SetDefaultColor (impl->pango_context, &impl->font_color);
}

void Canvas::set_font_size (int value) {
    SDLPango_FreeContext (impl->pango_context);
    font.size = value;
    ustring desc = font.name + " " + to_string<int>(value);
    impl->pango_context = SDLPango_CreateContext_GivenFontDesc (desc.c_str ());
    SDLPango_SetDefaultColor (impl->pango_context, &impl->font_color);
}

int Canvas::get_width () const {
    return width;
}

int Canvas::get_height () const {
    return height;
}

void Canvas::draw_point (int x, int y, Uint32 color) {
    pixelColor (impl->surface, x, y, color);
}

void Canvas::draw_hline (int x, int y, int w, Uint32 color) {
    hlineColor (impl->surface, x, x + w - 1, y - 1, color);
}

void Canvas::draw_vline (int x, int y, int h, Uint32 color) {
    vlineColor (impl->surface, x, y - 1, y + h - 1, color);
}

void Canvas::draw_rectangle (int x, int y, int w, int h, Uint32 color) {
    rectangleColor (impl->surface, x, y, x + w - 1, y + h - 1, color);
}

void Canvas::draw_box (int x, int y, int w, int h, Uint32 color) {
    boxColor (impl->surface, x, y, x + w - 1, y + h - 1, color);
}

void Canvas::draw_line (int x1, int y1, int x2, int y2, Uint32 color) {
    lineColor (impl->surface, x1, y1, x2, y2, color);
}

void Canvas::draw_aaline (int x1, int y1, int x2, int y2, Uint32 color) {
    aalineColor (impl->surface, x1, y1, x2, y2, color);
}

void Canvas::draw_circle (int x, int y, int r, Uint32 color) {
    circleColor (impl->surface, x, y, r, color);
}

void Canvas::draw_arc (int x, int y, int r, int start, int end, Uint32 color) {
    arcColor (impl->surface, x, y, r, start, end, color);
}

void Canvas::draw_filled_circle (int x, int y, int r, Uint32 color) {
    filledCircleColor (impl->surface, x, y, r, color);
}

void Canvas::draw_aacircle (int x, int y, int r, Uint32 color) {
    circleColor (impl->surface, x, y, r, color);
}

void Canvas::draw_trigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    trigonColor (impl->surface, x1, y1, x2, y2, x3, y3, color);
}

void Canvas::draw_filled_trigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    filledTrigonColor (impl->surface, x1, y1, x2, y2, x3, y3, color);
}

void Canvas::draw_aatrigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    aatrigonColor (impl->surface, x1, y1, x2, y2, x3, y3, color);
}

void Canvas::draw_image (int x, int y, Canvas* image) {
    SDL_Rect dest_area;
    dest_area.x = x;
    dest_area.y = y;

    SDL_BlitSurface (image->impl->surface, NULL, impl->surface, &dest_area);
}

void Canvas::draw_image (int x, int y, Canvas* image,
        int src_x, int src_y, int src_w, int src_h) {
    SDL_Rect src_area;
    src_area.x = src_x;
    src_area.y = src_y;
    src_area.w = src_w;
    src_area.h = src_h;

    SDL_Rect dest_area;
    dest_area.x = x;
    dest_area.y = y;

    SDL_BlitSurface (image->impl->surface, &src_area, impl->surface, &dest_area);
}

void Canvas::draw_text (int x, int y, int w, int h, HorizontalAling ha, VerticalAling va,
        const ustring& text) {
    SDL_Rect src;
    SDL_Rect dest;

    if (text == "") return;

    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (impl->pango_context);

    src.w = (face->w < w) ? face->w : w;
    src.h = (face->h < h) ? face->h : h;

    switch (ha) {
    case HA_left:
        src.x = 0;
        dest.x = x;
        break;
    case HA_center:
        src.x = (face->w - src.w) / 2;
        dest.x = x + (w - src.w) / 2;
        break;
    case HA_right:
        src.x = face->w - src.w;
        dest.x = x + w - src.w;
        break;
    }

    switch (va) {
    case VA_top:
        src.y = 0;
        dest.y = y;
        break;
    case VA_center:
        src.y = (face->h - src.h) / 2;
        dest.y = y + (h - src.h) / 2;
        break;
    case VA_bottom:
        src.y = face->h - src.h;
        dest.y = y + h - src.h;
        break;
    }

    SDL_BlitSurface (face, &src, impl->surface, &dest);

    SDL_FreeSurface (face);
}

void Canvas::draw_text (int x, int y, int w, int h, int x_shift, VerticalAling va, const ustring& text) {
    SDL_Rect src;
    SDL_Rect dest;

    if (text == "") return;

    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (impl->pango_context);

    src.w = (face->w - x_shift < w) ? face->w - x_shift : w;
    src.h = (face->h < h) ? face->h : h;

    if (x_shift >= 0) {
        src.x = x_shift;
        dest.x = x;
    } else {
        src.x = 0;
        dest.x = -x_shift;
    }

    switch (va) {
    case VA_top:
        src.y = 0;
        dest.y = y;
        break;
    case VA_center:
        src.y = (face->h - src.h) / 2;
        dest.y = y + (h - src.h) / 2;
        break;
    case VA_bottom:
        src.y = face->h - src.h;
        dest.y = y + h - src.h;
        break;
    }

    SDL_BlitSurface (face, &src, impl->surface, &dest);

    SDL_FreeSurface (face);
}

void Canvas::draw_wrapped_text (int x, int y, int w, int h, const ustring& text) {
    SDL_Rect src;
    SDL_Rect dest;

    if (text == "") return;

    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    SDLPango_SetMinimumSize (impl->pango_context, w, h);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (impl->pango_context);

    src.w = (face->w < w) ? face->w : w;
    src.h = (face->h < h) ? face->h : h;

    src.x = 0;
    dest.x = x;

    src.y = 0;
    dest.y = y;

    SDL_BlitSurface (face, &src, impl->surface, &dest);

    SDL_FreeSurface (face);
}

int Canvas::get_text_width (const ustring& text) {
    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    return SDLPango_GetLayoutWidth (impl->pango_context);
}

