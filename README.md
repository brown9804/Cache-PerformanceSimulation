# Cache Performance Simulation
 Cache simulation in order to obtain performance metrics

## University of Costa Rica
#### October, 2020


## Integrants
1. Brown Ramírez, Timna Belinda  B61254
2. Esquivel Molina, Brandon B52571
3. Ramírez Hernández, Jonathan B55730


## Advance cache simulator project baseline

Code baseline for IE0521 cache simulation project on Ubutu 14.04 

### Dependencies
Make sure gtest is install:
```
$ sudo apt-get update
$ sudo apt-get install libgtest-dev
$ sudo apt-get install cmake # install cmake
$ cd /usr/src/gtest
$ sudo cmake CMakeLists.txt
$ sudo make
```
See https://cit.dixie.edu/faculty/larsen/google-test-installation.php for more information

## How to build the project
Create a build directory and run all targets there
```
>> mkdir build
>> cd build
>> cmake ..
>> make <target> (cache or cachetest)
```
## How to run the simulation
The simulation executable is located inside the build directory (src/cache)

```
gunzip -c <trace> | <l1cache executable>  -a <associativity>  -s <cache size KB> -l <block size in bytes> -rp <replacement policy>

Using -  LRU, NRU, RRIP or RANDOM
Ex: gunzip -c ./input/mcf.trace.gz | ./build/src/cache -t 32 -a 4 -l 64 -rp 0
```

## How to run the tests
Go to build and make cachetest. There are several options to run the tests.

1. Run all tests:
```
  ./test/cachetest
```
2. Run only one test:
```
  ./test/cachetest  --gtest_filter=<test_name>
  ##### Google's gtest framework
  Ex: ./test/cachetest  --gtest_filter=L1cache.hit_miss_srrip
  Ex: ./test/cachetest  --gtest_filter=L1cache.hit_miss_lru
  Ex: ./test/cachetest  --gtest_filter=L1cache.hit_miss_nru
  ###### Advanced Optimizations
  Ex: ./test/cachetest  --gtest_filter=L2cache.l1_hit_l2_hit
  Ex: ./test/cachetest  --gtest_filter=VCcache.l1_miss_vc_hit
```
3. Run a set of test using regular expressions
```
./test/cachetest  --gtest_filter=<regexp>
./test/cachetest  --gtest_filter=VC*
```
4. Run a test n times:
```
./test/cachetest  --gtest_filter=<test_name> --gtest_repeat=<n>
Ex: ./test/cachetest  --gtest_filter=L1cache.hit_miss_srrip --gtest_repeat=2
```
5. Replicate test results:
```
  Each test is run with base seed, to replicate the result the same seed must be used
  ./test/cachetest  --gtest_filter=<test_name> --gtest_random_seed=<test_seed>
  ./test/cachetest  --gtest_filter=L1cache.hit_miss_srrip --gtest_random_seed=2126
```  
To enable further debug on the test you need to enable debug_on variable, on your terminal
run:
```
export TEST_DEBUG=1
```
To disable the extra logging, set the  environment variable to zero.



