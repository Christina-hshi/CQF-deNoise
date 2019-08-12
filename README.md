# CQF-deNoise
CQF-deNoise enables fast k-mer counting with memory usage unaffected by sequencing errors.

Required libraries
-------

* libboost
* libz
* libbz2


Build
-------

```bash
 $ mkdir release
 $ cmake ..
 $ make all
```

Two programs ("CQF-deNoise" and "cqf_operations") will be produced.
CQF-deNoise is the the program for counting k-mers.
cqf_operations is the program for manipulation of the CQFs.
For more details, please run the program with "-h".


Authors
-------
- Christina SHI <hshi@cse.cuhk.edu.hk>
- Kevin Yip <kevinyip@cse.cuhk.edu.hk>