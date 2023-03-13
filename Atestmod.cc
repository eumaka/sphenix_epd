#include "Atestmod.h"


#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>
#include <fun4all/Fun4AllServer.h>

#include <calobase/TowerInfoContainer.h>
#include <calobase/TowerInfoContainerv1.h>
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfov1.h>


#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4Particle.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHObject.h>
#include <phool/getClass.h>
#include <phool/phool.h>


#include <phgeom/PHGeomUtility.h>

#include <g4detectors/PHG4CylinderCellGeomContainer.h>
#include <g4detectors/PHG4CylinderCellGeom_Spacalv1.h>
#include <g4detectors/PHG4CylinderGeomContainer.h>
#include <g4detectors/PHG4CylinderGeom_Spacalv3.h>
#include <g4detectors/PHG4CellDefs.h>

#include <TTree.h>
#include <TH2D.h>
#include <TVector3.h>
#include <TRandom3.h>
#include <TMath.h>

#include <trackbase_historic/SvtxTrackMap.h>
#include <g4jets/Jet.h>
#include <g4jets/JetMap.h>
#include <g4jets/JetMapv1.h>
#include <g4main/PHG4Utils.h>
#include <epd/EPDDefs.h>

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
  _event_tree->Branch("tile_e", &_tile_e, "_tile_e/F");
    
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

    string towerinfonodename_calib = "TOWERINFO_CALIB_EPD";
    TowerInfoContainerv1 *towers_calib = findNode::getClass<TowerInfoContainerv1>(topNode, towerinfonodename_calib.c_str());
    
    if (!towers_calib)
      {
          std::cout << "Could not locate SEPD CALIB tower info node " << std::endl;
          exit(1);
      }

  
    unsigned int ntowers = towers_calib->size();
   for (unsigned int ch = 0; ch < ntowers;  ch++)
   {

     TowerInfo *_tower = towers_calib->get_tower_at_channel(ch);
     unsigned int thiskey =_towerinfos->encode_epd(ch);
  
     tile_e = _tower->get_energy();
     
   }
    
 


return;

}






