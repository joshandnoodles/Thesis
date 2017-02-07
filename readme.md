# Unnamed MS Thesis Project

An optically centric underwater wireless communication system

*This project uses the OpenROV 2.8 as a testing medium to verify the system’s ability to create a communication link in saltwater conditions. No affiliation with OpenROV has been declared. Learn more about OpenROV's vision here (http://www.openrov.com/).*

## Structure

  - **/app** *-placeholder-*
    - Chrome App source code files for debug interface
    - *not necessary for normal operation, only if debugging is needed/desired*
  - **/doc**
    - all documentation relating to the development of the system
    - **/datasheets**
      - datasheets, user-guides, schematics, and layout files for vendor components
    - **/design**
      - preliminary research of high-level design (feasibility studies, calculation notes, block diagrams, preliminary component selection tasks)
    - **/dvt**
      - design verification test of system (rework diagrams, board tracking, next generation revision notes, validation test data)
    - **/lit**
      - published papers and other external literature relevant to this project
  - **/firmware** *-placeholder-*
    - MCU firmware source code files and libraries
  - **/schema**
    - board schematic files
    - board layout files
  
## Installation

This system acts as a standalone communication system to replace the Ethernet link between two endpoints. Therefore, no software installation is needed for normal operation. Physical connections to the system will need to made to begin wireless transmission. To bring the link up, simply plug the Ethernet interface of the system into the two Ethernet ports of the endpoints to be connected (in the case of the OpenROV, the Ethernet port of the on-board BeagleBone and the Ethernet port of the PC running the Cockpit software). Then, provide power to the system using a USB on the PC side and the 3-pin adapter connector on the OpenROV side. At this point, the system will initialize and bring up the communication link independently of any user interaction.

A second installation step may be necessary if the Chrome App debug interface is needed. The source code files are located in the /app directory. Google Chrome must be installed on the PC endpoint wishing to operate the debug interface. With Chrome open, follow the steps to access the Chrome App debug interface:
  - Open Chrome and go to **Menu > (More) Tools > Extensions**
  - Click **Developer mode > Load unpacked extensions...**
  - Browse to and **Select** the **/app** directory
  - Click **Launch**
