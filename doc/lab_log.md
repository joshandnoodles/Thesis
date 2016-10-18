
[TOC]

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
  
[s1_s2_10k]: https://github.com/joshandnoodles/MS/raw/master/doc/img/s1_s2_10k.bmp
	"Figure 1: Transient response after Stage 1 & 2 with input 20mA 10kHz signal from point-blank distance"
[s1_s2_76k]: https://github.com/joshandnoodles/MS/raw/master/doc/img/s1_s2_76k.bmp
	"Figure 2: Transient response after Stage 1 & 2 with input 20mA 76kHz signal from point-blank distance"
[s1_s2_133k]: https://github.com/joshandnoodles/MS/raw/master/doc/img/s1_s2_133k.bmp
	"Figure 3: Transient response after Stage 1 & 2 with input 20mA 133kHz signal from point-blank distance"
[s1_s2_350k]: https://github.com/joshandnoodles/MS/raw/master/doc/img/s1_s2_350k.bmp
	"Figure 4: Transient response after Stage 1 & 2 with input 20mA 350kHz signal from point-blank distance"
[s1_s2_670k]: https://github.com/joshandnoodles/MS/raw/master/doc/img/s1_s2_670k.bmp
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

Once the video streaming was up, the FPS averaged right above 20 fps although extremes were seen at both 10 fps and 40 fps. As can be seen in the figure !!, after the OpenROV powered up fully and video streaming begain, download rates held at ~21Mbps for about 5 minutes until jumping to **~25Mbps**. Uploads held farily consistently throughout at 

![][networx_bw_post_stream_res_nominal_fps_nominal]

Investigating whether the video resolution or frame could be downgraded to allow for lower download rates, I found there is modifications that can be done on the node.js side of platform (within the openrov-cockpit). This modification will affect the frame rate. It is made in the following file: */opt/openrov/cockpit/src/lib/config.js*.

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


[networx_bw_pre_stream_res_nominal_fps_nominal]: https://github.com/joshandnoodles/MS/raw/master/doc/img/networx_bw_pre_stream_res_nominal_fps_nominal.png
  "Figure !!: Nominal network activity prior to video streaming"
[networx_bw_post_stream_res_nominal_fps_nominal]: https://github.com/joshandnoodles/MS/raw/master/doc/img/networx_bw_post_stream_res_nominal_fps_nominal.png
  "Figure !!: Nominal network activity with video streaming"
[networx_bw_post_stream_res_nominal_fps_3]: https://github.com/joshandnoodles/MS/raw/master/doc/img/networx_bw_post_stream_res_nominal_fps_3.png
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
https://rawgit.com/joshandnoodles/MS/master/doc/img/hld_generic_controller_v1.svg
  "Figure !!: High-level Design of communication system with generic, independent controllers "
  
# Weekly One-One - September 22, 2016

## Action Items

  - Test OpenROV without Topside Interface board and both Tenda Homeplug
	  - should be nominal, but important to test
	  - could raise issues about using using our system in place of Tenda Homeplug Adapters
  - *Cont'd* - further reduce BW usage of OpenROV's video stream by reducing resolution
   - Blackbox design control loop system to better outline the interaction between all mechanisms as well as 

# Week Six

## Tenda-less OpenROV Testing

A nominal test was done on the OpenROV was done to confirm preconceived notion of communication system. It has been generally assumed that the Tenda Homeplug Adapters act as an entirely modular piece within the communication path of the system (i.e the adapters take in communication packets from the Ethernet interface, transmit them over their properietery hardware, and reconstruct the packets entirely to hand back 

To complete this test, the following items were removed:
  - Tenda Homeplug Adapter from OpenROV
  - Tenda Homeplug Adapter from Topside Interface Board
  - Tenda Interface Board
  - Twisted-pair tether

pass through eth and decephier

Once all components had been removed, a small CAT5e cable was connected directly from the Ethernet port of my computer to the BeagleBone that is housed within the OpenROV. The OpenROV was kept partially deconstructed throughout this test as there is no trivial way to run this Ethernet line out of the OpenROV while keeping.

Powering up the system with the modification listed previously, no issues were found. The OpenROV and land-side Cockpit interface performed nominally throughout the duration of the test.

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


![][networx_bw_post_stream_res_640x480_fps_nominal]

[networx_bw_post_stream_res_640x480_fps_nominal]: "https://raw.githubusercontent.com/joshandnoodles/MS/master/doc//img/networx_bw_post_stream_res_640x480_fps_nominal.png"
  "Figure !!: Network activity of streaming videa at a resolution of 640x480"



# Weekly One-One - September 29, 2016

## Action Items

  - *Cont'd* - Test OpenROV without Topside Interface board and both Tenda Homeplug Adapters
	  - should be nominal, but important to test
	  - could raise issues about using using our system in place of Tenda Homeplug Adapters
  - *Cont'd* - Blackbox design control loop system to better outline the interaction between all mechanisms as well as resources/components needed for design

# Weekly One-One w/ Chelsea - October 11, 2016

## Action Items


| Tables   |      Are      |  Cool |
|----------|:-------------:|------:|
| col 1    |  left-aligned | $1600 |
| col 2 is |    centered   |   $12 |
| col 3 is | right-aligned |    $1 |