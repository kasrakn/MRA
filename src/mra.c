/**
 * @file mpa.c
 * @author Kasra Korminejad (korminejad.kasra@gmail.com)
 * @brief 
 * @version 0.2
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "inexact.h"  
#include "utils.h"     


// Default values of options used in the code
#define DEFAULT_CORESIZE_1 6
#define DEFAULT_CORESIZE_2 3
#define DEFAULT_CORESIZE_3 2
#define DEFAULT_CORESIZE_4 2
#define DEFAULT_MIN_CORE 1
#define DEFAULT_MAX_CORE 100
#define DEFAULT_MIN_TR 8
#define DEFAULT_MAX_TR -1
#define DEFAULT_THRESHOLD  2
#define DEFAULT_TABLE_SIZE 10000000
#define DEFAULT_BS_TR_RATIO 10  // len(bluestring) / len(tandemRepeat) must be <= DEFAULT_BS_TR_RATIO

long int counter = 0;

struct TandemRepeat 
{
    int patternSize;
    int inexact;
    long int firstLocation;
    long int endLocation;
    long int repeates;
};

struct Args 
{
    struct TandemRepeat ms;
    int mlen;
};

struct TandemRepeat table[DEFAULT_TABLE_SIZE];

/* Calculate the length of a text file */
long long getGenomeSize(FILE *fp){
    size_t pos = ftell(fp);      // Current position
    fseek(fp, 0, SEEK_END);      // Go to end
    size_t length = ftell(fp);   // read the position which is the size
    fseek(fp, pos, SEEK_SET);    // restore original position
    return length + 1;
}


