#include "inexact.h"

#include <string.h>
#include <stdlib.h>

static int iabs(int n) {
    int mask = n >> (sizeof(int) * __CHAR_BIT__ - 1);
    return (mask + n) ^ mask;
}

static long int minl(long int a, long int b) {
    return a < b ? a : b;
}

int shiftCounter(const char *core1, const char *core2) {
    int len = (int)strlen(core1);
    char shifted[len + 1];
    strcpy(shifted, core1);

    for (int shifts = 0; shifts < len; shifts++) {
        if (strcmp(shifted, core2) == 0)
            return shifts;
        char first = shifted[0];
        memmove(shifted, shifted + 1, len - 1);
        shifted[len - 1] = first;
        shifted[len]     = '\0';
    }
    return -1;
}

int bsCorrector(char *output, const char *blueString,
                const char *core1, const char *core2)
{
    int bsLen   = (int)strlen(blueString);
    int coreLen = (int)strlen(core1);
    int shift   = shiftCounter(core1, core2);
    if (shift < 0) shift = 0;  /* cores must be compatible; guard against misuse */

    int copies = bsLen / coreLen;
    int diff   = bsLen - (copies * coreLen + shift);
    if (iabs(diff) > coreLen / 2) {
        if (diff < 0) copies--;
        else          copies++;
    }

    for (int i = 0; i < copies; i++)
        strcat(output, core1);

    char tail[shift + 1];
    strncpy(tail, core1, shift);
    tail[shift] = '\0';
    strcat(output, tail);

    return 1;
}

int levenshtein(const char *s, const char *t) {
    int m = (int)strlen(s);
    int n = (int)strlen(t);

    long int (*d)[n + 1] = malloc((size_t)(m + 1) * (size_t)(n + 1) * sizeof(long int));
    if (!d) return -1;

    for (int i = 0; i <= m; i++) d[i][0] = i;
    for (int j = 0; j <= n; j++) d[0][j] = j;

    for (int j = 1; j <= n; j++) {
        for (int i = 1; i <= m; i++) {
            int sub = (s[i - 1] != t[j - 1]) ? 1 : 0;
            d[i][j] = minl(minl(d[i-1][j] + 1, d[i][j-1] + 1), d[i-1][j-1] + sub);
        }
    }

    int result = (int)d[m][n];
    free(d);
    return result;
}
