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
 * Complete the 'almostSorted' function below.
 *
 * The function accepts INTEGER_ARRAY arr as parameter.
 */

void almostSorted(int arr_count, int* arr) {
    // Make a sorted copy
    int* sorted = malloc(arr_count * sizeof(int));
    for (int i = 0; i < arr_count; i++) {
        sorted[i] = arr[i];
    }
    // sort copy
    for (int i = 0; i < arr_count - 1; i++) {
        for (int j = i + 1; j < arr_count; j++) {
            if (sorted[i] > sorted[j]) {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }

    // Find mismatch indices
    int l = -1, r = -1;
    for (int i = 0; i < arr_count; i++) {
        if (arr[i] != sorted[i]) {
            if (l == -1) l = i;
            r = i;
        }
    }

    // Already sorted
    if (l == -1) {
        printf("yes\n");
        free(sorted);
        return;
    }

    // Try swap
    int* temp = malloc(arr_count * sizeof(int));
    for (int i = 0; i < arr_count; i++) temp[i] = arr[i];
    int tmp = temp[l];
    temp[l] = temp[r];
    temp[r] = tmp;

    bool ok = true;
    for (int i = 0; i < arr_count; i++) {
        if (temp[i] != sorted[i]) {
            ok = false;
            break;
        }
    }
    if (ok) {
        printf("yes\nswap %d %d\n", l + 1, r + 1);
        free(sorted);
        free(temp);
        return;
    }

    // Try reverse
    for (int i = 0; i <= (r - l) / 2; i++) {
        int t = arr[l + i];
        arr[l + i] = arr[r - i];
        arr[r - i] = t;
    }

    ok = true;
    for (int i = 0; i < arr_count; i++) {
        if (arr[i] != sorted[i]) {
            ok = false;
            break;
        }
    }
    if (ok) {
        printf("yes\nreverse %d %d\n", l + 1, r + 1);
    } else {
        printf("no\n");
    }

    free(sorted);
    free(temp);
}


int main()
{
    int n = parse_int(ltrim(rtrim(readline())));

    char** arr_temp = split_string(rtrim(readline()));

    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int arr_item = parse_int(*(arr_temp + i));

        *(arr + i) = arr_item;
    }

    almostSorted(n, arr);

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
