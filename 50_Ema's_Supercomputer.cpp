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
 * Complete the 'twoPluses' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts STRING_ARRAY grid as parameter.
 */

int twoPluses(int grid_count, char** grid) {
    int n = grid_count;
    if (n == 0) return 0;
    int m = strlen(grid[0]);

    int *up = malloc(n * m * sizeof(int));
    int *down = malloc(n * m * sizeof(int));
    int *left = malloc(n * m * sizeof(int));
    int *right = malloc(n * m * sizeof(int));
    if (!up || !down || !left || !right) {
        return 0;
    }

    #define IDX(i,j) ((i) * m + (j))

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'G') {
                up[IDX(i,j)] = (i > 0) ? up[IDX(i-1,j)] + 1 : 1;
                left[IDX(i,j)] = (j > 0) ? left[IDX(i,j-1)] + 1 : 1;
            } else {
                up[IDX(i,j)] = 0;
                left[IDX(i,j)] = 0;
            }
        }
    }

    for (int i = n-1; i >= 0; --i) {
        for (int j = m-1; j >= 0; --j) {
            if (grid[i][j] == 'G') {
                down[IDX(i,j)] = (i < n-1) ? down[IDX(i+1,j)] + 1 : 1;
                right[IDX(i,j)] = (j < m-1) ? right[IDX(i,j+1)] + 1 : 1;
            } else {
                down[IDX(i,j)] = 0;
                right[IDX(i,j)] = 0;
            }
        }
    }

    typedef struct {
        int r, c, k, area;
    } Plus;


    Plus *pluses = NULL;
    int P = 0;
    int cap = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] != 'G') continue;
            int max_arm = up[IDX(i,j)];
            if (down[IDX(i,j)] < max_arm) max_arm = down[IDX(i,j)];
            if (left[IDX(i,j)] < max_arm) max_arm = left[IDX(i,j)];
            if (right[IDX(i,j)] < max_arm) max_arm = right[IDX(i,j)];
            
            max_arm = max_arm - 1;
            for (int k = 0; k <= max_arm; ++k) {
                if (P >= cap) {
                    cap = cap ? cap * 2 : 128;
                    pluses = realloc(pluses, cap * sizeof(Plus));
                    if (!pluses) {
                        free(up); free(down); free(left); free(right);
                        return 0;
                    }
                }
                int area = 4 * k + 1;
                pluses[P].r = i;
                pluses[P].c = j;
                pluses[P].k = k;
                pluses[P].area = area;
                P++;
            }
        }
    }

    int max_product = 0;
    char *visited = malloc(n * m * sizeof(char));
    if (!visited) {
        free(up); free(down); free(left); free(right); free(pluses);
        return 0;
    }

    for (int a = 0; a < P; ++a) {
        memset(visited, 0, n * m);

        int ra = pluses[a].r, ca = pluses[a].c, ka = pluses[a].k;
        visited[IDX(ra,ca)] = 1;
        for (int t = 1; t <= ka; ++t) {
            visited[IDX(ra + t, ca)] = 1;
            visited[IDX(ra - t, ca)] = 1;
            visited[IDX(ra, ca + t)] = 1;
            visited[IDX(ra, ca - t)] = 1;
        }

        for (int b = a + 1; b < P; ++b) {
            int rb = pluses[b].r, cb = pluses[b].c, kb = pluses[b].k;
            int overlap = 0;
            if (visited[IDX(rb, cb)]) {
                overlap = 1;
            } else {
                for (int t = 1; t <= kb && !overlap; ++t) {
                    if (visited[IDX(rb + t, cb)]) { overlap = 1; break; }
                    if (visited[IDX(rb - t, cb)]) { overlap = 1; break; }
                    if (visited[IDX(rb, cb + t)]) { overlap = 1; break; }
                    if (visited[IDX(rb, cb - t)]) { overlap = 1; break; }
                }
            }
            if (!overlap) {
                int prod = pluses[a].area * pluses[b].area;
                if (prod > max_product) max_product = prod;
            }
        }
    }

    free(up); free(down); free(left); free(right); free(pluses); free(visited);

    return max_product;
}


int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int m = parse_int(*(first_multiple_input + 1));

    char** grid = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* grid_item = readline();

        *(grid + i) = grid_item;
    }

    int result = twoPluses(n, grid);

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
