/**
 * @file mra.c
 * @author Kasra Korminejad (korminejad.kasra@gmail.com)
 * @brief Multi-head Reader Arm (MRA) — CLI entry point
 * @version 0.2
 * @date 2022-07-05
 *
 * @copyright Copyright (c) 2022
 */

#define _POSIX_C_SOURCE 2

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "scanner.h"
#include "utils.h"

#define DEFAULT_CORESIZE_1  6
#define DEFAULT_CORESIZE_2  3
#define DEFAULT_CORESIZE_3  2
#define DEFAULT_CORESIZE_4  2
#define DEFAULT_MIN_CORE    1
#define DEFAULT_MAX_CORE    100
#define DEFAULT_MIN_TR      8
#define DEFAULT_MAX_TR      -1   /* negative = no upper limit */
#define DEFAULT_THRESHOLD   2
#define DEFAULT_BS_RATIO    10
#define DEFAULT_CHAR_ALIGN  6

static long long getFileSize(FILE *fp) {
    long pos  = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    return (long long)size + 1;
}

/*
 * Reads the genome from fp into a heap-allocated buffer. Appends padSize '*'
 * sentinel characters followed by a null terminator so the scan loop flushes
 * all pending matches. Returns NULL on failure.
 *
 * On success:
 *   *seqLen  — number of sequence characters stored (before padding)
 *   *bufSize — total allocated buffer size
 */
static char *loadGenome(FILE *fp, int isFasta, int padSize,
                         long long *seqLen, long long *bufSize)
{
    long long fileSize = getFileSize(fp);
    *bufSize = fileSize + padSize + 1;

    char *genome = malloc((size_t)*bufSize);
    if (!genome) return NULL;

    *seqLen = 0;

    if (isFasta) {
        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            if (line[0] == '>') {
                genome[(*seqLen)++] = '>';
            } else {
                for (int c = 0; line[c] != '\0'; c++) {
                    if (line[c] != '\n' && line[c] != '\r')
                        genome[(*seqLen)++] = (char)toupper((unsigned char)line[c]);
                }
            }
        }
    } else {
        if (!fgets(genome, (int)fileSize, fp)) genome[0] = '\0';
        *seqLen = (long long)strlen(genome);
    }

    for (int i = 0; i < padSize; i++)
        genome[*seqLen + i] = '*';
    genome[*seqLen + padSize] = '\0';
    *bufSize = *seqLen + padSize + 1;

    return genome;
}

static void writeOutput(
    FILE *fp, const char *genome, long long seenMarkers,
    const TandemRepeat *table, long int count, int dot2dot)
{
    if (!dot2dot)
        fprintf(fp, "Pattern,Start location,End location,Number of repeats,Inexact\n");

    for (long int i = 0; i < count; i++) {
        long long first = table[i].firstLocation;
        long long end   = table[i].endLocation;
        long int  reps  = table[i].repeats;
        int       ps    = table[i].patternSize;
        int       inex  = table[i].inexact;

        if (genome[first + seenMarkers] == 'N') continue;

        if (dot2dot) {
            fprintf(fp, "%lld,%lld\n", first, end);
        } else {
            for (long long j = first + seenMarkers; j < first + seenMarkers + ps; j++)
                fputc(genome[j], fp);
            fprintf(fp, ",%lld,%lld,%ld,%d\n", first, end, reps, inex);
        }
    }
}

