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
/// \file A2/src/SteppingAction.cc
/// \brief Implementation of the A2::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "Analysis.hh"

#include "G4SystemOfUnits.hh" 

#include "G4Ions.hh"

namespace A2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4String particle = track->GetParticleDefinition()->GetParticleName();
  G4double postEnergy = track->GetKineticEnergy();
  G4String procName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  if ( (procName == "RadioactiveDecay") //  <= 11.1.2
       || (procName == "Radioactivation") ) { // >= 11.1.3
    G4cout << "radioactive decay of " << particle << " with kinetic energy " << postEnergy / keV << " keV \n";
    G4cout << "(should be the same as " << step->GetPostStepPoint()->GetKineticEnergy() / keV << " keV)\n";
  }

  G4double preEnergy = step->GetPreStepPoint()->GetKineticEnergy();
  if (track->GetCurrentStepNumber() == 1){
    const G4VProcess* creatorProcess = track->GetCreatorProcess();
    if (creatorProcess && (
			   (creatorProcess->GetProcessName() == "RadioactiveDecay") //  <= 11.1.2
			   || (creatorProcess->GetProcessName() == "Radioactivation")) // >= 11.1.3
	) {
      G4cout << "radioactive decay produced " << particle << " with kinetic energy " << preEnergy / keV << " keV\n";
    }
  }

  // for a "single decay"
  // (assumes the daughter nucleus won't decay from an excited state)
  G4int isomer_level = ((const G4Ions*)(track->GetParticleDefinition()))->GetIsomerLevel();
  G4int mass_number = ((const G4Ions*)(track->GetParticleDefinition()))->GetBaryonNumber();
  if ((track->GetParentID() != 0) && (postEnergy == 0) && (isomer_level == 0) && (mass_number != 0)){
    track->SetTrackStatus(fStopAndKill);
  }
  
  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

  // fill histogram
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0, edepStep);

  // fill ntuple
  G4int evnt_id = -1;
  const G4Event* evnt = G4RunManager::GetRunManager()->GetCurrentEvent();
  if (evnt) {evnt_id = evnt->GetEventID();} else
  { G4cout << "Oh dear" << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
  }

  analysisManager->FillNtupleIColumn(0, evnt_id);
  analysisManager->FillNtupleDColumn(1, edepStep);
  analysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
