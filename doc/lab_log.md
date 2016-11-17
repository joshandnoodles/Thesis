
[TOC]


# Scratchpad

http://learn.mikroe.com/ebooks/picmicrocontrollersprogramminginassembly/chapter/ccp-modules/

https://www.mbeckler.org/coursework/2005-2006/ee2361lab10.pdf

http://www.jensign.com/opto/ledlaserdrivers/

PD TOO
https://www.aptechnologies.co.uk/index.php/support/photodiodes/photodiode-theory-of-operation
https://www.aptechnologies.co.uk/index.php/support/photodiodes/photodiode-operating-circuits


# Take-away from Senior Design (UC the Fish - Fall 2015 and Spring 2016)

## Ethernet Compatibility

Great idea to allow for easy implementation in already existing devices. However, the drawbacks are the following:
- created for a wired system, not wireless (and the protocol is tailored to this fact)
- inflexibility for testing and debugging (more or less binary, it works or it doesn't)

## Signal Integrity of High Frequency Signals 

Signal integrity of square wave signal falls apart after 100kHz. 

![][s1_s2_10k]
![][s1_s2_76k]
![][s1_s2_133k]
![][s1_s2_350k]
![][s1_s2_670k]

This could be corrected by:

  - reducing speed to 100kHz or less
    - slowest standard is 10BASE-T and runs at 10Mbps
    - probably not fast enough for what we need
  - using carrier signal to make space more passable - **OFDM**
    - need a mediator between Ethernet signal and transmitted signal (no longer directly transmitting physical Ethernet layer)
    - sophisticated communication systems like this have much higher barriers to entry:
      - expensive equipment to measure and analyze signal characteristics
      - high-functioning components to be able to accurately create these signals
      - large teams of experts in many fields needed to build system 
      - a large industry competes at the highest caliber to perfect these systems
  - reduce EM noise by using **opto-isolator**
  
[s1_s2_10k]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/s1_s2_10k.bmp
	"Figure 1: Transient response after Stage 1 & 2 with input 20mA 10kHz signal from point-blank distance"
[s1_s2_76k]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/s1_s2_76k.bmp
	"Figure 2: Transient response after Stage 1 & 2 with input 20mA 76kHz signal from point-blank distance"
[s1_s2_133k]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/s1_s2_133k.bmp
	"Figure 3: Transient response after Stage 1 & 2 with input 20mA 133kHz signal from point-blank distance"
[s1_s2_350k]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/s1_s2_350k.bmp
	"Figure 4: Transient response after Stage 1 & 2 with input 20mA 350kHz signal from point-blank distance"
[s1_s2_670k]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/s1_s2_670k.bmp
	"Figure 5: Transient response after Stage 1 & 2 with input 20mA 670kHz signal from point-blank distance"


## Additional Considerations

### Should not limit to a specific color of light
  
  - handicapping ourselves by doing this
  - more range of components to utilize if not limited by certain LED color (brighter, faster LEDs)
  - can always add optical filters for wavelength ranges after the fact
  - limits potential uses (i.e. Li Fi home lighting systems)
  
### Need faster LED driver

  - start by focusing on getting a bright light source switching at max freq. before proceeding
  - maybe this is connected to a LED IC driver being controlled by a microcontroller pin
  - if signal is not good from transmitter, there is no hope it will be good at receiver

### Simplify receiver amplifier stages

  - research current Li-Fi circuit configurations and design approaches
  - leverage existing photodiode/amplifier configurations from TI, Analog Devices, etc.
  - **buffers** (may be able to be considered for only certain rates)

# Weekly One-One - September 8, 2016

## Get away from Ethernet-centricity

Ethernet is great. We should continue to utilize it as the main endpoint for communications for both the OpenROV and landside for two reasons.
  - OpenROV side has multiple communication endpoints we can utilize such as Beagle Bone and Ethernet. However, the landside endpoint goes from the Tenda, into Ethernet, and directly to the computer. I.e. The only viable node to intercept communication stream is the Ethernet.
  - It is a modular interface. Compliance with Ethernet as the input/output stream means this could easily be implemented in other systems.

However, we should not using the Ethernet physical later to modulate to the LED directly. If we do, we are constrained to meet a minimum bandwidth requirement defined by the Ethernet standard (10BASE-T at 10Mbps). Additionally, using the Ethernet physical layer for direct modulation limits us to simple OOK (on-off keying). Reading in the Ethernet stream and processing it before transmission allows us to minimize the bandwidth needed for the circuitry and use any modulation scheme (dependent on transmission medium and circumstances).

## Fell short on optical side
  
Soccer-ball array of LEDs/PDs was a good try. However, it had some fairly major drawbacks. Scalability for both power and cost of components would be an issue. Additionally, it would be difficult to package in a relatively small package. However, there may be better options:
  - Directed collimated light (laser)
  - Gimbal system to direct beam
  - Beam expander optics to increase beam size for easier targeting

## Action Items
  
  - Determine bandwidth needed for OpenROV activities
    - If there is a bandwidth range based on resolution/refresh rate, etc., find the range.
  - Research Ethernet switches for "middle-man" communications
    - This could be a standalone system (i.e. Raspberry PI/Beagle Bone or a Ethernet port and microcontroller).
    - The smaller the better.

# Week Four

## Bandwidth Measurements on Nominal Connection

Used the tool, NetWorx, for experimental measurements. Networx allows real-time bandwidth monitoring of a single or multiple network interfaces. This allows collection of both upstream and downstream traffic on the interface the OpenROV is connected to (in this case eth0).

Data collected began with OpenROV off. OpenROV was power-up up and data collection continued for about 10 minutes. First, data was collected to examine bandwidth usage prior to video streaming coming up. The results can be seen in Figure !!. There was minimal activity seen other than a few upstream bursts not exceeding 60kbps.

![][networx_bw_pre_stream_res_nominal_fps_nominal]

Once the video streaming was up, the FPS averaged right above 20 fps although extremes were seen at both 10 fps and 40 fps. As can be seen in the figure !!, after the OpenROV powered up fully and video streaming begain, download rates held at ~21Mbps for about 5 minutes until jumping to **~25Mbps**. Uploads held farily consistently throughout at ~500kbps

![][networx_bw_post_stream_res_nominal_fps_nominal]

Investigating whether the video resolution or frame could be downgraded to allow for lower download rates, I found there is modifications that can be done on the node.js side of platform (within the openrov-cockpit). This modification will affect the frame rate. It is made in the following file: */opt/openrov/cockpit/src/lib/config.js*. 

These modifications can be done using Putty to connect to the OpenROV using the following settings:

  - IP Address: 192.168.254.1
  - Port: 22
  - Username : rov
  - Password : OpenROV


*Original*

    'sample_freq': 30,
    'dead_zone':  10,
    'video_frame_rate': 30,
    'video_resolution': 'SXGA',

*Modified*

    'sample_freq': 3,
    'dead_zone':  10,
    'video_frame_rate': 3,
    'video_resolution': 'SXGA',

  However, I see major lags (>1 sec) in addition to this slower frame rate. This may be correctable but will need to investigated. Reducing the FPS by a factor of 10 (from 30 fps to 3 fps), I saw a decrease in bandwith by approximately a factor of 10 as expected to **~2.5Mbps**, a large improvement.

![][networx_bw_post_stream_res_nominal_fps_3]


[networx_bw_pre_stream_res_nominal_fps_nominal]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/networx_bw_pre_stream_res_nominal_fps_nominal.png
  "Figure !!: Nominal network activity prior to video streaming"
[networx_bw_post_stream_res_nominal_fps_nominal]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/networx_bw_post_stream_res_nominal_fps_nominal.png
  "Figure !!: Nominal network activity with video streaming"
[networx_bw_post_stream_res_nominal_fps_3]: https://github.com/joshandnoodles/MS/raw/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/networx_bw_post_stream_res_nominal_fps_3.png
  "Figure !!: Network activity with video streaming and config modified for 3 fps"


## Methods for Pre-processing Network Data

The desire here is to have a platform that is not reliant on directly using the physical layer of Ethernet to be the driver for the tranmitted signal. This is necessary for:
  - control over the modulation rates of the tranmitted signal (do not want to have to meet the minimum of 10Mbps that 10BASE-T runs at)
  - more complex modulation schemes (frequency modulation, PPM, etc.)

If there is a "middle-man", processing the Ethernet signal, it is possible to operate at a much lower bandwidth that is only determine by the actual amount of data being sent. This will mean that there will need to a sort of translator both from the OpenROV ethernet to transmitter as well as a translater from the receiver to the OpenROV ethernet. Discussion over possible devices to provide this functionality will be discussed below.

### PIC MCU

When considering using a PIC, we would still need MAC & PHY to interface with RJ-45 Ethernet.

![](http://www.microchip.com/images/default-source/design-centers/ethernet/figure1.jpg?sfvrsn=2&MaxWidth=310&MaxHeight=&ScaleUp=false&Quality=High&Method=ResizeFitToAreaArguments&Signature=45F2761CDD8A81BECCD91A8649B63D464F31E828)

![](http://www.microchip.com/images/default-source/design-centers/ethernet/figure2.jpg?sfvrsn=2&MaxWidth=310&MaxHeight=&ScaleUp=false&Quality=High&Method=ResizeFitToAreaArguments&Signature=26B01888CE530FAB37FBD0E1DC8BBAAB749EA175)

Options:
  - ~~PIC18F97J60 - MAC & PHY)~~
    - only 8-bit, we don't gain much by having an underpowered controller (at least for the first run of proof-of-concept) 
  - PIC24 - no MAC, no PHY, 
    - already have setup with USB debugging interface
    - still need to handle TCP/IP stack on MCU to control input/output
    - would need Ethernet controller for MAC & PHY such as ENC28J60
![](http://www.mikroe.com/img/development-tools/accessory-boards/click/eth/gallery/eth_click_02.jpg)
![](http://www.mikroe.com/img/development-tools/accessory-boards/communication/serial-ethernet/gallery/serial_ethernet_03.jpg)
    
  - PIC32MZ (faster) and PIC32MX6/7 - MAC, no PHY
    - have starter boards such as PIC32 Ethernet Starter Kit w/ PHY already implemented
    ![](http://www.microchip.com/_ImagedCopy/DM320004.jpg)
    - still need to handle TCP/IP stack on MCU to control input/output

# Weekly One-One - September 15, 2016

First, we talked about OpenROV bandwidth usage data (surprising amount of data being sent by video stream, ~25Mbps). Also discussed controller needs and the flexibility in choosing what route to go (MCU, Raspberry Pi, Arduino, etc.)

## Action Items

  - Further reduce BW usage of OpenROV's video stream by reducing resolution
    - need to first determine current resolution stream is (trying) to run at
  - Begin very high-level block diagrams of design
    - keep controller device modular, no need to decide on exact controller type yet
    - this will help decide what peripherals are needed and therefore what the controller needs to support
    - need to consider case for single or dual system where controller drives gimbal and does Ethernet rx/tx jointly and independently, respectively 

# Week Five

## Bandwidth Reduction

*will be cont'd next week when I have computer available for Ethernet stream measurements (lappy died)*

## High-level Design (generic controller)

![][hld_generic_controller_v1]

Notes:

  - debug interface used for development
	  - first thing we'll want to do is read received packets sent from dummy Ethernet host (this will be hard to do with real-time  debugging interface in place)
	  - same interfacing on both OpenROV- and land-side
	  - likely connected to PC: USB, serial, Bluetooth, etc.
	  - maybe even possible to implement this in OpenROV Cockpit interface in the future
  - removing Tenda Homeplug Adapters and Topside Interface Boards has never been done before, this should be done soon
  - position control loop for real-time optical alignment is needed
	  - What does the look like?
	  - What are the inputs/outputs?
	  - Can we do this without any data from other end?
		  - If not, we need to count on being able to correct alignment before comm signal is lost. Always.
  - power source for Data Plane Controller, Mechanical Plane Controller, and Gimbal still indefinite
	  - hopefully this can be powered with 5V, max=5V (available on OpenROV)

[hld_generic_controller_v1]: 
https://rawgit.com/joshandnoodles/MS/747357e0e8dca8728b961f16eb45695172f6b3a1/doc/img/hld_generic_controller_v1.svg
  "Figure !!: High-level Design of communication system with generic, independent controller(s)"
  
# Weekly One-One - September 22, 2016

## Action Items

  - Test OpenROV without Topside Interface board and both Tenda Homeplug
	  - should be nominal, but important to test
	  - could raise issues about using using our system in place of Tenda Homeplug Adapters
  - *Cont'd* - further reduce BW usage of OpenROV's video stream by reducing resolution
   - Blackbox design control loop system to better outline the interaction between all mechanisms as well as 

# Weekly One-One - September 29, 2016

## Action Items

  - *Cont'd* - Test OpenROV without Topside Interface board and both Tenda Homeplug Adapters
	  - should be nominal, but important to test
	  - could raise issues about using using our system in place of Tenda Homeplug Adapters
  - *Cont'd* - further reduce BW usage of OpenROV's video stream by reducing resolution
  - *Cont'd* - Blackbox design control loop system to better outline the interaction between all mechanisms as well as resources/components needed for design

# Week Six and Seven

## Tenda-less OpenROV Testing

A nominal test was done on the OpenROV was done to confirm preconceived notion of communication system. It has been generally assumed that the Tenda Homeplug Adapters act as an entirely modular piece within the communication path of the system (i.e the adapters take in communication packets from the Ethernet interface, transmit them over their properietery hardware, and reconstruct the packets entirely to hand back 

To complete this test, the following items were removed:
  - Tenda Homeplug Adapter from OpenROV
  - Tenda Homeplug Adapter from Topside Interface Board
  - Tenda Interface Board
  - Twisted-pair tether

Once all components had been removed, a small CAT5e cable was connected directly from the Ethernet port of my computer to the BeagleBone that is housed within the OpenROV. The OpenROV was kept partially deconstructed throughout this test as there is no trivial way to run this Ethernet line out of the OpenROV.

Upon testing, the OpenROV did not power up and/or connect to the OpenROV Cockpit interface as expected. I initially expected that the Tenda provided some sort of enable signal to the OpenROV once the land-side end was connected to power. This turned out not to be the case after analyzing the circuitry. Investigating further, I found that the only suspect left was the tether. Measuring the tether pair directly with a differently ended probe on the oscilloscope, I found it provided a signal centered at a DC value of 40V with small variations caused by the modulation. As it turns out, the OpenROV uses this signal from the tether in its power up sequence for the BeagleBone. This can be seen in the OpenROV 2.8 schematic.

![][schema_J12]

[schema_J12]: 
https://github.com/joshandnoodles/MS/raw/495c809e913dbe1651ba9bfd9f822c1a214aa586/doc/img/schema_J12.png
  "Figure !!: OpenROV schematic showing manual power switch override to not wait for tether signal"


Once this jumper, J12, was shorted, the OpenROV proceeded to power up and operate nominally without the tether or Tendas.

## Bandwidth Reduction

The initial bandwidth reduction investigation yielded full control over the frame-rate of video being sent. However, even if we drop agree on a modest frame-rate of 10fps, bandwidth usage drops only by a factor of 3 (25Mbps to 9Mbps). Control of the resolution and potentially color encoding is needed to drop the bandwidth to a resaonable rate. Throughout this testing, the NetWorx tool was used for bandwidth measurements. 

Further investigation found that the resolution is able to be changed within the Cockpit architecture. The modifications were done on the file located at */opt/openrov/cockpit/src/lib/config.js*. The changes are shown below.

*Note: SXGA is equivalent to 1280x1024*

*Original*

    'sample_freq': 30,
    'dead_zone':  10,
    'video_frame_rate': 30,
    'video_resolution': 'SXGA',

*Modified*

    'sample_freq': 30,
    'dead_zone':  10,
    'video_frame_rate': 30,
    'video_resolution': '640x480',


The data collected using the NetWorx tool shows an average bandwidth usage of **~1.4Mbps** after switching to a resolution of 640x480. Originally, the OpenROV operates using a nominal resolution of 1280x1024 (SXGA) and requires a bandwidth of ~25Mbps. A simple calculation shows that the bandwidth usage scales as a function of resolution unexpectedly. More investigation will be done on this at a later time.

(1.4Mbps) / (25Mbps) = (reduced BW) / (nominal BW) = (pixels @ reduced resolution) / (pixels @ nominal resolution) = (640x480) / (1280x1024) = (307200 pixels) / (1310720 pixels) = 15 / 64 = 0.234375

![][networx_bw_post_stream_res_640x480_fps_nominal]

[networx_bw_post_stream_res_640x480_fps_nominal]: 
https://github.com/joshandnoodles/MS/raw/7b967a3bff429ad4abd5ebafc3a9e7fb5c885d17/doc/img/networx_bw_post_stream_res_640x480_fps_nominal.png
  "Figure !!: Network activity of streaming video at a resolution of 640x480"


# Weekly One-One w/ Chelsea - October 11, 2016

## Action Items

  - create pro-cons list for different feedback control systems (i.e. photodiode array versus camera sensor)
  - *Cont'd* - Blackbox design control loop system to better outline the interaction between all mechanisms as well as resources/components needed for design

# Week Eight and Nine

## Exploration of different control feedback systems

### Camera Sensor and Computer Vision

#### Pros/cons

Pros

  - independent of other transceiver (feedback is received in a closed look on the same transceiver, not reliant on communication link being up)
  - robust in its ability to track other transceiver's orientation (having transmitter and receiver circuitry is no longer an issue if we can determine relative orientation between boards)

Cons

  - entirely different medium of sensing than communication link, different wavelengths, more issues for different ranges of the EM spectrum
  - relatively slow (probably)
  - will most likely need additional controller for processing image frames and running computer vision algorithms

#### Block diagram of control loop

![][hld_control_loop_camera_v1]

[hld_control_loop_camera_v1]: 
https://rawgit.com/joshandnoodles/MS/5c2f29b4d1f9f393043a2c0f38d69b342fda525c/doc/img/hld_control_loop_camera.svg
  "Figure !!: High-level design of control loop using camera/computer vision feedback system"

### Photodiode Array

Pros

  - elegant, simple concept decreases chance for complications
  - will be able to operate quickly
  - should be able to be supported by data plane controller, won't need additional controller (second controller=more complexity)
  - majority of system will have to be already present for transceiver communication, using exisiting architecture

Cons

  - relys on inital calibration (data to align is held by opposite system, needs active communication link for feedback)
  - potential issues with orientation relative to other gimbal (if the receiver and transceiver are offset on board)

#### Block diagram of control loop

![][hld_control_loop_pd_v1]

[hld_control_loop_pd_v1]: 
https://rawgit.com/joshandnoodles/MS/5c2f29b4d1f9f393043a2c0f38d69b342fda525c/doc/img/hld_control_loop_pd.svg
  "Figure !!: High-level design of control loop using photodiode array feedback system"

## *Updated* High-level Design (generic controller)

![][hld_generic_controller_v2]

[hld_generic_controller_v2]: 
https://rawgit.com/joshandnoodles/MS/cd88693408b3beadbc45515974884ea5a7c1ab7e/doc/img/hld_generic_controller_v2.svg
  "Figure !!: High-level design of communication system with generic, independent controller(s)"


# Weekly One-One - October 20, 2016

Talked about geometry of board and placement concerns of receiver and transmitter in a directional system. Still an open-ended item and will need to be revisited in the future.

## Action Items

  - research potentially easy camera solutions that could be added to the system with minimal effort (even if their control implementation will be left as a second priority)
    - is it possible to have the same controller support a camera (if it runs at a slow rate, even ~1fps)
  - begin making choosing devices and making diagrams showing actual connectivity between different elements in the system
    - this may mean doing this multiple times with different devices to show comparisons of different options

# Week Ten & Eleven


## Camera Solutions Investigation

Time was spent researching different options for an on board camera system to use computer vision techniques for assistance in gimbal alignment. The results of this investigation are promising in certain cases and hopeless in others. Regardless, the want of a camera solution is able to be easily implemented and able to be supported by the Data Plane Controller seems unlikely. It looks like it might be possible to implement one of these systems, but it will not be trivial.

UPDATE: Use of Microchip's 32-bit MCU products makes this feat much more reasonable.

http://www.microchip.com/forums/m357664.aspx

### CMOS/CCD Camera Sensor

  - **small**, even board mounted CMOS cameras with supporting circuitry are **~3cm x 3cm**
  - **inexpensive**, 728x488 CMOS camera module on Sparkfun for only **$32** (*https://www.sparkfun.com/products/11745*
  - storage of the raw data on a microcontroller becomes an issue (external memory modules interfacing with the PIC are an option as well as SD memory cards)
    - this will obviously require more complexity, board space, and cost
  - it is difficult to tell the frame rate that can be achieved, other microchip users have reported a few seconds for a single image acquisition (*http://www.microchip.com/forums/m402377.aspx*)
    - also no one on the forms is discussing image processing (only image acquisition to send to some other device/medium)

To make the amount of image data reasonable, some have recommended to install a **JPEG encoder chip** alongside the PIC24 which will make the process faster (but probably not twice as fast) and the amount of image data is still significant enough to require some form of external memory module. 

![](https://cdn.sparkfun.com//assets/parts/7/9/6/7/11745-02b.jpg)

However, there is much more ongoing development with CMOS image sensors on **Microchip's 32-bit line of MCU's** (more multimedia-centric). Still not a whole lot of fps data and such, but there is many more code examples and connectivity discussion for their 32-bit architecture. Microchip offers an color VGA CMOS expansion card that is pluggable to their 32-bit Starter Kits as well (more information about this below).

**Conclusions...**

Attempting to interface a CMOS image sensor with a 16-bit PIC alone seems like convoluted task with little to no support on the hardware or firmware items needed to get the sensor up. And once it is up, achieving reasonable rates and storing the frame data will be a significant issue (and this is assuming the PIC is devoted to camera processing, which it will not be).

If a CMOS sensor is to be used, Microchip's 32-bit line of PICs should be used. The color VGA CMOS expansion card Microchip offers is an attractive options since it also has firmware code examples that will get the sensor up and fully working immediately. However, if a standalone CMOS sensor is to be used, OmniVision's offerings seem best suited and relatively well documented. Specifically, OmniVision's OV9653 chip with a resolution of 1280x1024 pixels looks to be the best candidate if a standalone sensor is to be pursued (it is the replacement for the OV9650 which has been heavily used in the microcontroller community).

*http://www.techtoys.com.hk/Components/OmniVision%20Cameras/OV%20CMOS%20Camera%20Modules.htm*

![](http://www.techtoys.com.hk/Components/OmniVision%20Cameras/OmniVision_hires.PNG)

### AVRcam

*http://www.jrobot.net/Projects/AVRcam.html*

  - startard UART interface return **real-time tracking statistics**, not raw image data, not compressed image data, but instead **number of objects, color, bounding box, and more**
  - using Omnivision OV6620 CMOS image sensor along with a whole bunch of internal processing components
  - in-circuit reprogrammability and **open-source** (we can add new capabilities and modify existing ones)
  - 88x144 resolution at 30fps
  - large standalone board with dimensions **2.4" x 1.9"**
  - 

![](http://www.jrobot.net/pictures/AVRcam_v1_1_front1_small.jpg)
![](http://www.jrobot.net/pictures/AVRcam_v1_1_back1_small.jpg)


## Microcontroller Selection

### Necessary features

  - ADCs (>=4)
    - read power rails, quantify intensity of light incident on photodiode(s)
    - general purpose, 10-/12-/16-bit resolution
    - general purpose, 16-bit resolution
  -  PWM (>=1)
    - necessary in order to develop a carrier signal and modulate at a lower frequency
    - 16-bit with up to nine independent time bases
  - Input Capture (IC)/ Output Capture (OC)
    -  ability to capture an external event with an independent timer and ability to trigger an output (single output pulse or train of output pulses) on a compare match event (comparing value to compare registers)
    - may be very helpful when dealing with Ethernet packet data and directly translating data directly to next tx/rx step in process, also fairly standard feature (built into the timers)
  - USB
    - although this will not be used in production operation, this is critical for debugging issue from the entire system during active operation
    - architecture for PC based debug console for this system is already 85% there and already have an understanding of the Microchip USB stack
  - I2C/UART/SPI
    - basic communication protocols needed to interface with any outside components (camera, sensors, etc.)
  - Program memory >=128kB
    - it is likely more than this will be needed, 256kB, but minimum 128kB

### Optional Features
  
  - DACs (>=1)
    - laser driving, having on board is a must since this will be key in creating a signal with quick modulation patterns
    - reconsidering, the benefit of a DAC and being able to set a very specific analog DC voltage is near entirely eliminated if a simple MOSFET switching circuit is used to drive laser from generic I/O pin (which is the more likely route at this point)
  - High-speed ADC
    - would be very nice to potentially use for photodiode tracking
    - high-speed SAR ADC with 12-bit resolution and sampling speed of 10Msps 
  - High-speed Comparator
    - again, would be very nice have to condition received signal
    - general purpose, rail-to-rail comparator with <1ns response time

### Non-needed Features (Externalize)

*This section just convinces myself that there are certain features that I can either replicate at the same level as the PIC or better using external components*

  - Opamps
    - if we need opamps for laser driving or signal conditioning (which we will), I would rather choose the amplifier that best suits the purpose and have access to pins for troubleshooting/characterizing if necessary
 
### Packaging 

It is important to consider package types when choosing devices. Certain package types will be more conducive to prototyping as well as hand soldering on copper clad PCBs using the in-lab PCB maker. Even though certain packages with pads on the bottom of the IC may be able to be used in prototyping with some creativity (flipping component and wire bonding from each pad), they will not be able to be soldered to any PCB in-house. This is undesirable (both time and money loss).

![](https://hackadaycom.files.wordpress.com/2016/10/ne555_h.jpg?w=800)

Any variant of **BGA** (Ball Grid Array packages) (i.e. TFBGA) are **ruled out**. Even the thought of wire bonding to densely packed pads on the bottom of such devices reminds me of some of my worst nightmares.

**QFN** (Quad Fat No-leads) is still **undesirable but still a possibility**. They do not have legs that extend beyond the IC and are meant to be flowed from underneath. However, they are prototypable and have pads that run slightly up the edges which make them possible to hand solder on PCBs with the right solder pad construction. 

Other variants of packaging offered by Microchip such as **QFP** (Quad Flat Package) are **very desirable** as they have legs that extend away from the 

### PIC24 Families

Although features are important, not all PICs are equal. From the lowest performance to highest (excluding the dsPICs), we have the following for both the Microchip's 16-bit architecture product line and 32-bit architecture:

  - 16-bit
    - PIC24F - *Lowest Power*
      - **16 MIPS**
      - 3.3V/5.0V
      - Flash 4K to 1024K
      - RAM 512 to 96K
    - PIC24H - *Higher Performance*
      - **40 MIPS**
      - 3.3V
      - Flash 12K to 256K
      - RAM 1K to 16K
    - PIC24E - *Highest Performance MCU*
      - **70 MIPS**
      - 3.3V
      - Flash 32K to 512K
      - RAM 8K to 52K
  - 32-bit
    - PIC32MZ
      - Up to **252 MHz**, 415 DMIPS
      - Up to 512 KB High Speed SRAM
      - FPU for fast single- and double- precision math
      - **12-bit, 18 MSPS ADC module with up to 48 channels and 6 S&H circuits.**
    - PIC32MX
      - Up to **120 MHz**, 1.65 DMIPS/MHz or 3.28 Coremarks™ MHz
      - Up to 512 KB Flash
      - Up to 128 KB High-speed SRAM
      - 100 MHz Internal Bus and Cache
      - **backwards compatibility with Peripheral, Pin and source of  PIC24F 16-bit microcontroller**
    - PIC32MM
      - Small Packages (4x4mm, 5x5mm 6x6mm)
      - Core Independent Peripheral Integration:
          - ADC, Comparators, RTCC, WDT
          - Configurable Logic Cells (CLC)
          - Flexible PWMs / IC / OC / Timers - MCCP & SCCP
      - Analog Integration:
          - 12-bit 200ksps ADC
          - 5-bit DAC
          - Comparators

*https://www.microchip.com/design-centers/32-bit*

### What's Left

From the 16-bit PICs

  - dsPIC33EP256MU806
  - dsPIC33EP[256/512]MU810
  - dsPIC33EP[256/512]MU814
  - PIC24EP[256/512]GU810
  - PIC24EP[256/512]GU814

From the 32-bit PICs

  - Non-Ethernet PIC32MX's
    - PIC32MX230F[256B/256D/256H/256L]
    - PIC32MX270F[256B/256D/512H/512L]
    - PIC32MX440F[256H/512H]
    - PIC32MX450F[256H/256L]
    - PIC32MX460F[256L/512L]  -  *Clicker 2 for PIC32MX [512L]*
    - PIC32MX470F[512H/512L]
    - PIC32MX550F[256H/256L]
    - PIC32MX570F[512H/512L]
    - PIC32MX575F[256H/256L/512H/512L]
  - Ethernet PIC32MX's
    - PIC32MX675F[256H/256L/512H/512L]
    - PIC32MX695F[512H/512L]
    - PIC32MX775F[256H/256L/512H/512L]
    - PIC32MX795F[512H/512L] - *PIC32 Ethernet Starter Kit II (DM320004-2) [512L]* **&** *Mikromedia Plus for PIC32MX7 [512L]*
  - Ethernet PIC32MZ's
    - PIC32MZ[0512/1024]EFE[064/100/124/144]
    - PIC32MZ[0512/1024]EFF[064/100/124/144]
    - PIC32MZ[0512/1024]EFK[064/100/124/144]
    - PIC32MZ[1024/2048]ECG[064/100/124/144]
    - PIC32MZ[1024/2048]ECH[064/100/124/144] - *PIC32MZ EC STARTER KIT (DM320006) [2048] [144]*
    - PIC32MZ[1024/2048]ECM[064/100/124/144]
    - PIC32MZ[1024/2048]EFG[064/100/124/144]
    - PIC32MZ[1024/2048]EFH[064/100/124/144] - *PIC32MZ EF STARTER KIT (DM320007) [2048] [144]*
    - PIC32MZ[1024/2048]EFM[064/100/124/144]

*~~In addition to the selection criteria applied to the 16-bit PIC, an additional restriction was imposed on the 32-bit PICs: 10/100 Base-TX Mac. This was done since on-board MAC would be the main motivation behind choosing a 32-bit architecture (along with faster processing speed).~~ This was later nixed as many of the options would use a compact SPI Ethernet dongle as opposed to a direct PHY port. Later on, the main motivation for a 32-bit architecture was no longer Ethernet related but rather processing speed for camera capabilities.*

**PIC32 Ethernet Starter Kit II (DM320004-2)**

  - Includes **10/100 Fast Ethernet LAN8740 PHY Daughter Board**
  - Integrated programmer/debugger
  - PIC32 running at 80 MHz with 512K Flash, 128K RAM, 8 channel DMA + 8 channel DMA dedicated to Ethernet, CAN and USB
  - CAN 2 0b peripheral and **USB host/device/OTG**
**Expansion connector** enables the addition of Microchip’s PIC32 expansion boards or the creation of your own board

![](http://www.microchip.com/_ImagedCopy/EthSKII6475.JPG)

*http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=DM320004-2*

**Color VGA Camera Sensor (VCS) PICtail Plus (AC164150)**

  - **640x480 VGA Color CMOS Sensor** on our standard PICTail™ plus form factor
  - Based on the Omnivision OVM7690 CameraCubeChipTM
  - Convenient test points for video sync and clock sync
  - **Interfaces** to the I/O expansion board (DM320002) with **PIC32 starter kits** (DM320004 or DM320003-2)

![](http://www.microchip.com/_ImagedCopy/AC164150.png)

*http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ac164150*

**PIC32MZ EC Starter Kit (DM320006)**

*basically the PIC32 Ethernet Starter Kit II but with a PIC32MZ instead of a PIC32MX*

![](http://www.microchip.com/_ImagedCopy/DM320006.png)

*http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=Dm320006*

**PIC32MZ Starter Kit Adaptor Board (AC320006)**

168-pin to 132-pin connector to enables the use of PIC32MZ starter kits that have a 168-pin interface with the following PIC32 hardware:

  - Multimedia Expansion Board (MEB) – Part # DM320005
  - PIC32 I/O Expansion Board – Part # DM320002
  - Low-Cost Controller-less (LCC) Graphics PICtail Plus Daughter Board – Part # AC164144
  - Graphics LCD Controller PICtail Plus SSD1926 Board – Part # AC164127-5
  - Graphics PICtail Plus Epson S1D13517 Board – Part # AC164127-7

![](http://www.microchip.com/_ImagedCopy/AC320006.png)

*http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=AC320006*

**Starter Kit I/O Expansion Board (DM320002)**

Needed to interface with the PICtail Plus camera sensor. Also gives access to MCU signals and debug headers.

Compatible w/ ..., PIC32 Ethernet Starter Kit (DM320004), PIC32 Ethernet Starter Kit II (DM320004-2), ...

A PIC32MZ Adapter Board (AC320006) is needed to connect the following PIC32MZ Starter Kits: : ..., PIC32MZ EF Starter Kit (DM320007), ..., PIC32MZ EC Starter Kit (DM320006), ...

![](http://www.microchip.com/_ImagedCopy/DM320002.png)

*http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=dm320002&utm_source=MicroSolutions&utm_medium=&utm_term=&utm_content=&utm_campaign=DM320002*

## OpenROV Power Distribution Research

In an attempt to begin selecting components and shsowing pin connectivity, the concern of what power supply rails the OpenROV has on board (and which rails may still have leftover amperage available) became a growing issue. Because of this, the OpenROV schematics were explored, and the power distribution diagrammed.

*The power distribution system also contains many current sense points and voltage measurement points. It may be helpful to label these at a later time as they might be helpful when diagnosing power issues on the sub.*

![][hld_openrov_power]

[hld_openrov_power]: 
https://rawgit.com/joshandnoodles/MS/cd88693408b3beadbc45515974884ea5a7c1ab7e/doc/img/hld_openrov_power.svg
  "Figure !!: High-level design of power distribution system on board the OpenROV"


# Weekly One-One - November 2, 2016

## Action items

  - explore MikroElektronika's offerings compared to Microchip's
  - look at Arduino Chips (with cameras)
  - consider Raspberry Pi/BeagleBone
  - investigate PD arrays

# Week Twelve

## MikroElektronika's Offerings

Doing an initial evaluation of MikroElektronika's products based on our systems needs...

The **camera will again be a large constraint** in design choices. MikroElektronika recommeds a 32-bit MCU to be able to achieve QCIF resolution video (**176 x 144 px**). This is explained here: http://www.mikroe.com/click/camera/. This alone is enough to limit our search to 32-bit MCUs. 

![](http://www.mikroe.com/img/development-tools/accessory-boards/click/camera/camera_click.png)
![](http://www.mikroe.com/img/development-tools/accessory-boards/click/eth_wiz/eth_wiz_click.png)

MikroElektronika has a total of **6 PIC32 development boards** as per http://www.mikroe.com/store/pic32/development-boards/. None of these boards have on-board Ethernet PHY or a camera. However, MikroElektronika offers both of these functionalities through it's mikroBUS click boards (http://www.mikroe.com/click/camera/ and http://www.mikroe.com/click/eth/, respectively. This means the PIC32 development board chosen will need spaces for **2 Click boards** (this excludes the possibility of supplementing boards without two slots with the very large MikroMedia Workstation v7). This narrows down the original six development boards to two:

  - Mikromedia Plus for PIC32MX7 with Shield
    - the board itself does not have any mikroBUS slots, thus the shield is needed
    - http://www.mikroe.com/mikromedia/plus/pic32mx7/
  - Clicker 2 for PIC32MX
    - 2 mikroBUS slots on board
    - http://www.mikroe.com/pic32/clicker-2-pic32mx/

----------

Conclusion...

![](http://cdn.mikroe.com/img/clicker/2/pic32mx/overview/clicker_2_pc32mx_combinations.png)

The **Clicker 2 for PIC32MX** would be the choice from MikroElektronika's offerings (the other sytem will be grossly more expensive). However, it appears that MikroElektronika's PIC32 line of products focuses heavily on multimedia more than lower level embedded system applications. This is fine but is the reason so many products were elimated. MikroElektronika favors LCDs over Ethernet and cameras. The prices of everything may be compariable between a MikroElektronika setup and a Microchip setup. It is also worth noting that does not have a development board for any of the PIC32MZ chips (the fastest in Microchip's 32-bit line). However, this will need to be investigated further.

## Consideration of Arduino/Raspberry Pi/BeagleBone

Raspberry Pi/BeagleBone are more or less full blown computers with real-time operating systems. There is a lot that is constantly happening that is near impossible to know the intimate details of. This could be devastating when trying to build a high-speed communication system where every bit of processing needs to be intelligently distributed to different processes.

Arduino was built with hobbyist at the forfront of users. This is fine, but raises issues similar to Raspberry Pi and BeagleBone. The ability to have control over all low level operations (although less friendly during development) is necessary for the system being built here.

## Photodiode Array Investigation

After research into what PD arrays are available (and if they even exist), it is apparent that the **line begins to blur between image sensors and PDs**. A full-fledged image sensor ~~is~~ may be overkill for this situation. Additionally, normal image sensors will not offer the fast responsivity that PDs will in a communication system.

Offerings for **photodiode arrays** that are not many pixel sensors are either **highly specialized** (each section divided to be sensitive to a specific wavelength) or are **linear** (will not fully requirement of a two-dimensional dynamic alignment system).

However, **quadrant photodiodes** are a proven technology used in systems where precise laser alignment is a necessity. These look to be a perfect fit for what is desired in this system.

*Note: The original thought to approach this problem by using a series of intelligently positioned discrete photodiodes is still possible. However, when the application of QPs are analyzed in depth, the shortcomings of a discrete solution become obvious.*

### Quadrant Photodiodes (QP) *or* Avalanche Quadrant Photodiodes (APD)

![](http://www.first-sensor.com/cms/upload/productimages/products/quadrant_pin_photodiodes_first_sensor.jpg)
*http://www.first-sensor.com/en/products/optical-sensors/detectors/quadrant-pin-photodiodes-qp/*
 
  - **four optically active areas separated by small gap**
  - used for detecting the **positioning** of laser beams (**collimators**, **adjustment** applications, etc)
  - reliable incidence of beam on segments on photodiode
    - eliminates potential beam -> photodiode geometry issues that will arise if discrete photodiodes are used
    - **detector gap < spot size < detector size**
  - **many resources/examples** of people using this photodiode within laser alignment systems in research settings
    - good candidate for implementation in a FSO communication system 


----------


Still costly but more readily accessabile and less specialized than photodiode array products. Mouser has a relatively reasonably priced QP for **~$50** from a respected optoelectronics manufacture, First Sensor (http://www.mouser.com/ProductDetail/First-Sensor/QP58-6-TO5/?qs=sGAEpiMZZMtWNtIk7yMEsWYqMtd76IRNp6trVSLtLDk%3d). I have reached out to First Sensor to inquire about samples/eval boards. It may be worth doing the same with other QP manufactures (only if we would actually see ourselves buying other QPs in the future).


----------


Use cases of QP:

  - www.conspiracyoflight.com/Quadrant/Quadrant.html
    - laser stability measurment device using PD **and amplifier circuit**
  - https://www.aptechnologies.co.uk/index.php/support/photodiodes/bi-cell-a-quadrant-photodiodes
    - standalone beam centering system using QP
    - frequencies up to **500kHz** and input powers down to **1uW**
    - brief description of components chosen
  - https://www.aptechnologies.co.uk/index.php/support/photodiodes/bi-cell-a-quadrant-photodiodes (theory of spot size vs gap size vs detector 

# Weekly One-One - November 10, 2016

## Action items

  - re-research light propagation of light through water/saltwater
    - attenuation at different wavelengths
    - can we get away with green/red light (blue is 
  - create comparison matrix for PIC development boards
  - create comparison matrix for QPs
  - PIC32 questions (features from the PIC24 that would be beneficial
	  - does PIC32 have multiple sample and hold inputs?
	  - how fast can we sample ADCs on PIC32?

# Week Thirteen

## Comparison Matrix - PIC development boards

At this point we are down to two offerings from each Microchip and MikroElektronika. 

On the MikroElektronika side, there are only two viable PIC32 development boards options both of which are based on the PIC32MX MCU. These are the **Mikromedia Plus for PIC32MX7** and the **Clicker 2 for PIC32MX**.

On the Microchip side, we have narrowed down development board options to a PIC32MX (for comparison to MikroElektronika PIC32MX board offerings) and a PIC32MZ development board. These are the **PIC32 Ethernet Starter Kit II (DM320004-2)** and **PIC32MZ EC Starter Kit (DM320006)**, respectively.

Comparison of notable features/specs that vary across platforms are shown in the comparison matrix as well as general information for reference of each potential direction. Notice that many options will need additional boards to fulfill all required functionality (non-negotiables).
 
## Matrix - Quad Photodiodes

peak wavelength, package type, active area of elements, cost,

## Attenuation of Light in (salt)Water

![](http://www1.lsbu.ac.uk/water/images/water_spectrum_2.gif)

![][atten_coeff_smith_baker]

[atten_coeff_smith_baker]: 
https://github.com/joshandnoodles/MS/raw/68a3c44e42285979283155432f06d40e25332567/doc/img/atten_coeff_smith_baker.PNG
  "Figure !!: **"