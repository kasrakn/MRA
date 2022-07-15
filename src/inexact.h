#include <string.h>


/**
 * @brief Calculates the absolute value of the n
 * 
 * @param n An integer number
 * @return int 
 */
inline int abs(int n) {
    int mask = n >> (sizeof(int) * __CHAR_BIT__ - 1);
    return (mask + n) ^ mask;
}

/**
 * @brief returns the minimum number between two numbers.
 * 
 * @param a First number
 * @param b Second Nu
 * @return long int 
 */
static inline long int minl(long int a, long int b) {
    return (a >= b) ? b : a;
}

/**
 * @brief calculates the number of left shifts to convert
 *        the core of the second TR to the core of the 
 *        first TR. It returns the number of shifts if it
 *        the cores can be coverted, and -1 if they cannot
 *        be converted to each other.
 * 
 * @param core1 The core of the first TR in the form of string
 * @param core2 The core of the second TR in the form of string
 * @return int 
 */
int shiftCounter(char* core1, char* core2) {
    int coreLength = strlen(core1);
    int counter = 0;
    int found = 0;

    char shifted[coreLength + 1];
    char buffer[coreLength + 1];

    strcpy(shifted, core1);

    while (counter < coreLength)
    {
        strcpy(buffer, shifted);

        if (strcmp(shifted, core2) != 0) 
        {
            for (int i = 0; i < coreLength; i++)
            {
                if (i == 0) 
                    shifted[coreLength - 1] = buffer[0];
                else
                    shifted[i - 1] = buffer[i];
            }
            shifted[coreLength] = '\0';
        } else
            return counter;

        counter++;
    }
    return -1;
}

/**
 * @brief form a new string of core of the first 
 *        tandem repeat to repalce the current
 *        blue string and make a larger TR.
 * 
 * @param output the new substring
 * @param blueString the substring between the two adjacent TRs.
 * @param core1 The core of the first TR in the form of string
 * @param core2 The core of the second TR in the form of string
 * 
 * If the cores are identical or cicular-shifted identical, they are compatible,
 * so that it is possible to make a new substring to replace the blue string and
 * make a larger exact tandem repeat.
 * If the cores are not identical or circular-shifted indentical, they are not 
 * compatible so it is not possible to make a new substring to replace the blue 
 * string.
 * 
 *  @return int (1 as successful process, and 0 as a failure in forming the string)
 */
int BsCorrector(char* output, char* blueString, char* core1, char* core2) {
    int differenceValue;
    int addingCount = strlen(blueString) / strlen(core1);
    int shiftCount = shiftCounter(core1, core2);
    
    differenceValue = strlen(blueString) - (addingCount * strlen(core1) + shiftCount);

    if (abs(differenceValue) > strlen(core1) / 2) {
        if (differenceValue < 0) addingCount--;
        else addingCount++;
    }

    for (int i = 0; i < addingCount; i++) {
        strcat(output, core1);
    }


    char portion[shiftCount + 1];
    strncpy(portion, core1, shiftCount);
    portion[shiftCount] = '\0';
    
    strcat(output, portion);
    return 1;
}


/**
 * @brief Computes the minimum edit distance of two strings
 *        using levestein method.
 * 
 * @param mainString The first string involved in the process
 * @param targetString The string that is the purpose to convert
 *                     the first string to it.
 * @return int
 */
int levenshtein(char* mainString, char* targetString) {
    long int mainStringLen = strlen(mainString);
    long int targetStringLen = strlen(targetString);

    // long int distance[mainStringLen + 1][targetStringLen + 1];
    long int (*distance)[targetStringLen + 1] = malloc( (mainStringLen + 1) * (targetStringLen + 1) * sizeof(long int));

    int insertCost = 1;
    int deleteCost = 1;
    int changeCost = 1;

    memset(distance, 0, sizeof(distance[0][0]) * (mainStringLen + 1) * (targetStringLen + 1));

    // source prefixes can be transformed into empty string by
    // dropping all characters
    for (long int i = 1; i <= mainStringLen; i++){
        distance[i][0] = distance[i - 1][0] + deleteCost;
    }

    // target prefixes can be reached from empty source prefix
    // by inserting every character
    for (long int j = 1; j <= targetStringLen; j++){
        distance[0][j] = distance[0][j - 1] + insertCost;
    }

    for (long int j = 1; j <= targetStringLen; j++){
        for (long int i = 1; i <=  mainStringLen; i++) {

            long int minID = minl(distance[i - 1][j] + deleteCost, distance[i][j - 1] + insertCost);
            int subtitueCost;
            
            if (mainString[i - 1] != targetString[j - 1])
                subtitueCost = changeCost;
            else
                subtitueCost = 0;

            distance[i][j] = minl(minID, distance[i - 1][j - 1] + subtitueCost);
        }
    }


    // Print the table
    // for (long int j = 0; j <= targetStringLen; j++) {
    //     for (long int i = 0; i <= mainStringLen; i++) {
    //         printf("%ld    ", distance[i][j]);
    //     }
    //     printf("\n");
    // }
    return distance[mainStringLen][targetStringLen];
}
