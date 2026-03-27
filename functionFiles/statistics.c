/* statistics.c
*
* Implements statistics for WorDex 
* This file handles:
*   - creating default stats for new users
*   - loading user stats to a txt file
*   - updating user stats after each game
*   - print current stats for user

* Zainab Mirza
* Date: 2025/12/03
*/

#include "../headerFiles/statistics.h"

/* Statistics txt file that stores user data */
#define STATS_FILE "textFiles/stats.txt" 

/* File format for each user (one line per user):

*    <username> <wins> <losses> <total games played> <currentStreak> <maxStreak> <total guesses> <last played time> <w1> <w2> <w3> <w4> <w5> <w6>

*     where: w1 to w6 —> are the amount of wins for each 1,2,3,4,5,6 guesses 
*/

/* defaultStats:
* Creates default statistics for each new user and appends to the stats file */
int defaultStats(const char *username) {
    FILE *file = fopen(STATS_FILE, "a");   // Open stats file in append mode

    // print default stats (all zeros)
    fprintf(file, "%s 0 0 0 0 0 0 0 0 0 0 0 0 0\n", username);

    fclose(file);
    return 0;
}


/* loadStats:
* Loads statistics for existing users from stats file into a stats struct. If user is not in the file, it returns are error */ 
int loadStats(const char *username, Stats *s) {

    FILE *file = fopen(STATS_FILE, "r");  //open file for reading

    // varibles to read each value from lines in the text file 
    char name[MAX_USERNAME];
    int wins, losses, gamesPlayed;
    int currentStreak, maxStreak, totalGuesses;
    long lastPlayed;
    int w1, w2, w3, w4, w5, w6;

    // Read each line until we find the matching username
    while (fscanf(file, "%s %d %d %d %d %d %d %ld %d %d %d %d %d %d",
                    name, &wins, &losses, &gamesPlayed, &currentStreak, 
                    &maxStreak, &totalGuesses, &lastPlayed, &w1, &w2, &w3,
                    &w4, &w5, &w6) == 14) {

        // check if the username in file matches 
        if (strcmp(name, username) == 0) {
            
            // copy username to struct
            strncpy(s->username, name, MAX_USERNAME - 1);
            s->username[MAX_USERNAME - 1] = '\0';

            // store all data in the struct
            s->wins = wins;
            s->losses = losses;
            s->gamesPlayed = gamesPlayed;
            s->currentStreak = currentStreak;
            s->maxStreak = maxStreak;
            s->totalGuesses = totalGuesses;
            s->lastPlayed = lastPlayed;
            s->winsByGuess[0] = w1;
            s->winsByGuess[1] = w2;
            s->winsByGuess[2] = w3;
            s->winsByGuess[3] = w4;
            s->winsByGuess[4] = w5;
            s->winsByGuess[5] = w6;

            fclose(file);
            return 0;
        }       
    }
   
    // No matching user found
    fclose(file);
    return 1; // user DNE

}

