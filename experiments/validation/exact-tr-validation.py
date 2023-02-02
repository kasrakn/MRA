data_dir = './data'
exp = '/100'
chr = '/chr2'

seq_dir = './datasets/seq'
output_dir = './outputs'



def pattern_validation(tr_pattern: str, start_loc: int, tr_repeat: int):
    return 1 if sequence[start_loc: start_loc + len(tr_pattern)] == tr_pattern else 0


def repeat_validation(tr_pattern: str, start_loc: int, tr_repeat: int):
    return 1 if (sequence[start_loc:start_loc + len(tr_pattern) * tr_repeat] == tr_pattern * tr_repeat) else 0


if __name__ == "__main__":
    outputFile = open(output_dir + exp + chr + '-result.csv', 'w')
    outputFile.write('pattern,start,end,repeat,inexact\n')

    with open(seq_dir + chr + '.txt', 'r') as datasetFile:
        global sequence 
        sequence = datasetFile.readline()

        with open (data_dir + exp + chr + '.csv', 'r') as inpFile:
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

                # if not repeat_validation(pattern, start, repeat):
                #     outputFile.write(
                #         f"repeat,{pattern},{start},{end},{repeat},{inexact}\n"
                #     )

    outputFile.close()

                