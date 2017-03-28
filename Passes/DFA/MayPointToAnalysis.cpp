//Wenbin Zhu A53211044
#include "231DFA.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

class MayPointToInfo : public Info {
	private:
		map<pair<char, unsigned>, set<unsigned>> infoMap;

	public:
		MayPointToInfo() {}

		MayPointToInfo(map<pair<char, unsigned>, set<unsigned>> m) {
			infoMap = m;
		}

		// getter and setter
		map<pair<char, unsigned>, set<unsigned>> getInfoMap() {
			return infoMap;
		}

		set<unsigned> getMemSet(pair<char, unsigned> key) {
			return infoMap[key];
		}

		void setInfoMap(map<pair<char, unsigned>, set<unsigned>> m) {
			infoMap = m;
		}

		void print() {
			for (auto ri = infoMap.begin(); ri != infoMap.end(); ++ri) {
				if (ri->second.size() == 0)
					continue;

				errs() << (char) toupper(ri->first.first) << ri->first.second << "->(";

				for (auto mi = ri->second.begin(); mi != ri->second.end(); ++mi) {
					errs() << "M" << *mi << "/";
				}
				errs() << ")|";
			}
			errs() << "\n";
		}

		static bool equals(MayPointToInfo * info1, MayPointToInfo * info2) {
			return info1->getInfoMap() == info2->getInfoMap();
		}

		static MayPointToInfo* join(MayPointToInfo * info1, MayPointToInfo * info2, MayPointToInfo * result) {
			map<pair<char, unsigned>, set<unsigned>> retMap = info1->getInfoMap();
			map<pair<char, unsigned>, set<unsigned>> info2Map = info2->getInfoMap();

			for (auto it = info2Map.begin(); it != info2Map.end(); ++it) {
				retMap[make_pair(it->first.first, it->first.second)].insert(it->second.begin(), it->second.end());
			}

			result->setInfoMap(retMap);

			return nullptr;
		}
};


class MayPointToAnalysis : public DataFlowAnalysis<MayPointToInfo, true> {
	private:
		typedef pair<unsigned, unsigned> Edge;
		const char R = 'R';
		const char M = 'm';
	
	public:
		MayPointToAnalysis(MayPointToInfo & bottom, MayPointToInfo & initialState) : 
			DataFlowAnalysis(bottom, initialState) {}

