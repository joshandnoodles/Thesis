# Weekly One-One - January 12, 2016

Updates for work done over Christmas Break

Major items:
  - literature search on QP and control aspects
  - rework optical calculations correcting errors and implementing Chelsea's inital beam diamters following optical lensing
  - created optics board r001
    - initial high-level and logic block designs done in Visio
    - SPICE simulation for RX QP amplication and characterization and load switch operability
    - schematics completed in Eagle (expect addition of test points)
    - PCB layout started
    
## Week 1

  - finished optics board r001 layout with minor changes to schematics
  - created 3D model of optics board along with servos and gimbal
  - corrected mechanical issues found through 3D modeling
  - picked new (better) optical lensing system with proper M9 PCB focusing mount

# Week Two

# Order Parts

## Action items to prepare for parts arriving

  - create power connector jack for:
    - OpenROV to USB Mini
  - aquire two USB Type-A to USB Mini (with data connections)
  - create bring-up sequence for optics board
    - checking impedences between power rails and ground w/o ICs loaded
    - mounting basic ICs needed for connectivity (exclued PD and LSR diode)
    - measure and characterize power rails (specifically P5V)
    - test load switch functionality with PIC I/O (ensure P5V_LSR off at start)
    - check controllability of current limit voltage reference with potentiometer
    - etc...
  - look at firmware code examples, give priority to development to the following:
    - basic I/O toggling
    - clock speed settings
    - wait functions
    - operation of debugging LEDs