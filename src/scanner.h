#ifndef SCANNER_H
#define SCANNER_H

typedef struct {
    int       patternSize;
    int       inexact;
    long long firstLocation;
    long long endLocation;
    long int  repeats;
} TandemRepeat;

typedef struct {
    int minPattern;
    int maxPattern;
    int threshold;
    int minTrLength;
    int maxTrLength;           /* negative = no upper limit */
    int maxBsTrRatio;
    int charPerAlign;
    int inexactMode;
    int patternSize1minRepeat;
    int patternSize2minRepeat;
    int patternSize3minRepeat;
    int patternSize4minRepeat;
} ScannerConfig;

typedef struct {
    long int  count;
    long long seenMarkers;
} ScanResult;

#define TABLE_CAPACITY 10000000

ScanResult mraScan(
    const char          *genome,
    long long            bufferSize,
    const ScannerConfig *cfg,
    TandemRepeat        *table,
    long int             tableCapacity
);

#endif /* SCANNER_H */
