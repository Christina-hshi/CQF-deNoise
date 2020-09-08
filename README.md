# CQF-deNoise
CQF-deNoise enables fast k-mer counting with memory usage nearly unaffected by sequencing errors.

## External libraries
- **z**    (at least 1.2.3.5)
- **bz2**
- **boost**

Build
-------

```bash
 $ mkdir release
 $ cd release
 $ cmake ..
 $ make all
```

Two programs ("CQF-deNoise" and "cqf_operations") will be generated.
CQF-deNoise is for counting k-mers.
cqf_operations is for manipulation of the CQFs.
For more details, please run the program with "-h".

## User manual
```
./CQF-deNoise  <options>
Options:
  -h [ --help ]            print help messages
  -k arg                   k-mer size
  -n [ --trueKmer ] arg    number of unique true k-mers
  -N arg                   total number of k-mers
  -e [ --alpha ] arg (=-1) average base error rate, when specified, the 
                           <errorProfile> is ignored
  --errorProfile arg       error profile in a file, each line with error rate 
                           for the corresponding base, e.g. the error rate of 
                           the second base is specified in the second line
  --fr arg (=0)            tolerable rate of true k-mers being wrongly removed,
                           default: 1/<trueKmer>
  --deNoise arg (=-1)      number of rounds of deNoise, when specified, the 
                           <fr> is ignored
  --endDeNoise             call deNoise after processing all the k-mers (not 
                           counted into the <deNoise>)
  -t arg (=16)             number of threads
  -f [ --format ] arg      format of the input: g(gzip); b(bzip2); f(plain 
                           fastq)
  -i [ --input ] arg       a file containing list of input file name(s), should
                           be in the same directory as the fastq file(s)
  -o [ --output ] arg      output file name
```
Users are required to specify either ```<alpha>``` or ```<errorProfile>```. 
If you have no idea about the error rate or the error profile of the sequencing techniques used to generate your data, you can use [ntCard](https://github.com/bcgsc/ntCard.git) to first estimate the k-mer frequency histogram. 
[NtCard](https://github.com/bcgsc/ntCard.git) is quite efficient and it produces a lot of useful statistics, including the total number of k-mers (```<N>```), number of k-mers with different occurrence counts. 
And then by taking k-mers with low counts (e.g. singletons or k-mers with counts <=2) as potential false k-mers and the rest of the k-mers as true k-mers, you can estimate an average base error rate by computing ```1 - (T/N)^(1/k)```, where ```T``` is the total number of true k-mers (please notice that ```T``` is not ```<n>```). 
If you don't know the number of unique true k-mers (```<n>```), you can use the number of k-mers with enough occurrence counts (e.g. >=2 or >=3) as an approximation, or you can use the size of the genome as the approximation of the number of unique true k-mers (```<n>```). 


Following is an example of how to set the parameters to run CQF-deNoise based on the k-mer frequency histogram estimated by ntCard to count the 28-mers in a C.elegans data set.

The output of the ntCard (run with '-k28') contains 
```
F1      22238370946 #this is the total number of k-mers <N>
F0      2004956528  #this is the total number of unique k-mers
f1      1852352895  #this is the number of unique k-mers occuring once
f2      34025186    #this is the number of unique k-mers occuring twice
f3      7174180
f<x>      ....
```

Based on the output of the ntCard, we can set 
```
N = F1 = 22238370946
n = F0 - f1 - f2 = 118578447 (which is similar to the size of the C.elegans genome, ~100 Mb)
e = 1 - ((F1 - f1 - 2*f2)/F1)^(1/28) = 0.00322
```

Authors
-------
- Christina Shi <hshi@cse.cuhk.edu.hk>
- Kevin Yip <kevinyip@cse.cuhk.edu.hk>