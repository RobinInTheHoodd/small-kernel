#pragma once

#include <stdbool.h>
#include <stdint.h>

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes);
int string_length(char s[]);
void reverse(char s[]);
void int_to_string(int n, char str[]);
bool backspace(char s[]);
void append(char s[], char n);
int compare_string(char s1[], char s2[]);
void parse_color_command(char *input);
int strcmp(const char* str1, const char* str2);
char* strtok(char* str, const char* delimiters);
char* strchr(const char* str, int c);
char* strpbrk(const char* str, const char* accept);
bool starts_with(const char *str, const char *prefix);