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
// G4MicroElecLOPhononModel.cc, 
//                   	    	2020/05/20 P. Caron, C. Inguimbert are with ONERA [b] 
//				       	   Q. Gibaru is with CEA [a], ONERA [b] and CNES [c]
//				            M. Raine and D. Lambert are with CEA [a]
//
// A part of this work has been funded by the French space agency(CNES[c])
// [a] CEA, DAM, DIF - 91297 ARPAJON, France
// [b] ONERA - DPHY, 2 avenue E.Belin, 31055 Toulouse, France
// [c] CNES, 18 av.E.Belin, 31401 Toulouse CEDEX, France
//
// Based on the following publications
//
//      - J. Pierron, C. Inguimbert, M. Belhaj, T. Gineste, J. Puech, M. Raine
//	      Electron emission yield for low energy electrons: 
//	      Monte Carlo simulation and experimental comparison for Al, Ag, and Si
//	      Journal of Applied Physics 121 (2017) 215107. 
//               https://doi.org/10.1063/1.4984761
//
//      - P. Caron,
//	      Study of Electron-Induced Single-Event Upset in Integrated Memory Devices
//	      PHD, 16th October 2019
//
//	- Q.Gibaru, C.Inguimbert, P.Caron, M.Raine, D.Lambert, J.Puech, 
//	      Geant4 physics processes for microdosimetry and secondary electron emission simulation : 
//	      Extension of MicroElec to very low energies and new materials
//	      NIM B, 2020, in review.
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "G4MicroElecLOPhononModel.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4CsvAnalysisManager.hh"
#include "G4VAnalysisManager.hh"								  
G4MicroElecLOPhononModel::G4MicroElecLOPhononModel(const G4ParticleDefinition*,
				 const G4String& nam) 
  : G4VEmModel(nam),isInitialised(false)
{
  fParticleChangeForGamma = GetParticleChangeForGamma();
	G4cout << "Phonon model is constructed " << G4endl
		<< "Phonon Energy = " << phononEnergy / eV << " eV  "<< G4endl;											   															 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4MicroElecLOPhononModel::~G4MicroElecLOPhononModel() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4MicroElecLOPhononModel::Initialise(const G4ParticleDefinition*,
				          const G4DataVector& /*cuts*/)
{ 
// correction CI 12/1/2023 remove
/*
  if (isInitialised) { return; }
  fParticleChangeForGamma = GetParticleChangeForGamma();
  isInitialised = true;
 */
 // correction CI 12/1/2023 remove
 // correction CI 12/1/2023 add
 if (isOkToBeInitialised == true && isInitialised == false) {
		
		//if (verboseLevel > 3)
		G4cout << "Calling G4LOPhononModel (" << phononEnergy / eV << " eV)" << "::Initialise()" << G4endl;
		if (isInitialised) { return; }
		fParticleChangeForGamma = GetParticleChangeForGamma();
		isInitialised = true;//*/
	}
 // correction CI 12/1/2023 add
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4MicroElecLOPhononModel::CrossSectionPerVolume(const G4Material* material,
						const G4ParticleDefinition* p,
						G4double ekin,
						G4double emin, G4double emax)
{
	
// correction CI 12/1/2023 remove
/*
  if (material->GetName()!="G4_SILICON_DIOXIDE") return 0.0;

  const G4double e = CLHEP::eplus / CLHEP::coulomb;
  const G4double m0 = CLHEP::electron_mass_c2 / (CLHEP::c_squared*CLHEP::kg);
  const G4double h = CLHEP::hbar_Planck * CLHEP::s/ (CLHEP::m2*CLHEP::kg);
  const G4double eps0 = CLHEP::epsilon0 * CLHEP::m/ (CLHEP::farad);
  const G4double kb = CLHEP::k_Boltzmann * CLHEP::kelvin/ CLHEP::joule;

  // Parameters SiO2  
  phononEnergy = (0.75*0.153+0.25*0.063 )* CLHEP::eV;
  const G4double eps = 3.84;
  const G4double einf = 2.25;
  const G4double T = 300;  // should be taken from material property
      
  G4double E =(ekin/CLHEP::eV)*e;
  
  G4double hw = (phononEnergy / CLHEP::eV) * e;
  G4double n = 1.0 / (std::exp(hw / (kb*T)) - 1); //Phonon distribution
    
  G4double signe = (absor) ? -1. : 1.;
    
  G4double racine = std::sqrt(1. + ((-signe*hw) / E));
  
  G4double P = (std::pow(e, 2) / (4 * pi*eps0*h*h)) * (n + 0.5 + signe*0.5) * ((1 / einf) - (1 / eps)) 
    * std::sqrt(m0 / (2 * E)) *hw* std::log((1 + racine) / (signe * 1 + ((-signe)*racine)));
  
  G4double MFP = (std::sqrt(2. * E / m0) / P)*m;
  return 2. / MFP;   
*/													
// correction CI 12/1/2023 remove
// correction CI 12/1/2023 add
const G4double e = CLHEP::eplus / CLHEP::coulomb;
	const G4double m0 = CLHEP::electron_mass_c2 / (CLHEP::c_squared*CLHEP::kg);
	const G4double h = CLHEP::hbar_Planck * CLHEP::s/ (CLHEP::m2*CLHEP::kg);
	const G4double eps0 = CLHEP::epsilon0 * CLHEP::m/ (CLHEP::farad);
	const G4double kb = CLHEP::k_Boltzmann * CLHEP::kelvin/ CLHEP::joule;
	const G4double T = 300;
	G4double eps = 9;
	G4double	einf = 3;
	
	
	isOkToBeInitialised = true;
	const G4MaterialCutsCouple* cup = CurrentCouple();
	const G4DataVector cuts;
	Initialise(p, cuts);
	
	if (material->GetName() != "G4_SILICON_DIOXIDE" && material->GetName() != "G4_ALUMINUM_OXIDE" && material->GetName() != "G4_BORON_NITRIDE") return 1 / DBL_MAX;
	
   G4double E =(ekin/eV)*e;

	if (material->GetName() == "G4_ALUMINUM_OXIDE")
	{
		eps = 9;
		einf = 3;
		phononEnergy = 0.1*eV;
	}
	if (material->GetName() == "G4_SILICON_DIOXIDE")
	{
		
		eps = 3.84;
		einf = 2.25;	
		phononEnergy = (0.75*0.153+0.25*0.063 )* eV;
	}
	
	// Nuclear Instruments and Methods in Physics Research Section B: Beam Interactions with Materials and Atoms
	//Volume 454, 1 September 2019, Pages 14 - 22
	//Nuclear Instruments and Methods in Physics Research Section B : Beam Interactions with Materials and Atoms
	//Monte Carlo modeling of low - energy electron - induced secondary electron emission yields in micro - architected boron nitride surfaces
	//Author links open overlay panelHsing - YinChangaAndrewAlvaradoaTreyWeberaJaimeMarianab
	if (material->GetName() == "G4_BORON_NITRIDE")
	{
		eps = 7.1;
		einf = 4.5;
		phononEnergy = 0.17 * eV;
	}


	G4double hw = (phononEnergy / eV) * e;
	G4double n = 1.0 / (exp(hw / (kb*T)) - 1); //Phonon distribution

	if (absor) { //Absorption
		Eprim = E + hw;
		signe = -1;
	}
	else { //Emission 
		Eprim = E - hw;
		signe = +1;
	}//*/
	
	G4double racine = sqrt(1 + ((-signe*hw) / E));
  
	G4double P = (pow(e, 2) / (4 * pi*eps0*h*h)) * (n + 0.5 + signe*0.5) * ((1 / einf) - (1 / eps)) * sqrt(m0 / (2 * E)) *hw* log((1 + racine) / (signe * 1 + ((-signe)*racine)));
																							
  
	G4double MFP = (sqrt(2 * E / m0) / P)*m;
	
	if (material->GetName() == "G4_SILICON_DIOXIDE") { return 2 / MFP; }
	return 1/(MFP);
// correction CI 12/1/2023 add					
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4MicroElecLOPhononModel::SampleSecondaries(
                               std::vector<G4DynamicParticle*>*,
			       const G4MaterialCutsCouple*,
			       const G4DynamicParticle* aDynamicElectron,
			       G4double tmin, G4double maxEnergy)
{

  G4double E = aDynamicElectron->GetKineticEnergy();
  G4double Eprim = E;
  Eprim = (absor) ? E + phononEnergy : E - phononEnergy;   

  G4double rand = G4UniformRand();
  G4double B = (E + Eprim + 2 * std::sqrt(E*Eprim)) / (E + Eprim - 2 * std::sqrt(E*Eprim));
  G4double cosTheta = ((E + Eprim) / (2 * std::sqrt(E*Eprim)))*(1 - std::pow(B, rand)) + std::pow(B, rand);
  
  if(Interband){
    cosTheta = 1 - 2 * G4UniformRand(); //Isotrope
  }
  G4double phi = twopi * G4UniformRand();
  G4ThreeVector zVers = aDynamicElectron->GetMomentumDirection();
  G4ThreeVector xVers = zVers.orthogonal();
  G4ThreeVector yVers = zVers.cross(xVers);
  
  G4double xDir = std::sqrt(1. - cosTheta*cosTheta);
  G4double yDir = xDir;
  xDir *= std::cos(phi);
  yDir *= std::sin(phi);
  
  G4ThreeVector zPrimeVers((xDir*xVers + yDir*yVers + cosTheta*zVers));
  
  fParticleChangeForGamma->ProposeMomentumDirection(zPrimeVers.unit());
  fParticleChangeForGamma->SetProposedKineticEnergy(Eprim);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
