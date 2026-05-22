#ifndef UTILS_H
#define UTILS_H

void help(void);

/*
 * Returns the minimum number of '*' padding characters required at the end of
 * the genome buffer to flush all pending pattern matches through the scan loop.
 */
int endingStringSize(int minPatternSize, int minRepeat, int minLength);

#endif /* UTILS_H */
