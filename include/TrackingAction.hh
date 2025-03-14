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

#ifndef TrackingAction_h
#define TrackingAction_h

#include "G4UserTrackingAction.hh"
#include <map>
#include "globals.hh"

class G4Region;
class G4ParticleDefinition;
class DetectorConstruction;

class TrackingAction : public G4UserTrackingAction
{
public:
    TrackingAction(DetectorConstruction* detector = 0);
    ~TrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);


    void SetDetector(DetectorConstruction* detector)
    {
        fDetector = detector;
    }

    std::map<const G4ParticleDefinition*, int>& GetNParticlesCreatedInTarget()
    {
        return fNParticleInTarget;
    }

    std::map<const G4ParticleDefinition*, int>& GetNParticlesCreatedInWorld()
    {
        return fNParticleInWorld;
    }

   std::map<const G4ParticleDefinition*, int>& GetNParticlesCreatedOutTarget()
    {
        return fNParticleOutTarget;
    }


private:
    DetectorConstruction* fDetector;
    G4Region* fTargetRegion;
    //G4Region* fWorldRegion;
    std::map<const G4ParticleDefinition*, int> fNParticleInTarget;
    std::map<const G4ParticleDefinition*, int> fNParticleInWorld;
    std::map<const G4ParticleDefinition*, int> fNParticleOutTarget;
};


#endif 