int main(int argc, char** argv)
{
    clock_t beginTime = clock();

    int minPattern            = DEFAULT_MIN_CORE;
    int maxPattern            = DEFAULT_MAX_CORE;
    int threshold             = DEFAULT_THRESHOLD;
    int minTrLength	          = DEFAULT_MIN_TR;
    int maxTrLength	          = DEFAULT_MAX_TR;
    int maxBsTrRatio          = DEFAULT_BS_TR_RATIO;
    int patternSize1minRepeat = DEFAULT_CORESIZE_1;
    int patternSize2minRepeat = DEFAULT_CORESIZE_2;
    int patternSize3minRepeat = DEFAULT_CORESIZE_3;
    int patternSize4minRepeat = DEFAULT_CORESIZE_4;


    char* inputFilePath  = ""; 
    char* outputFilePath = "";

    int dot2dot       = 0;
    int sameThreshold = 0;
    int isFasta       = 0;
    int inexact       = 0;
    int charPerAlign  = 6;
    int opt;

    // TODO: sort the flags aphabetically.
    /* This while loop is responsible for handling the command line input arguments. */
    while((opt = getopt(argc, argv, ":i:o:t:m:e:M:l:L:R:1:2:3:4:5:hTFDI")) != -1) 
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
            case 'e':
                charPerAlign = atoi(optarg);
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
            case 'R':
                maxBsTrRatio = atoi(optarg);
                break;
            case 'D':
                dot2dot = 1;
                break;
            case 'I':
                inexact = 1;
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
                help();
                break;
        } 
    } 

    /**
    * This if statement ignores the pre-defined values for little size patterns
    * and just set them to the value of the threshold variable
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
        long long genomeSize = getGenomeSize(fp);
        int endingTrSize = endingStringSize(minPattern, threshold, minTrLength);
        long long bufferSize =  genomeSize + endingTrSize + 1;

        char* genome = (char*) malloc(sizeof(char) * bufferSize);

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
            fgets(genome, genomeSize, (FILE*)fp);
        }

        for (int i = 0; i < endingTrSize; i++) {
            genome[genomeSize + i - 1] = '*';
        }
        genome[bufferSize] = '\0';

        // Start of processing
        long long lastPlace = -1;

        while (f[0] < bufferSize){
            int firstOneFound = 0;
            /**
            * The '>' is added to the genome array to separate probable tandem repeats 
            * in end of previous region and in start of the next one 
            */
            if (genome[fb] == '>' || toascii(genome[fb]) == 10) seenMarkers++;

            for(int i = minPattern; i <= maxPattern; i++){    
                if (f[i - minPattern] == bufferSize || firstOneFound) continue;

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

                    /**
                    * Store the tandem repeat in the table If there is a detected tandem repeat 
                    * when the number of the repeats is greater than the specified threshold
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
                                // Check whether the size of the TR satisfies the conditions or not
                                (ps * rc[i - minPattern] >= minTrLength) &&
                                (maxTrLength < 0 || (ps * rc[i - minPattern] <= maxTrLength))
                            ) {
                                int mlen = rc[i - minPattern] * ps;

                                char tandemRepeat[mlen];
                                char motifPattern[mlen];
                                
                                struct TandemRepeat tr;
                                tr.firstLocation = fl[i - minPattern] - seenMarkers;
                                tr.endLocation   = (fl[i - minPattern] - seenMarkers) + (rc[i - minPattern] * ps) - 1;
                                tr.repeates      = rc[i - minPattern];
                                tr.patternSize   = ps;
                                tr.inexact = 0;

                                /**
                                 *  This "if" statement checks for atomicity of the Microsatellite.
                                 * Because tandem repeat with larger pattern sizes are found sooner 
                                 * so if a tandem repeat with the same start location is found, it
                                 * replaces the previous one in the table.
                                */
                                if(fl[i - minPattern] == lastPlace) 
                                    table[counter - 1] = tr;
                                else {
                                    lastPlace = fl[i - minPattern];

                                    // Ignoring the unnecessary TRs (Those overlap with prior TRs more than the size of the pattern of it)
                                    struct TandemRepeat lastTr = table[counter - 1];
                                    if ((lastTr.endLocation - (lastTr.patternSize - 1) < tr.firstLocation) || 
                                        (lastTr.repeates < tr.repeates)
                                    ) {
                                        
                                        // Checking for Inexact TRs
                                        if (inexact) {
                                            if (counter >= 1) {
                                                    struct TandemRepeat core1 = table[counter - 2];
                                                    struct TandemRepeat core2 = table[counter - 1];

                                                    /**
                                                     * calculate the number of required shifts to be identical cores;
                                                    */

                                                    char core1String[core1.patternSize + 1];
                                                    char core2String[core2.patternSize + 1];
                                                    core1String[0] = '\0';    
                                                    core2String[0] = '\0';   


                                                    strncpy(core1String, &genome[core1.firstLocation + seenMarkers], core1.patternSize);
                                                    strncpy(core2String, &genome[core2.firstLocation + seenMarkers], core2.patternSize); 

                                                    core1String[core1.patternSize] = '\0';
                                                    core2String[core2.patternSize] = '\0';

                                                    int shiftCount = shiftCounter(core1String, core2String);


                                                    // If the two TRs are compatible and there is a potential to make a larger TR.
                                                    if (shiftCount >= 0) {

                                                        // It is compatible -> processing the blue string
                                                        // ** bs stands for blue string, which has named in the paper;
                                                        int bsFirstLocation = core1.firstLocation + (core1.patternSize * core1.repeates);
                                                        int bsLastLocation  = core2.firstLocation - 1;
                                                        int bsLength        = bsLastLocation - bsFirstLocation + 1;

                                                        // When two TRs are overlapping then the size of the bluestring is 0.
                                                        if (bsLength < 0) bsLength = 0;

                                                        // The case of having two TRs with a very long bluestring between them is not acceptable.
                                                        if (bsLength / (core1.patternSize * core1.repeates) <= maxBsTrRatio) {
                                                            // char blueString[bsLastLocation - bsFirstLocation + 2];
                                                            char blueString[bsLength + 1];
                                                            blueString[0] = '\0';                                                

                                                            strncpy(blueString, &genome[bsFirstLocation + seenMarkers], bsLength);
                                                            // blueString[bsLastLocation - bsFirstLocation + 1] = '\0';
                                                            blueString[bsLength] = '\0';

                                                            // Initilize a empty string for storing the new substring.
                                                            char newSubstring[bsLength * 2 + core1.patternSize]; // TODO: find a fewer value for the size of the array.
                                                            newSubstring[0] = '\0';

                                                            if (bsLength > 0){

                                                                BsCorrector(newSubstring, blueString, core1String, core2String);
                                                            } else {

                                                                BsCorrector(newSubstring, blueString, core2String, core1String);
                                                            }

                                                            int editDistance = levenshtein(newSubstring, blueString);
                                                            
                                                            // TODO: Edit this to use 1 change per x character
                                                            int desiredEditDistance = 0;
                                                            if (strlen(blueString) > 0)
                                                                desiredEditDistance = bsLength / charPerAlign;

                                                            if (bsLength <= charPerAlign || editDistance <= desiredEditDistance) {
                                                                // The two TRs can join together and make a larger eact TR;

                                                                struct TandemRepeat inexactTR;
                                                                inexactTR.firstLocation = core1.firstLocation;
                                                                inexactTR.endLocation   = core2.endLocation;
                                                                inexactTR.patternSize   = core1.patternSize;
                                                                inexactTR.repeates      = (inexactTR.endLocation - inexactTR.firstLocation) / core1.patternSize;
                                                                inexactTR.inexact = 1;

                                                                table[counter - 2] = inexactTR;
                                                                counter--;
                                                            }

                                                        }

                                                    }
                                                }
                                            } 

                                        if (genome[tr.firstLocation + seenMarkers] != 'N'){
                                            table[counter] = tr;
                                            counter++; 
                                        }
                                    }

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
                if (!dot2dot) {
                    fprintf(fp2, "Pattern");
                    fprintf(fp2, ",Start location");
                    fprintf(fp2, ",End location");
                    fprintf(fp2, ",Number of repeats");
                    fprintf(fp2, ",Inexact\n");
                }

                for (long long i = 0; i < counter - 1; i++){
                    int patternsize         = table[i].patternSize;
                    long int repeats        = table[i].repeates;
                    long long firstLocation = table[i].firstLocation;
                    long long endLocation   = table[i].endLocation;
                    int inexact             = table[i].inexact;

                    if (genome[firstLocation + seenMarkers] != 'N'){
                        if (dot2dot) {
                        fprintf(fp2, "%lld,", firstLocation);
                        fprintf(fp2, "%lld", endLocation);
                        fputs("\n", fp2);
                        } else {
                            for(long long j = firstLocation + seenMarkers; j < firstLocation + seenMarkers + patternsize; j++){
                                fputc(genome[j], fp2);
                            }

                            fprintf(fp2, ",%lld", firstLocation);
                            fprintf(fp2, ",%lld", endLocation);
                            fprintf(fp2, ",%ld", repeats);
                            fprintf(fp2, ",%d", inexact);
                            fputs("\n", fp2);
                        }
                    }
                }
            }
            fclose(fp2);
            // End of saving in file

            fclose(fp);
            free(genome);
        }
        clock_t endTime = clock();
        double time_spent = 0.0;
        time_spent += (double)(endTime - beginTime) / CLOCKS_PER_SEC;

        printf("\nThe execution time: %f seconds\n", time_spent);
        printf("Number of detected tandem repeats: %ld\n", counter - 1);
    }
    
    return 0;
}