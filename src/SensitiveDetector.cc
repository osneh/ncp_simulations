// ********************************************************************
//---------------------------------------------------------------------------
//
// ClassName:   Sensitive Detector Class
//
// Description: The process to kill e- to save CPU
//
// Author:      Henso ABREU 24/04/2023
//----------------------------------------------------------------------------
//
// Class description:
//
// SEY : Secondary Electron Emission Yield
// detector to be used to count the number of secondary electrons
// emitted by a NCP
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include	<iostream>
#include	<fstream>
#include	<sstream>


//#include "G4RunManager.hh"

//#include "DepthSampling.hh"
#include	"SensitiveDetector.hh"
//#include	"SteppingAction.hh"
//#include	"RunAction.hh"
//#include	"G4StepPoint.hh"
//#include	"G4HCofThisEvent.hh"
//#include	"G4Step.hh"
//#include	"G4ThreeVector.hh"
//#include	"G4SDManager.hh"
//#include	"G4ios.hh"
//#include	"G4VProcess.hh"
//#include	"G4EmCalculator.hh"
//#include	"G4Material.hh"
//#include	"G4UnitsTable.hh"
//#include	"G4EnergyLossTables.hh"  
//#include	"G4VPhysicalVolume.hh"
//#include	"PrimaryGeneratorAction.hh"
//#include	"G4RunManager.hh"
//#include	"DetectorConstruction.hh"
//#include    "G4ParticleDefinition.hh"
//#include	"G4SystemOfUnits.hh"
//#include	"G4VHit.hh"
//#include	"G4AttDef.hh"
//#include "Analysis.hh"
#include "G4SystemOfUnits.hh"




using namespace std;

#include	<vector>
/*
SensitiveDetector::SensitiveDetector(const G4String& name,
	const G4String& hitsCollectionName)
	: G4VSensitiveDetector(name), fHitsCollection(NULL)
{
	compteurPrimaire, compteurSec, compteurTot, compteur50, nbPrim, nbSec , nbSup50= 0;
	ResetCounters();

	G4String HCname;
	collectionName.insert(HCname = "SensitiveDetector");
}
*/
SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) { }
SensitiveDetector::~SensitiveDetector() { }

//***********************************************************************************

