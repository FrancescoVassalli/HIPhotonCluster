#include "SubtractCEMC.h"

#include <jetbackground/TowerBackground.h>

// sPHENIX includes
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTowerGeomContainer_Cylinderv1.h>
#include <calobase/RawTowerv1.h>

#include <g4jets/Jet.h>
#include <g4jets/JetMap.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHTypedNodeIterator.h>
#include <phool/getClass.h>

// standard includes
#include <iomanip>
#include <iostream>
#include <vector>

SubtractCEMC::SubtractCEMC(const std::string &name)
  : SubsysReco(name)
  , _use_flow_modulation(false)
{
}

int SubtractCEMC::InitRun(PHCompositeNode *topNode)
{
  CreateNode(topNode);

  return Fun4AllReturnCodes::EVENT_OK;
}

int SubtractCEMC::process_event(PHCompositeNode *topNode)
{
  if (Verbosity() > 0) {
    //std::cout << "SubtractCEMC::process_event: entering, with _use_flow_modulation = " << _use_flow_modulation << std::endl;
  }

  // pull out the tower containers and geometry objects at the start
  RawTowerContainer *towersEM3 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_CEMC");

  RawTowerGeomContainer *geomEM = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_CEMC");

  if (Verbosity() > 0)
  {
    //std::cout << "SubtractCEMC::process_event: " << towersEM3->size() << " TOWER_CALIB_CEMC towers" << std::endl;
  }

  // these should have already been created during InitRun()
  RawTowerContainer *emcal_towers = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_CEMC_SUB1");

  RawTowerGeomContainer *geomIH = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALIN");

  if (Verbosity() > 0)
  {
    //std::cout << "SubtractCEMC::process_event: starting with " << emcal_towers->size() << " TOWER_CALIB_CEMC_SUB1 towers" << std::endl;
  }

  TowerBackground *towerbackground = findNode::getClass<TowerBackground>(topNode, "TowerBackground_Sub2");

  //towerbackground->identify();

  // read these in to use, even if we don't use flow modulation in the subtraction
  float background_v2 = towerbackground->get_v2();
  float background_Psi2 = towerbackground->get_Psi2();

  // EMCal

  // replicate existing towers
  RawTowerContainer::ConstRange begin_end_EM = towersEM3->getTowers();
  for (RawTowerContainer::ConstIterator rtiter = begin_end_EM.first; rtiter != begin_end_EM.second; ++rtiter)
  {
    RawTower *tower = rtiter->second;

    int this_etabin = tower->get_bineta();
    int this_phibin = tower->get_binphi();
    float this_E = tower->get_energy();

    RawTower *new_tower = new RawTowerv1();
    new_tower->set_energy(this_E);
    emcal_towers->AddTower(this_etabin, this_phibin, new_tower);
  }

  // now fill in additional towers with zero energy to fill out the full grid
  // but note: after retowering, all of these should already exist...
  for (int eta = 0; eta < geomEM->get_etabins(); eta++)
  {
    for (int phi = 0; phi < geomEM->get_phibins(); phi++)
    {
      if (!emcal_towers->getTower(eta, phi))
      {
        RawTower *new_tower = new RawTowerv1();
        new_tower->set_energy(0);
        emcal_towers->AddTower(eta, phi, new_tower);
      }
    }
  }

  // update towers for background subtraction...
  for (RawTowerContainer::ConstIterator rtiter = emcal_towers->getTowers().first; rtiter != emcal_towers->getTowers().second; ++rtiter)
  {
    RawTower *tower = rtiter->second;
    float raw_energy = tower->get_energy();

    // look up what the equivalent IHCal eta bin is... 

    RawTowerGeom *tower_geom = geomEM->get_tower_geometry( tower->get_key() );
    float local_eta = tower_geom->get_eta();
    int lookup_eta_bin = geomIH->get_etabin( local_eta );

    //std::cout << " at CEMC tower with local_eta = " << local_eta << " , this maps to IHCal eta bin = " << lookup_eta_bin << std::endl;

    float UE = towerbackground->get_UE(0).at( lookup_eta_bin );

    UE = UE * 0.25 * 0.25; // correct for different area...

    if (_use_flow_modulation)
    {
      float tower_phi = geomEM->get_tower_geometry(tower->get_key())->get_phi();
      UE = UE * (1 + 2 * background_v2 * cos(2 * (tower_phi - background_Psi2)));
    }

    float new_energy = raw_energy - UE;
    tower->set_energy(new_energy);

    //if (Verbosity() > 5 && raw_energy > 1 )
    //std::cout << " SubtractCEMC::process_event : EMCal tower at eta / phi = " << tower->get_bineta() << " / " << tower->get_binphi() << ", pre-sub / after-sub E = " << raw_energy << " / " << tower->get_energy() << std::endl;
  }

  if (Verbosity() > 0)
  {
    //std::cout << "SubtractCEMC::process_event: ending with " << emcal_towers->size() << " TOWER_CALIB_CEMC_SUB1 towers" << std::endl;
  }

  //if (Verbosity() > 0) std::cout << "SubtractCEMC::process_event: exiting" << std::endl;

  return Fun4AllReturnCodes::EVENT_OK;
}

int SubtractCEMC::CreateNode(PHCompositeNode *topNode)
{
  PHNodeIterator iter(topNode);

  // Looking for the DST node
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
  if (!dstNode)
  {
    std::cout << PHWHERE << "DST Node missing, doing nothing." << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }

  // store the new EMCal towers
  PHCompositeNode *emcalNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "CEMC"));
  if (!emcalNode)
  {
    std::cout << PHWHERE << "EMCal Node note found, doing nothing." << std::endl;
  }

  RawTowerContainer *test_emcal_tower = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_CEMC_SUB1");
  if (!test_emcal_tower)
  {
    if (Verbosity() > 0) std::cout << "SubtractCEMC::CreateNode : creating TOWER_CALIB_CEMC_SUB1 node " << std::endl;

    RawTowerContainer *emcal_towers = new RawTowerContainer(RawTowerDefs::CalorimeterId::CEMC);
    PHIODataNode<PHObject> *emcalTowerNode = new PHIODataNode<PHObject>(emcal_towers, "TOWER_CALIB_CEMC_SUB1", "PHObject");
    emcalNode->addNode(emcalTowerNode);
  }
  else
  {
    std::cout << "SubtractCEMC::CreateNode : TOWER_CALIB_CEMC_SUB1 already exists! " << std::endl;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}
