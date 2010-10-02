/* 
 * File:   stat_column.h
 * Author: deedrah
 *
 * Created on 19. srpen 2010, 11:56
 */

#ifndef STAT_COLUMN_H
#define	STAT_COLUMN_H

/*
 * Columns of statistic table.
 */
enum StatColumn {
    /*
     * Name of player.
     */
    STC_name,
    /*
     * Score of player.
     */
    STC_score,
    /*
     * Whole lenght of player.
     */
    STC_length,
    /*
     * How many times has player died.
     */
    STC_deaths,
    /*
     * How many times has player crashed into a wall.
     */
    STC_crashes,
    /*
     * How many other players has player killed.
     */
    STC_kills,
    /*
     * How many times has been player killed.
     */
    STC_killed,
    /*
     * How many suicides has player made.
     */
    STC_selfs,
    /*
     * How many times has player jumped.
     */
    STC_jumps,
    /*
     * Smiles counts.
     */
    STC_pozi,
    STC_nega,
    STC_fleg,
    STC_iron,
    STC_cham,
    STC_ham,
    /*
     * Count of columns.
     */
    STC_count
};

#endif	/* STAT_COLUMN_H */

