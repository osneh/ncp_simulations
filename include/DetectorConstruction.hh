#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Cache.hh"

#include "SensitiveDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4LogicalVolume;
class G4Region;
class DetectorMessenger;
class G4UserLimits;
class G4Box;
class G4Tubs;

class F02ElectricFieldSetup;
class G4GlobalMagFieldMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  DetectorConstruction();
  ~DetectorConstruction();

  G4VPhysicalVolume* Construct();

  void SetSize     (G4double);              
  void SetMaterial (const G4String&);

  inline const G4VPhysicalVolume* GetWorld() const {return fPhysiWorld;};
  inline G4double GetSize() const                  {return fBoxSize;};      
  inline const G4Material* GetMaterial() const     {return fMaterial;};

  G4double GetWorldSizeZ() const                {return fWorldSizeZ;}; // Added by Henso
  G4double GetTargetHalfHeight() const          {return fTargetHeight*0.5;}; // Added by Henso

  void   PrintParameters();
  void   DefineMaterials(); 
  void   SetTargetHeight (G4double);

  G4Region* GetTargetRegion()  {return fRegion;}
  //G4Region* GetWorldRegion()  {return fWorld;}

  void ConstructSDandField() override;


private:
   
  G4double           fWorldSizeX;
  G4double           fWorldSizeY;
  G4double           fWorldSizeZ;

  G4VPhysicalVolume* fPhysiWorld;
  G4LogicalVolume*   fLogicWorld;  
  G4Box*             fSolidWorld;

  G4Material*        fWorldMaterial;
  G4LogicalVolume*   flogicTarget;
  //G4Box*             ftargetSolid;
  G4Tubs*             ftargetSolid;
 
  G4double           fXposTarget;
  G4double           fXstartTarget, fXendTarget;

  G4double           fBoxSize;  
  G4Material*        fMaterial; 
  G4Region*          fRegion;
  //G4Region*          fWorld;

  G4double           fTargetHeight;
  G4double           fTargetSizeXY;


  G4UserLimits* fStepLimit;

  DetectorMessenger* fDetectorMessenger;
  G4VPhysicalVolume* ConstructDetector();     
  
  G4Cache<F02ElectricFieldSetup*> fEmFieldSetup;
  G4Cache<G4GlobalMagFieldMessenger*> fEmFieldMessenger;


  G4LogicalVolume*   fLogicDetector;  // Sensitive Detector 
  G4LogicalVolume*   fLogicDetectorTop;  // Sensitive Detector TOP


};
#endif
