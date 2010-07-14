/* 
 * File:   listener.h
 * Author: bobo
 *
 * Created on 13. červenec 2010, 15:55
 */

#ifndef LISTENER_H
#define	LISTENER_H

#include <SDL.h>

class Listener;

class Control;

struct Event0 {
private:
    Listener* listener;
    void (Listener::*callback)(Control*);
public:

    Event0 () :
    listener (NULL),
    callback (NULL) {
    }

    template <class T>
    Event0 (T* list, void (T::*call) (Control*)) {
        listener = reinterpret_cast<Listener*> (list);
        callback = reinterpret_cast<void (Listener::*) (Control*)> (call);
    }

    void operator() (Control* ctl) {
        if (listener != NULL && callback != NULL) {
            return (listener->*callback) (ctl);
        }
    }
};

template <typename P>
struct Event1 {
private:
    Listener* listener;
    void (Listener::*callback)(Control*, P);
public:

    Event1 () :
    listener (NULL),
    callback (NULL) {
    }

    template <class T>
    Event1 (T* list, void (T::*call) (Control*, P)) {
        listener = reinterpret_cast<Listener*> (list);
        callback = reinterpret_cast<void (Listener::*) (Control*,P)> (call);
    }

    void operator() (Control* ctl, P p1) {
        if (listener != NULL && callback != NULL) {
            (listener->*callback) (ctl, p1);
        }
    }
};

struct OnKeyPressed {
private:
    Listener* listener;
    bool (Listener::*callback)(Control*, SDL_KeyboardEvent);
public:

    OnKeyPressed () :
    listener (NULL),
    callback (NULL) {
    }

    template <class T>
    OnKeyPressed (T* list, void (T::*call) (Control*, SDL_KeyboardEvent)) {
        listener = reinterpret_cast<Listener*> (list);
        callback = reinterpret_cast<void (Listener::*) (Control*)> (call);
    }

    bool operator() (Control* ctl, SDL_KeyboardEvent p1) {
        if (listener != NULL && callback != NULL) {
            return (listener->*callback) (ctl, p1);
        }
        return false;
    }
};

typedef Event0 OnClicked;
typedef Event1<SDL_MouseButtonEvent> OnMouseButton;
typedef Event1<SDL_MouseMotionEvent> OnMouseMove;
typedef Event0 OnFocusGained;
typedef Event0 OnFocusLost;
typedef Event0 OnMouseEnter;
typedef Event0 OnMouseLeave;
typedef Event1<int> OnValueChanged;
typedef Event1<int> OnXChanged;
typedef Event1<int> OnYChanged;
typedef Event1<int> OnWidthChanged;
typedef Event1<int> OnHeightChanged;
typedef Event1<int> OnXOffsetChanged;
typedef Event1<int> OnYOffsetChanged;
//typedef



#endif	/* LISTENER_H */

