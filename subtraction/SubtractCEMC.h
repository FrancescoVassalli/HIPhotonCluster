#ifndef __SUBTRACTCEMC_H
#define __SUBTRACTCEMC_H

//===========================================================
/// \file SubtractCEMC.h
/// \brief creates new UE-subtracted towers
/// \author Dennis V. Perepelitsa
//===========================================================

#include <fun4all/SubsysReco.h>

// forward declarations
class PHCompositeNode;

/// \class SubtractCEMC
///
/// \brief creates new UE-subtracted towers
///
/// Using a previously determined background UE density, this module
/// constructs a new set of towers by subtracting the background from
/// existing raw towers
///
class SubtractCEMC : public SubsysReco
{
 public:
  SubtractCEMC(const std::string &name = "SubtractCEMC");
  virtual ~SubtractCEMC() {}

  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);

  void SetFlowModulation(bool use_flow_modulation) { _use_flow_modulation = use_flow_modulation; }

 private:
  int CreateNode(PHCompositeNode *topNode);

  bool _use_flow_modulation;
};

#endif
