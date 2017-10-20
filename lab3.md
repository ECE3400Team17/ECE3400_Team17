[To Home Page](./index.md)

# Lab 3: FPGA Video Controller and Sound Generation

## Objective: 

## Work Distribution:

### Graphics Team:
* Stephanie
* Yijia
* Thinesiya 

### Acoustic Team:
* Mira
* Peter
* Alan

## Graphics Team:

### Description of how the DAC on the provided VGA connectors works and how the resistor values were chosen.

We used an 8-bit DAC (Digital to Analog Converter) to encode colors to the display. It uses 3 bits to represent values for green and red, and 2 bits for blue. The DAC uses 8 pins to connect to the FPGA board, and the sections for each color combine to produce a voltage between 0 and 1V. We use the resistors to step down the voltage from 3.3 volts to designated voltages calculated using a voltage divider. The resistor values calculated allow us to step down the voltage to our desired values. Since the red and green colors are comprised of 3 bits each and blue is comprised of 2, there should be 8 possible colors and therefore 8 different voltages (with a 1/7V difference).

[]!(./images/Lab3/FPGA_Graphic/VGA_cable.JPG)

The DAC converter connected to a VGA cable is shown above. Each color is labeled by R, G, and B. They have resistors in parallel along with the VGA cable having 50Ω resistance in series. The calculations and diagram shown below are set up so that each color pin sums to a total of 1V with each pin representing a bit. The most significant bit has twice the voltage of the second most significant bit, four times as much voltage as the third, and so on and so forth. The voltages turned out to be 1/7V, 2/7V, and 4/7V in order of increasing bit significance. (For blue, since there are only two bits, the voltages were ⅓V and ⅔V instead). 

[]!(./images/Lab3/FPGA_Graphic/Resistor_calcs.JPG)

## Acoustic Team:





[To Home Page](./index.md)
