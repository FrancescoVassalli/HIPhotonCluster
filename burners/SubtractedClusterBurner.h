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
#include <calobase/RawClusterContainer.h>
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
    static const unsigned int _kMINCLUSTERENERGY=2;//in GeV
    static const unsigned int _kMAXCLUSTERS=400;
    TFile *_f=NULL; ///< output file
    TTree *_ttree=NULL; ///<signal data for making track pair cuts
    RawClusterContainer* _subClusterContainer=NULL;
    std::string _foutname;

    float _b_clustersub_E    [ _kMAXCLUSTERS ];
    float _b_clustersub_ecore[ _kMAXCLUSTERS ] ;
    float _b_clustersub_eta  [ _kMAXCLUSTERS ];
    float _b_clustersub_phi  [ _kMAXCLUSTERS ];
    unsigned _b_clustersub_n;                            
};



#endif // __SUBTRACTEDCLUSTERBURNER_H__



