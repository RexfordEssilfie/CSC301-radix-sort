/* Academic Honesty Certification
* Written sources used:
* (Include textbook(s), complete citations for web or other written sources.
* Introduction to Algorithms by Cormen et. al
* Note that you are not allowed to use the web for this assignment.
* Write "none" if no sources used.) *
*
*
* Help obtained:
* (Include names of anyone other than the instructor.)
* None.
*
*
* My written or typed signature below confirms that the above list * of sources is complete * Signature: Rexford Essilfie
*/


#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>

int findMaxDigits(int x[], int n);
int digitAtPos(int num, int pos);
int absolute(int n); // Intentionally redefined since stdlib.h has conflicts with this implementation of radixsort
int checkPositive(int n);

void radixsort(int x[], int n);
void stablesort(int x[], int n, int digitPosition);
void stableabsolutesort(int x[], int n, int digitPosition);
void stablesignsort(int x[], int n);
void pArray(int x[], int n);

/**
 * Procedure:
 *  radixsort
 * Purpose: 
 *  sorts a given array of integers in ascending order
 * Parameters:
 *  x, an array of integers
 *  n, the number of elements in x
 * Produces:
 *  [None]
 * Preconditions:
 *  n must be greater than 0 AND x must have at least one element
 *  each of x is a base 10 number
 * Postconditions:
 *  result contains each of the numbers originally in x
*/
void radixsort(int x[], int n)
{
    /** For runtime Big-Theta(d(n+k)), d=maxDigits*/
    int maxDigits = findMaxDigits(x, n);

    /** Loop through from last digit position to the first.
     * eg. for the number, a = 546,
     *  we have,    maxDigits = 3
     *  so go from  pos = 0, where a[0] = 6
     *  then to     pos = 1, where a[1] = 4,
     *  then to,    pos = 2, where a[2] = 5
    */
    for (int pos = 0; pos < maxDigits; pos++)
    {
        //printf("Current Digit Position: %d\n", pos);
        stablesort(x, n, pos);

        //printf("SORT AFTER DIGIT POSITION, %d ---->", pos);
        //pArray(x, n);
    }
}

/**
 * Procedure:
 *  stablesort
 * Purpose: 
 *  sorts an array of elements in ascending order
 * Parameters:
 *  x, an array of integers
 *  n, the number of elements in x
 * Produces:
 *  [None]
 * Preconditions:
 *  n must be greater than 0 AND x must have at least one element
 *  each of x is a base 10 number
 * Postconditions:
 *  result contains each of the numbers originally in x
*/
void stablesort(int x[], int n, int digitPosition)
{
    stableabsolutesort(x, n, digitPosition);
    //printf("BEFORE SIGN SORT ---> ");
    //pArray(x, n);
    stablesignsort(x, n);
}

