#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_OUTPUT_SIZE 1000000

int main(int argc, char *argv[]) {
    if (argc != 4) {
        return 1;
    }

    const char *pattern = argv[1];
    const char *text = argv[2];
    const char *replacement = argv[3];

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        return 1;
    }

    char output[MAX_OUTPUT_SIZE] = "";
    const char *p = text;
    regmatch_t match;

    while (!regexec(&regex, p, 1, &match, 0)) {
        strncat(output, p, match.rm_so);
        strcat(output, replacement);
        p += match.rm_eo;
    }

    strcat(output, p);
    puts(output);

    regfree(&regex);
    return 0;
}