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
/// \file A2/src/DetectorConstruction.cc
/// \brief Implementation of the A2::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace A2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Outer Sphere parameters
  //
  G4double sphere_radius = 4*m; // 4*mm;
  G4Material* sphere_mat = nist->FindOrBuildMaterial("G4_WATER");

  // Inner Sphere parameters
  //
  G4double inner_sphere_radius = 1*m; // 1*mm;
  G4Material* inner_sphere_mat = nist->FindOrBuildMaterial("G4_WATER");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 2.3*sphere_radius;  // full length
  G4double world_sizeZ = world_sizeXY;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size (half-lengths)

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "WorldLogic");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "WorldPhysical",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

  //
  // Sphere
  //
  auto solidSphere = new G4Sphere("SphereSolid",                    // its name
			       0, // r start
			       sphere_radius, // delta r
			       0, // phi start
			       CLHEP::twopi, // delta phi
			       0, // theta start
			       CLHEP::pi); // delta theta

  auto logicSphere = new G4LogicalVolume(solidSphere,  // its solid
    sphere_mat,                                     // its material
    "SphereLogic");                                 // its name

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),          // at (0,0,0)
    logicSphere,                 // its logical volume
    "SpherePhysical",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking


  //
  // Inner sphere
  //
  auto solidInnerSphere = new G4Sphere("InnerSphereSolid",                    // its name
			       0, // r start
			       inner_sphere_radius, // delta r
			       0, // phi start
			       CLHEP::twopi, // delta phi
			       0, // theta start
			       CLHEP::pi); // delta theta

  auto logicInnerSphere = new G4LogicalVolume(solidInnerSphere,  // its solid
    inner_sphere_mat,                                     // its material
    "InnerSphereLogic");                                 // its name

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),          // at (0,0,0) w.r.t. mother
    logicInnerSphere,                 // its logical volume
    "InnerSpherePhysical",               // its name
    logicSphere,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking


  fScoringVolume = logicInnerSphere;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
