#include "../headerFiles/game_logic.h"

#ifdef TESTING
    #define CLEAR_SCREEN()
#else
    #define CLEAR_SCREEN() system("clear")
#endif

/*
 * setRandomWordAndCount
 *
 * Selects a random 5-letter word from the dictionary and counts how many
 * valid words exist in the file. In TESTING mode, the function bypasses
 * randomness and uses a fixed word.
 *
 * Parameters:
 *   randomWord — a char array to store the selected word
 *   count      — pointer to an int where the total number of words is stored
 *
 * Return:
 *   This function does not return a value.
 */
void setRandomWordAndCount(char *randomWord, int *count) {
    #ifdef TESTING  
        strcpy(randomWord, "banes");
        *count = 1;
        return;
    #endif

    FILE *file = fopen("textFiles/words.txt", "r");
    if (!file) {
        printf("Could not open file.\n");
        *count = 0;
        return;
    }

    char words[10000][6];
    char line[100];
    *count = 0;

    while (fgets(line, sizeof(line), file)) {

        // remove newline
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 5) {
            strcpy(words[*count], line);
            (*count)++;

            if (*count >= 10000) break;
        }
    }

    fclose(file);

    if (*count == 0) {
        printf("No 5-letter words in the file.\n");
        return;
    }

    srand(time(NULL));
    int index = rand() % *count;
    strcpy(randomWord, words[index]);
}

/*
 * convertToLower
 *
 * Converts all alphabetic characters in a string to lowercase.
 *
 * Parameters:
 *   str — the string to be converted
 *
 * Return:
 *   This function does not return a value.
 */
