# MRA — Multi-head Reader Arm Algorithm for Tandem Repeat Discovery

MRA finds all exact and inexact tandem repeats in a genome sequence in a single scan. It supports any repeating pattern length range in one run, covering microsatellites, minisatellites, and satellites simultaneously.

## How it works

The algorithm places one read head per pattern size of interest, plus a shared base head, and advances all heads one character at a time. Each pair (base head, pattern head) independently detects tandem repeats of its assigned pattern size. In inexact mode, adjacent compatible TRs are merged using Levenshtein edit distance on the gap between them. The time complexity is O(n·m), where n is the sequence length and m is the number of pattern sizes searched.

See the paper in the [Citation](#citation) section for full algorithm details.

## Source structure

```
src/
├── mra.c        — CLI entry point: argument parsing, genome loading, output writing
├── scanner.h    — TandemRepeat, ScannerConfig, ScanResult types; mraScan() declaration
├── scanner.c    — MRA scan loop and inexact TR merging
├── inexact.h    — Declarations for shiftCounter, bsCorrector, levenshtein
├── inexact.c    — String distance utilities used by the inexact mode
├── utils.h      — Declarations for help() and endingStringSize()
└── utils.c      — CLI help text and scan buffer sizing
Makefile
```

## Build

**Recommended:**
```bash
make
```

This produces the `MRA` executable in the project root.

For a debug build (no optimisation, debug symbols):
```bash
make debug
```

**Manual (GCC):**
```bash
gcc -std=c99 -O2 -o MRA src/mra.c src/scanner.c src/inexact.c src/utils.c
```

## Usage

```
./MRA -i <input> -o <output> [options]
```

`-i` and `-o` are required. If the input is a FASTA file, add `-F`.

### Options

| Flag | Description | Default |
|------|-------------|---------|
| `-i` | Input file path | — |
| `-o` | Output file path | — |
| `-F` | Input is in FASTA format | off |
| `-m` | Minimum pattern (core) size | 1 |
| `-M` | Maximum pattern (core) size | 100 |
| `-t` | Minimum number of repeats to report a TR | 2 |
| `-T` | Set minimum repeats for all core sizes equal to `-t` | off |
| `-1` | Minimum repeats for core size 1 | 6 |
| `-2` | Minimum repeats for core size 2 | 3 |
| `-3` | Minimum repeats for core size 3 | 2 |
| `-4` | Minimum repeats for core size 4 | 2 |
| `-l` | Minimum total TR length (bp) | 8 |
| `-L` | Maximum total TR length (bp) | unlimited |
| `-I` | Enable inexact TR detection | off |
| `-e` | Characters per alignment window for inexact merging | 6 |
| `-R` | Maximum ratio of gap length to TR length (inexact mode) | 10 |
| `-D` | Output start/end locations only (dot2dot format) | off |
| `-h` | Print help and exit | — |

### Output format

The output is a CSV file with one TR per line:

```
Pattern,Start location,End location,Number of repeats,Inexact
AT,0,11,6,0
```

`Inexact` is `0` for exact TRs and `1` for inexact TRs. Positions are 0-based.

### Examples

Find all exact TRs with pattern sizes 1–6 in a FASTA file:
```bash
./MRA -F -i chr1.fa -o results.csv -m 1 -M 6 -t 3
```

Find both exact and inexact TRs with pattern sizes 8–100:
```bash
./MRA -F -i chr1.fa -o results.csv -m 8 -M 100 -l 20 -I -e 6
```

## Citation

If you use MRA in your research, please cite:

> Naghibzadeh, M., Korminejad, K., Krishnan, V., & Bahmani, A. (2023). *A Novel Multi-head Algorithm to Discover all Tandem Repeats in One Scan of DNA Sequences*. https://doi.org/10.21203/rs.3.rs-2879921/v1

**BibTeX:**
```bibtex
@misc{naghibzadeh2023mra,
  title        = {A Novel Multi-head Algorithm to Discover all Tandem Repeats
                 in One Scan of {DNA} Sequences},
  author       = {Mahmoud Naghibzadeh, Kasra Korminejad,
                 Vandhana Krishnan, Amir Bahmani},
  year         = {2023},
  doi          = {10.21203/rs.3.rs-2879921/v1},
  url          = {https://doi.org/10.21203/rs.3.rs-2879921/v1},
}
```

## License

See [LICENSE](LICENSE).
