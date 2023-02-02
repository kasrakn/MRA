#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Display description for arguments for running the executable file.
 *        It is shown using -h flag
 * 
 */
void help() {
    printf(
                    "DESCRIPTION:\n\tMPA is a software which finds all tandem repeats in a genome sequence.\n\n"
                    "PARAMETERS:"
                    "\n\t-1\t Minimum number of repeats for core size 1."
                    "\n\t-2\t Minimum number of repeats for core size 2."
                    "\n\t-3\t Minimum number of repeats for core size 3."
                    "\n\t-4\t Minimum number of repeats for core size 4."
                    "\n\t-D\t This flag should be used only for experience 4."
                    "\n\t-e\t Number of characters per align (default = 6)."
                    "\n\t-F\t This flag should be used just for FASTA type input files."
                    "\n\t-I\t This flag is to set the program to find inexact TRs as well as exacts ones."
                    "\n\t-i\t Input file path."
                    "\n\t-L\t Maximum length of a tandem repeat (default = 50)."
                    "\n\t-l\t Minimum length of a tandem repeat (default = 8)."
                    "\n\t-M\t Maximum size of a core (default = 100)."
                    "\n\t-m\t Minimum size of a core (default = 1)."
                    "\n\t-o\t Output file path."
                    "\n\t-R\t The largest ratio of the length of inexact tandem repeat to its prior exact TR (default = 10)."
                    "\n\t-T\t Sets the threshold of all core size equal to threshold value (see -t flag discription)."
                    "\n\t-t\t Minimum number of repeats which defines if a tandem repeats is appropriate or not (default = 2)."
                );
    exit(0);
};

/**
 * @brief Calcualtes the number of repeats, which is needed to make a 
 *        TR with consists of multiple * characters.
 * 
 * @param minPatternSize Minimum number of repeats acceptable for a TR
 * @param minRepeat Minimum number of repeats that a TR must have.
 * @param minLength Minimum length of a acceptable TR.
 * @return int 
 */
int endingStringSize(int minPatternSize, int minRepeat, int minLength) {
    int repeats = minPatternSize * minRepeat;

    while (repeats < minLength)
    {
        repeats += minPatternSize;
    }

    return repeats;
}