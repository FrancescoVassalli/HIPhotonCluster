/**
 * @ChaseTower.h
 * @author Chase Smith
 * @author Francesco Vassalli <Francesco.Vassalli@colorado.edu>
 * @version 2.0
 *
 * @adapted tower object for doing math
 */
#ifndef CHASETOWER_H__
#define CHASETOWER_H__

#include <vector>

class ChaseTower
{

  public:
    //constructors
    ChaseTower(){}; 
    ~ChaseTower(){}; 
    ChaseTower(double eta_in, double phi_in, double energy_in, unsigned int keytype_in)
    {
      eta = eta_in;
      phi = phi_in;
      energy = energy_in;
      keytype = keytype_in;
    }

    inline void setEta(double eta2){eta = eta2;}
    inline void setPhi(double phi2){phi = phi2;}
    inline void setEnergy(double energy2){energy = energy2;}
    inline void setKey(unsigned int keytype2){keytype = keytype2;}
    inline double getEta(){return eta;}
    inline double getPhi(){return phi;}
    inline double getEnergy(){return energy;}
    inline unsigned getKey(){return keytype;}

    static ChaseTower findMaxTower(std::vector<ChaseTower> towers);

  private:

    double eta;
    double phi;
    double energy;
    unsigned int keytype;

};



#endif // __CHASETOWER_H__
