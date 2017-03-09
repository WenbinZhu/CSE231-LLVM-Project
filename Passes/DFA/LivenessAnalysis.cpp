//Wenbin Zhu A53211044
#include "231DFA.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

class LivenessInfo : public Info {
	private:
		set<unsigned> infoSet;

	public:
		LivenessInfo() {}

		LivenessInfo(set<unsigned> s) {
			infoSet = s;
		}

		// getter and setter
		set<unsigned> & getInfoSet() {
			return infoSet;
		}

		void setInfoSet(set<unsigned> s) {
			infoSet = s;
		}

		void print() {
			for (auto it = infoSet.begin(); it != infoSet.end(); ++it) {
				errs() << *it << '|';
			}

			errs() << '\n';
		}

		static bool equals(LivenessInfo * info1, LivenessInfo * info2) {
			return info1->getInfoSet() == info2->getInfoSet();
		}

		static LivenessInfo* join(LivenessInfo * info1, LivenessInfo * info2, LivenessInfo * result) {
			set<unsigned> retSet = info1->getInfoSet();
			set<unsigned> info2Set = info2->getInfoSet();

			for (auto it = info2Set.begin(); it != info2Set.end(); ++it) {
				retSet.insert(*it);
			}

			result->setInfoSet(retSet);

			return nullptr;
		}

		// Remove info2 from info1
		static LivenessInfo* remove(LivenessInfo * info1, LivenessInfo * info2, LivenessInfo * result) {
			set<unsigned> retSet = info1->getInfoSet();
			set<unsigned> info2Set = info2->getInfoSet();

			for (auto it = info2Set.begin(); it != info2Set.end(); ++it) {
				retSet.erase(*it);
			}

			result->setInfoSet(retSet);

			return nullptr;
		}
};


class LivenessAnalysis : public DataFlowAnalysis<LivenessInfo, false> {
	private:
		typedef pair<unsigned, unsigned> Edge;
		map<string, int> opToCategory = {{"alloca", 1}, {"load", 1}, {"select", 1},
										 {"icmp", 1}, {"fcmp", 1}, {"getelementptr", 1},
										 {"br", 2}, {"switch", 2}, {"store", 2}, {"phi", 3}};
	
	public:
		LivenessAnalysis(LivenessInfo & bottom, LivenessInfo & initialState) : 
			DataFlowAnalysis(bottom, initialState) {}

		void flowfunction(Instruction * I, std::vector<unsigned> & IncomingEdges,
									   std::vector<unsigned> & OutgoingEdges,
									   std::vector<LivenessInfo *> & Infos) {
			string opName = I->getOpcodeName();

			// Get category of this instruction
			int category = opToCategory.count(opName) ? opToCategory[opName] : 2;
			category = I->isBinaryOp() ? 1 : category;
			
			std::map<Edge, LivenessInfo *> edgeToInfo = getEdgeToInfo();
			map<Instruction *, unsigned> instrToIndex = getInstrToIndex();
			map<unsigned, Instruction *> indexToInstr = getIndexToInstr();
			
			set<unsigned> operands;
			unsigned instrIdx = instrToIndex[I];
			LivenessInfo * outInfo = new LivenessInfo();

			for (unsigned i = 0; i < I->getNumOperands(); ++i) {
				Instruction *instr = (Instruction *) I->getOperand(i);

				if (instrToIndex.count(instr) != 0)
					operands.insert(instrToIndex[instr]);
			}

			// Join all incoming infos
			for (size_t i = 0; i < IncomingEdges.size(); ++i) {
				Edge inEdge = make_pair(IncomingEdges[i], instrIdx);
				LivenessInfo::join(outInfo, edgeToInfo[inEdge], outInfo);
			}

			if (category == 1) {
				set<unsigned> index = {instrIdx};

				LivenessInfo::join(outInfo, new LivenessInfo(operands), outInfo);
				LivenessInfo::remove(outInfo, new LivenessInfo(index), outInfo);

				// Distribute result to outgoing edges
				for (size_t i = 0; i < OutgoingEdges.size(); ++i)
					Infos.push_back(outInfo);
			}

			else if (category == 2) {
				LivenessInfo::join(outInfo, new LivenessInfo(operands), outInfo);

				// Distribute result to outgoing edges
				for (size_t i = 0; i < OutgoingEdges.size(); ++i)
					Infos.push_back(outInfo);
			}

			else {
				set<unsigned> index;
				Instruction * firstNonPhi = I->getParent()->getFirstNonPHI();
				unsigned firstNonPhiIdx = instrToIndex[firstNonPhi];

				for (unsigned i = instrIdx; i < firstNonPhiIdx; ++i)
					index.insert(i);

				LivenessInfo::remove(outInfo, new LivenessInfo(index), outInfo);

				// Handle each output respectively
				for (unsigned k = 0; k < OutgoingEdges.size(); ++k) {
					set<unsigned> operand;
					LivenessInfo *out_k = new LivenessInfo();

					for (unsigned i = instrIdx; i < firstNonPhiIdx; ++i) {
						Instruction *instr = indexToInstr[i];

						for (unsigned j = 0; j < instr->getNumOperands(); ++j) {
							Instruction *opInstr = (Instruction *) instr->getOperand(j);

							if (instrToIndex.count(opInstr) == 0)
								continue;

							// When label_k == label_ij, update out[k]
							if (opInstr->getParent() == indexToInstr[OutgoingEdges[k]]->getParent()) {
								operand.insert(instrToIndex[opInstr]);
								break;
							}
						}
					}
					LivenessInfo::join(outInfo, new LivenessInfo(operand), out_k);
					Infos.push_back(out_k);
				}
			}
		}
};


namespace {
    struct LivenessAnalysisPass : public FunctionPass {
        static char ID;
        LivenessAnalysisPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
        	LivenessInfo bottom;
        	LivenessInfo initialState;
        	LivenessAnalysis * rda = new LivenessAnalysis(bottom, initialState);

        	rda->runWorklistAlgorithm(&F);
        	rda->print();

            return false;
        }
    }; // end of struct LivenessAnalysisPass
}  // end of anonymous namespace

char LivenessAnalysisPass::ID = 0;
static RegisterPass<LivenessAnalysisPass> X("cse231-liveness", "Do liveness analysis on DFA CFG",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
