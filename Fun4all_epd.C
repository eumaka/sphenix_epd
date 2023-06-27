#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)

#include "Atestmod.h"

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libAtestmod.so)
#endif

void Fun4all_epd()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();

  Atestmod *epdeval = new Atestmod();
  se->registerSubsystem(epdeval);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("EPDDST");
  in->AddFile("/sphenix/user/eumaka/ProducedCommissionigDSTS/00014266-0000.root");


  se->registerInputManager(in);

  se->run(-1);
  se->End();
  se->PrintTimer();
  gSystem->Exit(0);
}


