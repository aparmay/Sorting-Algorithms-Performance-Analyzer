#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_SIZE 1000
#define INPUT_SIZE 128

typedef struct {
    unsigned long long comparisons;
    unsigned long long swaps;
    unsigned long long shifts;
} SortStats;

static int read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL)
        return 0;

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }

    return 1;
}

static int read_int(const char *prompt, int *value) {
    char line[INPUT_SIZE];
    char *end;
    long parsed;

    for (;;) {
        printf("%s", prompt);

        if (!read_line(line, sizeof(line)))
            return 0;

        errno = 0;
        parsed = strtol(line, &end, 10);

        while (*end == ' ' || *end == '\t' ||
               *end == '\r')
            end++;

        if (line[0] != '\0' && *end == '\0' &&
            errno != ERANGE &&
            parsed >= -2147483647L - 1L &&
            parsed <= 2147483647L) {
            *value = (int)parsed;
            return 1;
        }

        printf("Invalid input. Enter a whole number.\n");
    }
}

static void copy_array(const int source[],
                       int destination[], int n) {
    int i;

    for (i = 0; i < n; i++)
        destination[i] = source[i];
}

static void display_array(const int values[], int n) {
    int i;

    if (n == 0) {
        printf("No data available. Enter numbers first.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        printf("%d", values[i]);

        if (i + 1 < n)
            printf(" ");
    }

    printf("\n");
}

static SortStats selection_sort(int values[], int n) {
    SortStats stats = {0, 0, 0};
    int i, j, min_index, temp;

    for (i = 0; i < n - 1; i++) {
        min_index = i;

        for (j = i + 1; j < n; j++) {
            stats.comparisons++;

            if (values[j] < values[min_index])
                min_index = j;
        }

        if (min_index != i) {
            temp = values[i];
            values[i] = values[min_index];
            values[min_index] = temp;
            stats.swaps++;
        }
    }

    return stats;
}

static SortStats insertion_sort(int values[], int n) {
    SortStats stats = {0, 0, 0};
    int i, j, key;

    for (i = 1; i < n; i++) {
        key = values[i];
        j = i - 1;

        while (j >= 0) {
            stats.comparisons++;

            if (values[j] > key) {
                values[j + 1] = values[j];
                stats.shifts++;
                j--;
            } else {
                break;
            }
        }

        values[j + 1] = key;
    }

    return stats;
}

static int is_sorted(const int values[], int n) {
    int i;

    for (i = 1; i < n; i++) {
        if (values[i - 1] > values[i])
            return 0;
    }

    return 1;
}

static void compare_algorithms(const int original[], int n) {
    int selection[MAX_SIZE];
    int insertion[MAX_SIZE];

    SortStats selection_stats;
    SortStats insertion_stats;

    if (n == 0) {
        printf("Please enter numbers first.\n");
        return;
    }

    copy_array(original, selection, n);
    copy_array(original, insertion, n);

    selection_stats = selection_sort(selection, n);
    insertion_stats = insertion_sort(insertion, n);

    printf("\nSelection Sort result: ");
    display_array(selection, n);

    printf("Comparisons: %llu | Swaps: %llu\n",
           selection_stats.comparisons,
           selection_stats.swaps);

    printf("\nInsertion Sort result: ");
    display_array(insertion, n);

    printf("Comparisons: %llu | Shifts: %llu\n",
           insertion_stats.comparisons,
           insertion_stats.shifts);

    if (is_sorted(selection, n) &&
        is_sorted(insertion, n) &&
        memcmp(selection, insertion,
               (size_t)n * sizeof(int)) == 0) {
        printf("Correctness check: PASSED\n");
    } else {
        printf("Correctness check: FAILED\n");
    }
}

int main(void) {
    int data[MAX_SIZE];
    int n = 0;
    int choice, i, count;

    for (;;) {
        printf("\n=== Sorting Algorithm Performance Analyzer ===\n");
        printf("1. Enter numbers\n");
        printf("2. Display original numbers\n");
        printf("3. Run Selection Sort\n");
        printf("4. Run Insertion Sort\n");
        printf("5. Compare both algorithms\n");
        printf("6. Exit\n");

        if (!read_int("Enter your choice: ", &choice)) {
            printf("\nInput ended. Exiting safely.\n");
            return 0;
        }

        switch (choice) {
            case 1:
                if (!read_int("How many numbers (1-1000)? ",
                              &count)) {
                    printf("\nInput ended. Exiting safely.\n");
                    return 0;
                }

                if (count < 1 || count > MAX_SIZE) {
                    printf("Enter a count between 1 and %d.\n",
                           MAX_SIZE);
                    break;
                }

                n = count;

                for (i = 0; i < n; i++) {
                    char prompt[64];

                    snprintf(prompt, sizeof(prompt),
                             "Enter number %d: ", i + 1);

                    if (!read_int(prompt, &data[i])) {
                        printf("\nInput ended. Exiting safely.\n");
                        return 0;
                    }
                }

                printf("Numbers saved successfully.\n");
                break;

            case 2:
                printf("Original numbers: ");
                display_array(data, n);
                break;

            case 3: {
                int sorted[MAX_SIZE];
                SortStats stats;

                if (n == 0) {
                    printf("Please enter numbers first.\n");
                    break;
                }

                copy_array(data, sorted, n);
                stats = selection_sort(sorted, n);

                printf("Sorted numbers (Selection Sort): ");
                display_array(sorted, n);

                printf("Comparisons: %llu | Swaps: %llu\n",
                       stats.comparisons, stats.swaps);

                printf("Correctness check: %s\n",
                       is_sorted(sorted, n) ? "PASSED" : "FAILED");
                break;
            }

            case 4: {
                int sorted[MAX_SIZE];
                SortStats stats;

                if (n == 0) {
                    printf("Please enter numbers first.\n");
                    break;
                }

                copy_array(data, sorted, n);
                stats = insertion_sort(sorted, n);

                printf("Sorted numbers (Insertion Sort): ");
                display_array(sorted, n);

                printf("Comparisons: %llu | Shifts: %llu\n",
                       stats.comparisons, stats.shifts);

                printf("Correctness check: %s\n",
                       is_sorted(sorted, n) ? "PASSED" : "FAILED");
                break;
            }

            case 5:
                compare_algorithms(data, n);
                break;

            case 6:
                printf("Thank you for using the analyzer.\n");
                return 0;

            default:
                printf("Invalid choice. Select 1 to 6.\n");
        }
    }
}
