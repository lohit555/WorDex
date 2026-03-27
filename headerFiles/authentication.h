/* authentication.h
 *
 * Header file for the Wordex user authentication module.
 * Declares functions for user registration, login, password hashing,
 * credential validation, and session management.
 *
 * Nithya Majeti
 * 3rd December, 2025
 */

#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* checkUserName:
 * Validates the format and uniqueness of a username.
 * Returns 1 if valid, 0 otherwise.
 */
int checkUserName(char *username);

/* hashPassword:
 * Applies a hashing algorithm to the plain-text password and stores
 * the result in the output buffer for secure storage.
 */
void hashPassword(char *password, char *result);

/* passwordCheck:
 * Validates password strength requirements (length, character types).
 * Returns 1 if valid, 0 otherwise.
 */
int passwordCheck(char *password);

/* registerUser:
 * Handles new user registration by prompting for credentials,
 * validating input, and storing the account securely.
 * Returns 1 on success, 0 on failure.
 */
int registerUser();

/* loginUser:
 * Authenticates an existing user by verifying credentials against
 * stored records.
 * Returns 1 on success, 0 on failure.
 */
int loginUser();

/* getUsername:
 * Retrieves the username of the currently logged-in user.
 * Returns pointer to username string, or NULL if not logged in.
 */
char* getUsername();

/* getPassword:
 * Retrieves the password of the currently logged-in user.
 * Returns pointer to password string, or NULL if not logged in.
 */
char* getPassword();

/* logoutUser:
 * Ends the current user session and clears session data.
 */
void logoutUser();

#endif