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
// -------------------------------------------------------------------
// -------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  
#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"

#include "DetectorConstruction.hh"	// Added by Henso

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//PrimaryGeneratorAction::PrimaryGeneratorAction()
//PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC),
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
	:G4VUserPrimaryGeneratorAction(),fDetector(DC)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default gun parameter
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 
  G4ParticleDefinition* particle
                    = particleTable->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);

  //fParticleGun->SetParticleEnergy(260.*eV);
  fParticleGun->SetParticleEnergy(300.*eV);
//  fParticleGun->SetParticleEnergy(1000.*eV);
//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1,-1.,0.1));

  G4double z0 = -0.5*nm -1*fDetector->GetTargetHalfHeight();

  G4cout<< " --> z0 = " << z0/nm << G4endl;
  //fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-0.01));
  fParticleGun->SetParticlePosition(G4ThreeVector(-100*nm,-100*nm,z0));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
