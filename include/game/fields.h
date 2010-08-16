/* 
 * File:   fields.h
 * Author: bobo
 *
 * Created on 22. červenec 2010, 14:13
 */

#ifndef FIELDS_H
#define	FIELDS_H

#include "int_type.h"
#include "fpoint.h"

//typedef plpart_tu Fields[3][3];

/*struct FieldsRow {
private:
    plpart_tu row[3];
public:
    template <class R>
    inline plpart_tu& operator[] (R index) {
        return row[index];
    }

    template <class R>
    inline const plpart_tu& operator[] (R index) const {
        return row[index];
    }
};*/

struct Fields {
private:
    plpart_tu data[3][3];
public:
    template <class T>
    inline plpart_tu* operator[] (T index) {
        return data[index];
    }

    template <class T>
    inline const plpart_tu* operator[] (T index) const {
        return data[index];
    }

    void calc (const FPoint& pos);

};


#endif	/* FIELDS_H */

