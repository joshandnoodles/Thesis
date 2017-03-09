# Weekly One-One - January 12, 2017

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

# Week 1

  - finished optics board r001 layout with minor changes to schematics
  - created 3D model of optics board along with servos and gimbal
  - corrected mechanical issues found through 3D modeling
  - picked new (better) optical lensing system with proper M9 PCB focusing mount

# Weekly One-One - January 12, 2017

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

# Week Four

Busy wee, got parts and board in. 

  - working through bring-up Excel for first optics_r001 board
  - all components mounted except LD and QP
  - power-up with no issues as of yet
  - currently characterizing power rails and simultaneously writing firmware for inital DVT

# Weekly One-One - February 2, 2017

Nominal.

Week Five

  - initial firmware writting for basic functionality
  - laser diode functional and operating within limits (capped at ~40mA due to 5V bias on diode rather than 12V, not a major issue in the short term)
  - beam focusing with collimating lense
  - beginnging to put together second board which will then be the first candidate board to receive a QP
  
# Weekly One-One - February 8, 2017

Nominal.

# Week Six
  
  - QP functioning well as far as can tell
  - implemented bulk quadrant photodiode reading functionality
    - allows fast ADC reading of all QP channels
    - additions could be made to just read one channel repeatedly rather than cycling through channels
    - this may be running at > 1Msps, I do not know how this will affect results
  - initial work on servos and getting ouput capture abstraction classes

# Weekly One-One - February 15, 2017

Very short meeting discussing arising issues such as power shortage and servo current spikes. These are not seen as bottlenecks yet. There may be potential fixes but first better gimbal functionailty needs to be had to realize what we are dealing with.

# Week Seven

  - soldered header hardware to OpenROV board for power switch and power leads
  - gained full gimbal functionality
  - discovered mechanical clearance issue with plastic tilt arm, not sure how to proceed with this issue
  - servo current spikes remain but mitigated with second USB power supply or powering via OpenROV (initial investigative waveforms captured mainly looking at main 5V power but need to see how 3.3V are affected, seeing weired behavior on ADCs during servo movement)

# Week Eight

  - attempted fixes for servo current spikes after anaylzing current draw and voltage drop over a variety of scenaries
  - final solution is a pair of 4700uF capacitors each with a 100nF filter on the servos (this is good but current spike and subsequent voltage drop still exisits)
  - captured effect drop has on sensing components (QP), from a 0.35 spike (no caps) to a 0.2 spike, need to consider other fixes

# Weekly One-One - March 2, 2017

  - discussed potential fixes for servo current spike
    - external battery for QP bias voltage
    - full blown seperate power supply IC exclusively for servos
  - talked about weird QP phase shift on odd numbered channels and further testing options
    - look at waveforms directly at QP channel output and after amplifier using oscilloscope to see where this behavior is occuring (if not seen in HW could be in FW)
  - theoretical discussion on serial communication techniques, specifically with independent clocks
   
   