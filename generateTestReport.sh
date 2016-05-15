#!/bin/bash

# http://ltp.sourceforge.net/coverage/lcov/readme.php 
# paragraph 4

mkdir -p test_results/
lcov -d simulation/ -d _build_test/ -c -o test_results/profile_test.info -q --no-external 
lcov -d simulation/ -d _build_test/ --zerocounters
genhtml -o test_results/ test_results/profile_test.info -q
see test_results/index.html