void convertToLower(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

/*
 * validateGuess
 *
 * Continuously prompts the user for a 5-letter word until a valid dictionary
 * word is entered. Converts the guess to lowercase and stores it if valid.
 *
 * Parameters:
 *   setWord — destination array for the validated 5-letter guesses
 *   counter — pointer to the number of successful guesses made so far
 *
 * Return:
 *   This function does not return a value.
 */
void validateGuess(char *setWord, int *counter) {

    // Big array to hold whatever the user types
    char userGuess[1000]; 
    int valid = false;  
    
    while (!valid) {
        printf("Enter a 5-letter word: ");
        scanf("%s", userGuess);  
        
        if (strlen(userGuess) != 5) {
            printf("That word has %ld letters. Please enter exactly 5 letters.\n", strlen(userGuess));
            continue;  
        }

        convertToLower(userGuess);
        
        FILE *file = fopen("textFiles/words.txt", "r");
        
        char line[6];
        int found = false;  
        
        // Read the file line by line
        while (fgets(line, sizeof(line), file) != NULL) {

            // Remove the newline character
            line[strcspn(line, "\n")] = '\0';  
            convertToLower(line);

            // Compare the user's word with the word from the file
            if (strcmp(line, userGuess) == 0) {
                found = true;  
                break;  
            }
        }
        
        fclose(file);  
        
        if (found) {
            strcpy(setWord, userGuess);  
            (*counter)++;  
            valid = true;  
        } else {
            printf("Sorry, %s is not in our word list. Try another word.\n", userGuess);
        }
    }
}

/*
 * defaultInterface
 *
 * Displays the empty WorDex game board and keyboard status before any guesses
 * have been entered. Used at the beginning of a game or when resetting the UI.
 *
 * Parameters:
 *   None
 *
 * Return:
 *   This function does not return a value.
 */
void defaultInterface() {
    CLEAR_SCREEN();
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║                    WORDEX                     ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║       _       _       _       _       _       ║\n");
    printf("║       _       _       _       _       _       ║\n");
    printf("║       _       _       _       _       _       ║\n");
    printf("║       _       _       _       _       _       ║\n");
    printf("║       _       _       _       _       _       ║\n");
    printf("║       _       _       _       _       _       ║\n");
    printf("╚═══════════════════════════════════════════════╝\n\n");

    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║              KEYBOARD STATUS                  ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");

    char letters[] = "qwertyuiopasdfghjklzxcvbnm";

    for (int i = 0; i < 26; i++) {

        if (letters[i] == 'q') {
            printf("║    ");
        } else if (letters[i] == 'a') {
            printf("║      ");
        } else if (letters[i] == 'z') {
            printf("║         ");
        }

        printf(" %c  ", toupper(letters[i]));  

        if (letters[i] == 'p') {
            printf("   ║\n");
        } else if (letters[i] == 'l') {
            printf("     ║\n");
        } else if (letters[i] == 'm') {
            printf("          ║\n");
        }
    }

    printf("╚═══════════════════════════════════════════════╝\n\n");
}

/*
 * interface
 *
 * Displays the full WorDex game interface, including:
 *   - all previous guesses with colour coding (green, yellow, gray)
 *   - the updated on-screen keyboard showing discovered letter statuses
 *
 * Recalculates letter colouring based on all guesses stored so far.
 *
 * Parameters:
 *   userGuesses   — 2D array containing all previous 5-letter guesses
 *   counter       — pointer to the number of valid guesses made
 *   randomWord    — the correct answer word for this round
 *   lettersGreen  — array containing letters confirmed in correct position
 *   lettersYellow — array containing letters in word but wrong position
 *   lettersGray   — array containing letters not in the word
 *
 * Return:
 *   This function does not return a value.
 */
void interface(char userGuesses[][6], int *counter, char *randomWord, char *lettersGreen, char *lettersYellow, char *lettersGray) {
    CLEAR_SCREEN();

    if (*counter > 0) {

        lettersGreen[0] = '\0';
        lettersYellow[0] = '\0';
        lettersGray[0] = '\0';
        
        for (int guessNum = 0; guessNum < *counter; guessNum++) {
            for (int i = 0; i < 5; i++) {
                char letter = userGuesses[guessNum][i];
                
                if (strchr(lettersGreen, letter) != NULL) {
                    continue;
                }
                
                if (letter == randomWord[i]) {
                    int len = strlen(lettersGreen);
                    lettersGreen[len] = letter;
                    lettersGreen[len + 1] = '\0';
                    continue;
                }
                
                if (strchr(lettersYellow, letter) != NULL) {
                    continue;
                }
                
                bool foundInWord = false;
                for (int j = 0; j < 5; j++) {
                    if (letter == randomWord[j]) {
                        foundInWord = true;
                        break;
                    }
                }
                
                if (foundInWord) {
                    int len = strlen(lettersYellow);
                    lettersYellow[len] = letter;
                    lettersYellow[len + 1] = '\0';
                } else {
                    if (strchr(lettersGray, letter) == NULL) {
                        int len = strlen(lettersGray);
                        lettersGray[len] = letter;
                        lettersGray[len + 1] = '\0';
                    }
                }
            }
        }
    }

    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║                    WORDEX                     ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");

    for (int row = 0; row < 6; row++) {
        printf("║       ");
        
        if (row < *counter) {
            char colours[5];
            int available[5];
            
            for (int k = 0; k < 5; k++) {
                available[k] = 1;
                colours[k] = 'X';
            }
            
            for (int col = 0; col < 5; col++) {
                if (userGuesses[row][col] == randomWord[col]) {
                    colours[col] = 'G';
                    available[col] = 0;
                }
            }
            
            for (int col = 0; col < 5; col++) {
                if (colours[col] == 'X') {
                    char currentLetter = userGuesses[row][col];
                    
                    for (int j = 0; j < 5; j++) {
                        if (available[j] && currentLetter == randomWord[j]) {
                            colours[col] = 'Y';
                            available[j] = 0;
                            break;
                        }
                    }
                }
            }
            
            for (int col = 0; col < 5; col++) {
                char currentLetter = userGuesses[row][col];
                
                if (colours[col] == 'G') {
                    printf("\033[32m%c\033[0m       ", toupper(currentLetter));
                } else if (colours[col] == 'Y') {
                    printf("\033[33m%c\033[0m       ", toupper(currentLetter));
                } else {
                    printf("\033[90m%c\033[0m       ", toupper(currentLetter));
                }
            }
        } else {
            printf("_       _       _       _       _       ");
        }
        
        printf("║\n");
    }

    printf("╚═══════════════════════════════════════════════╝\n\n");

    char letters[] = "qwertyuiopasdfghjklzxcvbnm";

    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║              KEYBOARD STATUS                  ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");

    for (int i = 0; i < 26; i++) {
        if (letters[i] == 'q') {
            printf("║    ");
        } else if (letters[i] == 'a') {
            printf("║      ");
        } else if (letters[i] == 'z') {
            printf("║         ");
        }
        
        if (strchr(lettersGreen, letters[i]) != NULL) {
            printf(" \033[32m%c\033[0m  ", toupper(letters[i]));
        } else if (strchr(lettersYellow, letters[i]) != NULL) {
            printf(" \033[33m%c\033[0m  ", toupper(letters[i]));
        } else if (strchr(lettersGray, letters[i]) != NULL) {
            printf(" \033[90m%c\033[0m  ", toupper(letters[i]));
        } else {
            printf(" %c  ", toupper(letters[i]));
        }

        if (letters[i] == 'p') {
            printf("   ║\n");
        } else if (letters[i] == 'l') {
            printf("     ║\n");
        } else if (letters[i] == 'm') {
            printf("          ║\n");
        }
    }

    printf("╚═══════════════════════════════════════════════╝\n\n");
}