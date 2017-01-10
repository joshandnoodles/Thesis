# Optics_and_Laser_Technology_39_5-pp_1098_1100-2007-Ng_Tan_Foo

Small Gaussian laser beam diameter measurement using a quadrant photodiode

This article if very similar to Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan. The only difference here is that the investigators are using a modified approach to measure much smaller Gaussian laser beam diameters not limited by the ~50um gap in between the quadrants of the device. An important note is that the laser illumination should be normal to the detector plane to achieve a circular beam profile. Quadrant photodiodes use proven technology as well as being low-cost and quite robust devices. Again, the accuracy of these measurements is only limited by the resolution of the translator used.

**Figures/Tables:**

![][Optics_and_Laser_Technology_39_5-pp_1098_1100-2007-Ng_Tan_Foo-fig_4]
[Optics_and_Laser_Technology_39_5-pp_1098_1100-2007-Ng_Tan_Foo-fig_4]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/Optics_and_Laser_Technology_39_5-pp_1098_1100-2007-Ng_Tan_Foo-fig_4.png
  "Fig 4: An elliptical laser beam where the principal axis is not coincident with the major axes of the quadrant photodiode. This may present interesting/difficult analysis not only for beam diameter measurements but also for control feedback situations."

# Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan

Gaussian laser beam diameter measurement using a quadrant photodiode

Although existing techniques exist to measure Gaussian laser beam diameters, they require precise optical alignment and require rotation of optical components to achieve diameter measurements in two orthogonal axes. This approach allows diameter measurements in two orthogonal axes and without rotation of components by exploiting a quadrant photodiode. Quadrant photodiodes have been already used successfully in applications such as atomic force microscopy, particle tracking, and photothermal diffusivity measurements. As light is incident on each quadrant, the voltages on each quadrant will be proportional to the amount of light power incident. By moving the quadrant photodiode along the y-axis, and interrogating V_L or V_R, the beam radius can be found. This technique is only limited by the resolution of the translator. The benefit of this setup is the possibility of integrating this beam measurement system into a system that is already tracking beam deflections. Relationships were also given to calculate the left, right, top, and bottom components of the control signal. Indices follow the standard quadrant numbering as used in the unit circle. These are known as the following:

> V_L = V_1 + V_4

> V_R = V_2 + V_3

> V_T = V_1 + V_2

> V_B = V_3 + V_4

**Figures/Tables:**

![][Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_1]
[Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_1]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_1.png
  "Fig 1: The description of the system used to measure the diameter of a Gaussian laser beam. The translator in this figure should be able to move the quadrant photodiode in two dimensions parallel to the orientation of the quadrant photodiode. Conversely, the laser beam itself could be translated similarly. Notice that as the beam traverses the quadrants of the quadrant photodiode, information is able to be determined about the diameter of the laser beam assuming Gaussian laser beam characteristics."
![][Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_2]
[Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_2]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_2.png
  "Fig 2: Plots of the voltage response of the left and right components of the received signal. These results were obtained using a He-Ne laser with 10mW operating at a wavelength of 632.8nm. The beam was translated on an optical mount with 10 microns resolution."
![][Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_3]
[Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_3]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/Review_of_Scientific_Instruments_76_6-pp_1_3-2005-Ng_Foo_Tan-fig_3.png
  "Fig 3: Plots of the voltage response of the top and bottom components of the received signal. These results were obtained using a He-Ne laser with 10mW operating at a wavelength of 632.8nm. The beam was translated on an optical mount with 10 microns resolution.""

# US_Patent_3714491-1970-McIntyre

Quadrant Photodiode

The quadrant photodiode consists of a high-resistivity semiconductor substrate, four quadrant regions of opposite conductivity types, and V-shaped grooves to refract incident light redirecting them toward their corresponding quadrants to prevent optical cross-talk between regions. This PIN device can be useful for position tracking of a beam of light moving across the diode. The positional resolution is determined by the distance between adjacent quadrant electrodes as well as the optical crosstalk between adjacent regions. The position of the beam can be determined by measuring the response of the different quadrant electrodes.

**Figures/Tables:**

![][US_Patent_3714491-1970-McIntyre-fig_3]
[US_Patent_3714491-1970-McIntyre-fig_3]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/US_Patent_3714491-1970-McIntyre-fig_3.png
  "Fig 3: Original drawing detailing the quadrant photodiode. Notice the V-shaped grooves to redirect incident light into the quadrants. The ray traces of this phenomenon are detailed well in US_Patent_3714491-1970-McIntyre-fig_4."
![][US_Patent_3714491-1970-McIntyre-fig_5]
[US_Patent_3714491-1970-McIntyre-fig_5]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/US_Patent_3714491-1970-McIntyre-fig_5.png
  "Fig 5: The relative response of the device as the laser beam moves across the regions. Notice the center region of the graph has an increasing response where light is incident on the angled surface of the V-shaped groove. This is attributed to enhanced quantum efficiency due to the light incident on the side of groove becoming totally internally reflected and trapped in the diode."

# US_Patent_5790291-1998-Britz

Beam Steering and Tracking of Laser Communication Links By Dual Quadrant Tracker and Photodiode Assembly

This patent details a full-duplex communication link using a dual-quadrant tracker and high-speed photodiode assembly. In this novel design, a variant quadrant photodiode is used that has a central aperture. When a cone of light is incident, the inner area of the quadrant photodiode is illuminated. A high-speed photodetector element sits centrally behind the aperture to detect the modulated signal contained within the laser beam. This setup is meant to be a no-compromise solution where the relatively slower quadrant photodiode takes care of position determination and a communication-focused diode can be chosen independently. The central aperture eliminated the need to use a beam splitter (i.e. a prism) to individually redirect to the appropriate detectors. This exact setup also allows the detector behind the central aperture to receive maximum intensity at the information detection area due to the Gaussian nature of the beam (this is shown well in US_Patent_5790291-1998-Britz-fig_5) The incoming beam can be focused into the system appropriately using lens if needed.

**Figures/Tables:**

![][US_Patent_5790291-1998-Britz-fig_2]
[US_Patent_5790291-1998-Britz-fig_2]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/US_Patent_5790291-1998-Britz-fig_2.png
  "Fig 2: Diagram showing bean dual beam transmission and receiving. Notice that lens is placed in front of receiving device to direct light appropriately. This can be used to solve problems of a diverging beam becoming too large at distance."
![][US_Patent_5790291-1998-Britz-fig_3]
[US_Patent_5790291-1998-Britz-fig_3]: https://github.com/joshandnoodles/MS/raw/master/doc/lit/controls/img/US_Patent_5790291-1998-Britz-fig_3.png
  "Fig 3: High-level diagram of system described by patent. Even though the patent describes the benefits of this system (independence of detectors decreases cross-talk and Gaussian beam profile maintains highest intensity for central detector), this could be considered a negative. Separate devices increase intensity and still divides light source intensity between two detectors rather that dual utilization of control and data in the same detector."
