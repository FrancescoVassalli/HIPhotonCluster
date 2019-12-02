#include <algorithm>

struct Average{
  double value;
  vector<double> *allVals;
  int n;
  Average(){
    value=0;
    n=0;
    allVals=new vector<double>();
  }
  //allv should include v
  Average(double v,int n=1,vector<double> *allv=NULL){
    value=v;
    this->n=n;
    allVals=allv;
    if(!allVals) allVals=new vector<double>();
  }
  Average(float v,int n=1){
    value=(double)v;
    this->n=n;
    allVals=new vector<double>();
  }
  ~Average(){
    if(s){
      delete s;
      s=NULL;
    }
  }
  Average operator +(double v){
    allVals->push_back(v);
    return Average((value*n)/(n+1)+v/(n+1),n+1,allVals);
  }
  Average operator +(float fv){
    double v = (double) fv;
    allVals->push_back(v);
    return Average((value*n)/(n+1)+v/(n+1),n+1,allVals);
  }
  void operator +=(double v){
    if(!allVals) allVals=new vector<double>();
    allVals->push_back(v);
    value=(value*n)/(n+1)+v/(n+1);
    ++n;
  }
  void operator +=(float fv){
    if(!allVals) allVals=new vector<double>();
    double v = (double) fv;
    allVals->push_back(v);
    value=(value*n)/(n+1)+v/(n+1);
    ++n;
  }
  void operator =(Average a){
    value=a.value;
    allVals=a.allVals;
    n=a.n;
  }
  double getError(){
    if(n<=1) return 0;
    return getS()/sqrt(n);
  }
  double getS(){
    double cm2 =0;
    for(double x : *allVals){
      cm2+=(x-value)*(x-value);
    }
    if(!s) s= new double(sqrt(cm2/(n-1)));
    else *s = sqrt(cm2/(n-1));
    return *s;
  }
  double getSError(){ 
    if(n<=1) return 0;
    static const float tolAdjust =.0001; //this is a hyper paramater for how the tolerance is made 
    TH1D *toy = new TH1D("toy","",1,*std::min_element(allVals->begin(),allVals->end()),*std::max_element(allVals->begin(),allVals->end()));
    for(double x : *allVals){
      toy->Fill(x);
    }
    Average error;
    double tolerance=0;
    double lastError=-1.;
    double change=-1.;
    do{
      //draw a random sample
      Average sample;
      for (int i = 0; i < n; ++i)
      {
        sample+=toy->GetRandom();
      }
      //calculate the statistic
      double ierror = sample.getS();
      //add the statistic to an average
      error+=ierror;
      //get the error on the average
      //find out how much the error changed 
      if (lastError>0)
      {
        change=std::abs(error.getError()-lastError);
        tolerance=error.getError()*tolAdjust;
      }
      lastError=error.getError();
      // if the error changed less than the tolerance do this again 
    }while(change<0||change>tolerance);
    delete toy;
    return error.getError();
  }

  private:
  double *s=NULL;

};

int aTest(){
  vector<int> v_average(10);
  for (int i = 0; i < 10; ++i)
  {
    v_average.push_back(10);
  }
  cout<<v_average.value()<<endl;
}