/**
 * Procedure:
 *  stablesignsort
 * Purpose: 
 *  sorts an array of elements sorted by their absolute values, in order of their signs
 * Parameters:
 *  x, an array of integers
 *  n, the number of elements in x
 * Produces:
 *  [None]
 * Preconditions:
 *  x must be sorted in ascending order of the absolute values of elements in x
 *  n must be greater than 0 AND x must have at least one element
 *  each of x is a base 10 number
 * Postconditions:
 *  result contains each of the numbers originally in x
*/
void stablesignsort(int x[], int n)
{
    /** Two possible signs: + or -*/
    int NUM_SIGNS = 2;

    /** Initialize space for the sorted array of this iteration */
    int sorted[n];

    /** Initialize array c, which will store the correct (sorted) index positions of numbers in x as
     * given by their digitPosition's and sign to zero
     */
    int c[NUM_SIGNS];
    for (int i = 0; i <= NUM_SIGNS; i++)
    {
        c[i] = 0;
    }

    //printf("C ARRAY (SIGNS) --> ");
    //pArray(c, NUM_SIGNS);

    /** Count the number of occurrences of positive and negative numbers */
    for (int i = 0; i < n; i++)
    {
        int indexBasedOnSign = checkPositive(x[i]);
        c[indexBasedOnSign] += 1;
    }
    //printf("C ARRAY (SIGNS) --> ");
    //pArray(c, NUM_SIGNS);

    /** Combine the elements in c, such that each index position contains the number of digits
     * that are less than or equal to the index.
     */
    for (int i = 1; i <= NUM_SIGNS; i++)
    {
        c[i] += c[i - 1];
    }

    //printf("C ARRAY (SIGNS) --> ");
    //pArray(c, NUM_SIGNS);

    /** We will have to switch direction when sorting positive numbers */
    int startIndexOfPositiveNumbers = -1;

    /** Sort negative numbers. The largest negative number is at the beginning */
    for (int i = 0; i < n; i++)
    {
        int num = x[i];

        if (num >= 0)
        {
            /** Time to switch direction! Store the location of the first positive number and break */
            startIndexOfPositiveNumbers = i;
            break;
        }

        int indexBasedOnSign = checkPositive(num);
        int finalNumPosition = c[indexBasedOnSign] - 1;
        sorted[finalNumPosition] = num;
        c[indexBasedOnSign] -= 1;
    }

    /** Sort positive numbers, from end of array till the start of the first positive number. The largest positive number is at the end of the array*/
    if (startIndexOfPositiveNumbers >= 0)
    {
        for (int i = n - 1; i >= startIndexOfPositiveNumbers; i--)
        {
            int num = x[i];
            int indexBasedOnSign = checkPositive(num);
            int finalNumPosition = c[indexBasedOnSign] - 1;
            sorted[finalNumPosition] = num;
            c[indexBasedOnSign] -= 1;
        }
    }

    // printf("SORTED SIGN ----> ");
    // pArray(sorted, n);

    /** Copy sorted array back into place*/
    memcpy(x, sorted, n * sizeof(int));
}

/**
 * Procedure:
 *  stableabsolutesort
 * Purpose: 
 *  sorts the elements of array x (in place), based on a given digit
 * Parameters:
 *  x, an array of integers
 *  n, the number of elements in x
 *  digit, the digit position to sort on, with the least significant digit starting at position 0
 * Produces:
 *  result, a positive integer if n>0, or NULL if n<=0
 * Preconditions:
 *  each of x is a base 10 number
 * Postconditions:
 *  result >= 1 (each integer has at least one digit)
*/
void stableabsolutesort(int x[], int n, int digitPosition)
{
    /** In Big-Theta(d(n+k), Let k, be the largest digit possible for any digit position of a number in x
     *  We compute this dynamically based on the numbers at digitPosition for all elements in x
    */
    int MAX_DIGIT_VALUE = 0;
    for(int i=0; i<n; i++){
        int number = digitAtPos(x[i], digitPosition);
        if (number>MAX_DIGIT_VALUE){
            MAX_DIGIT_VALUE=number;
        }
    }

    /** Initialize space for the sorted array of this iteration */
    int sorted[n];

    /** Initialize array c, which will store the correct (sorted) index positions of numbers in x as
     * given by their digitPosition's to zero
     */
    int c[MAX_DIGIT_VALUE + 1];
    for (int i = 0; i <= MAX_DIGIT_VALUE; i++)
    {
        c[i] = 0;
    }

    //printf("C ARRAY --> ");
    //pArray(c, MAX_DIGIT_VALUE+1);

    /** Count the number of occurrences of each number in the given digitPosition */
    for (int i = 0; i < n; i++)
    {
        int digit = digitAtPos(x[i], digitPosition);
        c[digit] += 1;
    }

    /** Combine the elements in c, such that each index position contains the number of digits
     * that are less than or equal to the index value.
     */
    for (int i = 1; i <= MAX_DIGIT_VALUE; i++)
    {
        c[i] += c[i - 1];
    }

    //printf("C ARRAY --> ");
    //pArray(c, MAX_DIGIT_VALUE+1);

    /** Place the numbers n, in their correct positions based on the value of digit at digitPosition
     * and the number of elements less than or equal to digit at digitPosition
     */
    for (int i = n - 1; i >= 0; i--)
    {
        int num = x[i];
        int digit = digitAtPos(num, digitPosition);
        int finalNumPosition = c[digit] - 1;

        sorted[finalNumPosition] = num;
        c[digit] -= 1;
    }

    // printf("SORTED ABSOLUTE ----> ");
    // pArray(sorted, n);

    memcpy(x, sorted, n * sizeof(int));
}

