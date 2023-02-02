# import os, psutil
# from memory_profiler import profile, memory_usage
import os
import subprocess
from time import sleep, time


def run(chr):
    cmd = f"mpirun -np 14  ./dot -c config.cfg  -s ../../datasets/full-sequence/fasta/chr{chr}.fa -o outputs/{chr}"
    os.system(cmd)

if __name__ == '__main__':
    program_name = 'Dot2Dot'

    for i in range(1, 6):
        print("===" * 10)
        print(f"[PROCESS START]:\tRunning {program_name} on chromosome = {i}")

        t0 = time()
        run(chr=i)
        t1 = time();

        print(f"[PROCESS END] Processing finished on chromosome {i} in {t1 - t0} seconds\n\n")
