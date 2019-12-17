/**
 * @IDBurner.h
 * @author Francesco Vassalli <Francesco.Vassalli@colorado.edu>
 * @version 1.0
 *
 * @section get the photon ID for clusters
 */
#ifndef IDBURNER_H__
#define IDBURNER_H__

#include <fun4all/SubsysReco.h>

#include <TFile.h>
#include <TMath.h>
#include <TTree.h>

#include <string>
#include <cmath>

class PHCompositeNode;
class PHG4TruthInfoContainer;
class RawClusterContainer;
class RawTowerContainer;
class TowerMap;


class TLorentzVector;

class IDBurner: public SubsysReco
{

  public:

    IDBurner(bool isHI);
    ~IDBurner();

    int process_cluster(RawCluster *cluster);
    float getTowerEnergy(unsigned mapPosition);


  private:

    bool doNodePointers(PHCompositeNode* topNode);

    const bool _kISHI;
    static const unsigned int _kMINCLUSTERENERGY=1;//in GeV
    static const unsigned int _kMAXCLUSTERS=600;
    static const unsigned _kTOPOSIZE=7;
    static const float _kSEGMENATION;

    RawClusterContainer* _subClusterContainer=NULL;
    PHG4TruthInfoContainer *_truthinfo=NULL;
    RawTowerContainer* _towerContainer=NULL;
    RawTowerGeomContainer *_geomEM=NULL;
    TowerMap *_towerMap=NULL;
             
};



#endif // __IDBURNER_H__



