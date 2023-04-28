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

#include "TrackingAction.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4Region.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "DetectorConstruction.hh"

using namespace std;

TrackingAction::TrackingAction(DetectorConstruction* detector)
{
    fDetector = detector;
    fTargetRegion = 0;
    //fWorldRegion = 0;
}

TrackingAction::~TrackingAction()
{
    fDetector = 0;
    fTargetRegion = 0;
    //fWorldRegion = 0;
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();

    if(particleDefinition == G4Electron::Definition() || particleDefinition == G4Gamma::Definition())
    {
        if(fTargetRegion == 0) // target region is initialized after detector construction instantiation
        {
            fTargetRegion = fDetector->GetTargetRegion();
        }

        //if(fWorldRegion == 0) // target region is initialized after detector construction instantiation
        //{
        //    fWorldRegion = fDetector->GetTargetRegion();
        //}


        const G4ThreeVector& position = track->GetPosition();
//        G4double energy = track->GetKineticEnergy();
//        G4String volume = track->GetVolume()->GetName();
//        G4int trackID = track->GetTrackID();

//	G4cout << " @TrackID = " << trackID;
//	G4cout << " @NKinEnergy = " << energy;
//	G4cout << " @Volume = " << volume << G4endl;
        
	
	int N =  fTargetRegion->GetNumberOfRootVolumes();
        //int Nworld =  fWorldRegion->GetNumberOfRootVolumes();
	//G4cout << " @Number volumes = " << Nworld << G4endl;
	std::vector<G4LogicalVolume*>::iterator it_logicalVolumeInRegion =
                fTargetRegion->GetRootLogicalVolumeIterator();

        bool inside_target = false;

        for(int i = 0; i < N ; i++, it_logicalVolumeInRegion++)
        {
            EInside test_status = (*it_logicalVolumeInRegion)->GetSolid()->Inside(position) ;
     	    if(test_status == kInside)
            {
                inside_target = true;
                break;
            }
            /*
            else if (test_status == kSurface)
            {
            }
            */
        }

        if(inside_target == true)
        {
            fNParticleInTarget[particleDefinition]++;
        }
        else
        {
            fNParticleInWorld[particleDefinition]++;
        }
    }
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{

    const G4ParticleDefinition* particleDefinition = aTrack->GetParticleDefinition();
	
	//  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //Run* run = static_cast<Run*>(
   //          G4RunManager::GetRunManager()->GetNonConstCurrentRun());

/*
    G4ThreeVector position = aTrack->GetPosition();
  G4ThreeVector vertex   = aTrack->GetVertexPosition();
  G4double charge        = aTrack->GetDefinition()->GetPDGCharge();
  G4double energy        = aTrack->GetKineticEnergy();
*/

  //G4cout << " @ Event       : " << run->GetRunID() << G4endl;
  //G4cout << " @ Track ID    : " << aTrack->GetTrackID() ;
  //G4cout << ", @ Track Def   : " << aTrack->GetDefinition()->GetParticleName() ;
  //G4cout << ", @ Charge PDG  : " << charge ;
  //G4cout << ", @ Energy      : " << energy ;
  //G4cout << ", @ Volume      : " <<  aTrack->GetVolume()->GetName() << G4endl;
  //G4cout << ", @ Momentum Dir      : " <<  aTrack->GetMomentumDirection().z()  << G4endl;
  //G4cout << ", @ Vertex Pos  : " << position.x() << ";" << position.y() << ";"<< position.z() << G4endl;
//        G4int trackID = track->GetTrackID();



  if ( (aTrack->GetMomentumDirection().z() < 0. )  && (aTrack->GetVolume()->GetName() == "World") && (aTrack->GetPosition().z() < 0. ) ){

            fNParticleOutTarget[particleDefinition]++;
  }

  //G4cout << " @ fDirX       : " << aTrack->GetMomentumDirection().x() << G4endl;
  //G4cout << " @ x0 out      : " << x0_outOfWorld << G4endl;
/*  G4bool transmit = (((fDirX >= 0.0 && position.x() >= fXendAbs) ||
                      (fDirX < 0.0 && position.x() <= fXstartAbs))
                      && energy > 0.0);
  G4bool reflect  = (((fDirX >= 0.0 && position.x() <= fXstartAbs) ||
                      (fDirX < 0.0 && position.x() <= fXendAbs))
                      && energy > 0.0);
  G4bool notabsor = (transmit || reflect);
i*/
}

