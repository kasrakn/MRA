#include "scanner.h"
#include "inexact.h"

#include <string.h>
#include <stdlib.h>

/*
 * Checks whether the last two TRs in the table can be merged into one inexact
 * TR. If they are compatible (same or circularly-shifted pattern) and the gap
 * between them is within the edit-distance budget, replaces both with a single
 * merged TR and decrements *trCount.
 */
static void tryMergeInexact(
    const char   *genome,
    long long     seenMarkers,
    int           charPerAlign,
    int           maxBsTrRatio,
    TandemRepeat *table,
    long int     *trCount)
{
    if (*trCount < 2) return;

    TandemRepeat tr1 = table[*trCount - 2];
    TandemRepeat tr2 = table[*trCount - 1];

    char core1[tr1.patternSize + 1];
    char core2[tr2.patternSize + 1];
    strncpy(core1, &genome[tr1.firstLocation + seenMarkers], tr1.patternSize);
    strncpy(core2, &genome[tr2.firstLocation + seenMarkers], tr2.patternSize);
    core1[tr1.patternSize] = '\0';
    core2[tr2.patternSize] = '\0';

    if (shiftCounter(core1, core2) < 0) return;

    long long bsFirst = tr1.firstLocation + (long long)tr1.patternSize * tr1.repeats;
    long long bsLast  = tr2.firstLocation - 1;
    int bsLen         = (int)(bsLast - bsFirst + 1);
    if (bsLen < 0) bsLen = 0;

    if (bsLen > 0 && bsLen / (tr1.patternSize * (int)tr1.repeats) > maxBsTrRatio) return;

    char blueString[bsLen + 1];
    if (bsLen > 0)
        strncpy(blueString, &genome[bsFirst + seenMarkers], bsLen);
    blueString[bsLen] = '\0';

    char newSubstring[bsLen * 2 + tr1.patternSize + 1];
    newSubstring[0] = '\0';

    if (bsLen > 0)
        bsCorrector(newSubstring, blueString, core1, core2);
    else
        bsCorrector(newSubstring, blueString, core2, core1);

    int editDist    = levenshtein(newSubstring, blueString);
    int allowedEdit = (bsLen > 0) ? bsLen / charPerAlign : 0;

    if (bsLen > charPerAlign && editDist > allowedEdit) return;

    TandemRepeat merged;
    merged.firstLocation = tr1.firstLocation;
    merged.endLocation   = tr2.endLocation;
    merged.patternSize   = tr1.patternSize;
    merged.repeats       = (merged.endLocation - merged.firstLocation) / tr1.patternSize;
    merged.inexact       = 1;

    table[*trCount - 2] = merged;
    (*trCount)--;
}

ScanResult mraScan(
    const char          *genome,
    long long            bufferSize,
    const ScannerConfig *cfg,
    TandemRepeat        *table,
    long int             tableCapacity)
{
    int len = cfg->maxPattern - cfg->minPattern + 1;

    int       f2c[len];
    int       rc[len];
    int       ft[len];
    long long fl[len];
    long long f[len];

    for (int c = 0; c < len; c++) {
        f[c]   = c + cfg->minPattern;
        f2c[c] = 0;
        fl[c]  = 0;
        rc[c]  = 0;
        ft[c]  = 1;
    }

    long long fb          = 0;
    long long lastPlace   = -1;
    long long seenMarkers = 0;
    long int  trCount     = 0;

    while (f[0] < bufferSize) {
        if (genome[fb] == '>' || genome[fb] == '\n')
            seenMarkers++;

        for (int ps = cfg->minPattern; ps <= cfg->maxPattern; ps++) {
            int idx = ps - cfg->minPattern;

            if (f[idx] == bufferSize) continue;

            if (genome[fb] == genome[f[idx]]) {
                f2c[idx]++;
                if (f2c[idx] == ps) {
                    if (ft[idx]) {
                        fl[idx] = fb - ps + 1;
                        ft[idx] = 0;
                    }
                    f2c[idx] = 0;
                    rc[idx]++;
                }
            } else {
                rc[idx]++;

                long long trLength    = (long long)rc[idx] * ps;
                int meetsMinRepeats   =
                    !(ps == 1 && rc[idx] < cfg->patternSize1minRepeat) &&
                    !(ps == 2 && rc[idx] < cfg->patternSize2minRepeat) &&
                    !(ps == 3 && rc[idx] < cfg->patternSize3minRepeat) &&
                    !(ps == 4 && rc[idx] < cfg->patternSize4minRepeat);
                int meetsLengthBounds =
                    trLength >= cfg->minTrLength &&
                    (cfg->maxTrLength < 0 || trLength <= cfg->maxTrLength);

                if (rc[idx] >= cfg->threshold && meetsMinRepeats && meetsLengthBounds) {
                    TandemRepeat tr;
                    tr.firstLocation = fl[idx] - seenMarkers;
                    tr.endLocation   = tr.firstLocation + trLength - 1;
                    tr.repeats       = rc[idx];
                    tr.patternSize   = ps;
                    tr.inexact       = 0;

                    if (fl[idx] == lastPlace) {
                        /* Atomicity: a larger pattern was found at the same start — replace */
                        table[trCount - 1] = tr;
                    } else {
                        lastPlace = fl[idx];

                        TandemRepeat *last = &table[trCount - 1];
                        int noOverlap = (trCount == 0) ||
                            (last->endLocation - (last->patternSize - 1) < tr.firstLocation) ||
                            (last->repeats < tr.repeats);

                        if (noOverlap && trCount < tableCapacity) {
                            if (cfg->inexactMode)
                                tryMergeInexact(genome, seenMarkers,
                                                cfg->charPerAlign, cfg->maxBsTrRatio,
                                                table, &trCount);

                            if (genome[tr.firstLocation + seenMarkers] != 'N') {
                                table[trCount] = tr;
                                trCount++;
                            }
                        }
                    }
                }

                f2c[idx] = 0;
                ft[idx]  = 1;
                rc[idx]  = 0;
            }
            f[idx]++;
        }
        fb++;
    }

    return (ScanResult){ .count = trCount, .seenMarkers = seenMarkers };
}
