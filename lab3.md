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

![](./images/Lab3/FPGA_Graphic/VGA_cable.JPG)

The DAC converter connected to a VGA cable is shown above. Each color is labeled by R, G, and B. They have resistors in parallel along with the VGA cable having 50Ω resistance in series. The calculations and diagram shown below are set up so that each color pin sums to a total of 1V with each pin representing a bit. The most significant bit has twice the voltage of the second most significant bit, four times as much voltage as the third, and so on and so forth. The voltages turned out to be 1/7V, 2/7V, and 4/7V in order of increasing bit significance. (For blue, since there are only two bits, the voltages were ⅓V and ⅔V instead). 

![](./images/Lab3/FPGA_Graphic/Resistor_calcs.JPG)

### Green Screen

To begin with, we set the whole screen color in green. The idea is let every pixel be green.

```
always @ (posedge CLOCK_25) begin
     PIXEL_COLOR <= 8’b000_000_11
```

### Reading external inputs to FPGA

We can  read external inputs to FPGA by assign certain pins and connecting them with wires.  The wires are connected with external inputs.
```
       wire highlighted_x;
       wire highlighted_y;	 
      //Switch input through GPIO pins
       assign highlighted_x = GPIO_0_D[33];
       assign highlighted_y = GPIO_0_D[31];
```
### Correctly updating a 4-bit array dependent on the inputs

We divide the screen into 4 parts with the background in black and display blue in the chosen part. We use two switches to control the location,one for x, one for y.  [0,0] represents up-left, [0,1] represents up-right, [1,0] represents down-left, [1,1] represents down-left. the chosen part will display blue otherwise it’s in black.
```
always @ (posedge CLOCK_25) begin
        pixel_grid[0][0] <= (highlighted_x==0 && highlighted_y==0)? 8'b000_000_11 : 8'b000_000_00; 
        pixel_grid[0][1] <= (highlighted_x==0 && highlighted_y==1)? 8'b000_000_11 : 8'b000_000_00; 
        pixel_grid[1][0] <= (highlighted_x==1 && highlighted_y==0)? 8'b000_000_11 : 8'b000_000_00; 
        pixel_grid[1][1] <= (highlighted_x==1 && highlighted_y==1)? 8'b000_000_11 : 8'b000_000_00; 

```
### Drawing one box on the screen
To draw a box, we set the corresponding pixels in red, and displayed it.
```
	always @ (posedge CLOCK_25) begin
		pixel_grid[0][0] <=  8'b111_000_00;  // red
       		pixel_grid[0][1] <=  8'b000_000_00;  // black
        	pixel_grid[1][0] <=  8'b000_000_00;  // black
        	pixel_grid[1][1] <=  8'b000_000_00;  // black
		  
	  	if(PIXEL_COORD_X < 10'd320 && PIXEL_COORD_Y < 10'd240) begin
			PIXEL_COLOR <= pixel_grid[0][0];
		end
			
		if(PIXEL_COORD_X >= 10'd320 && PIXEL_COORD_Y < 10'd240) begin
			PIXEL_COLOR <= pixel_grid[0][1];
		 end
			
		 if(PIXEL_COORD_X < 10'd320 && PIXEL_COORD_Y >= 10'd240) begin
			PIXEL_COLOR <= pixel_grid[1][0];
		 end
		 	
		 if(PIXEL_COORD_X >= 10'd320 && PIXEL_COORD_Y >= 10'd240) begin
			PIXEL_COLOR <= pixel_grid[1][1];
		 end
			
```

### Toggle using LED:
Before writing Verilog to take in inputs that would switch among the four squares we first used buttons on the FPGA board to control one and then multiple LED.  We used as a starting step so that we will have the input logic down before adding in the VGA component.

We first used the FPGA to toggle one LED.
```	
 // Simple state machine to toggle LED0 every one second
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				grid_array[0][0] <= 1'b0;
				grid_array[1][0] <= 1'b0;
				grid_array[0][1] <= 1'b0;
				grid_array[1][1] <= 1'b0;
				led_counter <= 25'b0;
		  end
		  
		  if (led_counter == HALF_SEC) begin
				// at 1/2 second, turn on LED 1/3
				grid_array[0][0] <= ~grid_array[0][0];
				grid_array[1][0] <= ~grid_array[1][0];
				//led_state   <= ~led_state;
				led_counter <= led_counter + 25'b1;
		  end
		  
		  else if (led_counter == ONE_SEC) begin
				// at 1 second, turn off LED 0/3, turn on LED 2/4
				grid_array[0][0] <= ~grid_array[0][0];
				grid_array[1][0] <= ~grid_array[1][0];
				grid_array[0][1] <= ~grid_array[0][1];
				grid_array[1][1] <= ~grid_array[1][1];
				led_counter <= 25'b0; // reset clock
		  end 
		  
		  else begin	
				// stay the same, clock increments
				grid_array[0][0] <= grid_array[0][0];
				grid_array[0][1] <= grid_array[0][1];
				grid_array[1][0] <= grid_array[1][0];
				grid_array[1][1] <= grid_array[1][1];
				led_counter <= led_counter + 25'b1;
		  end 
	 end
```

We next used the button on the FPGA board to control multiple LEDs:

