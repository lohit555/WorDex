#include "../headerFiles/authentication.h"

char currentUsername[50]; 
char currentHashedPassword[70]; 

/*
 * checkUserName
 *
 * checks a provided username to see if it exists in the record of userprofiles.txt
 *
 * Parameters:
 *   *username: the provided username
 *
 * Return:
 *   This function returns an integer, 0: success, 1: failure
 */
int checkUserName(char *username) { 

    FILE *userinfo;
    char line[200];

    username[strcspn(username, "\n")] = '\0';

    userinfo = fopen("textFiles/userprofiles.txt", "r");

    // if (userinfo == NULL) {
    //     printf("\033[0;32mSuccesful, this username is available!\n\n\033[0m");
    //     return 0;
    // }

    while(fgets(line, sizeof(line), userinfo) != NULL) {
       line[strcspn(line, "\n")] = '\0';
        
       char searchedUsername[100];
       sprintf(searchedUsername, "username: %s", username);

       if (strcmp(line, searchedUsername) == 0) {
            fclose(userinfo);
            fprintf(stderr, "\033[0;31mUsername already exists, please choose a different username\n\n\033[0m");
            return 1;
        }
    }

    fclose(userinfo);
    printf("\033[0;32mSuccesful, this username is available!\n\n\033[0m");
    return 0;
}

/*
 * hashPassword
 *
 * This function is responsible for hashing the user password to keep the user information safe. It 
 * hashes the passwird by mixing it with a string win and doing multiple mixes so it's hard to revert
 *
 * Parameters:
 *   *password: the provided password to hash
 *   *result: the buffer to store the resulting hash string
 *
 * Return:
 *   This function returns void (no return value)
 */
void hashPassword(char *password, char *result) {
    int i;
    unsigned long long startNum = 4137;
    unsigned long long saltNum = 0;
    unsigned long long passwordNum = 0;
    int count = 0;
    char saltStr[] = "win";

    for (i = 0; i < strlen(saltStr); i++) {
        saltNum += (startNum * 31) + (int)saltStr[i];
    }

    for (i = 0; i < strlen(password); i++) {
        passwordNum += (startNum * 31) + (int)password[i];
    }

    // Variable to store large unsigned integer values for hash calculations
    unsigned long long mixedNum = saltNum + passwordNum;

    while (count < 679) {
        mixedNum = mixedNum * 31;
        count++;
    }

    // Convert the mixed number to a 64-character hexadecimal string and store in result
    sprintf(result, "%064llx", mixedNum);
}
/*
 * passwordCheck
 *
 * Checks if a password meets security requirements including minimum length,
 * maximum length, presence of special characters, and presence of uppercase letters.
 *
 * Parameters:
 *   *password: the password string to validate
 *
 * Return:
 *   This function returns an integer, 0: success, 1: failure
 */
int passwordCheck(char *password) { 
    int i;
    int specialCheck = 0;
    int uppercaseCheck = 0;

    if (strlen(password) < 8) {
        fprintf(stderr, "\033[0;31mThe password must be at least 8 characters long\n\n\033[0m");
        return 1;
    }

    if (strlen(password) > 50) {
        fprintf(stderr, "\033[0;31mThe password is too long, maximum 50 character limit\n\n\033[0m");
        return 1;
    }

    for (i = 0; i < strlen(password); i++) {
        // checking if the passowrd has a special character
        if (!(isalpha(password[i]) || isdigit(password[i]))) {
            specialCheck = 1;
        }
        // Checking for an uppercase letter
        if (isupper(password[i])) {
            uppercaseCheck = 1;
        }
    }

    if (specialCheck == 0) {
        fprintf(stderr, "\033[0;31mThe password must contain a special character\n\n\033[0m");
        return 1;
    }

    if (uppercaseCheck == 0) {
        fprintf(stderr, "\033[0;31mThe password must contain an uppercase character\n\n\033[0m");
        return 1;
    }

    return 0;
}

/*
 * registerUser
 *
 * This function handles the user registration process by prompting for a username and password,
 * validating both inputs, hashing the password, and storing the user credentials in the textfile. 
 *
 * Parameters:
 *   None
 *
 * Return:
 *   This function returns an integer, 0: registration successful, 1: registration failed
 */
