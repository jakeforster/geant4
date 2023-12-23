//
// Copyright (C) 2023 Copyright Holders of the Geant4 Collaboration
// Copyright (C) 2023 Jake Forster
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file exampleA2.cc
/// \brief Main program of the A2 example

#include "PhysicsList.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
// #include "QBBC.hh"

// #include "G4EmStandardPhysics_option4.hh"


#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

using namespace A2;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }

  // do asserts work? 
  // #include <cassert>
  // assert(true)
  //
  #ifdef NDEBUG
    G4cout << "NDEBUG is defined. Asserts are disabled.\n";
  #else
    G4cout << "NDEBUG is not defined. Asserts are enabled.\n";
  #endif

  // Randomise the seed to produce unique results each run
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4long seeds[2];
  G4long systime = time(NULL);
  seeds[0] = systime;
  seeds[1] = systime*G4UniformRand();
  // Set values for the seeds instead...
  // seeds[0] = 1699508102;
  // seeds[1] = 262926411;
 
  G4Random::setTheSeeds(seeds);
  G4cout << "Seeds are " << seeds[0] << " and " << seeds[1] << G4endl;

  std::ofstream file("seeds.txt");
  // file.precision(16);
  file << seeds[0] << " " << seeds[1];
  file.close();

  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  // G4VModularPhysicsList* physicsList = new QBBC;
  // physicsList->SetVerboseLevel(1);
  // runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new PhysicsList());

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
