#include <stdio.h>
#include <string.h>
#include <time.h>   // ✅ added for logging
#include "../security/hash.h"

#define FILE_PATH "data/users.txt"
#define LOG_FILE "data/logs.txt"   // ✅ log file

void register_user() {
    char username[50];
    char password[50];
    char file_username[50];
    int file_hash;

    printf("\n--- User Registration ---\n");

    // Input username
    while (1) {
        printf("Enter username (no spaces): ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
    
        if (strchr(username, ' ') != NULL) {
            printf("No spaces allowed. Try again!\n");
            continue;
        }

        // 🔍 Check if username already exists
        FILE *fp_check = fopen(FILE_PATH, "r");
        if (fp_check != NULL) {
            while (fscanf(fp_check, "%s %d", file_username, &file_hash) != EOF) {
                if (strcmp(username, file_username) == 0) {
                    printf("Username already exists! Try another.\n");
                    fclose(fp_check);
                    goto retry_username;
                }
            }
            fclose(fp_check);
        }

        break;

        retry_username:
        continue;
    }

    // Input password
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    // Hash password
    int hashed = hash_password(password);

    // Open file
    FILE *fp = fopen(FILE_PATH, "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Save user
    fprintf(fp, "%s %d\n", username, hashed);
    fclose(fp);

    printf("User registered successfully!\n");

    // ✅ LOGGING (only added part)
    FILE *log_fp = fopen(LOG_FILE, "a");
    if (log_fp != NULL) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        fprintf(log_fp, "[REGISTER] %s at %04d-%02d-%02d %02d:%02d:%02d\n",
                username,
                t->tm_year + 1900,
                t->tm_mon + 1,
                t->tm_mday,
                t->tm_hour,
                t->tm_min,
                t->tm_sec);

        fclose(log_fp);
    }
}