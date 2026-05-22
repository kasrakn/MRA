#ifndef INEXACT_H
#define INEXACT_H

/*
 * Returns the number of left-circular shifts needed to convert core1 into
 * core2, or -1 if no rotation makes them equal.
 */
int shiftCounter(const char *core1, const char *core2);

/*
 * Builds the ideal replacement string for the gap (blue string) between two
 * adjacent compatible TRs. output must be at least
 * strlen(blueString)*2 + strlen(core1) + 1 bytes.
 */
int bsCorrector(char *output, const char *blueString,
                const char *core1, const char *core2);

/*
 * Computes the Levenshtein (edit) distance between strings s and t.
 * Returns -1 on allocation failure.
 */
int levenshtein(const char *s, const char *t);

#endif /* INEXACT_H */
