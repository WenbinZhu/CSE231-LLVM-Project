// Wenbin Zhu A53211044
#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

namespace {
    struct BranchBias : public FunctionPass {
        static char ID;
        BranchBias() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            Module *M = F.getParent();
            LLVMContext &Context = M->getContext();

            Function *update_func = dyn_cast<Function>(M->getOrInsertFunction("updateBranchInfo", Type::getVoidTy(Context), Type::getInt1Ty(Context), NULL));
            Function *print_func = dyn_cast<Function>(M->getOrInsertFunction("printOutBranchInfo", Type::getVoidTy(Context), NULL));
            
            // Iterate over all basic blocks
            for (Function::iterator blk = F.begin(), blk_end = F.end(); blk != blk_end; ++blk) {
                IRBuilder<> Builder(&*blk);
                Builder.SetInsertPoint(blk->getTerminator());

                // Branch must be at the end of a basic block
                BranchInst *branch_inst = dyn_cast<BranchInst>(blk->getTerminator());

                if (branch_inst != NULL && branch_inst->isConditional()) {
                    vector<Value *> args;
                    args.push_back(branch_inst->getCondition());

                    Builder.CreateCall(update_func, args);
                }

                for (BasicBlock::iterator ist = blk->begin(), end = blk->end(); ist != end; ++ist) {
                    if ((string) ist->getOpcodeName() == "ret") {
                        Builder.SetInsertPoint(&*ist);
                        Builder.CreateCall(print_func);
                    }
                }
            }

            return false;
        }
    }; // end of struct TestPass
}  // end of anonymous namespace

char BranchBias::ID = 0;
static RegisterPass<BranchBias> X("cse231-bb", "Compute Branch Bias",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
