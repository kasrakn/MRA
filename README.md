# MPA: A Novel Millipede Algorithm to Discover all Tandem Repeats in One Scan of DNA 
MPA is an efficient software written in C language to find perfect tandem repeats in a genome sequence.
<br>
<br>
## **Compile the source code**:
To compile this code, you can use the GCC compiler. Also, you can compile it with any other available compilers for the C language. To compile this code with GCC compiler, use the command below:

```
gcc -O3 -o -w tandem tandem.c
```
As can be seen in the command above, optimization number 3 has been used. This option can help to run faster, especially in larger genome sequences. You can read more about it [here]('https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html').

<br>

## **Run the executable file**:

### **Parameters**:
To run the program, firstly, you need to specify the path of the input and output files. This can be done by using -i and -o flags (These two flags are mandatory). If the input file is in the FASTA format, -F flag is also required. Not using this flag with a FASTA input can cause some errors during the execution.\
Other flags are used to specify the properties of the tandem repeats. If not specified, their default values are replaced.

<br>

	-m	 Minimum size of a core (default = 1)

	-M	 Maximum size of a core (default = 100)

	-l	 Minimum length of a tandem repeat (default = 8)

	-L	 Maximum length of a tandem repeat (default = 50)

	-t	 Minimum number of repeats defines if a tandem repeat is acceptable or not (default = 2)

	-T	 Sets the threshold of all core size equal to threshold value (see -t flag discription)

	-F	 This flag should be used when the input sequence is in FASTA format

	-1	 Minimum number of repeats for core size 1

	-2	 Minimum number of repeats for core size 2

	-3	 Minimum number of repeats for core size 3

	-4	 Minimum number of repeats for core size 4

	-i	 input file path

	-o	 output file path

	-h	 show the help

<br>

#### **Example**:
```
./mpa.bin -m 8 -M 100 -l 20 -L 200 -t 5 -2 4 -3 4 -F -i inputSeq.fa -o output-results.txt
```

