import argparse


ap = argparse.ArgumentParser()
ap.add_argument("-i", "--input", type=str, required=True, help="Path to the output file of the program.")
ap.add_argument("-s", "--seq", type=str, required=True, help="Path to the genome sequence file.")
ap.add_argument("-o", "--output", type=str, required=True, help="Path the directory of the result file.")
args = vars(ap.parse_args())
experiment = 1  # default: microsatellite


def pattern_validation(tr_pattern: str, start_loc: int, tr_repeat: int):
    return 1 if sequence[start_loc: start_loc + len(tr_pattern)] == tr_pattern else 0


def repeat_validation(tr_pattern: str, start_loc: int, tr_repeat: int):
    return 1 if (sequence[start_loc:start_loc + len(tr_pattern) * tr_repeat] == tr_pattern * tr_repeat) else 0


if __name__ == "__main__":
    experiment = int(input("Experiment number: 1: microsatellite , 2: satellite"))

    outputFile = open(args['output'] + '/' + chr + '-result.csv', 'w')
    outputFile.write('pattern,start,end,repeat,inexact\n')

    with open(args['seq'], 'r') as datasetFile:
        global sequence 
        sequence = datasetFile.readline()

        with open (args['input'], 'r') as inpFile:
            lines = inpFile.readlines()[1:]

            for line in lines:
                tr = line.split(',')
                pattern = tr[0]
                start = int(tr[1])
                end = int(tr[2])
                repeat = int(tr[3].strip('\n'))
                inexact = int(tr[4].strip())

                if not pattern_validation(pattern, start, repeat):
                    outputFile.write(
                        f"{pattern},{start},{end},{repeat},{inexact}\n"
                    )
                    print(tr)

    outputFile.close()

                