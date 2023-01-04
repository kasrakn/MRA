# A Novel Multi-head Algorithm to Discover all Tandem Repeats in One Scan of DNA Sequences
MRA is an efficient software written in C language to find both **exact** and **inexact** tandem repeats in a genome sequence.
<br>
<br>
## **Compile the source code**:
To compile this code, you can use the GCC compiler. Also, you can compile it with any other available compilers for the C language. To compile it with GCC compiler, use the command below:

```
gcc -O3 -o -w tandem tandem.c
```
As can be seen in the command above, optimization number 3 has been used. This option can help to run faster, especially in larger genome sequences. You can read more about GCC optizations [here]('https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html').

<br>

## **How to run the executable file?**
<br>

### **Parameters**:
Firstly, to run the program, you need to specify the path to the input and output files. This can be done by using -i and -o flags (These two flags are mandatory). If the input file is in the FASTA format, -F flag is also required. Not mentioning this flag when using a FASTA format input can cause some errors during the execution.\
Other flags are used to specify the properties of the tandem repeats. If not specified, their default values are replaced.

<br>

| Flag | Definition                                                                                                  | Default |
|------|-------------------------------------------------------------------------------------------------------------|---------|
|  -m  | Minimum size of a core                                                                                      |    1    |
|  -M  | Maximum size of a core                                                                                      |   100   |
|  -l  | Minimum length of a tandem repeat                                                                           |    8    |
|  -L  | Maximum length of a tandem repeat                                                                           |    50   |
|  -t  | Minimum number of repeats defines if a tandem repeat is acceptable or not                                   |    2    |
|  -T  | Sets the threshold of all core size equal to threshold value (see -t flag description)                      |         |
|  -F  | This flag should be used when the input sequence is in FASTA format                                         |         |
|  -D  | This flag should be used only for experience 4. The output will contain only start start and end locations. |         |
|  -1  | Minimum number of repeats for core size 1                                                                   |    6    |
|  -2  | Minimum number of repeats for core size 2                                                                   |    3    |
|  -3  | Minimum number of repeats for core size 3                                                                   |    2    |
|  -4  | Minimum number of repeats for core size 4                                                                   |    2    |

#### **Example**:
```
./mra.bin -m 8 -M 100 -l 20 -L 200 -t 5 -2 4 -3 4 -F -i -I -e 6 inputSeq.fa -o output-results.txt
```

