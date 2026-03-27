/* leaderboard.c
 * 
 * Builds and displays the leaderboard for the Wordex game.
 * 
 * FUNCTIONALITY:
 *  - Reads all completed games from history.txt
 *  - Aggregates per-player stats (games played, wins, losses, total guesses)
 *  - Computes win rate and average guesses per player
 *  - Sorts players in descending order of wins
 *  - Prints a coloured leaderboard table to the terminal
 *
 * AUTHOR:  Lohitashwa Madhan
 * DATE:    2nd December, 2025
 * VERSION: 2.0
 */

#include <stdio.h>
#include <string.h>
#include "../headerFiles/leaderboard.h"

#define HISTORY_FILE   "textFiles/history.txt"
#define COLOR_MAGENTA  "\033[35m"
#define COLOR_RESET    "\033[0m"

/* init_entry:
 *   Initializes one leaderboard entry for a player.
 *   PARAMETERS:
 *     e        - pointer to the entry to initialize
 *     username - name of the player to be stored in this entry
 *   SIDE EFFECTS:
 *     Writes into *e, setting username and all stats to 0.
 */

static void init_entry(struct LeaderboardEntry *e, const char *username) {
    strncpy(e->username, username, NAME_MAX_LEN - 1);
    e->username[NAME_MAX_LEN - 1] = '\0';

    // Starts all the stats at zero for a new player
    e->gamesPlayed  = 0;
    e->wins         = 0;
    e->losses       = 0;
    e->totalGuesses = 0;
    e->avgGuesses   = 0.0;
    e->winRate      = 0.0;
}

/* find_user:
 *   Searches for a username in the entries array.
 *   PARAMETERS:
 *     entries  - array of LeaderboardEntry structures
 *     count    - number of valid entries stored in the array
 *     username - name to search for
 *   RETURNS:
 *     Index of the matching entry, or -1 if not found.
 */
