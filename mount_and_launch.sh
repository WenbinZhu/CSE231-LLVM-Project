#!/bin/bash

SOURCE_PATH=`pwd`/Passes
TESTS_PATH=`pwd`/Tests
OUTPUT_PATH=`pwd`/Output

# MOUNT POINTS IN CONTAINER: 
# --------------------------
#
# SOURCE CODE: /LLVM_ROOT/llvm/lib/Transforms/CSE231_Project
# TESTS: /tests
# OUTPUT: /output

docker run --rm -it -v ${SOURCE_PATH}:/LLVM_ROOT/llvm/lib/Transforms/CSE231_Project -v ${TESTS_PATH}:/tests -v ${OUTPUT_PATH}:/output prodromou87/llvm:3.9