		void flowfunction(Instruction * I, std::vector<unsigned> & IncomingEdges,
									   std::vector<unsigned> & OutgoingEdges,
									   std::vector<MayPointToInfo *> & Infos) {
			
			string opName = I->getOpcodeName();
			map<Edge, MayPointToInfo *> edgeToInfo = getEdgeToInfo();
			map<Instruction *, unsigned> instrToIndex = getInstrToIndex();
			map<unsigned, Instruction *> indexToInstr = getIndexToInstr();

			unsigned instrIdx = instrToIndex[I];
			MayPointToInfo * inInfo = new MayPointToInfo();
			MayPointToInfo * outInfo = new MayPointToInfo();
			map<pair<char, unsigned>, set<unsigned>> rmMap;

			// Join all incoming infos
			for (size_t i = 0; i < IncomingEdges.size(); ++i) {
				Edge inEdge = make_pair(IncomingEdges[i], instrIdx);
				MayPointToInfo::join(inInfo, edgeToInfo[inEdge], inInfo);
				MayPointToInfo::join(outInfo, edgeToInfo[inEdge], outInfo);
			}

			if (isa<AllocaInst>(I)) {
				rmMap[make_pair(R, instrIdx)].insert(instrIdx);
				MayPointToInfo::join(outInfo, new MayPointToInfo(rmMap), outInfo);
			}

			else if (isa<BitCastInst>(I)) {
				unsigned rv = instrToIndex[(Instruction *) I->getOperand(0)];
				set<unsigned> X = inInfo->getMemSet(make_pair(R, rv));
				
				rmMap[make_pair(R, instrIdx)].insert(X.begin(), X.end());
				MayPointToInfo::join(outInfo, new MayPointToInfo(rmMap), outInfo);
			}

			else if (isa<GetElementPtrInst>(I)) {
				GetElementPtrInst * instr = cast<GetElementPtrInst> (I);
				unsigned rp = instrToIndex[(Instruction *) instr->getPointerOperand()];
				set<unsigned> X = inInfo->getMemSet(make_pair(R, rp));

				rmMap[make_pair(R, instrIdx)].insert(X.begin(), X.end());
				MayPointToInfo::join(outInfo, new MayPointToInfo(rmMap), outInfo);
			}

			else if (isa<LoadInst>(I)) {
				if (I->getType()->isPointerTy()) {
					LoadInst * instr = cast<LoadInst> (I);
					unsigned rp = instrToIndex[(Instruction *) instr->getPointerOperand()];
					set<unsigned> X = inInfo->getMemSet(make_pair(R, rp));
					set<unsigned> Y;

					for (auto it = X.begin(); it != X.end(); ++it) {
						set<unsigned> s = inInfo->getMemSet(make_pair(M, *it));
						Y.insert(s.begin(), s.end());
					}

					rmMap[make_pair(R, instrIdx)].insert(Y.begin(), Y.end());
					MayPointToInfo::join(outInfo, new MayPointToInfo(rmMap), outInfo);
				}
			}

			else if (isa<StoreInst>(I)) {
					StoreInst * instr = cast<StoreInst> (I);
					unsigned rv = instrToIndex[(Instruction *) instr->getValueOperand()];
					unsigned rp = instrToIndex[(Instruction *) instr->getPointerOperand()];
					set<unsigned> X = inInfo->getMemSet(make_pair(R, rv));
					set<unsigned> Y = inInfo->getMemSet(make_pair(R, rp));

					for (auto xi = X.begin(); xi != X.end(); ++xi) {
						for (auto yi = Y.begin(); yi != Y.end(); ++yi) {
							rmMap[make_pair(M, *yi)].insert(*xi);
						}
					}

					MayPointToInfo::join(outInfo, new MayPointToInfo(rmMap), outInfo);
			}

			else if (isa<SelectInst>(I)) {
				SelectInst * instr = cast<SelectInst> (I);
				unsigned r1 = instrToIndex[(Instruction *) instr->getTrueValue()];
				unsigned r2 = instrToIndex[(Instruction *) instr->getFalseValue()];
				set<unsigned> X = inInfo->getMemSet(make_pair(R, r1));
				set<unsigned> Y = inInfo->getMemSet(make_pair(R, r2));

				rmMap[make_pair(R, instrIdx)].insert(X.begin(), X.end());
				rmMap[make_pair(R, instrIdx)].insert(Y.begin(), Y.end());
				MayPointToInfo::join(outInfo, new MayPointToInfo(rmMap), outInfo);
			}

			else if (isa<PHINode>(I)) {
				Instruction * firstNonPhi = I->getParent()->getFirstNonPHI();
				unsigned firstNonPhiIdx = instrToIndex[firstNonPhi];

				for (unsigned i = instrIdx; i < firstNonPhiIdx; ++i) {
					Instruction *instr = indexToInstr[i];

					for (unsigned j = 0; j < instr->getNumOperands(); ++j) {
						unsigned rv = instrToIndex[(Instruction *) instr->getOperand(j)];
						set<unsigned> X = inInfo->getMemSet(make_pair(R, rv));
						rmMap[make_pair(R, i)].insert(X.begin(), X.end());
					}
				}

				MayPointToInfo::join(outInfo, new MayPointToInfo(rmMap), outInfo);
			}

			// Distribute result to outgoing edges
			for (size_t i = 0; i < OutgoingEdges.size(); ++i)
				Infos.push_back(outInfo);
		}
};


namespace {
    struct MayPointToAnalysisPass : public FunctionPass {
        static char ID;
        MayPointToAnalysisPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
        	MayPointToInfo bottom;
        	MayPointToInfo initialState;
        	MayPointToAnalysis * rda = new MayPointToAnalysis(bottom, initialState);

        	rda->runWorklistAlgorithm(&F);
        	rda->print();

            return false;
        }
    }; // end of struct MayPointToAnalysisPass
}  // end of anonymous namespace

char MayPointToAnalysisPass::ID = 0;
static RegisterPass<MayPointToAnalysisPass> X("cse231-maypointto", "Do maypointto analysis on DFA CFG",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
