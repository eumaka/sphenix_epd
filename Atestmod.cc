#include "Atestmod.h"


#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>
#include <fun4all/Fun4AllServer.h>

#include <epd/EpdGeom.h>

#include <calobase/TowerInfoContainer.h>
#include <calobase/TowerInfoContainerv1.h>
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfov1.h>
#include <calobase/TowerInfoDefs.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHObject.h>
#include <phool/getClass.h>
#include <phool/phool.h>

#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>

using namespace std;

//----------------------------------------------------------------------------//
//-- Constructor:
//--  simple initialization
//----------------------------------------------------------------------------//

Atestmod::Atestmod(const string &name) :
  SubsysReco(name)
{
	//initialize
	_event = 0;
	_outfile_name = ".root";

}

//----------------------------------------------------------------------------//
//-- Init():
//--   Intialize all histograms, trees, and ntuples
//----------------------------------------------------------------------------//
int Atestmod::Init(PHCompositeNode *topNode) {


	cout << PHWHERE << " Opening file " << _outfile_name << endl;

	PHTFileServer::get().open(_outfile_name, "RECREATE");
	PHTFileServer::get().cd(_outfile_name);

	_event_tree = new TTree("event", "EPD => event info");
    
        _event_tree->Branch("event", &_event, "_event/I");
        _event_tree->Branch("tile_e", &_t);   
	_event_tree->Branch("tile_r", &_r);    
        _event_tree->Branch("tile_z", &_z);    
        _event_tree->Branch("tile_phi", &_phi);    
        _event_tree->Branch("tile_rbin", &_rbin);   
	_event_tree->Branch("tile_phibin", &_phibin);    
        _event_tree->Branch("tile_armbin", &_armbin);    
        _event_tree->Branch("tile_sectorid", &_sectorid);    

	return Fun4AllReturnCodes::EVENT_OK;
}

int Atestmod::InitRun(PHCompositeNode *topNode)
{
	return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- process_event():
//--   Call user instructions for every event.
//--   This function contains the analysis structure.
//----------------------------------------------------------------------------//

int Atestmod::process_event(PHCompositeNode *topNode)
{
	_event++;

	GetNodes(topNode);

	fill_tree(topNode);

	return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- End():
//--   End method, wrap everything up
//----------------------------------------------------------------------------//

int Atestmod::EndRun(PHCompositeNode *topNode)
{

    return Fun4AllReturnCodes::EVENT_OK;

}

int Atestmod::End(PHCompositeNode *topNode)
{

	PHTFileServer::get().cd(_outfile_name);
	PHTFileServer::get().write(_outfile_name);

	return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- fill_tree():
//--   Fill the various trees...
//----------------------------------------------------------------------------//

void Atestmod::fill_tree(PHCompositeNode *topNode)
{

    cout << _event << endl;

    string towerinfonodename_calib = "TOWERS_EPD";
    TowerInfoContainer *towers_calib = findNode::getClass<TowerInfoContainerv1>(topNode, towerinfonodename_calib.c_str());
    
    if (!towers_calib)
      {
          std::cout << "Could not locate SEPD CALIB tower info node " << std::endl;
          exit(1);
      }

   string geominfonodename = "TOWERGEOM_EPD";
    EpdGeom *epdtilegeom = findNode::getClass<EpdGeom>(topNode, geominfonodename.c_str());

    if (!epdtilegeom)
      {
          std::cout << "Could not locate SEPD geometry node " << std::endl;
          exit(1);
      }

   float tile_e = 0.;
   float tile_z = 0.;
   float tile_r = 0.;
   float tile_phi = 0.;
   int arm  = -1; int phibin = -1; int rbin = -1; int sectorid = -1;
   unsigned int ntowers = towers_calib->size();
   for (unsigned int ch = 0; ch < ntowers;  ch++)
   {

     TowerInfo *_tower = towers_calib->get_tower_at_channel(ch);
     unsigned int thiskey =_towerinfos->encode_epd(ch);
  
     tile_e = _tower->get_energy();
     tile_phi = epdtilegeom->get_phi(key);
     tile_r = epdtilegeom->get_r(key);
     tile_z = epdtilegeom->get_z(key);

     arm = TowerInfoDefs::get_epd_arm(key);
     phibin = TowerInfoDefs::get_epd_phibin(key);
     rbin = TowerInfoDefs::get_epd_rbin(key);
     sectorid = TowerInfoDefs::get_epd_sector(key);

      _t.push_back(tile_e);
      _z.push_back(tile_z);
      _r.push_back(tile_r);
      _phi.push_back(tile_phi);
      _rbin.push_back(rbin);
      _phibin.push_back(phibin);
      _armbin.push_back(arm);
      _sectorid.push_back(sectorid);
	     
   }
    
 

return;

}






