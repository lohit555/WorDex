/* history.h
 *
 * Header file for the Wordex game history module.
 * Defines the GameResult struct and declares the functions used for
 * recording completed games and printing the game history.
 *
 * AUTHOR:  Lohitashwa Madhan
 * DATE:    2nd December, 2025
 */

#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximum lengths for fields stored in the history.txt file 
#define NAME_MAX_LEN 32    // Maximum length for player names
#define WORD_MAX_LEN 6     // Maximum length for target words
#define TIME_MAX_LEN 20    // Maximum length for timestamp 

/* GameResult:
 * Stores a single completed game record for one player.
 */
struct GameResult {
    char username[NAME_MAX_LEN];   
    char word[WORD_MAX_LEN];       
    int  guesses;                  
    int  won;                      
    char timestamp[TIME_MAX_LEN];  
};

/* logGame:
 * Records a finished game in the history file.
*/

void logGame(const char *username, const char *word, int guesses, int won);

/* CheckHistory:
 * Prints the full game history table to terminal.
 */

void CheckHistory(void);

#endif