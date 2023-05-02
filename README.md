# Nano Channel Plate SIMULATION
GEANT4 Simulation for a Nano Channel Plate, a Nano tube with an emisive layer for Secondary Electron Yield (SEY) prodcution. The current configuration includes the a developement (physics processes) implemented by the ONERA Group (C. Inguimbert). This new model has an optimization of the stopping power for low energy particles and takes into account the work function and for certain marerials when needed phonon interactions in order to simulate properly the SEY for several materials.



## Getting started

```
cd existing_repo
git remote add origin https://gitlab.in2p3.fr/h.abreu/ncp_sim.git
git branch -M main
git push -uf origin main
```



## Getting Started

These instructions will get you a copy of the project up and running on your machine.

### Prerequisites

Required Packages:

```
GEANT4
ROOT
GSL
CMake
```

### Build and Run

To build and run the simulation with the above packages installed, create and move to a build directory

```
mkdir build && cd build
```

Use CMake to then build and make the simulation executable

```
cmake .. && make
```

In addition, in order to include microelectronics (ONERA) latest models and data
into the simulation, please copy the content of the data.7z at
G4installPATH/share/Geant4-11.0.3/data/G4EMLOW8.0/microelec

To run the simulation, use the created executable with a configuration file (a sample is provided)

```
./ncp_sim
```

### Configuration Files

There is a configuration file provided needed to run the simulation: ncp_sim.mac

#### ncp_sim.mac

The current example ncp_sim.mac file looks like this:
```
#/control/execute vis.mac
/tracking/verbose 1


#/microelectronics/det/setMat G4_Ag
#/microelectronics/det/setMat G4_Al
#/microelectronics/det/setMat G4_C
#/microelectronics/det/setMat G4_Cu
#/microelectronics/det/setMat G4_Ge
#/microelectronics/det/setMat G4_KAPTON
#/microelectronics/det/setMat G4_Ni
#/microelectronics/det/setMat G4_Si   # default material
#/microelectronics/det/setMat G4_SILICON_DIOXIDE
#/microelectronics/det/setMat G4_Ti
#/microelectronics/det/setMat G4_W


#/gun/particle ion
#/gun/ion 20 48 15
/gun/particle e-
/gun/energy 300 eV

#/run/numberOfThreads 2 #if MT is activated

/run/initialize
#/run/printProgress 1
/run/beamOn 1

```

This will run 50,000 events and print the progress every 1,000 events.

