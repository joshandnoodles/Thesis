# Unnamed MS Thesis Project

An optically centric underwater wireless communication system

*This project uses the OpenROV 2.8 as a testing medium in saltwater conditions. However, no affiliation with OpenROV has been declared. Learn more about OpenROV's vision here (http://www.openrov.com/).*

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
      - design verification test of system
    - **/lit**
      - published papers and other external literature relevant to this project
  - **/firmware** *-placeholder-*
    - MCU firmware source code files and libraries
  - **/schema**
    - board schematic files
    - board layout files
  
## Installation

This system acts as a standalone communication system to replace the Ethernet link between two endpoints. Therefore, no software installation is needed for normal operation. Physical connections to the system will need to made to begin wireless transmission. To bring the link up, simply plug the Ethernet interface of the system into two Ethernet ports of endpoints to be connected (in the case of the OpenROV, the Ethernet port of the on-board BeagleBone and the Ethernet port of the PC running the Cockpit software). Then, provide power to the system using *-placeholder-*.

A second installation step may be necessary if the Chrome App debug interface is needed. The source code files are located in the /app directory. Chrome must be installed to operate the debug interface. With Chrome open, follow the following steps to access the Chrome App debug interface:

  - Open Chrome and go to **Menu > (More) Tools > Extensions**
  - Click **Developer mode > Load unpacked extensions...**
  - Browse to and **Select** the folder where your manifest.json file is and logo (wherever your local **/app** folder resides)

ref *https://support.google.com/chrome/a/answer/2714278?hl=en*
