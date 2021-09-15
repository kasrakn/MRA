#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Default values of options used in the code
#define DEFAULT_CORESIZE_1 6
#define DEFAULT_CORESIZE_2 3
#define DEFAULT_CORESIZE_3 2
#define DEFAULT_CORESIZE_4 2
#define DEFAULT_MIN_CORE 1
#define DEFAULT_MAX_CORE 100
#define DEFAULT_MIN_TR 8
#define DEFAULT_MAX_TR 50
#define DEFAULT_THRESHOLD  2
#define DEFAULT_TABLE_SIZE 10000000

long int counter = 0;

struct TandemRepeat 
{
    int patternSize;
    long int firstLocation;
    long int repeates;
};

struct Args 
{
    struct TandemRepeat ms;
    int mlen;
};

struct TandemRepeat table[DEFAULT_TABLE_SIZE];

/* Calculate the length of a text file */
long long bufferSize(FILE *fp){
    size_t pos = ftell(fp);      // Current position
    fseek(fp, 0, SEEK_END);      // Go to end
    size_t length = ftell(fp);   // read the position which is the size
    fseek(fp, pos, SEEK_SET);    // restore original position
    return length + 1;
}


int main(int argc, char** argv)
{
    int minPattern            = DEFAULT_MIN_CORE;
    int maxPattern            = DEFAULT_MAX_CORE;
    int threshold             = DEFAULT_THRESHOLD;
    int minTrLength	          = DEFAULT_MIN_TR;
    int maxTrLength	          = DEFAULT_MAX_TR;
    int patternSize1minRepeat = DEFAULT_CORESIZE_1;
    int patternSize2minRepeat = DEFAULT_CORESIZE_2;
    int patternSize3minRepeat = DEFAULT_CORESIZE_3;
    int patternSize4minRepeat = DEFAULT_CORESIZE_4;

    char* inputFilePath  = ""; 
    char* outputFilePath = "";

    int sameThreshold = 0;
    int isFasta = 0;
    int opt;

    /* This while loop is responsible for handling the command line input arguments. */
    while((opt = getopt(argc, argv, ":i:o:t:m:M:l:L:1:2:3:4:5:hTF")) != -1) 
    { 
        switch(opt) 
        { 
            case 'i': 
                inputFilePath = optarg; 
                break; 
            case 'o':
                outputFilePath = optarg;
                break; 
            case 'F':
                isFasta = 1;
                break;
            case 'm': 
                minPattern = atoi(optarg); 
                break;
            case 'M':
                maxPattern = atoi(optarg);
                break;
            case 't':
                threshold = atoi(optarg);
                break;
            case 'T':
                sameThreshold = 1;
                break;
            case 'l':
            	minTrLength = atoi(optarg);
              break;
            case 'L':
            	maxTrLength = atoi(optarg);
              break;
            case '1':
                patternSize1minRepeat = atoi(optarg);
                break;
            case '2':
                patternSize2minRepeat = atoi(optarg);
                break;
            case '3':
                patternSize3minRepeat = atoi(optarg);
                break;
            case '4':
                patternSize4minRepeat = atoi(optarg);
                break;
            case '?': 
                printf("Unknown option: %c\n", optopt);
                break; 
            case 'h':
                printf(
                    "DESCRIPTION:\n\tMPA is a software which finds all tandem repeats in a genome sequence.\n\n"
                    "PARAMETERS:"
                    "\n\t-m\t Minimum size of a core (default = 1)"
                    "\n\t-M\t Maximum size of a core (default = 100)"
                    "\n\t-l\t Minimum length of a tandem repeat (default = 8)"
                    "\n\t-L\t Maximum length of a tandem repeat (default = 50)"
                    "\n\t-t\t Minimum number of repeats which defines if a tandem repeats is appropriate or not (default = 2)"
                    "\n\t-T\t Sets the threshold of all core size equal to threshold value (see -t flag discription)"
                    "\n\t-F\t This flag should be used just for FASTA type input files"
                    "\n\t-i\t input file path"
                    "\n\t-o\t output file path"
                    "\n\t-1\t Minimum number of repeats for core size 1"
                    "\n\t-2\t Minimum number of repeats for core size 2"
                    "\n\t-3\t Minimum number of repeats for core size 3"
                    "\n\t-4\t Minimum number of repeats for core size 4"
                );
                exit(0);
                break;
        } 
    } 

    /* 
    This if statement ignores the pre-defined values for little size patterns
    and just set them to the value of the threshold variable
    */
    if (sameThreshold == 1) {
        patternSize1minRepeat = threshold;
        patternSize2minRepeat = threshold;
        patternSize3minRepeat = threshold;
        patternSize4minRepeat = threshold;
    }
    

    int len = maxPattern - minPattern + 1;

    int f2c[len];         // Number of each foot pattern characters
    int rc[len];          // Number of repeats of each foot
    int ft[len];          // (Boolean) Is it the first time which a repeat found for each foot?
    long long fb = 0;     // Base foot current location
    long long f[len];     // Location of feet on the genome sequence except for base foot
    long long  fl[len];   // Start location of tandem repeats which has been appeared for each foot

    /* Initializing the arrays */
    for (int c = 0; c <= maxPattern - minPattern; c++){
        f[c]  = c + minPattern;
        f2c[c] = 0;
        fl[c]  = 0;
        rc[c]  = 0;
        ft[c]  = 1;
    }

    if (inputFilePath == "") {
        printf("ERROR: The input file path is not specified\n");
        return(0);
    }

    if (outputFilePath == "") {
        printf("ERROR: The output file path is not specified\n");
        return(0);
    } else {
        FILE *fp_out = fopen(outputFilePath, "w");
        
        if (fp_out == NULL) {
            printf("ERROR: No such a directory with path %s\n", outputFilePath);
            exit(0);
        } 
        fclose(fp_out);
    }

    FILE *fp;  // FIle Pointer
    fp = fopen(inputFilePath, "r");

    if (fp == NULL)
        printf("\nERROR: No such a genome sequence file in %s\n", inputFilePath);

    else{
        long long n = bufferSize(fp);
        char* genome = (char*) malloc(sizeof(char) * n);
        int lastAddedLoc = 0;
        int seenMarkers = 0;

        int lineLen = 200;
        char line[lineLen];

        // Read the input file and store it into a buffer called "genome"
        if (isFasta) {
            while (fgets(line, lineLen, (FILE*)fp))
            {
                if (line[0] == '>') {
                    genome[lastAddedLoc] = '>';
                    lastAddedLoc++;
                } else {
                    for (int c = 0; c < strlen(line); c++) {
                        if (toascii(line[c] != 10)) {
                            genome[lastAddedLoc] = toupper(line[c]);
                            lastAddedLoc++;
                        }
                    }
                }
            }
        } else {
            fgets(genome, n, (FILE*)fp);
        }

        // Start of processing
        long long lastPlace = -1;

        while (f[0] < n){
            int firstOneFound = 0;
            /* 
            The '>' is added to the genome array for separating probable tandem repeats 
            in end of previous region and in start of the next one 
            */
            if (genome[fb] == '>' || toascii(genome[fb]) == 10) seenMarkers++;

            for(int i = minPattern; i <= maxPattern; i++){    
                if (f[i - minPattern] == n || firstOneFound) continue;

                int ps = i;  // ps stards for pattern size

                // if a repeat is found
                if (genome[fb] == genome[f[i - minPattern]]){
                    f2c[i - minPattern]++;
                    if (f2c[i - minPattern] == ps){
                        if (ft[i - minPattern]){
                            fl[i - minPattern] = fb - ps + 1;
                            ft[i - minPattern] = 0;
                        }
                        
                        f2c[i - minPattern] = 0;
                        rc[i - minPattern]++;
                    } else;
                } else {
                    rc[i - minPattern]++;
                    /* 
                    Store the Microsatellited in the table If there is a detected tandem repeat 
                    in which the repeat count is greater than the specified threshold
                    */
                    if(rc[i - minPattern] >= threshold){
                        // This if statement checks for minimum repeats of pattern size 1 and 2
                        if (
                            !((ps == 1) && (rc[i - minPattern] < patternSize1minRepeat)) &&
                            !((ps == 2) && (rc[i - minPattern] < patternSize2minRepeat)) &&
                            !((ps == 3) && (rc[i - minPattern] < patternSize3minRepeat)) &&
                            !((ps == 4) && (rc[i - minPattern] < patternSize4minRepeat))
                        ) {
                            if (
                                (ps * rc[i - minPattern] >= minTrLength) &&
                                (ps * rc[i - minPattern] <= maxTrLength)
                            ) {
                                int mlen = rc[i - minPattern] * ps;

                                char tandemRepeat[mlen];
                                char motifPattern[mlen];
                                
                                struct TandemRepeat tr;
                                tr.firstLocation = fl[i - minPattern] - seenMarkers;
                                tr.repeates      = rc[i - minPattern];
                                tr.patternSize      = ps;

                                /**
                                 *  This "if" statement checks for atomicity of the Microsatellite.
                                 * Because tandem repeat with larger pattern sizes are found sooner 
                                 * so if a tandem repeat with the same start point will be found, 
                                 * it will be replaced with the previous one in the table.
                                */
                                if(fl[i - minPattern] == lastPlace) 
                                    table[counter - 1] = tr;
                                else {
                                    lastPlace = fl[i - minPattern];
                                    table[counter] = tr;
                                    counter++; 
                                }
                            }
                        }
                    }
                    // Re-initilize the arrays
                    f2c[i - minPattern] = 0;
                    ft[i - minPattern]  = 1;
                    rc[i - minPattern]  = 0;
                }
                f[i - minPattern]++;
            }
            fb++;
        }
        // End the of rocessing 

        // saving into the output file
        if (outputFilePath != ""){
            FILE *fp2 = fopen(outputFilePath, "w");

            if (fp2 != NULL){
                fprintf(fp2, "Pattern");
                fprintf(fp2, "\t   Start location");
                fprintf(fp2, "\t   Number of repeats\n");

                for (long long i = 0; i < counter; i++){
                    int patternsize         = table[i].patternSize;
                    long int repeats        = table[i].repeates;
                    long long firstLocation = table[i].firstLocation;

                    for(long long j = firstLocation + seenMarkers; j < firstLocation + seenMarkers + patternsize; j++){
                        fputc(genome[j], fp2);
                    }

                    for (int i = 0; i < (patternsize / 12); i++) fprintf(fp2, " ");
                    fprintf(fp2, "\t\t%lld", firstLocation);

                    fprintf(fp2, "\t\t\t%ld", repeats);
                    fputs("\n", fp2);
                }
                fprintf(fp2, "\ncounter = %ld", counter);
            }
            fclose(fp2);
            // End of saving in file

            fclose(fp);
            free(genome);
        }
    }
    
    return 0;
}
