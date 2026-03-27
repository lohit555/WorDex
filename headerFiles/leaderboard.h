/* leaderboard.h
 *
 * Header file for the Wordex leaderboard module.
 * Defines the LeaderboardEntry struct and declares the functions used for
 * building and displaying the leaderboard based on player's game history.
 *
 * AUTHOR:  Lohitashwa Madhan
 * DATE:    2nd December, 2025
 */

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

/* Maximum number of distinct players that can appear
 * on the leaderboard at once.
 */
#define MAX_USERS 100       

/* LeaderboardEntry:
 * Stores aggregate statistics for a single player.
*/

struct LeaderboardEntry {
    char username[NAME_MAX_LEN];  
    int gamesPlayed;              
    int wins;                    
    int losses;                   
    int totalGuesses;             
    double avgGuesses;            
    double winRate;              
};

/* GetLeaderboard:
 * Builds the leaderboard from history.txt, sorts it and then prints it.
 */
void GetLeaderboard(void);

#endif 