//called for each step in sensitive volume
G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhits)
{
    
	G4Track* aTrack = aStep->GetTrack();
	
	//G4int trackID = aStep->GetTrack()->GetTrackID();
	//G4int ParentID = aStep->GetTrack()->GetParentID();
	
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();


    G4ThreeVector posElectron = preStepPoint->GetPosition();

    //G4cout << " -->> Electron Position : " << posElectron << G4endl;

	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

	G4int copyNb = touchable->GetCopyNumber();

	//G4cout << "Copy Number: " << copyNb << endl;
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();

	//G4cout << " -->> Detector position:" << posDetector << G4endl;
	G4double hitTime = preStepPoint->GetGlobalTime(); 
	//G4cout << " -->> Time:" << hitTime << G4endl;

	G4double energy = aTrack->GetKineticEnergy();



	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    //G4cout << " -->> Detector position:" << posDetector << G4endl;
//	G4cout<< " -->>TEST pos 0,1,2  : " << posDetector[0]<<","<<posDetector[1]<<","<<posDetector[2] << G4endl;
//	G4cout << " -->> Energy:" << energy << G4endl;
//	G4cout << " -->> EVENT:" << evt  << G4endl;
	// //////////////////////////////// // 
	//    		Filling Ntuple 			//
	// //////////////////////////////// // 
	analysisManager->FillNtupleIColumn(1,0,evt);
	analysisManager->FillNtupleDColumn(1,1,posDetector[0]/nm);
	analysisManager->FillNtupleDColumn(1,2,posDetector[1]/nm);
	analysisManager->FillNtupleDColumn(1,3,posDetector[2]/nm);
	analysisManager->FillNtupleDColumn(1,4,energy/eV );
	analysisManager->FillNtupleDColumn(1,5,hitTime);
	analysisManager->AddNtupleRow(1);
	/*
	G4String processName;
	const G4VProcess* creatorProcess = aStep->GetTrack()->GetCreatorProcess();
	if (creatorProcess) { processName = creatorProcess->GetProcessName(); }
	if (aStep->GetPostStepPoint()) { G4String processNamePSP = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName(); }
	
	
	const G4DynamicParticle* particleDyn = aStep->GetTrack()->GetDynamicParticle();
	G4ParticleDefinition* particleDef = particleDyn->GetDefinition();
	G4int PDGEncoding = particleDef->GetPDGEncoding();
	G4String ParticleTypeName = particleDef->GetParticleType();
	G4String ParticleName = particleDef->GetParticleName();

	G4int Z = particleDef->GetAtomicNumber();
	G4int A = particleDef->GetAtomicMass();
	G4double Vertex_kinetic_energy = aStep->GetTrack()->GetVertexKineticEnergy();
	G4ThreeVector Vertex_Position = aStep->GetTrack()->GetVertexPosition();
	G4ThreeVector Vertex_Moment = aStep->GetTrack()->GetVertexMomentumDirection();

	G4double	Particle_kinetic_energy=999.;
	G4double	Post_kinetic_energy=999.;
	G4ThreeVector PreImpulsion;
	G4ThreeVector PostImpulsion;
	G4ThreeVector PrePosition;
	G4ThreeVector PostPosition;
	G4int CopyNumber;

	if (aStep->GetPreStepPoint()) { 
		Particle_kinetic_energy = aStep->GetPreStepPoint()->GetKineticEnergy(); 
		PreImpulsion = aStep->GetPreStepPoint()->GetMomentumDirection();
		PrePosition = aStep->GetPreStepPoint()->GetPosition();
		CopyNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
	}
	if (aStep->GetPostStepPoint()) { 
		Post_kinetic_energy = aStep->GetPostStepPoint()->GetKineticEnergy(); 
		PostImpulsion = aStep->GetPostStepPoint()->GetMomentumDirection();
		PostPosition = aStep->GetPostStepPoint()->GetPosition();
	}


	G4double edep = aStep->GetTotalEnergyDeposit();
	G4double NIedep = aStep->GetNonIonizingEnergyDeposit();
	G4double stepLength = aStep->GetStepLength();

	G4String			originalVolume_;
	G4String			originalVolume_materialName;
	originalVolume_ = aStep->GetTrack()->GetTouchableHandle()->GetVolume()->GetName();
	originalVolume_materialName = aStep->GetTrack()->GetMaterial()->GetName();

	
	if (ParentID == 0)	{nbPrim++;}
	if (ParentID > 0)	{nbSec++;}
	if (Particle_kinetic_energy/eV >= 50.0) { nbSup50++; }



	MicroElecHitSey* newHit = new MicroElecHitSey();
	
	newHit->SetNbPrim(nbPrim);
	newHit->SetNbSec(nbSec);
	newHit->SetNbSup50(nbSup50); 
	newHit->SetPDGEncoding(PDGEncoding);
	newHit->SetParentID(ParentID);
	newHit->SetTrackID(trackID);
	newHit->SetParticleType(ParticleTypeName);
	newHit->SetParticleName(ParticleName);
	newHit->SetVolumeName(originalVolume_);
	newHit->SetZ(Z);
	newHit->SetA(A);
	newHit->SetVertexKineticEnergy(Vertex_kinetic_energy);
	newHit->SetVertexPos(Vertex_Position);
	newHit->SetVertexMomentum(Vertex_Moment);
	newHit->SetPreStepKineticEnergy(Particle_kinetic_energy);
	newHit->SetPostStepKineticEnergy(Post_kinetic_energy);
	newHit->SetEdep(edep);
	newHit->SetNi_Edep(NIedep);
	newHit->SetStepLength(stepLength);
	newHit->SetPreStepMomentum(PreImpulsion);
	newHit->SetPostStepMomentum(PostImpulsion);
	newHit->SetPrePos(PrePosition);
	newHit->SetPostPos(PostPosition);
	
	fHitsCollection->insert(newHit);
	aTrack->SetTrackStatus(fStopAndKill);
		
		// --------------------------------- Output check -------------------------------------------
		//G4cout << "Process hits" << G4endl;
		//G4cout << "Primaire:" << nbPrim << ", secondaire:" << nbSec << G4endl;
		//G4cout << "  [trackID, Parent ID]: [" << trackID << ", " << ParentID << "],  ParticleName :" << ParticleName << ", [PrimID, SecID]=[" << nbPrim << ", " << nbSec << "]" << G4endl;
		////G4cout << "   Kinetic energy : " << G4BestUnit(PreStepKineticEnergy, "Energy") << ", position : " << G4BestUnit(PrePos, "Length") << G4endl;
		//system("PAUSE");
		// --------------------------------- Output check -------------------------------------------
		//return true;
    */
   return true;

}
// END - Interactions::ProcessHits


			

 //***********************************************************************************
 //called at end of event
/*
void MicroElecSdSey::EndOfEvent(G4HCofThisEvent*)
{

	MicroElecRun* run = static_cast<MicroElecRun*>(
		G4RunManager::GetRunManager()->GetNonConstCurrentRun());

	std::stringstream ss;
	ss << 1 + run->GetRunID() << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

	//run->
	
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++

	compteurPrimaire = compteurPrimaire + nbPrim;
	compteurSec = compteurSec + nbSec;
	compteur50 = compteur50 + nbSup50;
	compteurTot = compteurTot+ nbPrim+ nbSec;

	run->AddElecPrimScorer(nbPrim);
	run->AddElecSecoScorer(nbSec);
	run->AddElecSup50Scorer(nbSup50);
	run->AddElecTotaScorer(nbPrim+ nbSec);
	
	

	// --------------------------------- Output check -------------------------------------------
	//G4cout << "Detector End of Event" << G4endl;
	//G4cout << "Primaire Evt:" << nbPrim << ", secondaire evt:" << nbSec << G4endl;
	//G4cout << "Primaire Sum:" << compteurPrimaire << ", secondaire Sum:" << compteurSec << G4endl;
	//system("PAUSE");
	// --------------------------------- Output check -------------------------------------------

	nbPrim = 0.0;
	nbSec = 0.0;
	nbSup50 = 0.0;


}
*/
/*

//***********************************************************************************
void Interactions::Clear()
{
}



//***********************************************************************************
void Interactions::PrintAll()
{
}
*/


