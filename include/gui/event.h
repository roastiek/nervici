/* 
 * File:   listener.h
 * Author: bobo
 *
 * Created on 13. červenec 2010, 15:55
 */

#ifndef LISTENER_H
#define	LISTENER_H

//#include <SDL_events.h>

class Listener;

template <typename C>
struct Event0 {
private:
    Listener* listener;
    void (Listener::*callback)(C);
public:

    Event0 () :
    listener (NULL),
    callback (NULL) {
    }

    template <class T>
    Event0 (T* list, void (T::*call) (C)) {
        listener = reinterpret_cast<Listener*> (list);
        callback = reinterpret_cast<void (Listener::*) (C)> (call);
    }

    void operator() (C ctl) {
        if (listener != NULL && callback != NULL) {
            return (listener->*callback) (ctl);
        }
    }
};

template <typename C, typename P>
struct Event1 {
private:
    Listener* listener;
    void (Listener::*callback)(C, P);
public:

    Event1 () :
    listener (NULL),
    callback (NULL) {
    }

    template <class T>
    Event1 (T* list, void (T::*call) (C, P)) {
        listener = reinterpret_cast<Listener*> (list);
        callback = reinterpret_cast<void (Listener::*) (C,P)> (call);
    }

    void operator() (C ctl, P p1) {
        if (listener != NULL && callback != NULL) {
            (listener->*callback) (ctl, p1);
        }
    }
};

/*struct OnKeyPressed {
private:
    Listener* listener;
    bool (Listener::*callback)(ControlPointer&, SDL_KeyboardEvent);
public:

    OnKeyPressed () :
    listener (NULL),
    callback (NULL) {
    }

    template <class T>
    OnKeyPressed (T* list, void (T::*call) (ControlPointer&, SDL_KeyboardEvent)) {
        listener = reinterpret_cast<Listener*> (list);
        callback = reinterpret_cast<void (Listener::*) (ControlPointer&)> (call);
    }

    bool operator() (ControlPointer& ctl, SDL_KeyboardEvent p1) {
        if (listener != NULL && callback != NULL) {
            return (listener->*callback) (ctl, p1);
        }
        return false;
    }
};*/

/*typedef Event0 OnClicked;
typedef Event1<SDL_MouseButtonEvent> OnMouseButton;
typedef Event1<SDL_MouseMotionEvent> OnMouseMove;
typedef Event0 OnFocusGained;
typedef Event0 OnFocusLost;
typedef Event0 OnMouseEnter;
typedef Event0 OnMouseLeave;*/
/*typedef Event1<int> OnValueChanged;
typedef Event1<int> OnXChanged;
typedef Event1<int> OnYChanged;
typedef Event1<int> OnWidthChanged;
typedef Event1<int> OnHeightChanged;
typedef Event1<int> OnXOffsetChanged;
typedef Event1<int> OnYOffsetChanged;*/
//typedef



#endif	/* LISTENER_H */