int registerUser() {

    char username[50];
    char password[50];

    printf("Please enter your username:\n");
    scanf("%s", username);

    if(checkUserName(username) != 0) {
        return 1;
    }

    printf("Please enter your password:\n");
    scanf("%s", password);

    if (passwordCheck(password) != 0) {
        return 1;
    }

    char hashedPassword[65];
    hashPassword(password, hashedPassword);

    strcpy(currentUsername, username);
    strcpy(currentHashedPassword, hashedPassword);

    FILE *userinfo;

    // appending info to the file
    userinfo = fopen("textFiles/userprofiles.txt", "a");
    fprintf(userinfo, "username: %s\n", username);
    fprintf(userinfo, "password: %s\n", hashedPassword);
    fprintf(userinfo, "\n");

    fclose(userinfo);

    printf("\033[0;32mRegistration succesful! Welcome %s\033[0m\n\n", username);

    return 0;
}

/*
 * loginUser
 *
 * This function handles the user login process by prompting for username and password,
 * validating the username exists in the system, verifying the password hash matches the
 * stored hash.
 *
 * Parameters:
 *   None
 *
 * Return:
 *   This function returns an integer, 0: login successful, 1: login failed
 */
int loginUser() {

    FILE *userinfo;
    char username[50];
    char password[50];
    int match = 0;
    int correctUsername = 0;
    int correctPassword = 0;
    
    while (correctUsername == 0) {
        printf("Please enter your username:\n");
        scanf("%s", username);

        // Remove trailing newline character from username string if present
        username[strcspn(username, "\n")] = '\0';
        userinfo = fopen("textFiles/userprofiles.txt", "r");

        char line[200];
        while(fgets(line, sizeof(line), userinfo) != NULL) {
            line[strcspn(line, "\n")] = '\0';

            char searchedUsername[100];
            sprintf(searchedUsername, "username: %s", username);
            if (strcmp(line, searchedUsername) == 0) {
                match = 1;
                correctUsername = 1;
                break;
            }
        }
        fclose(userinfo);

        if (match == 0) {
            fprintf(stderr, "\033[0;31mThis username does not exist, please try again.\n\n\033[0m");
        }
    }

    // Looping until password is correct
    while(correctPassword == 0) {
        printf("Please enter your password:\n");
        scanf("%s", password);

        password[strcspn(password, "\n")] = '\0';

        char storedHash[65];
        char enteredHash[65];
        char line[200];

        userinfo = fopen("textFiles/userprofiles.txt", "r");
        while(fgets(line, sizeof(line), userinfo) != NULL) {
            line[strcspn(line, "\n")] = '\0';

            char searchedUsername[100];
            sprintf(searchedUsername, "username: %s", username);
            if (strcmp(line, searchedUsername) == 0) {

                fgets(line, sizeof(line), userinfo);
                line[strcspn(line, "\n")] = '\0';

                // Locate password in the line and extract the hash value that follows it
                char *passwordStart = strstr(line, "password: ");
                if (passwordStart != NULL) {
                    passwordStart += strlen("password: ");
                    strcpy(storedHash, passwordStart);
                }
                break;
            }
        }

        fclose(userinfo);

        hashPassword(password, enteredHash);

        // Comparing stored hash and user converted hash matcjes
        if (strcmp(storedHash, enteredHash) == 0) {
            correctPassword = 1;
            strcpy(currentUsername, username);
            strcpy(currentHashedPassword, storedHash);
            printf("Login Succesful! Welcome, %s", username);
            return 0;
        }
        else {
            fprintf(stderr, "\033[0;31mIncorrect password, try again\n\n\033[0m");
        }
    }
}

char* getUsername() {
    return currentUsername;
}

/*
 * logoutUser
 *
 * This function clears the current username and password.
 *
 * Parameters:
 *   None
 *
 * Return:
 *   This function returns void (no return value)
 */
void logoutUser() {
    currentUsername[0] = '\0';
    currentHashedPassword[0] = '\0';

    printf("Logged out successfully \n");
}