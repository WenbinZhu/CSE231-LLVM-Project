// Wenbin Zhu A53211044
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"

#include <map>

using namespace llvm;
using namespace std;

namespace {
    struct CountStaticInstructions : public FunctionPass {
        static char ID;
        CountStaticInstructions() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            map<string, int> op_counter;

            // errs() << "Function: " << F.getName() << '\n';

            for (inst_iterator it = inst_begin(F), end = inst_end(F); it != end; ++it) {
                ++op_counter[it->getOpcodeName()];
            }

            for (map<string, int>::iterator it = op_counter.begin(), end = op_counter.end(); it != end; ++it) {
                errs() << it->first << '\t' << it->second << '\n';
            }

            return false;
        }
    }; // end of struct TestPass
}  // end of anonymous namespace

char CountStaticInstructions::ID = 0;
static RegisterPass<CountStaticInstructions> X("cse231-csi", "Count Static Instructions",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
