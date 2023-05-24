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
/// \file electromagnetic/microelectronics1/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),fDetector(Det),
 fmicroelectronicsDir(0),
 fDetDir(0),
 fMaterCmd(0),
 fSizeDCmd(0),
 fSizeLCmd(0),
 fDistancePlateCmd(0),
 fUpdateCmd(0)
{ 
  
  fmicroelectronicsDir = new G4UIdirectory("/ncp/");
  fmicroelectronicsDir->SetGuidance("commands specific to this example");
  
  fDetDir = new G4UIdirectory("/ncp/det/");
  fDetDir->SetGuidance("detector construction commands");
        
  fMaterCmd = new G4UIcmdWithAString("/ncp/det/setMat",this);
  fMaterCmd->SetGuidance("Select material of the box.");
  fMaterCmd->SetParameterName("choice",false);
  fMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fMaterCmd->SetToBeBroadcasted(false);
  
  fSizeDCmd = new G4UIcmdWithADoubleAndUnit("/ncp/det/setDiameter",this);
  fSizeDCmd->SetGuidance("Set Diameter of the tube");
  fSizeDCmd->SetParameterName("Diameter",false);
  fSizeDCmd->SetRange("Diameter>0.");
  fSizeDCmd->SetUnitCategory("Length");
  fSizeDCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSizeDCmd->SetToBeBroadcasted(false);

  fSizeLCmd = new G4UIcmdWithADoubleAndUnit("/ncp/det/setLenght",this);
  fSizeLCmd->SetGuidance("Set Lenght of the tube");
  fSizeLCmd->SetParameterName("Lenght",false);
  fSizeLCmd->SetRange("Lenght>0.");
  fSizeLCmd->SetUnitCategory("Length");
  fSizeLCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSizeLCmd->SetToBeBroadcasted(false);


  fDistancePlateCmd = new G4UIcmdWithADoubleAndUnit("/ncp/det/setDistancePlate",this);
  fDistancePlateCmd->SetGuidance("Set Distance of the end tube and SD plate");
  fDistancePlateCmd->SetParameterName("DistancePlate",false);
  fDistancePlateCmd->SetRange("DistancePlate>0.");
  fDistancePlateCmd->SetUnitCategory("Length");
  fDistancePlateCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fDistancePlateCmd->SetToBeBroadcasted(false);
  
  
  fUpdateCmd = new G4UIcmdWithoutParameter("/ncp/det/update", this);
  fUpdateCmd->SetGuidance("Update Nano-Channel-Plate geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fMaterCmd;
  delete fSizeDCmd; 
  delete fSizeLCmd;
  delete fDistancePlateCmd;
  delete fDetDir;
  delete fmicroelectronicsDir;
  delete fUpdateCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fMaterCmd )
   { fDetector->SetMaterial(newValue);}
   
  if( command == fSizeDCmd )
   { fDetector->SetDiameter(fSizeDCmd->GetNewDoubleValue(newValue));}

  if ( command == fSizeLCmd )
    {fDetector->SetLength(fSizeLCmd->GetNewDoubleValue(newValue)); }

  if ( command == fDistancePlateCmd )
    {fDetector->SetDistancePlate(fDistancePlateCmd->GetNewDoubleValue(newValue)); }

  if ( command == fUpdateCmd )
  { fDetector->UpdateGeometry();}
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
