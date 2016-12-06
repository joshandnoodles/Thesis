# Take-away from Senior Design (UC the Fish - Fall 2015-Spring 2016)

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


[s1_s2_10k]: ./img/s1_s2_10k.bmp
  "Figure 1: Transient response after Stage 1 & 2 with input 20mA 10kHz signal from point-blank distance"

[s1_s2_76k]: ./img/s1_s2_76k.bmp
	"Figure 2: Transient response after Stage 1 & 2 with input 20mA 76kHz signal from point-blank distance"
[s1_s2_133k]: ./img/s1_s2_133k.bmp
	"Figure 3: Transient response after Stage 1 & 2 with input 20mA 133kHz signal from point-blank distance"
[s1_s2_350k]: ./img/s1_s2_350k.bmp
	"Figure 4: Transient response after Stage 1 & 2 with input 20mA 350kHz signal from point-blank distance"
[s1_s2_670k]: ./img/s1_s2_670k.bmp
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

