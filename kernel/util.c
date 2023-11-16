#include <stdbool.h> 
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../drivers/display.h"

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

int string_length(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = string_length(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void int_to_string(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void append(char s[], char n) {
    int len = string_length(s);
    s[len] = n;
    s[len+1] = '\0';
}

bool backspace(char s[]) {
    int len = string_length(s);
    if (len > 0) {
        s[len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}



bool starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*prefix++ != *str++) {
            return false;
        }
    }
    return true;
}

static char* strtok_save;

char* strtok(char* str, const char* delimiters) {
    if (str == NULL) {
        str = strtok_save;
    }
    if (str == NULL) {
        return NULL;
    }
    
    while (*str && strchr(delimiters, *str)) {
        str++;
    }
    if (*str == '\0') {
        strtok_save = NULL;
        return NULL;
    }
    char* token_start = str;
    str = strpbrk(token_start, delimiters);
    if (str) {
        *str = '\0';
        strtok_save = str + 1;
    } else {
        strtok_save = NULL;
    }
    return token_start;
}

char* strchr(const char* str, int c) {
    while (*str) {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}


char* strpbrk(const char* str, const char* accept) {
    while (*str) {
        const char* a = accept;
        while (*a) {
            if (*a++ == *str) {
                return (char*)str;
            }
        }
        str++;
    }
    return NULL;
}


int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}


void parse_color_command(char *input) {
    char *token = strtok(input, " ");
    char *text_color = NULL;
    char *bg_color = NULL;

    while(token != NULL) {
        if (compare_string(token, "-T") == 0) {
            token = strtok(NULL, " ");
            if (token) {
                change_text_color_from_name(token);
            }
        } else if (compare_string(token, "-B") == 0) {
            token = strtok(NULL, " ");
            if (token) {
                change_bg_color_from_name(token);
            }
        } else if (compare_string(token, "-C") == 0) {
            token = strtok(NULL, " ");
            if (token) {
                text_color = token;
                token = strtok(NULL, " ");
                if (token) {
                    bg_color = token;
                    change_colors(text_color, bg_color);
                    return;
                }
            }
        }
        token = strtok(NULL, " ");    
    }
}

