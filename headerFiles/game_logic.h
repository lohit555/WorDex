/* game_logic.h
 *
 * Header file for the Wordex game core logic module.
 * Declares functions for word selection, input validation, user interface
 * rendering, and game state management for the Wordle-style guessing game.
 *
 * Mohitkumar Gedela
 * December 3rd, 2025
 */

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

/* setRandomWordAndCount:
 * Selects a random 5-letter word from the word list and initializes
 * the guess counter for a new game session.
 */
void setRandomWordAndCount(char *randomWord, int *count);

/* convertToLower:
 * Converts all characters in the input string to lowercase for
 * case-insensitive comparison.
 */
void convertToLower(char *str);

/* validateGuess:
 * Checks if the user's guess is valid (correct length, alphabetic characters)
 * and updates the guess counter accordingly.
 */
void validateGuess(char *setWord, int *counter);

/* defaultInterface:
 * Displays the initial empty game board at the start of a new game.
 */
void defaultInterface();

/* interface:
 * Renders the current game state including all previous guesses with
 * color-coded feedback (green for correct position, yellow for wrong position,
 * gray for not in word) and displays tracked letter states.
 */
void interface(char userGuesses[][6], int *counter, char *randomWord, char *lettersGreen, char *lettersYellow, char *lettersGray);

#endif