<iframe width="560" height="315" src="https://www.youtube.com/embed/iLnJGlyvGGo" frameborder="0" allowfullscreen></iframe>

```
	 // Simple state machine to toggle multiple LEDs controlled by button
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				key_counter<= 4'b0;
				
		  end
		  
		else if(~KEY[1]) begin
			
			if (key_counter == 4'b100) begin
				key_counter <= 4'b0;
			end
			else begin
				key_counter <= key_counter + 4'b1;
			end	
					
		end
		  
	 end
```
	

### Mapping external inputs to four different outputs on the screen
We first mapped out our switch inputs to positions on the screen:


![](./images/Lab3/FPGA_Graphic/00.PNG)

![](./images/Lab3/FPGA_Graphic/01.PNG)

![](./images/Lab3/FPGA_Graphic/10.PNG)

![](./images/Lab3/FPGA_Graphic/11.PNG)


We also had to build a circuit containing a circuit and voltage divider.  The voltage divider is used to decrease the 5V output from the Arduino to 3.3V which is the voltage limit for the FPGA board.

![](./images/Lab3/FPGA_Graphic/FPGA_input.JPG)

The following video shows us controlling the VGA from inputs given by switches:

<iframe width="560" height="315" src="https://www.youtube.com/embed/eVZMVpm_ZxY" frameborder="0" allowfullscreen></iframe>

Here is the full code that performs the functions shown in the video above: 
```
//=======================================================
// ECE3400 Fall 2017
// Lab 3: Switch Among f Squares
//=======================================================

`define ONE_SEC 25000000
`define HALF_SEC 12500000

module DE0_NANO(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	GPIO_0_D,
	GPIO_0_IN,

	//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	GPIO_1_D,
	GPIO_1_IN,
);

	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================

	 localparam ONE_SEC = 25000000; // one second in 25MHz clock cycles
	 localparam HALF_SEC = 12500000; // half a second in 25MHz clock cycles
	 
	 //=======================================================
	 //  PORT declarations
	 //=======================================================

	 //////////// CLOCK //////////
	 input 		          		CLOCK_50;

	 //////////// LED //////////
	 output		     [7:0]		LED;

	 /////////// KEY //////////
	 input 		     [1:0]		KEY;

	 //////////// SW //////////
	 input 		     [3:0]		SW;

	 //////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_0_D;
	 input 		     [1:0]		GPIO_0_IN;

	 //////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_1_D;
	 input 		     [1:0]		GPIO_1_IN;
	 


    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg         CLOCK_25;
    wire        reset; // active high reset signal 
	 
	 

    wire [9:0]  PIXEL_COORD_X; // current x-coord from VGA driver
    wire [9:0]  PIXEL_COORD_Y; // current y-coord from VGA driver
    reg [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [7:0]  pixel_grid [1:0][1:0]; 
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 //reg grid_array [1:0][1:0]; //[rows][columns]
	 
	//reg [3:0]	key_counter;  // counts how many times key1 has been pressed
	 
    // Module outputs coordinates of next pixel to be written onto screen
    VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIXEL_COORD_X),
        .PIXEL_Y(PIXEL_COORD_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5])
    );
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end // always @ (posedge CLOCK_50)
	
	 
	
//	Team alpha
	   //2-by-2 array of bits
       reg grid_array [1:0][1:0]; //[rows][columns]
       wire [1:0] grid_coord_x; //Index x into the array
       wire [1:0] grid_coord_y; //Index y into the array
      // current highlighted square
       wire highlighted_x;
       wire highlighted_y;	 
      //Switch input through GPIO pins
       assign highlighted_x = GPIO_0_D[33];
       assign highlighted_y = GPIO_0_D[31];
		 assign GPIO_CLK = GPIO_0_D[29];
		 
		always @ (posedge CLOCK_25) begin
		  pixel_grid[0][0] <= (highlighted_x==0 && highlighted_y==0)? 8'b000_000_11 : 8'b000_000_00; 
        pixel_grid[0][1] <= (highlighted_x==0 && highlighted_y==1)? 8'b000_000_11 : 8'b000_000_00; 
        pixel_grid[1][0] <= (highlighted_x==1 && highlighted_y==0)? 8'b000_000_11 : 8'b000_000_00; 
        pixel_grid[1][1] <= (highlighted_x==1 && highlighted_y==1)? 8'b000_000_11 : 8'b000_000_00; 
		  
		  if(PIXEL_COORD_X < 10'd320 && PIXEL_COORD_Y < 10'd240) begin
			PIXEL_COLOR <= pixel_grid[0][0];
			
		  end
	
		  if(PIXEL_COORD_X >= 10'd320 && PIXEL_COORD_Y < 10'd240) begin
			PIXEL_COLOR <= pixel_grid[0][1];
		
		  end
			
		  if(PIXEL_COORD_X < 10'd320 && PIXEL_COORD_Y >= 10'd240) begin
			PIXEL_COLOR <= pixel_grid[1][0];

		
		  end
		 	
		  if(PIXEL_COORD_X >= 10'd320 && PIXEL_COORD_Y >= 10'd240) begin
			PIXEL_COLOR <= pixel_grid[1][1];
		
		  end


endmodule


```


 

## Acoustic Team:





[To Home Page](./index.md)
