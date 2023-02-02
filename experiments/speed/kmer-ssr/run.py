# import os, psutil
# from memory_profiler import profile, memory_usage
import os
import subprocess
import argparse
from time import sleep, time


ap = argparse.ArgumentParser()
ap.add_argument("-i", "--input", type=str, required=True, help="Path to the genome sequence file.")
ap.add_argument("-o", "--output", type=str, required=True, help="Path the directory of the result file.")
args = vars(ap.parse_args())
experiment = 1  # default: microsatellite

def run(chr):
    cmd = f"kmer-ssr -p 1-{6 if experiment == 1 else 100} -r 2 -i {args['input']}  -o {args['output']}/{chr}-output.txt"
    os.system(cmd)


if __name__ == '__main__':
    program_name = 'TRF'
    
    experiment = int(input("Experiment number: 1: microsatellite , 2: satellite"))

    for i in range(1, 6):
        print("===" * 10)
        print(f"[PROCESS START]:\tRunning {program_name} on chromosome = {i}")

        t0 = time()
        run(chr=i)
        t1 = time();

        print(f"[PROCESS END] Processing finished on chromosome {i} in {t1 - t0} seconds\n\n")
