#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'queensAttack' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER k
 *  3. INTEGER r_q
 *  4. INTEGER c_q
 *  5. 2D_INTEGER_ARRAY obstacles
 */

int queensAttack(int n, int k, int r_q, int c_q, int obstacles_rows, int obstacles_columns, int** obstacles) {
    int up = n - r_q;
    int down = r_q - 1;
    int right = n - c_q;
    int left = c_q - 1;
    int up_right = (n - r_q < n - c_q) ? (n - r_q) : (n - c_q);
    int up_left = (n - r_q < c_q - 1) ? (n - r_q) : (c_q - 1);
    int down_right = (r_q - 1 < n - c_q) ? (r_q - 1) : (n - c_q);
    int down_left = (r_q - 1 < c_q - 1) ? (r_q - 1) : (c_q - 1);

    for (int i = 0; i < obstacles_rows; i++) {
        int r_o = obstacles[i][0];
        int c_o = obstacles[i][1];

        if (c_o == c_q) {
            if (r_o > r_q) { 
                int dist = r_o - r_q - 1;
                if (dist < up) up = dist;
            } else { 
                int dist = r_q - r_o - 1;
                if (dist < down) down = dist;
            }
        }
        else if (r_o == r_q) {
            if (c_o > c_q) { 
                int dist = c_o - c_q - 1;
                if (dist < right) right = dist;
            } else { 
                int dist = c_q - c_o - 1;
                if (dist < left) left = dist;
            }
        }
        else if (abs(r_o - r_q) == abs(c_o - c_q)) {
            if (r_o > r_q && c_o > c_q) { 
                int dist = r_o - r_q - 1;
                if (dist < up_right) up_right = dist;
            }
            else if (r_o > r_q && c_o < c_q) { 
                int dist = r_o - r_q - 1;
                if (dist < up_left) up_left = dist;
            }
            else if (r_o < r_q && c_o > c_q) { 
                int dist = r_q - r_o - 1;
                if (dist < down_right) down_right = dist;
            }
            else if (r_o < r_q && c_o < c_q) { 
                int dist = r_q - r_o - 1;
                if (dist < down_left) down_left = dist;
            }
        }
    }

    return up + down + left + right + up_right + up_left + down_right + down_left;
}


int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int k = parse_int(*(first_multiple_input + 1));

    char** second_multiple_input = split_string(rtrim(readline()));

    int r_q = parse_int(*(second_multiple_input + 0));

    int c_q = parse_int(*(second_multiple_input + 1));

    int** obstacles = malloc(k * sizeof(int*));

    for (int i = 0; i < k; i++) {
        *(obstacles + i) = malloc(2 * (sizeof(int)));

        char** obstacles_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 2; j++) {
            int obstacles_item = parse_int(*(obstacles_item_temp + j));

            *(*(obstacles + i) + j) = obstacles_item;
        }
    }

    int result = queensAttack(n, k, r_q, c_q, k, 2, obstacles);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
