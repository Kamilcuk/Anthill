#!/bin/bash

# http://ltp.sourceforge.net/coverage/lcov/readme.php 
# paragraph 4

mkdir -p test_results/
lcov -d . -c -o test_results/profile_test.info -q --no-external 
lcov -d . --zerocounters
genhtml -o test_results/ test_results/profile_test.info -q
see test_results/index.html