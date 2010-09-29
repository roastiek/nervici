/* 
 * File:   listener.h
 * Author: bobo
 *
 * Created on 13. červenec 2010, 15:55
 */

#ifndef LISTENER_H
#define	LISTENER_H

class Listener;

template<typename C>
struct Event0 {
private:
    Listener* listener;
    void (Listener::*callback) (C);
public:

    Event0 () :
        listener (NULL), callback (NULL) {
    }

    template<class T>
    Event0 (T* list, void(T::*call) (C)) :
        listener (reinterpret_cast<Listener*> (list)),
                callback (reinterpret_cast<void(Listener::*) (C)> (call)) {
    }

    void operator() (C ctl) {
        if (listener != NULL && callback != NULL) {
            return (listener->*callback) (ctl);
        }
    }

    bool operator == (const Event0<C> other) const {
        return listener == other.listener && callback == other.callback;
    }
};

template<typename C, typename P>
struct Event1 {
private:
    Listener* listener;
    void (Listener::*callback) (C, P);
public:

    Event1 () :
        listener (NULL), callback (NULL) {
    }

    template<class T>
    Event1 (T* list, void(T::*call) (C, P)) :
        listener (reinterpret_cast<Listener*> (list)),
                callback (reinterpret_cast<void(Listener::*) (C, P)> (call)) {
    }

    void operator() (C ctl, P p1) {
        if (listener != NULL && callback != NULL) {
            (listener->*callback) (ctl, p1);
        }
    }

    bool operator == (const Event1<C, P>& other) const {
        return listener == other.listener && callback == other.callback;
    }

};

template<typename C, typename P>
struct BoolEvent1 {
private:
    Listener* listener;
    bool (Listener::*callback) (C, P);
public:

    BoolEvent1 () :
        listener (NULL), callback (NULL) {
    }

    template<class T>
    BoolEvent1 (T* list, void(T::*call) (C, P)) :
        listener (reinterpret_cast<Listener*> (list)),
                callback (reinterpret_cast<void(Listener::*) (C, P)> (call)) {
    }

    bool operator() (C ctl, P p1) {
        if (listener != NULL && callback != NULL) {
            return (listener->*callback) (ctl, p1);
        }
        return false;
    }

    bool operator == (const BoolEvent1<C, P>& other) const {
        return listener == other.listener && callback == other.callback;
    }
};

#endif	/* LISTENER_H */

