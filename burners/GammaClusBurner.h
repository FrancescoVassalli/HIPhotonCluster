/**
 * @GammaClusBurner.h
 * @author Francesco Vassalli <Francesco.Vassalli@colorado.edu>
 * @version 1.0
 *
 * @section make photon cluster E spec
 */
#ifndef GAMMACLUSBURNER_H__
#define GAMMACLUSBURNER_H__

#include <fun4all/SubsysReco.h>
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>

#include <TFile.h>
#include <TMath.h>
#include <TTree.h>

#include <string>
#include <cmath>

class PHCompositeNode;
class PHG4TruthInfoContainer;
class RawCluster;

class TLorentzVector;

class GammaClusBurner: public SubsysReco
{

  public:

    GammaClusBurner(const std::string &name,unsigned run,bool isHI);
    ~GammaClusBurner();
    int InitRun(PHCompositeNode*);

    int process_event(PHCompositeNode*);
    int End(PHCompositeNode*);


  private:
    inline double get_eta(RawCluster* cluster){
      return -1 * log( tan( TMath::ATan2( cluster->get_r(), cluster->get_z()  ) / 2.0 ) );
    }

    inline double DeltaPhi (double phi1, double phi2, const bool sign=0) {
      double dphi = TMath::Abs(phi1 - phi2);
      while (dphi > TMath::Pi()) dphi = TMath::Abs (dphi - 2*TMath::Pi());

      if (sign &&  (phi2 + dphi) == phi1)
        dphi *= -1;

      return dphi;
    }
    inline double DeltaR (const double eta1, const double eta2, const double phi1, const double phi2 ) {
      const double deta = eta1 - eta2;
      const double dphi = DeltaPhi (phi1, phi2, false);
      return sqrt( pow( deta, 2 ) + pow( dphi, 2 ) );
    }
    ///Handle the conversion to the inline functions
    double DeltaR (TLorentzVector *tlv, RawCluster* cluster);

    bool doNodePointers(PHCompositeNode* topNode);
    RawCluster* getCluster(TLorentzVector* tlv);

    const unsigned int _kRunNumber;
    const bool _kISHI;
    static const unsigned int _kMINCLUSTERENERGY=1;//in GeV
    static const unsigned int _kMAXCLUSTERS=600;
    const static int s_kTPCRADIUS=21; //in cm there is a way to get this from the simulation I should implement?
    static const float _kCLUSTERDR;
    static const float _kMAXETA;

    TFile *_f=NULL; ///< output file
    TTree *_ttree=NULL; ///<data
    RawClusterContainer* _subClusterContainer=NULL;
    std::string _foutname;///<path+name for output file
    PHG4TruthInfoContainer *_truthinfo;

    /** \defgroup  variables  for the TTrees
      @{*/
    float _b_truthphoton_E   [ _kMAXCLUSTERS ];
    float _b_truthphoton_pT   [ _kMAXCLUSTERS ];
    float _b_clustersub_E    [ _kMAXCLUSTERS ];
    float _b_clustersub_ecore[ _kMAXCLUSTERS ] ;
    float _b_clustersub_eta  [ _kMAXCLUSTERS ];
    float _b_clustersub_phi  [ _kMAXCLUSTERS ];
    float _b_clustersub_prob  [ _kMAXCLUSTERS ];
    float _b_matchDR  [ _kMAXCLUSTERS ];
    float _b_matchEta  [ _kMAXCLUSTERS ];
    float _b_matchPhi  [ _kMAXCLUSTERS ];
    unsigned _b_clustersub_n;     
    /**@}*/                       
};



#endif // __GAMMACLUSBURNER_H__



