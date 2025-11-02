// reverse_ctf.c
// flag: flag{password}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    const unsigned char KEY = 0x42; /* XOR key */

    /* Each entry = (flag_char) XOR KEY */
    const unsigned char enc[] = {
        36, 46, 35, 37, 57, 50, 35, 49, 49, 53, 45, 48, 38, 63
    };
    const size_t FLAG_LEN = sizeof(enc) / sizeof(enc[0]);

    char input[128];

    printf("Enter flag: ");
    if (!fgets(input, sizeof(input), stdin)) {
        puts("Input error.");
        return 1;
    }

    /* remove trailing newline if present */
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }

    if (len != FLAG_LEN) {
        puts("Wrong length.");
        return 1;
    }

    for (size_t i = 0; i < FLAG_LEN; ++i) {
        unsigned char decoded = enc[i] ^ KEY;
        if ((unsigned char)input[i] != decoded) {
            puts("Incorrect.");
            return 1;
        }
    }

    puts("Correct");

    return 0;
}