/**
 * Procedure:
 *  findMaxDigits
 * Purpose: 
 *  returns the maximum number of digit places in an array of numbers
 * Parameters:
 *  x, an array of integers
 *  n, the number of elements in x
 * Produces:
 *  result, a positive integer if n>0, or NULL if n<=0
 * Preconditions:
 *  [no additional]
 * Postconditions:
 *  result >= 1 (each integer has at least one digit)
*/
int findMaxDigits(int x[], int n)
{
    /** Handle faulty input/empty array */
    if (n <= 0)
    {
        return 0;
    }

    /** Start with absolute value of first number as max.
     * Use absolutes since we will need to find the absolutely largest number
     * to get maximum number of digits.
    */
    int max = absolute(x[0]);

    for (int i = 1; i < n; i++)
    {
        /** Check if current number is absolutely greater than max and update max */
        if (absolute(x[i]) > max)
        {
            max = absolute(x[i]);
        }
    }

    /** Compute the number of digits in the absolute max number as
     * the number of times we would multiply 10 to get max, plus 1
     * Examples:
     * For n=8, log10(8) = 0.XX.., so number of digits is 0 + 1 = 1
     * For n=25, log10(25) = 1.XX.., so number of digits is 1 + 1 = 2
     * For n=25, log10(101) = 2.XX.., so number of digits is 2 + 1 = 3
     */
    int numberOfDigitsInMax = (int)log10(max) + 1;

    return numberOfDigitsInMax;
}

/**
 * Procedure:
 *  digitAtPos
 * Purpose: 
 *  returns the (absolute) number at specified position, where least significat digit starts at position 0,
 *      and position increases by 1 for the next significant digit
 * Parameters:
 *  num, a given number
 *  pos, the desired digit position
 * Produces:
 *  result, a digit that is in num
 * Preconditions:
 *  pos>=0
 * Postconditions:
 *  result is a digit in num
 *  result>=0
 *  result=0 if pos is greater than the position of the most significant digit
 * Example:
 *  For num=576 and pos = 1, (that is we want the number, 7)
 *  divisor = 10^1 = 10
 *  numberWithDigitAtEnd = 576 / 10 = 57
 *  result = 57 % 10 = 7
*/
int digitAtPos(int num, int pos)
{
    /** Make sure that the position is valid */
    assert(pos >= 0);

    /** We will divide the given number by some divisor to make the digit we want the least significant digit. */
    double divisor = pow(10, pos);

    /** Divide the given number by divisor to get our desired digit at the end of the number */
    int numberWithDigitAtEnd = absolute(num) / divisor;

    /** Modulo the numberWithDigitAtEnd by 10 to get our desired digit */
    int result = numberWithDigitAtEnd % 10;

    /** Make sure to return the absolute value in case num was negative*/
    return absolute(result);
}

/**
 * Procedure:
 *  pArray
 * Purpose: 
 *  prints the elements in an array
 * Parameters:
 *  x, an array of integers
 *  n, the number of elements in the array
 * Produces:
 *  [No Additional]
 * Preconditions:
 *  n>=0
 * Postconditions:
 *  [No Additional]
*/
void pArray(int x[], int n)
{
    printf("[");
    for (int i = 0; i < n - 1; i++)
    {
        printf("%d, ", x[i]);
    }
    printf("%d", x[n - 1]);
    printf("]\n");
}

/**
 * Procedure:
 *  absolute
 * Purpose: 
 *  returns the absolute value of a number
 * Parameters:
 *  n, a positive integer
 * Produces:
 *  result, an integer
 * Preconditions:
 *  [No Additional]
 * Postconditions:
 *  result<=n
*/
int absolute(int n)
{
    return (int)sqrt(n * n);
}

/**
 * Procedure:
 *  checkPositive
 * Purpose: 
 *  returns 1 if the given number is positive or 0 otherwise
 * Parameters:
 *  n, an integer
 * Produces:
 *  result, an integer
 * Preconditions:
 *  [No Additional]
 * Postconditions:
 *  result is either 1 or 0
*/
int checkPositive(int n)
{
    return n > 0 ? 1 : 0;
}