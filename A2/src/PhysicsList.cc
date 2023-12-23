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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// and papers
// M. Batmunkh et al. J Radiat Res Appl Sci 8 (2015) 498-507
// O. Belov et al. Physica Medica 32 (2016) 1510-1520
// The Geant4-DNA web site is available at http://geant4-dna.org
// 
// -------------------------------------------------------------------
// November 2016
// -------------------------------------------------------------------
//
// $Id$
//
/// \file PhysicsList.cc 
/// \brief Implementation of the PhysicsList class

#include "PhysicsList.hh"

// #include "G4PhysicsConstructorRegistry.hh"
// #include "CommandLineParser.hh"
// #include "G4EmParameters.hh"

// #include "G4EmLivermorePhysics.hh"
// #include "G4EmPenelopePhysics.hh"
// #include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

// #include "G4Radioactivation.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4NuclideTable.hh"
// #include "G4LossTableManager.hh"
// #include "G4UAtomicDeexcitation.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

namespace A2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
{
  //G4double currentDefaultCut = 1.*micrometer; 
  //SetDefaultCutValue(currentDefaultCut); 
  //SetVerboseLevel(1);

  //G4ProductionCutsTable::GetProductionCutsTable()->
  //                       SetEnergyRange(100*eV, 1*GeV);

  SetVerboseLevel(1);
  
  RegisterPhysics(new G4EmStandardPhysics_option4());


  // Mandatory for G4NuclideTable
  // Half-life threshold must be set small or many short-lived isomers 
  // will not be assigned life times (default to 0) 
  // G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*picosecond);
  // G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);

 
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // minimums
  // G4NuclideTable::GetInstance()->SetMeanLifeThreshold(1*nanosecond);  
  // G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV); // get to find default

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}


// A2PhysicsList::~A2PhysicsList()
// {
// }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
