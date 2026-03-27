/* wordex.c
* 
* Main program for the Wordex game.
* Handles help flags, menu navigation, user login/registration and running the game. 
* It allows users to view their statistics, the game leaderboard, and history.
*
* Lohitashwa Madhan, Mohit Gedela, Nithya Majeti and Zainab Mirza
* Date: 2025/12/03
*/

#include "wordex.h"

#ifdef TESTING
    #define CLEAR_SCREEN()
#else
    #define CLEAR_SCREEN() system("clear")
#endif

/* printHelp: Displays the program usage and game menu information */
void printHelp() {
    printf("\nWordex - A text-based Wordle style game\n\n");

    printf("Usage:\n");
    printf("  ./wordex           runs the wordex program\n\n");
    printf("  ./wordex --help    shows this help message and exits\n\n");

    printf("Description:\n");
    printf("  Wordex allows users to login / register, play a 5 letter wordle game, view their statistics,\n");
    printf("  check the leaderboard and game history.\n\n");

    printf("Login Menu:\n");
    printf("  When the program starts, users choose how to sign in:\n");
    printf("    1. Login        Login to an existing account\n");
    printf("    2. Register     Create a new account\n\n");

    printf("Game Menu Options:\n");
    printf("  1. Play Game            Start a new Wordex game round. You have 6 guesses to find the correct word\n");

    printf("  2. User Statistics      View your statistics (wins, losses, streaks, average guesses, and guess distribution)\n");

    printf("  3. Leaderboard          View all global player rankings (wins, win rate, or guesses)\n");

    printf("  4. Game History         Display a table of past games played by all users\n");

    printf("  5. Logout and Quit.     Log out of account and exit the Wordex program\n\n");

    printf("Select menu options by entering the numbers shown beside each option\n\n");

}

/* printBanner: Displays the games title banner */
void printBanner() {
    printf("\n");
    printf("\033[38;5;33m██╗    ██╗ ██████╗ ██████╗ ██████╗ ███████╗██╗  ██╗\033[0m\n");
    printf("\033[38;5;39m██║    ██║██╔═══██╗██╔══██╗██╔══██╗██╔════╝╚██╗██╔╝\033[0m\n");
    printf("\033[38;5;45m██║ █╗ ██║██║   ██║██████╔╝██║  ██║█████╗   ╚███╔╝ \033[0m\n");
    printf("\033[38;5;51m██║███╗██║██║   ██║██╔══██╗██║  ██║██╔══╝   ██╔██╗ \033[0m\n");
    printf("\033[38;5;87m╚███╔███╔╝╚██████╔╝██║  ██║██████╔╝███████╗██╔╝ ██╗\033[0m\n");
    printf("\033[38;5;123m ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝\033[0m\n");
    printf("\n");
}

/* main: 
* Entry point of the game. Displays the login menu and provides access to the main game menu where user can play games, 
view statistics, leaderboard, history, or quit the game.
* Returns 0 on quit and 1 for input or login errors
**/
int main(int argc, char *argv[]) {
    
    // Handles the help flag
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0) {
            printHelp();
            return 0;
        }
    }

    printBanner();

    // Login / Registration menu
    printf("Welcome to Wordex\n\n");
    printf("Please choose an option\n");
    printf("1. Login for Existing Users\n");
    printf("2. Register Account for New Users\n\n");
    printf("Please enter an option number: ");

    int menu;  // stores user option

    // Invalid option number
    if (scanf("%d", &menu) != 1) {
        fprintf(stderr, "Error: Invalid input.\n");
        return 1;
    }

    // Try to login user
    if (menu == 1) {
        loginUser();
    }

    // Try to register new user
    else if (menu == 2) {
        if (registerUser() != 0) {
            fprintf(stderr, "\nRegistration failed\n");
            return 1;
        }
    }

    else {
        fprintf(stderr, "Error: Invalid input. Please enter a valid option\n");
        return 1;
    }

    char *user = getUsername();  // store username 
 
    int option;   
    int keepPlaying = 1;   // Track if loop should continue 

    // Main game loop
    while (keepPlaying) {

        // Game options menu
        printf("\n\nHi %s! Please choose an option below: \n\n", user);
        printf("1. Play Game \n");
        printf("2. User Statistics \n");
        printf("3. Leaderboard \n");
        printf("4. Game History \n");
        printf("5. Logout and Quit\n");
        printf("\nPlease enter an option number: ");

        if (scanf("%d", &option) != 1) {
            fprintf(stderr, "\nError: Invalid input.\n") ;
            return 1;
        }

        // Option 1 - Play wordex game
        if (option == 1) {
            char randomword[6];
            int wordCount = 0;
            int counter = 0;

            // track letter states and store user guesses
            char lettersGreen[27] = "";
            char lettersYellow[27] = "";
            char lettersGray[27] = "";
            char userGuesses[6][6] = {"", "", "", "", "", ""};

            setRandomWordAndCount(randomword, &wordCount);   // load a random word 

            defaultInterface();

            int won = 0;   // track if user wins or loses

            for (int i = 0; i < 6; i++) {
                validateGuess(userGuesses[i], &counter);   // read and validate user's guess

                // update board and keyboard 
                interface(userGuesses, &counter, randomword, lettersGreen, lettersYellow, lettersGray);

                // check if guess matches the hidden word 
                if (strcmp(userGuesses[i], randomword) == 0) {
                    won = 1;
                    printf("Congrats, You won in %d guesses!!\n\n", counter);
#ifndef TESTING
                    sleep(2);
#endif
                    break;
                }
            }

            // If all guesses are wrong
            if (!won) {
                printf("Game Over!, The correct word was %s\n\n", randomword);
            }

            // Record game results in history file 
            logGame(user, randomword, counter, won);

            // Update the user's statistics
            time_t now = time(NULL);   // record time of played game

            updateStats(user, won, counter, (long) now);



            #ifndef TESTING 
                sleep(5);
            #endif
            
            // system("clear");
            CLEAR_SCREEN();

        }
        
        // Option 2 - View Statistics
        else if (option == 2) {
            printStats(user);
        }

        // Option 3 - View Leaderboard
        else if (option == 3) {
            GetLeaderboard();
        }

        // Option 4 - View Game History
        else if (option == 4) {
            CheckHistory();
        }

        // Option 5 - Logout and Quit Game
        else if (option == 5) {
            logoutUser();
            printf("Thanks for playing Wordex!\n\n");
            keepPlaying = 0;   // ends loop
        }
        
        // If input option is invalid
        else {
            fprintf(stderr, "\nError: Invalid option number, please choose an option 1 to 5.\n");
        }

    }

    return 0;
}