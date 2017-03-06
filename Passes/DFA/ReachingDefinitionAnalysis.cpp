//Wenbin Zhu A53211044
#include "231DFA.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

class ReachingInfo : public Info {
	private:
		set<unsigned> infoSet;

	public:
		ReachingInfo() {}

		ReachingInfo(set<unsigned> s) {
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

		static bool equals(ReachingInfo * info1, ReachingInfo * info2) {
			return info1->getInfoSet() == info2->getInfoSet();
		}

		static ReachingInfo* join(ReachingInfo * info1, ReachingInfo * info2, ReachingInfo * result) {
			set<unsigned> retSet = info1->getInfoSet();
			set<unsigned> info2Set = info2->getInfoSet();

			for (auto it = info2Set.begin(); it != info2Set.end(); ++it) {
				retSet.insert(*it);
			}

			result->setInfoSet(retSet);

			return nullptr;
		}
};


class ReachingDefinitionAnalysis : public DataFlowAnalysis<ReachingInfo, true> {
	private:
		typedef pair<unsigned, unsigned> Edge;
		map<Edge, ReachingInfo *> EdgeToInfo;
		map<string, int> opToCategory = {{"alloca", 1}, {"load", 1}, {"select", 1},
										 {"icmp", 1}, {"fcmp", 1}, {"getelementptr", 1},
										 {"br", 2}, {"switch", 2}, {"store", 2}, {"phi", 3}};
	
	public:
		ReachingDefinitionAnalysis(ReachingInfo & bottom, ReachingInfo & initialState) : 
			DataFlowAnalysis(bottom, initialState) {}

		void flowfunction(Instruction * I, std::vector<unsigned> & IncomingEdges,
									   std::vector<unsigned> & OutgoingEdges,
									   std::vector<ReachingInfo *> & Infos) {
			
			string opName = I->getOpcodeName();

			// Get category of this instruction
			int category = opToCategory.count(opName) ? opToCategory[opName] : 2;
			category = I->isBinaryOp() ? 1 : category;
			unsigned instrIdx = getInstrToIndex()[I];

			std::map<Edge, ReachingInfo *> edgeToInfo = getEdgeToInfo();
			ReachingInfo * outInfo = new ReachingInfo();

			// Join all incoming infos
			for (size_t i = 0; i < IncomingEdges.size(); ++i) {
				Edge inEdge = make_pair(IncomingEdges[i], instrIdx);
				ReachingInfo::join(outInfo, edgeToInfo[inEdge], outInfo);
			}

			// Join index/indices for category 1 and 3
			if (category == 1) {
				set<unsigned> current = {instrIdx};
				ReachingInfo::join(outInfo, new ReachingInfo(current), outInfo);
			}

			else if (category == 3) {
				Instruction * firstNonPhi = I->getParent()->getFirstNonPHI();
				unsigned firstNonPhiIdx = getInstrToIndex()[firstNonPhi];
				set<unsigned> current;

				for (unsigned i = instrIdx; i < firstNonPhiIdx; ++i) {
					current.insert(i);
				}

				ReachingInfo::join(outInfo, new ReachingInfo(current), outInfo);
			}

			// Distribute result to outgoing edges
			for (size_t i = 0; i < OutgoingEdges.size(); ++i)
				Infos.push_back(outInfo);
		}
};


namespace {
    struct ReachingDefinitionAnalysisPass : public FunctionPass {
        static char ID;
        ReachingDefinitionAnalysisPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            ReachingInfo bottom;
            ReachingInfo initialState;
            ReachingDefinitionAnalysis * rda = new ReachingDefinitionAnalysis(bottom, initialState);

            rda->runWorklistAlgorithm(&F);
            rda->print();

            return false;
        }
    }; // end of struct ReachingDefinitionAnalysisPass
}  // end of anonymous namespace

char ReachingDefinitionAnalysisPass::ID = 0;
static RegisterPass<ReachingDefinitionAnalysisPass> X("cse231-reaching", "Do reaching definition on CFG",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
