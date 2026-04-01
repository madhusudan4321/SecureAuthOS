#include "hash.h"

int hash_password(const char *password) {
    int hash = 0;

    while (*password) {
        hash = (hash * 31) + *password;
        password++;
    }

    return hash;
}