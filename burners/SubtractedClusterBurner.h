/**
 * @SubtractedClusterBurner.h
 * @author Francesco Vassalli <Francesco.Vassalli@colorado.edu>
 * @version 1.0
 *
 * @section Analyze the SubtractedCEMC module
 */
#ifndef SUBTRACTEDCLUSTERBURNER_H__
#define SUBTRACTEDCLUSTERBURNER_H__

#include <fun4all/SubsysReco.h>
#include <TFile.h>
#include <TTree.h>
#include <string>

class PHCompositeNode;

class SubtractedClusterBurner: public SubsysReco
{

	public:

		SubtractedClusterBurner(const std::string &name,unsigned run);
		~SubtractedClusterBurner();
		int InitRun(PHCompositeNode*);

		int process_event(PHCompositeNode*);
		int End(PHCompositeNode*);


	private:
		bool doNodePointers(PHCompositeNode* topNode);

		const unsigned int _kRunNumber;
		TFile *_f=NULL; ///< output file
		TTree *_ttree=NULL; ///<signal data for making track pair cuts
    std::string _foutname;
};



#endif // __SUBTRACTEDCLUSTERBURNER_H__



