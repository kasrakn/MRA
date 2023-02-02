# MRA - A Novel Multi-head Algorithm to Discover all Tandem Repeats in One Scan of DNA Sequences
MRA is an efficient software for finding repetitive patterns (tandem repeats) in genome sequences in both *exact* and *inexact* modes.
<br>
<br>
## **Compile the source code**:
In order to run MRA software, the 'MRA' executable file is provided on the root of the directory and in the releases section of this repository. However, if you want to compile the source code, you can use the command ```make```, which generates the 'MRA' executable file in the root of the directory for you.
### **Compile manually**: Users are also capable of compiling this code using any C language compiler. Here we have described the command needed to compile the code using the GCC compiler with optimization level 3. This optimization helps the program to run much faster, while at first time, it might take longer to compile compared to the other levels. You can read more about GCC optimizations at [GCC Optimization Options]('https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html').

```
gcc -O3 -o -w MRA src/mra.c
```

<br>

## **How to run the executable file?**
<br>

### **Parameters**:
Firstly, to run the program, you need to specify the path to the input and output files. This can be done by using -i and -o flags (These two flags are mandatory). If the input file is in the FASTA format, -F flag is also required. Not mentioning this flag when using a FASTA format input can cause some errors during the execution.\
Other flags are used to specify the properties of the tandem repeats. If not specified, their default values are replaced.

<br>

| Flag | Definition                                                                                                  | Default |
|------|-------------------------------------------------------------------------------------------------------------|---------|
|  -1  | Minimum number of repeats for core size 1                                                                   |    6    |
|  -2  | Minimum number of repeats for core size 2                                                                   |    3    |
|  -3  | Minimum number of repeats for core size 3                                                                   |    2    |
|  -4  | Minimum number of repeats for core size 4                                                                   |    2    |
|  -D  | This flag should be used only for experience 4. The output will contain only start start and end locations. |         |
|  -e  | Number of characters per align                                                                              |    6    |
|  -F  | This flag should be used when the input sequence is in FASTA format                                         |         |
|  -h  | Help - Guide for how to run the code                                                                        |         |
|  -I  | This flag is to set the program to find inexact TRs as well as exacts ones                                  |         |
|  -i  | Input file path                                                                                             |         |
|  -L  | Maximum length of a tandem repeat                                                                           |    50   |
|  -l  | Minimum length of a tandem repeat                                                                           |    8    |
|  -M  | Maximum size of a pattern                                                                                   |   100   |
|  -m  | Minimum size of a pattern                                                                                   |    1    |
|  -o  | Output file path                                                                                            |         |
|  -R  | The largest ratio of the length of inexact tandem repeat to its prior exact TR                              |    10   |
|  -T  | Sets the threshold of all core size equal to threshold value (see -t flag description)                      |         |
|  -t  | Minimum number of repeats defines if a tandem repeat is acceptable or not                                   |    2    |

#### **Example**:
```
./MRA -m 8 -M 100 -l 20 -L 200 -t 5 -2 4 -3 4 -F -i -I -e 6 inputSeq.fa -o output-results.txt
```

