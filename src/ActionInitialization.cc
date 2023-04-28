#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction):G4VUserActionInitialization(),
  //fDetectorConstruction(detConstruction)
    fDetector(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
	// In MT mode, to be clearer, the RunAction class for the master thread might be
	// different than the one used for the workers.
	// This RunAction will be called before and after starting the
	// workers.
	// For more details, please refer to :
	// https://twiki.cern.ch/twiki/bin/view/Geant4/Geant4MTForApplicationDevelopers
	//
	//RunAction* runAction= new RunAction(fDetectorConstruction);
	//RunAction* runAction= new RunAction(fDetector);
	//SetUserAction(runAction);
	SetUserAction(new RunAction(fDetector));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
	// G4cout << "Build for = "<< G4RunManager::GetRunManager()->GetRunManagerType() << G4endl;

  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(fDetector);
  SetUserAction(primary);


//	SetUserAction(new PrimaryGeneratorAction);

	TrackingAction* trackingAction = new TrackingAction(fDetector);
	SetUserAction(trackingAction);

	RunAction* runAction= new RunAction(fDetector);
	SetUserAction(runAction);

	SetUserAction(new SteppingAction());
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