static int find_user(struct LeaderboardEntry *entries, int count, const char *username) {
    int i = 0;

    while (i < count) {
        if (strcmp(entries[i].username, username) == 0) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

/* read_game_lb:
 *   Reads one game record from the history file into *r.
 *   File format: username word guesses won timestamp
 *   PARAMETERS:
 *     fp - open FILE pointer for the history file
 *     r  - pointer to GameResult struct to fill
 *   RETURNS:
 *     1 if a full record (5 values) is read successfully,
 *     0 otherwise (EOF or bad line).
 */

static int read_game_lb(FILE *fp, struct GameResult *r) {
    int scanned;

    // Reads one line of game info into the GameResult struct 
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

/* build_leaderboard:
 *   Builds per-player stats from all entries in history.txt.
 *   PARAMETERS:
 *     entries - array of LeaderboardEntry to fill
 *     count   - pointer to an int where the number of players is stored
 *   SIDE EFFECTS:
 *     - Reads all games from HISTORY_FILE if it exists
 *     - For each player, updates gamesPlayed, wins, losses, totalGuesses
 *     - Computes avgGuesses and winRate for each entry
 */

static void build_leaderboard(struct LeaderboardEntry *entries, int *count) {
    FILE *fp;
    struct GameResult r;
    int ok;
    int idx;

    *count = 0;  //Starts with zero players in the leaderboard

    fp = fopen(HISTORY_FILE, "r"); // Opens the history file in read mode

    // Read each game line and add its data into the correct player entry 
    ok = read_game_lb(fp, &r);
    while (ok == 1) {
        idx = find_user(entries, *count, r.username);

        if (idx == -1) {
            if (*count < MAX_USERS) {
                idx = *count;
                init_entry(&entries[idx], r.username);
                *count = *count + 1;
            } 
        }

        if (idx != -1) {
            // Updates the stats for this player's entry 
            entries[idx].gamesPlayed  = entries[idx].gamesPlayed + 1;
            entries[idx].totalGuesses = entries[idx].totalGuesses + r.guesses;

            if (r.won == 1) {
                entries[idx].wins = entries[idx].wins + 1;
            } else {
                entries[idx].losses = entries[idx].losses + 1;
            }
        }
       //Reads the next game from the history.txt file 
        ok = read_game_lb(fp, &r);
    }

    fclose(fp);

    //Compute averages and win rate for each player 
    {
        int i = 0;

        while (i < *count) {
            if (entries[i].gamesPlayed > 0) {
                //Average guesses per game for this player
                entries[i].avgGuesses =
                    (double) entries[i].totalGuesses /
                    (double) entries[i].gamesPlayed;
                //Win rate = wins / total games
                entries[i].winRate =
                    (double) entries[i].wins /
                    (double) entries[i].gamesPlayed;
            } else {
                entries[i].avgGuesses = 0.0;
                entries[i].winRate    = 0.0;
            }
            i = i + 1;
        }
    }
}

/* swap_entries:
 *   Swaps two leaderboard entries used by the sort function.
 *   PARAMETERS:
 *     a, b - pointers to the two entries to be swapped
 *   SIDE EFFECTS:
 *     Modifies the contents of *a and *b.
 */

static void swap_entries(struct LeaderboardEntry *a, struct LeaderboardEntry *b) {
    struct LeaderboardEntry temp;

    temp = *a;  
    *a   = *b;
    *b   = temp;
}

/* sort_by_wins:
 *   Sorts the entries in descending order of wins using selection sort.
 *   PARAMETERS:
 *     entries - array of LeaderboardEntry
 *     count   - number of valid entries in the array
 *   SIDE EFFECTS:
 *     Reorders the entries array so that players with more wins appear first.
 */

static void sort_by_wins(struct LeaderboardEntry *entries, int count) {
    int i = 0;

    while (i < count - 1) {
        int j = i + 1;
        int best = i;

        while (j < count) {
            if (entries[j].wins > entries[best].wins) {
                best = j;
            }
            j = j + 1;
        }

        if (best != i) {
            swap_entries(&entries[i], &entries[best]);
        }

        i = i + 1;
    }
}

/* print_leaderboard:
 *   Prints the leaderboard table with a magenta colored heading and footer.
 *   PARAMETERS:
 *     entries - array of LeaderboardEntry which is already sorted
 *     count   - number of entries to print
 *   SIDE EFFECTS:
 *     Writes the formatted leaderboard to standard output.
 */

static void print_leaderboard(const struct LeaderboardEntry *entries, int count) {
    int i;

    /* Coloured heading */
    printf("\n" COLOR_MAGENTA "---------------------------Leaderboard---------------------------" COLOR_RESET "\n");

    printf("%-6s %-12s %-6s %-6s %-6s %-9s %-10s\n",
           "Rank", "Player", "Games", "Wins", "Loss", "WinRate", "AvgGuess");
    printf("-----------------------------------------------------------------\n");

    i = 0;
    while (i < count) {
        printf("%-6d %-12s %-6d %-6d %-6d %-8.2f %-10.2f\n",
               i + 1,
               entries[i].username,
               entries[i].gamesPlayed,
               entries[i].wins,
               entries[i].losses,
               entries[i].winRate * 100.0,
               entries[i].avgGuesses);
        i = i + 1;
    }

    /* Coloured footer */
    printf(COLOR_MAGENTA "-----------------------------------------------------------------\n" COLOR_RESET);
}

/* GetLeaderboard:
 *   Public function that is called from wordex.c
 *   Builds the leaderboard from history, sorts by wins, and prints it.
 *   SIDE EFFECTS:
 *    Prints "No games in history." if the history file is empty or missing.
 *    Otherwise it prints the full leaderboard table to stdout.
 */

void GetLeaderboard(void) {
    struct LeaderboardEntry entries[MAX_USERS];
    int count;

    build_leaderboard(entries, &count);

    sort_by_wins(entries, count);
    print_leaderboard(entries, count);
}
