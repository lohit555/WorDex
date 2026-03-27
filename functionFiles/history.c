/* history.c
 *
 * Records and displays the game history for the Wordex app.
 *
 * FUNCTIONALITY:
 *  - logGame:  append a finished game (username, word, guesses, result, time)
 *              to textFiles/history.txt
 *  - CheckHistory: loads all saved games from the file, then print a table
 *                  of results from most recent game to oldest, with colour.
 *
 * AUTHOR:  Lohitashwa Madhan
 * DATE:    2nd December, 2025
 * VERSION: 2.0
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../headerFiles/history.h"

#define HISTORY_FILE "textFiles/history.txt"

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_MAGENTA "\033[35m"

/* Reasonable cap on how many history lines we read */
#define MAX_HISTORY_RECORDS 1000

/*
 * fill_timestamp:
 *   Writes the current local time into 'buffer' as a string
 *   using the format "YYYY-MM-DD_HH:MM".
 *   If localtime fails, it writes an empty string.
 *
 * PARAMETERS:
 *   buffer - destination character array
 *   size   - size of the buffer in bytes
 * SIDE EFFECTS:
 *   Overwrites the contents of buffer with a formatted timestamp or "".
 */

static void fill_timestamp(char *buffer, int size) {
    time_t now;
    struct tm *info;

    now = time(NULL);   // get current time
    info = localtime(&now);  // convert to local time components

    if (info != NULL) {
        // Format time as "YYYY-MM-DD_HH:MM" into buffer
        strftime(buffer, size, "%Y-%m-%d_%H:%M", info);
    } else {
        if (size > 0) {
            buffer[0] = '\0';
        }
    }
}

/*
 * read_game:
 *   Attempts to read one GameResult from the given file pointer 'fp'.
 *
 * PARAMETERS:
 *   fp - open FILE pointer for the history file
 *   r  - pointer to a GameResult struct to fill
 *
 * RETURNS:
 *   1 if a full record (5 values) was successfully read,
 *   0 otherwise (end-of-file or bad line).
 */

static int read_game(FILE *fp, struct GameResult *r) {
    int scanned;

    scanned = fscanf(fp, "%31s %5s %d %d %19s",
                     r->username,
                     r->word,
                     &r->guesses,
                     &r->won,
                     r->timestamp);

    if (scanned == 5) {
        return 1;
    }
    return 0;
}

/*
 * print_history_header:
 *   Prints the column titles for the history table.
 *
 * PARAMETERS:
 *   none
 *
 * SIDE EFFECTS:
 *   Writes a two-line header for the history table to stdout.
 */

static void print_history_header(void) {
    printf("%-12s %-8s %-8s %-8s %-16s\n",
           "User", "Word", "Guesses", "Result", "Time");
    printf("--------------------------------------------------------\n");
}

/*
 * logGame:
 *   Receives the details of a finished game and appends one line
 *   to HISTORY_FILE (history.txt) in the format:
 *     username word guesses won timestamp
 *
 * PARAMETERS:
 *   username - name of the player who just finished the game
 *   word     - target word used for that game
 *   guesses  - number of guesses the player took
 *   won      - non-zero if the player won, zero if they lost
 *
 * SIDE EFFECTS:
 *   Opens HISTORY_FILE in append mode and writes one new record line.
 *   If the file cannot be opened, the function quietly returns.
 */

void logGame(const char *username, const char *word, int guesses, int won) {

    FILE *fp;
    struct GameResult r;

    // Copies username into the struct 
    strncpy(r.username, username, NAME_MAX_LEN - 1);
    r.username[NAME_MAX_LEN - 1] = '\0';

    // Copies the guessed word safely into the struct
    strncpy(r.word, word, WORD_MAX_LEN - 1);
    r.word[WORD_MAX_LEN - 1] = '\0';

    r.guesses = guesses;

    if (won != 0) {
        r.won = 1;
    } else {
        r.won = 0;
    }

    // Adds a timestamp string for when this game was logged
    fill_timestamp(r.timestamp, TIME_MAX_LEN);

    // Opens the history file in append mode 
    fp = fopen(HISTORY_FILE, "a");

    // Writes one line with all five fields - username word guesses won timestamp
    fprintf(fp, "%s %s %d %d %s\n",
            r.username,
            r.word,
            r.guesses,
            r.won,
            r.timestamp);

    fclose(fp);
}

/*
 * CheckHistory:
 *   Reads all previous game entries from HISTORY_FILE (history.txt)
 *   and prints them in a formatted table.
 *
 *   Behaviour:
 *     - If the file does not exist, prints "No history yet." and returns.
 *     - Otherwise:
 *         * reads all records into an array,
 *         * prints them in **reverse order** (most recent first),
 *         * shows WIN in green and LOSS in red,
 *         * wraps the table with magenta heading and footer lines.
 *
 * PARAMETERS:
 *   none
 *
 * SIDE EFFECTS:
 *   Writes either an empty-history message or a full history table to stdout.
 */

void CheckHistory(void) {
    FILE *fp;
    struct GameResult records[MAX_HISTORY_RECORDS];
    int count;
    int ok;

// Opens the history file for reading

    fp = fopen(HISTORY_FILE, "r");
    if (fp == NULL) {
        printf("No history yet.\n");
        return;
    }

    // Reads all records into the array 
    count = 0;
    ok = read_game(fp, &records[count]);
    while (ok == 1 && count < MAX_HISTORY_RECORDS - 1) {
        count = count + 1;
        ok = read_game(fp, &records[count]);
    }

    fclose(fp);

    if (count == 0 && ok == 0) {
        //File exists but had no valid lines
        printf("No history yet.\n");
        return;
    }

    // Magenta heading 
    printf("\n" COLOR_MAGENTA "----------------------Game History----------------------" COLOR_RESET "\n");

    // Table header 
    print_history_header();

/* Prints from most recent to oldest:
     *   - last record read (index count-1) is newest
     *   - index 0 is the oldest
     */

    {
        int i;

        i = count - 1;
        while (i >= 0) {
            if (records[i].won == 1) {
                printf("%-12s %-8s %-8d %s%-8s%s %-16s\n",
                       records[i].username,
                       records[i].word,
                       records[i].guesses,
                       COLOR_GREEN, "WIN", COLOR_RESET,
                       records[i].timestamp);
            } else {
                printf("%-12s %-8s %-8d %s%-8s%s %-16s\n",
                       records[i].username,
                       records[i].word,
                       records[i].guesses,
                       COLOR_RED, "LOSS", COLOR_RESET,
                       records[i].timestamp);
            }
            i = i - 1;
        }
    }

    /* Magenta closing line */
    printf(COLOR_MAGENTA "--------------------------------------------------------" COLOR_RESET "\n");
}