int main(int argc, char **argv)
{
    clock_t startTime = clock();

    ScannerConfig cfg = {
        .minPattern            = DEFAULT_MIN_CORE,
        .maxPattern            = DEFAULT_MAX_CORE,
        .threshold             = DEFAULT_THRESHOLD,
        .minTrLength           = DEFAULT_MIN_TR,
        .maxTrLength           = DEFAULT_MAX_TR,
        .maxBsTrRatio          = DEFAULT_BS_RATIO,
        .charPerAlign          = DEFAULT_CHAR_ALIGN,
        .inexactMode           = 0,
        .patternSize1minRepeat = DEFAULT_CORESIZE_1,
        .patternSize2minRepeat = DEFAULT_CORESIZE_2,
        .patternSize3minRepeat = DEFAULT_CORESIZE_3,
        .patternSize4minRepeat = DEFAULT_CORESIZE_4,
    };

    char *inputFilePath  = NULL;
    char *outputFilePath = NULL;
    int   dot2dot        = 0;
    int   sameThreshold  = 0;
    int   isFasta        = 0;
    int   opt;

    while ((opt = getopt(argc, argv, ":i:o:t:m:e:M:l:L:R:1:2:3:4:hTFDI")) != -1) {
        switch (opt) {
        case 'i': inputFilePath              = optarg;             break;
        case 'o': outputFilePath             = optarg;             break;
        case 'F': isFasta                    = 1;                  break;
        case 'm': cfg.minPattern             = atoi(optarg);       break;
        case 'e': cfg.charPerAlign           = atoi(optarg);       break;
        case 'M': cfg.maxPattern             = atoi(optarg);       break;
        case 't': cfg.threshold              = atoi(optarg);       break;
        case 'T': sameThreshold              = 1;                  break;
        case 'l': cfg.minTrLength            = atoi(optarg);       break;
        case 'L': cfg.maxTrLength            = atoi(optarg);       break;
        case 'R': cfg.maxBsTrRatio           = atoi(optarg);       break;
        case 'D': dot2dot                    = 1;                  break;
        case 'I': cfg.inexactMode            = 1;                  break;
        case '1': cfg.patternSize1minRepeat  = atoi(optarg);       break;
        case '2': cfg.patternSize2minRepeat  = atoi(optarg);       break;
        case '3': cfg.patternSize3minRepeat  = atoi(optarg);       break;
        case '4': cfg.patternSize4minRepeat  = atoi(optarg);       break;
        case 'h': help(); break;
        case ':': fprintf(stderr, "Option -%c requires an argument\n", optopt); break;
        case '?': fprintf(stderr, "Unknown option: -%c\n",             optopt); break;
        }
    }

    if (sameThreshold) {
        cfg.patternSize1minRepeat = cfg.threshold;
        cfg.patternSize2minRepeat = cfg.threshold;
        cfg.patternSize3minRepeat = cfg.threshold;
        cfg.patternSize4minRepeat = cfg.threshold;
    }

    if (!inputFilePath) {
        fprintf(stderr, "ERROR: Input file path not specified (-i)\n");
        return 1;
    }
    if (!outputFilePath) {
        fprintf(stderr, "ERROR: Output file path not specified (-o)\n");
        return 1;
    }

    {
        FILE *fp = fopen(outputFilePath, "w");
        if (!fp) {
            fprintf(stderr, "ERROR: Cannot open output file: %s\n", outputFilePath);
            return 1;
        }
        fclose(fp);
    }

    FILE *fp = fopen(inputFilePath, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: Cannot open input file: %s\n", inputFilePath);
        return 1;
    }

    int padSize = endingStringSize(cfg.minPattern, cfg.threshold, cfg.minTrLength);
    long long seqLen, bufSize;
    char *genome = loadGenome(fp, isFasta, padSize, &seqLen, &bufSize);
    fclose(fp);

    if (!genome) {
        fprintf(stderr, "ERROR: Out of memory\n");
        return 1;
    }

    TandemRepeat *table = malloc((size_t)TABLE_CAPACITY * sizeof(TandemRepeat));
    if (!table) {
        fprintf(stderr, "ERROR: Out of memory\n");
        free(genome);
        return 1;
    }

    ScanResult result = mraScan(genome, bufSize, &cfg, table, TABLE_CAPACITY);

    FILE *fp_out = fopen(outputFilePath, "w");
    if (fp_out) {
        writeOutput(fp_out, genome, result.seenMarkers, table, result.count, dot2dot);
        fclose(fp_out);
    } else {
        fprintf(stderr, "ERROR: Cannot write output file: %s\n", outputFilePath);
    }

    free(table);
    free(genome);

    double elapsed = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.3f seconds\n", elapsed);
    printf("Tandem repeats detected: %ld\n", result.count);

    return 0;
}
