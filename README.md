# Wordex 

## Project Overview 

Wordex is a terminal-based wordle style game written in the C-language. Wordex extends the regular wordle game by incorporating the following 5 modules:

- User Authentication and Registration
- Updated User Statistics
- Global Leaderboard
- Complete Game History
- Game Logic

## My Contributions
- Developed a **full game history logging system**:
  - Stores every completed match with timestamps.
  - Displays results in a clean table format with colored WIN/LOSS indicators (green for WIN, red for LOSS).
  - Orders entries from most recent to oldest for easy access.

- Implemented a **global leaderboard system**:
  - Reads all recorded games and ranks users based on highest number of wins.
  - Displays games played, losses, win rate, and average guesses.

- Integrated timestamp functionality for all game entries to ensure accurate tracking.

## Compile and Run Wordex Program 

1. make
     - compiles all program files
2. ./wordex
     - run the wordex program
3. ./wordex --help
     - Display the help message, explaining how programs works
4. make clean
     - cleans build files
5. download wordex-executable
     - https://github.com/Sam-Scott-McMaster/the-team-assignment-team-35-formula4/actions/runs/19903467493/artifacts/4754498781 

## Program Usage 
When program starts, user must choose how to sign in:
  1. Login  - for exisiting accounts
  2. Register  - for new users

After signing in, users access the main menu to choose a game option:
1. Play Game
2. User Statistics
3. Leaderboard
4. Game History
5. Logout and Quit

After selecting an option, the game will run the option. 

## Help-Flag Output

Wordex - A text-based Wordle style game

Usage:
  ./wordex           runs the wordex program

  ./wordex --help    shows this help message and exits

Description:
  Wordex allows users to login / register, play a 5 letter wordle game, view their statistics,
  check the leaderboard and game history.

Login Menu:
  When the program starts, users choose how to sign in:
    1. Login        Login to an existing account
    2. Register     Create a new account

Game Menu Options:
  1. Play Game            Start a new Wordex game round. You have 6 guesses to find the correct word
  2. User Statistics      View your statistics (wins, losses, streaks, average guesses, and guess distribution)
  3. Leaderboard          View all global player rankings (wins, win rate, or guesses)
  4. Game History         Display a table of past games played by all users
  5. Logout and Quit.     Log out of account and exit the Wordex program

Select menu options by entering the numbers shown beside each option

## Testscript

The testscript only works with the current textfiles, using the application will cause the textfiles to be modified 
resulting in the failure of testcases. Please ensure to have the current textfiles when running the testscript.
       

