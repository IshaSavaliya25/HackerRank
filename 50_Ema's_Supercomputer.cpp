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

typedef struct {
    int area;
    int cells_count;
    int cells[1000][2];
} Plus;

int twoPluses(int grid_count, char** grid) {
    int n = grid_count;
    int m = strlen(grid[0]);
    
    Plus pluses[10000];
    int plus_count = 0;
    
    // find all possible pluses
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] != 'G') continue;
            
            int maxArm = 0;
            while (1) {
                int ni1 = i - (maxArm + 1);
                int ni2 = i + (maxArm + 1);
                int nj1 = j - (maxArm + 1);
                int nj2 = j + (maxArm + 1);
                if (ni1 < 0 || ni2 >= n || nj1 < 0 || nj2 >= m) break;
                if (grid[ni1][j] != 'G' || grid[ni2][j] != 'G' ||
                    grid[i][nj1] != 'G' || grid[i][nj2] != 'G') break;
                maxArm++;
            }
            
            for (int arm = 0; arm <= maxArm; arm++) {
                Plus p;
                p.area = 4 * arm + 1;
                p.cells_count = 0;
                // center
                p.cells[p.cells_count][0] = i;
                p.cells[p.cells_count][1] = j;
                p.cells_count++;
                // arms
                for (int k = 1; k <= arm; k++) {
                    p.cells[p.cells_count][0] = i - k; p.cells[p.cells_count][1] = j; p.cells_count++;
                    p.cells[p.cells_count][0] = i + k; p.cells[p.cells_count][1] = j; p.cells_count++;
                    p.cells[p.cells_count][0] = i; p.cells[p.cells_count][1] = j - k; p.cells_count++;
                    p.cells[p.cells_count][0] = i; p.cells[p.cells_count][1] = j + k; p.cells_count++;
                }
                pluses[plus_count++] = p;
            }
        }
    }
    
    int maxProd = 0;
    for (int a = 0; a < plus_count; a++) {
        for (int b = a + 1; b < plus_count; b++) {
            bool overlap = false;
            for (int x = 0; x < pluses[a].cells_count && !overlap; x++) {
                for (int y = 0; y < pluses[b].cells_count; y++) {
                    if (pluses[a].cells[x][0] == pluses[b].cells[y][0] &&
                        pluses[a].cells[x][1] == pluses[b].cells[y][1]) {
                        overlap = true;
                        break;
                    }
                }
            }
            if (!overlap) {
                int prod = pluses[a].area * pluses[b].area;
                if (prod > maxProd) maxProd = prod;
            }
        }
    }
    return maxProd;
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
        grid[i] = rtrim(grid_item);   // ✅ fix: remove trailing newline
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
        if (!data) data = '\0';
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
    if (!str) return '\0';
    if (!*str) return str;
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;
    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        if (!splits) return splits;
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
