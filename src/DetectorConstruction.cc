#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4SystemOfUnits.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4AutoDelete.hh"

#include "G4SolidStore.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"
#include "F02ElectricFieldSetup.hh"

#include "G4GeometryManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction(), 
fPhysiWorld(nullptr), fLogicWorld(nullptr), fSolidWorld(nullptr),  
fWorldMaterial(nullptr),  flogicTarget(nullptr),  ftargetSolid(nullptr),  
fSolidDetectorTop(nullptr),fLogicDetectorTop(nullptr), fPhysiDetectorTop(nullptr),
fSolidDetector(nullptr), fLogicDetector(nullptr),fPhysiDetector(nullptr),
fMaterial(nullptr), fRegion(nullptr)
{
  fTargetL = 16000.*nm;
  fTargetExternalD = 1200*nm;
  fTargetD = 400.*nm;;
  if ( fTargetL > fTargetExternalD ) fBoxSize = 1.2*fTargetL;
  else fBoxSize = 1.2*fTargetExternalD;
  fDistancePlate = 10.*nm;
  DefineMaterials(); 
  SetMaterial("G4_Galactic");

  fDetectorMessenger = new DetectorMessenger(this);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstruction::~DetectorConstruction()
{
  delete fPhysiWorld;
  delete fLogicWorld;  
  delete fSolidWorld;  
  //delete fWorldMaterial;   // no delete because link to database
  delete flogicTarget;
  delete ftargetSolid;
  delete fMaterial; 
  delete fRegion;
  delete fDetectorMessenger;

  delete fSolidDetectorTop;
  delete fLogicDetectorTop;
  //delete fPhysiDetectorTop;

  delete fSolidDetector;
  delete fLogicDetector;
  //delete fPhysiDetector;

  //delete fTargetL;
  //delete fTargetD;
  //delete fTargetExternalD;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstruction::DefineMaterials()
{ 

  // Silicon is defined from NIST material database
  G4NistManager * man = G4NistManager::Instance();
//  fMaterial =  man->FindOrBuildMaterial("G4_Cu");
//  fMaterial =  man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  fMaterial =  man->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  G4String symbol;             //a=mass of a mole;
  G4double A, Z, fractionmass;      //z=mean number of protons;  

  G4double density =  1.e-25*g/cm3; //, massfraction;
  G4int nel;
 
  G4Element* elN  = new G4Element("Nitrogen","N",  Z=7.,  A= 14.00674*g/mole);
  G4Element* elO  = new G4Element("Oxygen",  "O",  Z=8.,  A= 15.9994*g/mole);


  G4double atomicNumber = 1.;
G4double massOfMole = 1.01*g/mole;
//G4double temperature = 2.73*kelvin;
G4double temperature = 0.1*kelvin;
G4double pressure = 3.e-18*pascal;
  //density = universe_mean_density;
  //G4double a;


  //a = 1.01*g / mole;
  //pressure = 1.e-19*pascal;^M
  //temperature = 0.1*kelvin;^M
  //density = universe_mean_density;^M


  // vacuum
  //density = universe_mean_density;
  //G4Material* Vacuum = new G4Material("Vacuum", density, nel=2);
  //Vacuum-> AddElement(elN, .7);
  //Vacuum-> AddElement(elO, .3);
  G4Material* Vacuum = new G4Material("Vacuum", atomicNumber, massOfMole, 
		  density, kStateGas,
		  temperature, pressure);


  //fWorldMaterial =  man->FindOrBuildMaterial("G4_Galactic");  // world material;
  //fWorldMaterial =  man->FindOrBuildMaterial("G4_Galactic");  // world material;
  fWorldMaterial =  man->FindOrBuildMaterial("Vacuum");  // world material;
 
  G4cout << " -----------------------------------------------> WORLD MATERIAL " << fWorldMaterial << G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



void DetectorConstruction::SetMaterial(const G4String& materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = 
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  
  if (pttoMaterial) {
    fMaterial = pttoMaterial;
    if ( flogicTarget ) { flogicTarget->SetMaterial(fMaterial); }
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

// -----------------------
void DetectorConstruction::SetDiameter(G4double value)
{
  fTargetD = value;
}

// -----------------------
void DetectorConstruction::SetLength(G4double value)
{
  fTargetL = value;
}

// -----------------------
void DetectorConstruction::SetDistancePlate(G4double value)
{
  fDistancePlate = value;
}

// -----------------------

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
  fXposTarget = 0.;

  fXstartTarget = fXposTarget-0.5*fTargetL;
  fXendTarget   = fXposTarget+0.5*fTargetL;
  
  fWorldSizeX  = 1.1*fTargetExternalD; 
  fWorldSizeY  = 1.1*fTargetExternalD; 
  fWorldSizeZ  = 1.1*fTargetL; 

  fSolidWorld = new G4Box("World",				     //its name
			   fWorldSizeX/2,fWorldSizeY/2,fWorldSizeZ/2);  //its size
  

  fLogicWorld = new G4LogicalVolume(fSolidWorld,	//its solid
				   fWorldMaterial,		//its material
				   "World");		//its name
  
  fPhysiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 "World",		//its name
                                 fLogicWorld,		//its logical volume
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number


   // /////////////////  //
   //   Target ( NCP )  //
   // ////////////////  //

  //ftargetSolid = new G4Box("Target",				     //its name
  //				 TargetSizeX/2,TargetSizeY/2, TargetSizeZ/2);   //its size
  //				 //TargetSizeX/2,TargetSizeY/2, );   //its size
  

    ftargetSolid = new G4Tubs("Target",
                              //(fTargetExternalD-fTargetD)/2,(fTargetExternalD)/2, fTargetL/2,
                              (fTargetD)/2,(fTargetExternalD)/2, fTargetL/2,
                              //(fTargetD-600.*nm)/2,(fTargetD-200*nm)/2, fTargetL/2,
                              0. ,
                              360.*deg);



  flogicTarget = new G4LogicalVolume(ftargetSolid,       //its solid
						     fMaterial,	//its material
						     "Target");		//its name
  
  new G4PVPlacement(0,			                               //no rotation
		    G4ThreeVector(0.,0.,fXposTarget ),	                               //at (0,0,0)
		    "Target",		//its name
		    flogicTarget,	//its logical volume
		    fPhysiWorld,		//its mother  volume
		    false,		//no boolean operation
		    0);			//copy number




  // ///////////////////////////////////////////// //
  //  Sensitive detectors at the end of the NCPs   //
  // ///////////////////////////////////////////// //
  
  
   //fSolidDetector  = new G4Box("solidDetector", 0.5*nanometer, 0.5*nanometer, 0.5*nanometer);
   fSolidDetector  = new G4Box("solidDetector", 0.98*(fWorldSizeX/2), 0.98*(fWorldSizeY/2), 1*nanometer);
  //G4Box *fSolidDetector  = new G4Box("solidDetector", 0.5*micrometer, 0.5*micrometer, 0.5*micrometer);
  
  fLogicDetector = new G4LogicalVolume(fSolidDetector,fWorldMaterial,"logicDetector");

  fPhysiDetector = new G4PVPlacement(0,
                    G4ThreeVector(0,0,(0.5*fTargetL+fDistancePlate)),
                    fLogicDetector, "physDetector",fLogicWorld, false,true);

  /*
  G4int num = 10;
  //G4int num = 100;

    for (G4int i = 0 ; i < num; i++)
   {
      for (G4int j = 0 ; j < num; j++)
      {
        //char c = j+i*num;
        G4VPhysicalVolume *fPhysiDetector = new G4PVPlacement(0,
                                            G4ThreeVector( (-num/2 + i)*nanometer,
                                                            (-num/2 + j)*nanometer,
                                                            (fTargetL)/2 + 20*nanometer),
                                            fLogicDetector, "physDetector",fLogicWorld, false, j+i*num, true);
      }
  }
  */
  // ----
  //G4Tubs *fSolidDetectorTop  = new G4Tubs("solidDetectorTop", 0., (fTargetExternalD - fTargetD -1*nm)/2, 0.001*nm,0., 360.*deg);
  //G4Tubs *fSolidDetectorTop  = new G4Tubs("solidDetectorTop", 0., (fTargetD -1*nm)/2, 0.001*nm,0., 360.*deg);
  fSolidDetectorTop  = new G4Tubs("solidDetectorTop", 0., (fTargetD -1*nm)/2, 0.001*nm,0., 360.*deg);
  fLogicDetectorTop = new G4LogicalVolume(fSolidDetectorTop,fWorldMaterial,"logicDetectorTop");
  //G4VPhysicalVolume *fPhysiDetectorTop = new G4PVPlacement(0,
  fPhysiDetectorTop = new G4PVPlacement(0,
                      G4ThreeVector(0,0,(-0.5*fTargetL)),
                      fLogicDetectorTop, "physDetectorTop",fLogicWorld, false,true);

  // Visualization attributes
  G4VisAttributes* worldVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //White
  worldVisAtt->SetVisibility(true);
  fLogicWorld->SetVisAttributes(worldVisAtt);

  G4VisAttributes* worldVisAtt1 = new G4VisAttributes(G4Colour::Yellow()); 
  worldVisAtt1->SetVisibility(true);
  flogicTarget->SetVisAttributes(worldVisAtt1);

  // Create Target G4Region and add logical volume
  
  fRegion = new G4Region("Target");
  //fWorld = new G4Region("World");
  
  G4ProductionCuts* cuts = new G4ProductionCuts();
  
  G4double defCut = 1*nanometer;
  cuts->SetProductionCut(defCut,"gamma");
  cuts->SetProductionCut(defCut,"e-");
  cuts->SetProductionCut(defCut,"e+");
  cuts->SetProductionCut(defCut,"proton");
  
  fRegion->SetProductionCuts(cuts);
  fRegion->AddRootLogicalVolume(flogicTarget); 
  //fWorld->AddRootLogicalVolume(fLogicWorld); 

  return fPhysiWorld;
}
 
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSize(G4double value)
{
  fBoxSize = value;
  if(ftargetSolid) {
    //ftargetSolid->SetXHalfLength(fBoxSize/2);
    //ftargetSolid->SetYHalfLength(fBoxSize/2);
    ftargetSolid->SetZHalfLength(fTargetL/2);
  }
  fWorldSizeX = value*1.2;
  fWorldSizeY = value*1.2;
  fWorldSizeZ = value*1.2;
  
  if(fSolidWorld) {
    fSolidWorld->SetXHalfLength(fWorldSizeX/2);
    fSolidWorld->SetYHalfLength(fWorldSizeY/2);
    fSolidWorld->SetZHalfLength(fWorldSizeZ/2);
  }
  
}
// ---- 

void DetectorConstruction::UpdateGeometry()
{

  G4cout << "UpdateGeometry Begin" << G4endl;
  G4GeometryManager::GetInstance()->OpenGeometry();

  if (ftargetSolid){
      ftargetSolid->SetInnerRadius(fTargetD/2.);
      ftargetSolid->SetOuterRadius(fTargetExternalD/2.);
      ftargetSolid->SetZHalfLength(fTargetL/2.);

  }
  fWorldSizeX  = 1.1*fTargetExternalD; 
  fWorldSizeY  = 1.1*fTargetExternalD; 
  fWorldSizeZ  = 1.1*fTargetL; 

  if (fSolidWorld){
      fSolidWorld->SetXHalfLength(fWorldSizeX/2);
      fSolidWorld->SetYHalfLength(fWorldSizeY/2);
      fSolidWorld->SetZHalfLength(fWorldSizeZ/2+fDistancePlate);
  }

  if (fSolidDetectorTop){
      fSolidDetectorTop->SetInnerRadius(0.);
      fSolidDetectorTop->SetOuterRadius( (fTargetD-1*nm)/2. );
      //fSolidDetectorTop->SetTranslation(G4ThreeVector(0,0,-0.5*fTargetL));
  }

  if (fPhysiDetectorTop){
    fPhysiDetectorTop->SetTranslation(G4ThreeVector(0,0,-0.5*fTargetL));
  }

  if (fPhysiDetector){
    fPhysiDetector->SetTranslation(G4ThreeVector(0,0,0.5*fTargetL+fDistancePlate));
  }

  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << " New NCP diameter : " << GetTargetDiameter()/nm ;
  G4cout << " , New NCP Lenght = " << GetTargetLength()/nm ;
  G4cout << ", New NCP Thickness = " << GetTargetThickness()/nm << G4endl;
  G4cout << " , New NCP Lenght = " << GetTargetLength()/nm ;
  G4cout << ", New Distance End of Tube SD Plate  = " << GetDistanceToPlate()/nm << G4endl;
  G4cout << " ---------------- UpdateGeometry End --------------------- " << G4endl;
       // system("pause");^M


}


// ----
void DetectorConstruction::ConstructSDandField()
{


  SensitiveDetector * sensDetTop = new SensitiveDetector("SensitiveDetectorTop");
  fLogicDetectorTop->SetSensitiveDetector(sensDetTop);

  SensitiveDetector * sensDet = new SensitiveDetector("SensitiveDetector");
  fLogicDetector->SetSensitiveDetector(sensDet);

  //if ( fFieldMessenger.Get() == 0 ) {
  if ( !fEmFieldMessenger.Get()  ) {

    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    //G4ThreeVector fieldValue = G4ThreeVector();
    //G4GlobalMagFieldMessenger* msg =
    //  new G4GlobalMagFieldMessenger(fieldValue);
    //msg->SetVerboseLevel(1);
    //G4AutoDelete::Register(msg);
    //:0fFieldMessenger.Put( msg );

    F02ElectricFieldSetup* fieldSetup = new F02ElectricFieldSetup();
    G4AutoDelete::Register(fieldSetup); //Kernel will delete the messenger
    fEmFieldSetup.Put(fieldSetup);

  }
}