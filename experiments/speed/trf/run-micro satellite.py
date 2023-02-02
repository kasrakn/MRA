# import os, psutil
# from memory_profiler import profile, memory_usage
import os
import subprocess
from time import sleep, time


def run(chr):
    # cmd = f"./trf409.linux64 ../../datasets/full-sequence/fasta/chr{chr}.fa  2 5 7 80 10 50 6 -h"
    cmd = f"./trf409.linux64 ../../datasets/full-sequence/fasta/chr{chr}.fa  2 7 7 80 10 50 6 -f -d -m -h"
    os.system(cmd)
    # subprocess.run(["./tandem_lfs", "-m", '1', '-M', '6', '-t', '2', '-i', './dataset2/seq/chr17-seq.txt', '-o', './experiment4/chr-17.txt', '-T'])


if __name__ == '__main__':
    program_name = 'TRF'

    for i in range(1, 6):
        print("===" * 10)
        print(f"[PROCESS START]:\tRunning {program_name} on chromosome = {i}")

        t0 = time()
        run(chr=i)
        t1 = time();

        print(f"[PROCESS END] Processing finished on chromosome {i} in {t1 - t0} seconds\n\n")