/* updateStats:
* updates a user's statistics after each completed game and updates the users record in the stats file. */
int updateStats(const char *username, int won, int guessCount, long timePlayed) {

    Stats s;  // struct that holds current stats for the user

    // load stats for user, if user DNE, create default record and load it
    if (loadStats(username, &s) != 0) {
        defaultStats(username);
        loadStats(username, &s);   // load default stats
    }

    // update user stats
    s.gamesPlayed++;   

    // if user won the game
    if (won) {
        s.wins++;
        s.totalGuesses += guessCount; //update guess count 

        if (guessCount >= 1 && guessCount <= 6) {
            s.winsByGuess[guessCount - 1]++;     // update guess distribution
        }

        // streak = consecutive wins
        s.currentStreak++;
        if (s.currentStreak > s.maxStreak) {     // update max streak
            s.maxStreak = s.currentStreak;
        }
    
    // if user lost the game
    } else {
        s.losses++;
        s.currentStreak = 0;   // reset current streak on each loss 
    }

    s.lastPlayed = timePlayed; // record last played time

    // rewrite entire stats file for all users with updated statistics
    FILE *in = fopen(STATS_FILE, "r");   // open old file 
    FILE *out = fopen("textFiles/new_stats.txt", "w");  // open new temporary file

    // varibles for reading each stat in file
    char name[MAX_USERNAME];
    int wins, losses, gamesPlayed;
    int currentStreak, maxStreak, totalGuesses;
    long lastPlayed;
    int w1, w2, w3, w4, w5, w6;

    // read and copy each stat to the new file
    while (fscanf(in, "%s %d %d %d %d %d %d %ld %d %d %d %d %d %d",
                    name, &wins, &losses, &gamesPlayed, &currentStreak, 
                    &maxStreak, &totalGuesses, &lastPlayed, &w1, &w2, &w3,
                    &w4, &w5, &w6) == 14) {
        
        if (strcmp(name, username) == 0) {
            // write updated stats for this user
            fprintf(out, "%s %d %d %d %d %d %d %ld %d %d %d %d %d %d\n",
                s.username,
                s.wins, s.losses, s.gamesPlayed,
                s.currentStreak, s.maxStreak,
                s.totalGuesses, s.lastPlayed,
                s.winsByGuess[0], s.winsByGuess[1], s.winsByGuess[2],
                s.winsByGuess[3], s.winsByGuess[4], s.winsByGuess[5]);

        }
        else {
            // copy unchnaged stats for other users
            fprintf(out, "%s %d %d %d %d %d %d %ld %d %d %d %d %d %d\n", 
                name,
                wins, losses, gamesPlayed,
                currentStreak, maxStreak,
                totalGuesses, lastPlayed,
                w1, w2, w3, w4, w5, w6);
        }
    }

    fclose(in);
    fclose(out);

    // Delete and replace old file with updated file
    remove(STATS_FILE);
    rename("textFiles/new_stats.txt", STATS_FILE);  // renames back to "stats.txt"

    return 0;
}


/* printStats
* Loads and prints user's current statistics in legible format. If user is does not have 
any stats, an error message is printed */
int printStats(const char *username) {

    Stats s; // struct that holds stats for the user
    
    // try loading user stats, if user DNE, create and load default stats
    if (loadStats(username, &s) != 0) {
        defaultStats(username);
        loadStats(username, &s);  
    }

    // calculate average guesses per win
    double avgGuess = 0.0;
    if (s.wins > 0) {
        avgGuess = (double)s.totalGuesses / s.wins;     // avg guess count for games won
    }

    // Format lastPlayed time to small timestamp
    char time[100];
    if (s.lastPlayed == 0) {
        strcpy(time, "Never");     // no games played yet
    }
    else {
        time_t t = s.lastPlayed;
        struct tm *tm_info = localtime(&t);
        strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", tm_info);
    }

    // Print statistics for the user
    printf("\n\033[35m------------ Statistics for %s -----------\033[0m\n", s.username);

    printf("\nWins: %d\n", s.wins);
    printf("Losses: %d\n", s.losses);
    printf("Games Played: %d\n", s.gamesPlayed);
    printf("Current Streak: %d\n", s.currentStreak);
    printf("Max Streak: %d\n", s.maxStreak);
    printf("Average Guess Count for wins: %.2f\n", avgGuess);
    printf("\nGuess Distribution:\n");
    printf("Games won with 1 guess: %d\n", s.winsByGuess[0]);
    printf("Games won with 2 guesses: %d\n", s.winsByGuess[1]);
    printf("Games won with 3 guesses: %d\n", s.winsByGuess[2]);
    printf("Games won with 4 guesses: %d\n", s.winsByGuess[3]);
    printf("Games won with 5 guesses: %d\n", s.winsByGuess[4]);
    printf("Games won with 6 guesses: %d\n", s.winsByGuess[5]);
    
    #ifdef TESTING
        printf("\nLast played game: \033[34m%s\033[34m\n", "Never");
    #else
        printf("\nLast played game: \033[34m%s\033[34m\n", time);
    #endif


    printf("\n\033[35m--------------------------------------------\033[0m\n\n");
    
    return 0;
}