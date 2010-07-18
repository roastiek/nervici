/* 
 * File:   Float.h
 * Author: bobo
 *
 * Created on 17. červenec 2010, 21:16
 */

#ifndef FLOAT_H
#define	FLOAT_H

#include "int_type.h"

#define PRECISION 100

struct Float {
private:
    uint32_t value;
public:
    inline Float (int v):value (v * PRECISION) {
    }
    inline Float (float v): value (v * PRECISION) {
    }

    inline Float& operator = (int val) {
        value = val * PRECISION;
        return *this;
    }

    inline Float& operator = (float val) {
        value = val * PRECISION;
        return *this;
    }

    inline Float& operator + (const Float& val) {
        value+= val.value;
        return *this;
    }

    inline Float& operator + (int val) {
        value+= val * PRECISION;
        return *this;
    }

    inline Float& operator + (float val) {
        value+= val * PRECISION;
        return *this;
    }

    inline Float& operator - (const Float& val) {
        value-= val.value;
        return *this;
    }

    inline Float& operator - (int val) {
        value-= val * PRECISION;
        return *this;
    }

    inline Float& operator - (float val) {
        value-= val * PRECISION;
        return *this;
    }
};

#endif	/* FLOAT_H */

