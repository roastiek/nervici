/* 
 * File:   int_type.h
 * Author: deedrah
 *
 * Created on 8. červenec 2010, 11:55
 * 
 * Definitions of some integer types
 */

#ifndef INT_TYPE_H
#define	INT_TYPE_H

#include <cstdint>

/*
 * Used for players identificators.
 */
typedef uint8_t plid_tu;

/*
 * Used for players bodies and measures.
 */
typedef uint16_t plsize_tu;

/*
 * Used for players scores.
 */
typedef int32_t score_ti;

/*
 * Used for timers and speed.
 */
typedef int32_t timer_ti;

/*
 * Used for starts identificators. 
 */
typedef uint8_t startid_tu;

/*
 * Used for world dimensions.
 */
typedef uint16_t wsize_tu;

/*
 * Represent mass of players bodies. Needed for smoth lines. It is used as aplha
 * channel by rendering.
 */
typedef uint8_t plpart_tu;

/*
 * Used for counting of rounds.
 */
typedef uint8_t round_tu;

/*
 * Used for smiles identificators.
 */
typedef uint16_t smileid_tu;

/*
 * Used for smiles levels.
 */
typedef uint8_t smilelvl_tu;

#endif	/* INT_TYPE_H */

