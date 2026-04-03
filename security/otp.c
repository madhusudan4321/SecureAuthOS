#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "otp.h"

int generate_otp() {
    srand(time(0));  // seed random generator
    return rand() % 9000 + 1000;  // 4-digit OTP
}

int verify_otp(int generated_otp) {
    int user_otp;

    printf("Enter OTP: ");
    scanf("%d", &user_otp);
    getchar(); // clear buffer

    if (user_otp == generated_otp) {
        return 1; // success
    } else {
        return 0; // failure
    }
}