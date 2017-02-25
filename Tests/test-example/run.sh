#!/bin/bash

# path to clang++, llvm-dis, and opt
LLVM_BIN=/LLVM_ROOT/build/bin
# path to CSE231.so
LLVM_SO=/LLVM_ROOT/build/lib
# path to lib231.c
LIB_DIR=/lib231
# path to the test directory
TEST_DIR=.

$LLVM_BIN/clang -c -O0 $TEST_DIR/test1.c -emit-llvm -S -o /tmp/test1-c.ll
$LLVM_BIN/clang++ -c -O0 $TEST_DIR/test1.cpp -emit-llvm -S -o /tmp/test1.ll
$LLVM_BIN/clang++ -c $LIB_DIR/lib231.cpp -emit-llvm -S -o /tmp/lib231.ll
$LLVM_BIN/clang++ -c $TEST_DIR/test1-main.cpp -emit-llvm -S -o /tmp/test1-main.ll

$LLVM_BIN/opt -load $LLVM_SO/CSE231.so -cse231-csi < /tmp/test1-c.ll > /dev/null 2> /tmp/csi.result
$LLVM_BIN/opt -load $LLVM_SO/CSE231.so -cse231-cdi < /tmp/test1.ll -o /tmp/test1-cdi.bc
$LLVM_BIN/opt -load $LLVM_SO/CSE231.so -cse231-bb < /tmp/test1.ll -o /tmp/test1-bb.bc

$LLVM_BIN/llvm-dis /tmp/test1-cdi.bc
$LLVM_BIN/llvm-dis /tmp/test1-bb.bc

$LLVM_BIN/clang++ /tmp/test1-cdi.ll /tmp/lib231.ll /tmp/test1-main.ll -o /tmp/cdi_test1
$LLVM_BIN/clang++ /tmp/test1-bb.ll /tmp/lib231.ll /tmp/test1-main.ll -o /tmp/bb_test1

/tmp/cdi_test1 2> /tmp/cdi.result
/tmp/bb_test1 2> /tmp/bb.result

