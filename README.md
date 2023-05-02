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

To run the simulation, use the created executable with a configuration file (a sample is provided)

```
./ncp_sim
```

### Configuration Files

There is a configuration file provided needed to run the simulation: ncp_sim.mac

#### run.mac

The current example ncp_sim.mac file looks like this:
```
/run/printProgress 1000
/run/beamOn 50000
```

This will run 50,000 events and print the progress every 1,000 events.


## Integrate with your tools
