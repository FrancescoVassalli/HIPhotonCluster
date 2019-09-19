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
#include <calobase/RawClusterContainer.h>
#include <TFile.h>
#include <TTree.h>
#include <string>

class PHCompositeNode;

class GammaClusBurner: public SubsysReco
{

  public:

    GammaClusBurner(const std::string &name,unsigned run,bool isHI);
    ~GammaClusBurner();
    int InitRun(PHCompositeNode*);

    int process_event(PHCompositeNode*);
    int End(PHCompositeNode*);


  private:
    inline double DeltaPhi (double phi1, double phi2, const bool sign=0) {
      phi1 = (phi1);
      phi2 = (phi2);
      double dphi = abs(phi1 - phi2);
      while (dphi > TMath::Pi()) dphi = abs (dphi - 2*TMath::Pi());

      if (sign &&  (phi2 + dphi) == phi1)
        dphi *= -1;

      return dphi;
    }

    inline double DeltaR (const double eta1, const double eta2, const double phi1, const double phi2 ) {
      const double deta = eta1 - eta2;
      const double dphi = DeltaPhi (phi1, phi2, false);
      return sqrt( pow( deta, 2 ) + pow( dphi, 2 ) );
    }
    inline double DeltaR (TLorentzVector *tlv, RawCluster* cluster){
      float cluseta=-1 * log( tan( TMath::ATan2( cluster->get_r(), cluster->get_z()  ) / 2.0 ) );
      return DeltaR(tlv->Eta(),cluseta,tlv->Phi(),cluster->get_phi());
    }

    bool doNodePointers(PHCompositeNode* topNode);
    RawCluster* getCluster(TLorentzVector* tlv);


    const unsigned int _kRunNumber;
    const bool _kISHI;
    static const unsigned int _kMINCLUSTERENERGY=2;//in GeV
    static const unsigned int _kMAXCLUSTERS=400;
    static const float _kCLUSTERDR;
    TFile *_f=NULL; ///< output file
    TTree *_ttree=NULL; ///<signal data for making track pair cuts
    RawClusterContainer* _subClusterContainer=NULL;
    std::string _foutname;

    float _b_truthPhoton_E   [ _kMAXCLUSTERS ];
    float _b_clustersub_E    [ _kMAXCLUSTERS ];
    float _b_clustersub_ecore[ _kMAXCLUSTERS ] ;
    float _b_clustersub_eta  [ _kMAXCLUSTERS ];
    float _b_clustersub_phi  [ _kMAXCLUSTERS ];
    unsigned _b_clustersub_n;                            
};



#endif // __GAMMACLUSBURNER_H__



