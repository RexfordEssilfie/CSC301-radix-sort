#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "radixsort.c"

#define MAX_TEST_LENGTH 100
#define MAX_NAME_LENGTH 1000

typedef struct test_case
{
    int size;
    int input[MAX_TEST_LENGTH];
    int expected[MAX_TEST_LENGTH];
    char name[MAX_NAME_LENGTH];

} test_case;

bool runTestCase(test_case *t, int num);
void runTests(test_case cases[], int num);
void printTest(int original[], int expected[], int result[], int size);

int main()
{
    test_case cases[] = {
        // 1
        {.name = "Single Digits (Already sorted)",
         .size = 5,
         .input = {1, 2, 3, 4, 5},
         .expected = {1, 2, 3, 4, 5}},

        //2
        {.name = "Single Digits",
         .size = 5,
         .input = {5, 4, 3, 2, 1},
         .expected = {1, 2, 3, 4, 5}},

        // 3
        {
            .name = "Double Digits (Already Sorted)",
            .size = 5,
            .input = {91, 92, 93, 94, 95},
            .expected = {91, 92, 93, 94, 95}},

        // 4
        {.name = "Double Digits",
         .size = 5,
         .input = {15, 34, 16, 20, 72},
         .expected = {15, 16, 20, 34, 72}},

        // 5
        {.name = "Triple Digits",
         .size = 5,
         .input = {206, 105, 910, 377, 100},
         .expected = {100, 105, 206, 377, 910}},

        // 6
        {.name = "Mixed Number of Digits",
         .size = 5,
         .input = {566, 307, 1, 28, 459},
         .expected = {1, 28, 307, 459, 566}},

        // 7
        {.name = "Same Input",
         .size = 5,
         .input = {566, 566, 566, 566, 566},
         .expected = {566, 566, 566, 566, 566}},

        // 8
        {.name = "Single Element",
         .size = 1,
         .input = {1},
         .expected = {1}},

        // 9
        {.name = "Empty Array",
         .size = 0,
         .input = {},
         .expected = {}},

        // 10
        {.name = "All Negative Numbers",
         .size = 5,
         .input = {-1, -8, -2, -5, -4},
         .expected = {-8, -5, -4, -2, -1}},

        // 11
        {.name = "Mixed Negative and Positive Numbers",
         .size = 5,
         .input = {-1, 8, -2, 5, -4},
         .expected = {-4, -2, -1, 5, 8}},

        // 12
        {.name = "Mixed Negative and Positive Numbers of Different Lengths",
         .size = 5,
         .input = {-11, 89, -2, 50, -4},
         .expected = {-11, -4, -2, 50, 89}},

        // 13
        {.name = "Single Negative Number",
         .size = 1,
         .input = {-11},
         .expected = {-11}},

        // 14
        {.name = "All Zeros",
         .size = 5,
         .input = {0, 0, 0, 0, 0},
         .expected = {0, 0, 0, 0, 0}},

        // 15
        {.name = "Negative Same Input",
         .size = 5,
         .input = {-100, -100, -100, -100, -100},
         .expected = {-100, -100, -100, -100, -100}},

    };

    runTests(cases, sizeof(cases) / sizeof(test_case));
    //runTestCase(&cases[12], 12);
    return 0;
}

bool runTestCase(test_case *t, int num)
{
    printf("----------------------------\n");
    printf("Test Case %d: %s\n", num, t->name);

    // Reject faulty test case
    if (t == NULL)
    {
        printf("FAILED: Test Case %d\n (reason: NULL)*", num);
        return false;
    }

    /** Freeze input, i.e copy it into new array before applying sort,
     * so we can compare original with the final results */
    int inputFreeze[MAX_TEST_LENGTH];
    memcpy(inputFreeze, t->input, t->size * sizeof(int));

    /** Apply Radixsort */
    radixsort(t->input, t->size);

    /** Print the outcome of the test */
    printTest(inputFreeze, t->expected, t->input, t->size);

    for (int i = 0; i < t->size; i++)
    {
        if (t->input[i] != t->expected[i])
        {
            printf("FAILED: Test Case %d ❌\n", num);
            return false;
        }
    }

    printf("PASSED: Test Case %d ✅\n", num);
    return true;
}

void printTest(int original[], int expected[], int result[], int size)
{
    printf("\tInput: ");
    pArray(original, size);

    printf("\tExpected: ");
    pArray(expected, size);

    printf("\tResult: ");
    pArray(result, size);
}

void runTests(test_case cases[], int num)
{
    int totalPassed = 0;
    int totalFailed = 0;

    for (int i = 0; i < num; i++)
    {
        bool result = runTestCase(&cases[i], i + 1);
        result == true ? totalPassed++ : totalFailed++;
    }

    printf("Total Passed: %d, Total Failed: %d\n", totalPassed, totalFailed);
    assert(totalFailed == 0);
}
