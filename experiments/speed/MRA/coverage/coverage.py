import argparse


chr1_len = 248956422
chr2_len = 242193529
chr3_len = 198295559
chr4_len = 190214555
chr5_len = 181538259

chr_lengths = [
    chr1_len,
    chr2_len,
    chr3_len,
    chr4_len,
    chr5_len
]


def sort_trs(line:str):
    splited = line.split(',')
    return int(splited[1])

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--input", type=str, required=True, help="Path to the `directory` that contains the output file of the software")
args = vars(ap.parse_args())

if __name__ == "__main__":
    exp = int(input("Which experiment? 6 or 100\t"))

    sw_outupt_dir = args['input'] + f"{'/' if args['input'][-1] != '/' else ''}"

    for i in range(1, 6):
        print(f"Sequence: chromosome {i}")
        print("experiment: tr-", exp)

        file_path = sw_outupt_dir + f"chr{i}-output.csv"

        with open (file_path, 'r') as inputFile:
            lines = inputFile.readlines()
            data_lines = lines[1:]
            data_lines.sort(key=sort_trs)

            covered_area = 0
            double_covered_area = 0

            for ind, tr_line in enumerate(data_lines):
                spl_line = tr_line.split(',')
                # print(spl_line)

                pattern_size = len(spl_line[0])
                repeats = int(spl_line[3])
                
                if pattern_size == 4 and repeats < 3:
                    continue

                start_tr = int(spl_line[1])
                end_tr = int(spl_line[2])
                tr_length = end_tr - start_tr + 1

                try:
                    next_tr_line = data_lines[ind + 1]
                    next_tr_start = int(next_tr_line.split(',')[1])
                except:
                    pass
                else:
                    if end_tr - next_tr_start >= 0:
                        double_covered_area += (end_tr - next_tr_start)
                
                covered_area += tr_length
            
            print("number of lines: ", len(lines) - 1)
            print("Total tr lens : ", covered_area - double_covered_area)
            print(f"coverage = {((covered_area - double_covered_area) / chr_lengths[i - 1]) * 100} %\n")
            print("total double coverd area: ", double_covered_area)


        

