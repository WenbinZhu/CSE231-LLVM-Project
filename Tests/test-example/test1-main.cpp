#include <iostream>

extern int foo (volatile unsigned);

int main() {
  foo (0);
  std::cerr << "==================== \n";
  foo (100);
  return 0;
}
