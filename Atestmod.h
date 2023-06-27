
#ifndef __Atestmod_H__
#define __Atestmod_H__

#include <fun4all/SubsysReco.h>
#include <string>
#include <vector>



//Forward declerations
class PHCompositeNode;
class TFile; 
class TTree;
class RawTowerContainer;
class RawTowerGeomContainer;
class TH2F;
class TProfile;

//class TowerInfoContainerv1;

//TowerInfoContainerv1* towers


//Brief: basic ntuple and histogram creation for sim evaluation
class Atestmod: public SubsysReco
{
 public: 
  //Default constructor
    Atestmod(const std::string &name="Atestmod");

  //Initialization, called for initialization
  int Init(PHCompositeNode *);

  int InitRun(PHCompositeNode *); 

  //Process Event, called for each event
  int process_event(PHCompositeNode *);

  //End, write and close files
  int EndRun(PHCompositeNode *);
  int End(PHCompositeNode *);

  //Change output filename
  void set_filename(const char* file)
  { if(file) _outfile_name = file; }

 private:
  //output filename
  std::string _outfile_name;
    
  //Event counter
  int _event;
  std::vector<float> _t;
  std::vector<float> _r;
  std::vector<float> _z;
  std::vector<float> _phi;
  std::vector<int> _rbin;
  std::vector<int> _phibin;
  std::vector<int> _armbin;
  std::vector<int> _sectorid;


  //User modules
  void fill_tree(PHCompositeNode*);

 

};

#endif //* __Atestmod_H__ *//
