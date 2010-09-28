/*
 * list_item.h
 *
 *  Created on: 28.9.2010
 *      Author: bobo
 */

#ifndef LIST_ITEM_H_
#define LIST_ITEM_H_

#include <glibmm/ustring.h>

struct ListItem {
public:
    Glib::ustring text;
    uint32_t color;

    ListItem (const Glib::ustring& txt = "", uint32_t cl = C_INPUT_TEXT);
};

inline ListItem::ListItem (const Glib::ustring& txt, uint32_t cl) :
    text (txt), color (cl) {
}




#endif /* LIST_ITEM_H_ */
