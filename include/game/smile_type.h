/* 
 * File:   smyle_types.h
 * Author: deedrah
 *
 * Created on 23. červenec 2010, 1:49
 */

#ifndef SMILE_TYPES_H
#define	SMILE_TYPES_H

/*
 * Types of smiles
 */
enum SmileType {
    /*
     * A pozitiv smile.
     */
    ST_pozi,
    
    /*
     * A negativ smile.
     */
    ST_nega, 
    
    /*
     * A flegmatic smile.
     */
    ST_fleg, 
    
    /*
     * A ironic smile.
     */
    ST_iron, 
    
    /*
     * A chamelon smile.
     */
    ST_cham, 
    
    /*
     * A moving smile.
     */
    ST_ham, 
    
    /*
     * Count of smile types.
     */
    ST_count
};

#endif	/* SMYLE_TYPES_H */

