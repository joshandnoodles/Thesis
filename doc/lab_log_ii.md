# Weekly One-One - January 12, 2016

Updates for work done over Christmas Break

Major items:
  - literature search on QP and control aspects
  - reworked optical calculations correcting errors and implementing Chelsea's initial beam diameters following optical lensing
  - created optics board r001
    - initial high-level and logic block designs done in Visio
    - SPICE simulation for RX QP amplification and characterization of load switch
    - schematics completed in Eagle (expect addition of test points)
    - PCB layout started

## Action Items
  - finish PCB layout and review extensively

## Week 1

  - finished optics board r001 layout with minor changes to schematics
  - created 3D model of optics board along with servos and gimbal
  - corrected mechanical issues found through 3D modeling
  - picked new (better) optical lensing system with proper M9 PCB focusing mount

# Weekly One-One - January 12, 2016

Final review of optical communication system before parts are to be ordered

## Action Items

  - coordinate with Matt to request parts to be ordered through Quartz

# Week Two

  - more minor layout changes for PCB
  - ordered parts for two entire setups (minus second gimbal bracket and accompanying servos)
  - begin to put together plan for parts arriving as well as bring-up procedure for optics_r001 PCB

# Week Three

- parts approved over the weekend and ordered on Monday (2017-01-22)
- Gerbers created and sent to OSH Park Monday (2017-01-22)

## Action items to prepare for parts arriving

  - create one power connector jack for OpenROV to USB Mini
  - X acquire two USB Type-A to USB Mini (with data connections)
  - X create bring-up sequence for optics board
    - checking impedances between power rails and ground w/o ICs loaded
    - mounting basic ICs needed for connectivity (excluding PD and LSR diode)
    - measure and characterize power rails (specifically P5V)
    - test load switch functionality with PIC I/O (ensure P5V_LSR off at start)
    - check controllability of current limit voltage reference with potentiometer
    - etc...
  - look at firmware code examples, give priority to development to the following:
    - basic I/O toggling
    - clock speed settings
    - wait functions
    - operation of Clicker 2 LEDs
    - USB Debug interface
  - re-engineer Chrome App debug interface for generic debugging of issues that may arise in project, make it modular enough so it desires to be used quickly and efficiently
  - X create rework Excel sheet to prepare for any necessary rework documentation
  - X acquire items/tools helpful for assembly for personal use
    - X X-Acto for trace cutting
    - X Flux
    - X Magnet wire
    - X Kapton tape
    - X Isopropyl alcohol
    - X Tweezers
    - X De-soldering wick
    - X Tape for writing on and labeling
    - X Fine-tip Sharpie(s)
    - X 100MIL jumpers for header
  
## Action Items Dependent on Specific Parts Arriving

### optics_r001 PCB

Go through Excel sheet bring up procedure

### Breakaway Headers

Put together two PICKit3 programmer adapters to interface with the Clicker 2 five pin headers appropriately, see pinout Excel sheet for correct pinout. Although it possible to update PIC's firmware through USB, the PICkit3 is necessary for real-time debugging.

### Everything

Place components either on a 1:1 printout or actual PCB footprints (if optics_r001 has arrived yet) and examine against Eagle/3D model to prepare for any impending reworks and to check devices in Eagle for each component (silkscreen, tdocu, hole size, pad size, etc.)
