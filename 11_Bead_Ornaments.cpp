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
 * Complete the 'beadOrnaments' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY b as parameter.
 */

#define MOD 1000000007

long long mod_pow(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

int beadOrnaments(int b_count, int* b) {
    if (b_count == 1) {
        // Only one color: number of trees with b[0] nodes
        return (int)mod_pow(b[0], b[0] - 2);
    }

    long long total_nodes = 0;
    for (int i = 0; i < b_count; i++) {
        total_nodes += b[i];
    }

    long long result = 1;

    // Internal trees for each color group
    for (int i = 0; i < b_count; i++) {
        if (b[i] > 1) {
            result = (result * mod_pow(b[i], b[i] - 2)) % MOD;
        }
    }

    // Product of b[i] for inter-group connections
    long long prod_b = 1;
    for (int i = 0; i < b_count; i++) {
        prod_b = (prod_b * b[i]) % MOD;
    }

    // Multiply by product of b[i] and total_nodes^(b_count - 2)
    result = (result * prod_b) % MOD;
    result = (result * mod_pow(total_nodes, b_count - 2)) % MOD;

    return (int)result;
}


int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int t = parse_int(ltrim(rtrim(readline())));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int b_count = parse_int(ltrim(rtrim(readline())));

        char** b_temp = split_string(rtrim(readline()));

        int* b = malloc(b_count * sizeof(int));

        for (int i = 0; i < b_count; i++) {
            int b_item = parse_int(*(b_temp + i));

            *(b + i) = b_item;
        }

        int result = beadOrnaments(b_count, b);

        fprintf(fptr, "%d\n", result);
    }

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
