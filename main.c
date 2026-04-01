#include <stdio.h>
#include "user/user.h"
#include "auth/auth.h"

int main() {
    int choice;

    while (1) {
        printf("\n===== Secure Authentication System =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        // Input choice safely
        if (scanf("%d", &choice) != 1) {
            printf("❌ Invalid input! Enter a number.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }

        getchar(); // remove leftover newline

        switch (choice) {
            case 1:
                register_user();
                break;

            case 2:
                login_user();
                break;

            case 3:
                printf("Exiting program...\n");
                return 0;

            default:
                printf("❌ Invalid choice! Try again.\n");
        }
    }
}