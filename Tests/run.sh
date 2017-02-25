cd /tests/
clang++ -c -O0 -emit-llvm test.cpp -o test.bc
clang++ -c -O0 -emit-llvm lib231.cpp -o lib231.bc
cd /LLVM_ROOT/build/lib/Transforms/CSE231_Project/
make
opt -load /LLVM_ROOT/build/lib/CSE231.so -cse231-cdi < /tests/test.bc -o /tests/test_instrument.bc
cd /tests/
llvm-link lib231.bc test_instrument.bc -o final.bc
llc -filetype=obj final.bc -o final.o
clang++ final.o -o final
./final
