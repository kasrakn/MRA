# import os, psutil
# from memory_profiler import profile, memory_usage
import os
import subprocess
from time import sleep, time


def run(chr):
    cmd = f"./mreps -minperiod 1 -maxperiod 100 -minsize 8 -exp 2  -fasta ../datasets/full-sequence/fasta/chr{chr}.fa > ../term-outputs3/100/chr{chr}-output.txt "
    os.system(cmd)
    # subprocess.run(["./tandem_lfs", "-m", '1', '-M', '6', '-t', '2', '-i', './dataset2/seq/chr17-seq.txt', '-o', './experiment4/chr-17.txt', '-T'])


if __name__ == '__main__':
    program_name = 'Mreps'

    for i in range(1, 6):
        print("===" * 10)
        print(f"[PROCESS START]:\tRunning {program_name} on chromosome = {i}")

        t0 = time()
        run(chr=i)
        t1 = time();

        print(f"[PROCESS END] Processing finished on chromosome {i} in {t1 - t0} seconds\n\n")
