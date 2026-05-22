#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void help(void) {
    printf(
        "DESCRIPTION:\n"
        "\tMRA finds all tandem repeats in a genome sequence.\n\n"
        "PARAMETERS:"
        "\n\t-1\t Minimum number of repeats for core size 1."
        "\n\t-2\t Minimum number of repeats for core size 2."
        "\n\t-3\t Minimum number of repeats for core size 3."
        "\n\t-4\t Minimum number of repeats for core size 4."
        "\n\t-D\t Output dot2dot format (start and end columns only)."
        "\n\t-e\t Characters per alignment window for inexact merging (default = 6)."
        "\n\t-F\t Input file is in FASTA format."
        "\n\t-I\t Find inexact TRs in addition to exact ones."
        "\n\t-i\t Input file path."
        "\n\t-L\t Maximum length of a tandem repeat (default = unlimited)."
        "\n\t-l\t Minimum length of a tandem repeat (default = 8)."
        "\n\t-M\t Maximum core size (default = 100)."
        "\n\t-m\t Minimum core size (default = 1)."
        "\n\t-o\t Output file path."
        "\n\t-R\t Maximum ratio of gap length to TR length for inexact merging (default = 10)."
        "\n\t-T\t Set minimum repeats for all core sizes equal to the threshold (-t)."
        "\n\t-t\t Minimum number of repeats to report a tandem repeat (default = 2)."
        "\n"
    );
    exit(0);
}

int endingStringSize(int minPatternSize, int minRepeat, int minLength) {
    int size = minPatternSize * minRepeat;
    while (size < minLength)
        size += minPatternSize;
    return size;
}
