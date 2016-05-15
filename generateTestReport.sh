#!/bin/bash

# http://ltp.sourceforge.net/coverage/lcov/readme.php 
# paragraph 4

mkdir -p test_results/
lcov --directory simulation/ --capture --output-file test_results/report.info --no-external
lcov --directory simulation/ --zerocounters
genhtml -o test_results/ test_results/report.info
see test_results/index.html