#include <stdio.h>
#include <string.h>
#include <time.h>   // ✅ added for logging
#include "../security/hash.h"
#include "../security/otp.h"

#define FILE_PATH "data/users.txt"
#define LOG_FILE "data/logs.txt"   // ✅ log file

void login_user() {
    char username[50];
    char password[50];
    char file_username[50];
    int file_hash;

    int attempts = 0;

    while (attempts < 3) {
        int found = 0;

        printf("\n--- User Login ---\n");

        // Input username
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        // Input password
        printf("Enter password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        int hashed = hash_password(password);

        FILE *fp = fopen(FILE_PATH, "r");
        if (fp == NULL) {
            printf("Error opening file!\n");
            return;
        }

        // Check credentials
        while (fscanf(fp, "%s %d", file_username, &file_hash) != EOF) {
            if (strcmp(username, file_username) == 0 && hashed == file_hash) {
                found = 1;
                break;
            }
        }

        fclose(fp);

        // ❌ If credentials wrong → retry (NO OTP)
        if (!found) {
            attempts++;
            printf("Wrong credentials, try again..! (%d attempts left)\n", 3 - attempts);
            continue;
        }

        // ✅ If credentials correct → THEN OTP
        int otp = generate_otp();
        printf("OTP: %d\n", otp);

        if (verify_otp(otp)) {
            printf("Login Successful!\n");

            // ✅ LOGGING (only added part)
            FILE *log_fp = fopen(LOG_FILE, "a");
            if (log_fp != NULL) {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);

                fprintf(log_fp, "[LOGIN] %s at %04d-%02d-%02d %02d:%02d:%02d\n",
                        username,
                        t->tm_year + 1900,
                        t->tm_mon + 1,
                        t->tm_mday,
                        t->tm_hour,
                        t->tm_min,
                        t->tm_sec);

                fclose(log_fp);
            }

        } else {
            printf("OTP Verification Failed!\n");
        }

        return;
    }

    printf("Too many failed attempts. Access blocked.\n");
}