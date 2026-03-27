/* statistics.h
*
* Header file for Wordex statistics module.
* Defines the stats struct and declares all functions used for creating, loading, updating 
and printing user statistics.
*
* Zainab Mirza
* Date: 2025/12/03
*/

#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERNAME 50   // Max username length

/* Stats struct - stores all statistic values for each user*/
typedef struct {
    char username[MAX_USERNAME];
    int wins;
    int losses;
    int gamesPlayed; 
    int currentStreak;
    int maxStreak;
    int totalGuesses;
    long lastPlayed;      // timestamp of last game
    int winsByGuess[6];   // guess distribution for wins
} Stats;

int defaultStats(const char *username);
int loadStats(const char *username, Stats *s);
int updateStats(const char *username, int won, int guessCount, long timePlayed);
int printStats(const char *username);

#endif