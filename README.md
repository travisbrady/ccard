# CCard [![Build Status](https://travis-ci.org/travisbrady/ccard.svg?branch=master)](https://travis-ci.org/travisbrady/ccard) [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

A single binary written in C to estimate the count of unique strings in a stream of lines on stdin. 
Use `ccard` where you'd use `zcat some_huge_file.txt.gz | sort -u | wc -l`

## Description

This project exists because I often have a need to quickly know the cardinality of something at the command line. 
I've always used standard unix tools for this purpose, but they are much slower than they could be if you're willing to
sacrifice a small amount of accuracy (typically <5%) to get an estimate of the true cardinality.

`ccard` counts unique strings using the Qin, et al's LogLog-Beta algorithm translated mechanically from seiflotfy's
implementation Go [seiflotfy/loglogbeta](https://github.com/seiflotfy/loglogbeta).
Credit is also due to J. Andrew Rogers for his metrohash-c library [jedisct1/metrohash-c](https://github.com/jedisct1/metrohash-c)

## Examples

Using the data files from @seiflotfy's [loglogbeta](https://github.com/seiflotfy/loglogbeta) repo.
The `ratio` field below is computed as `100 - (100 * estimate/exact)`

```
$ make test
file: data/words-1
exact:      150
estimate: 149
ratio: 0.667%

file: data/words-2
exact:     1308
estimate: 1315
ratio: -0.535%

file: data/words-3
exact:    46685
estimate: 47169
ratio: -1.036%

file: data/words-4
exact:   235886
estimate: 232426
ratio: 1.467%

file: data/words-5
exact:   349900
estimate: 346596
ratio: 0.945%

file: data/words-6
exact:   479829
estimate: 477269
ratio: 0.534%
```

### Additional Examples with text8 and others
```bash
# exact
$ cat /usr/share/dict/words | sort -u | wc -l
# approximate with ccard
$ cat /usr/share/dict/words | ./ccard
estimated_cardinality =     236917
```
So in this case `ccard` error is 0.44%

```bash
# on text8 pre-split so as to be one-word-per-line
# using the trivial python script included herein
$ time pigz -c -d data/text8.lines.gz | python uniq.py
    5.96 real         0.45 user         0.10 sys
253855
# the slow way
$ time pigz -c -d data/text8.lines.gz | sort -u | wc -l
    293.16 real         0.51 user         0.09 sys
253855
$ time pigz -c -d data/text8.lines.gz | ./ccard
    1.40 real         0.48 user         0.13 sys
estimated_cardinality =     253801
```
Here `ccard` is off by -0.021%


## License

ccard is licensed under the MIT license and J. Andrew Rogers' metrohash-c library (vendored directly into this project) is as well.

## References

[LogLog-Beta and More: A New Algorithm for Cardinality Estimation Based on LogLog Counting](https://arxiv.org/abs/1612.02284)

```
@misc{1612.02284,
Author = {Jason Qin and Denys Kim and Yumei Tung},
Title = {LogLog-Beta and More: A New Algorithm for Cardinality Estimation Based on LogLog Counting},
Year = {2016},
Eprint = {arXiv:1612.02284},
}
```
