#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

class SensitiveDetector : public G4VSensitiveDetector 
{
    public:
    SensitiveDetector(G4String);
    ~SensitiveDetector();

    private:
        virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif 