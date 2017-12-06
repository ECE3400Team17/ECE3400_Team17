[To Home Page](/index.md)

# Lab 4: FPGA Video Controller and Sound Generation

## Over all Objective: 
Lab4 is an extension of the work that was done for lab3. The goal of lab4 is to use the maze information that is received from the radio to update the maze information that will be displayed on a VGA monitor.  Our team was divided into to two different groups. One group worked on setting up the radio and the other worked on setting up the FPGA.

## Radio Team:
Yijia Chen, Thinesiya Krishnthasan, Stephanie Lin 

### Objective:
The goal of this lab is to implement the wireless communication between two Arduinos so that they may send and receive the robot’s mapping information. 

### Materials Used:
* 2 Nordic nRF24L01+ transceivers
* 2 Arduino Unos (one must be shared with the other sub-team)
* 2 USB A/B cables
* 2 radio breakout boards with headers

### Prelab:
First, we downloaded the RF34 Arduino library and added it to our Arduino library directory. After downloading the Getting Started code from the Lab 4 webpage and replacing the example in the RF 24 library, we modified it to test out our radios and become familiar with the library. We changed the identifier numbers by using the formula 2(3D+N)+X, where D = 4 (Friday lab), N = 17 (team number), and X = 0, 1 for our two radios. We changed the channel numbers to 3A and 3B, implemented in the following code:

```arduino 
const uint64_t pipes[2] = { 0x000000002ALL, 0x000000002BLL };
```

We then programmed both Arduino boards with the Getting Started example, plugged them into two different laptops, and selected one board to be the transmitter by entering “T” into the serial monitor. Below is a screenshot of what we saw on each monitor. The left side is from the transmitter and the right side is the response. We received regular outputs for successfully sent and received signals. Then, we examined the effect of moving the radios far from one another. We then noticed that the sending failed (see that the screen on the right does not show payload 67313 at all) and that the failures that didn’t time out would return with an exceptionally long round-trip delay.

![](/images/Lab4/SendandReceiveGettingStarted.png)

### Sending an entire maze:

Next, we modified the code to send the maze as a single packet. We defined the maze to be a 4x5 array of unsigned chars. 

#### Defining the maze —

```arduino 
unsigned char maze[4][5] =
{
	2, 2, 2, 2, 2,
	3, 1, 1, 1, 3,
	3, 2, 0, 1, 2,
	3, 1, 3, 1, 3,
}; 
```

#### Modified code for the transmitting end —

```arduino  
if (role == role_ping_out)
{
	// First, stop listening so we can talk.
	radio.stopListening();

	// NOTE: the maze array is defined here

	// Send the maze in a single payload
	printf("Sending the maze!\n");
	bool ok = radio.write( maze, sizeof(maze) );

	if (ok)
		printf("ok, sending. \n");
	else
		printf("failed.\n\r");

	// Now, continue listening
	radio.startListening();

	// Try again 1s later
	delay(1000);
} 
```

*Note: the maze array is also defined in this section.*


#### Modified code for receiving end —

```arduino
if ( role == role_pong_back )
{
	// if there is data ready
	if ( radio.available() )
	{
		// Dump the payloads until we've gotten everything
		unsigned char got_maze[4][5];
		bool done = false;
		while (!done)
		{
			// Fetch the payload, and see if this was the last one.
			done = radio.read( got_maze, sizeof(got_maze) );

			// Spew it
			for (int i=0 ; i<4 ; i++){
				for (int j=0 ; j<5 ; j++){
					printf ("%d ", got_maze[i][j]);
				}
				printf ("\n");
			}

			// Delay just a little bit to let the other unit
			// make the transition to receiver
			delay(20);
		}
	}
}
```

The serial monitors display as we expect them to. The transmitting side sends the payload, and the second Arduino receives and displays the matrix.

*Transmitting:*

![](/images/Lab4/PongOut.JPG)

*Receiving:*

![](/images/Lab4/PongBack4x5.png)

*Note: The RFK library has an Auto-ACK feature.*

### Sending New Data:

We then modified the code to send new data. We sent 3 pieces of information: a 1-bit indicator, a 2-bit (cardinal) direction, and a 5-bit position coordinate. The indicator is for any extraneous information (i.e. treasures, walls) to be implemented in a suitable fashion in the future. If it turns out that this doesn’t work out well, our code can be easily modified to change the number of bits and the meaning of the indicator. The 2-bit direction denotes which way the robot is facing: North, South, East, or West. (Corresponding binary numbers to be assigned later when code is combined). Finally, the 5-bit position coordinate represents which cell the robot is in. Since the maze is a 4x5 grid, there are 20 boxes, and thus we need 5 bits. We will coordinate the position numbers as follows:

![](/images/Lab4/grid.JPG)

For the transmitter, we packed the bits by using a left shift and sent the package as a single payload.

#### Transmitter side code —

```arduino
if (role == role_ping_out)
{
	// First, stop listening so we can talk.
	radio.stopListening();


	// Define data.
	unsigned char new_data;
	// pack bits as follows:
	// indicator | direction (N/S/E/W) | position (cell 0 - 19)
	// 1 bit | 2 bits | 5 bits

		// test
	unsigned char ind = 0;  // indicator
	unsigned char dir = 3;  // direction
	unsigned char pos = 12; // robot position

	// shift bits in order to pack bits, then or them together
	new_data = ind << 6 | dir << 4 | pos;
	// (0,3,12) should give 0111100 or 60 in decimal

	// Send maze in single payload
	printf("Now sending new data!\n");
	bool ok = radio.write(&new_data, sizeof(unsigned char) );

	if (ok)
		printf("ok\n");
	else
		printf("failed.\n\r");

	// Now, continue listening
	radio.startListening();
}
```

The receiver side unpacks the bits.

#### Receiver side code —

```arduino
if ( role == role_pong_back )
{
	// if there is data ready
	if ( radio.available() )
	{
		unsigned char got_data;
		bool done = false;
		while (!done)
		{
			// Fetch the payload, and see if this was the last one.
			done = radio.read(&got_data, sizeof(unsigned char) );

		    	// Spew it
			printf("Got payload %d...\n",got_data); // display decimal

			// Delay just a little bit to let the other unit
			// make the transition to receiver
			delay(20);
		}
	}
}	
```

The serial monitor displayed as expected. We sent and received a payload of value 60 (00111100). Our results are pictured below. The left side is the transmitter screen, and the right side is the receiver screen.

![](/images/Lab4/NewDataSendReceive.png)

## FPGA Team:
Peter Slater, Mira Bhut, Yirong Alan

### Objective:

In the final design, we need to send back information about the vehicle location and the maze situation from the Arduino on the car to the base station in order to draw the maze. The maze is drawn by the FPGA, while the information is wirelessly transferred by the Arduino. Hence, we need to communicate between the Arduino and the FPGA through SPI.
 
### Hardware setup:

![](/images/Lab4/hardwareSetup.png)

*Note:  For all the signal sent from Arduino (5v) to FPGA (3.3v), it needs to go through a voltage divider.*

![](/images/Lab4/resistors.png)

So basically, the Arduino here is the master while the FPGA is the slave. The data is sent from the Arduino to the FPGA to let it know the maze situation and draw it. We use a 8-bit data to represent the vehicle’s location in the maze:
 
Indicator (for future use) | direction of the robot (N/S/E/W) | position (cell 0 – 19)
1 bit          	           | 2 bits (4 states)        	      | 5 bits

### Sending out data from Arduino:
 
The following Arduino code is to send a testing data out from Arduino (the testing data is 01101100, i.e. (0,3,12)):
 
```arduino 
#include "SPI.h" // necessary library
int ss=10; // using digital pin 10 for SPI slave select
int del=200; // used for various delays
 
void setup()
{
  Serial.begin(9600);
  pinMode(ss, OUTPUT); // we use this for SS pin
  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
  // our MCP4162 requires data to be sent MSB (most significant byte) first
}
 
void setValue(int value)
{
  digitalWrite(ss, LOW);
  SPI.transfer(0); // send command byte
  SPI.transfer(value); // send value (0~255)
  digitalWrite(ss, HIGH);
}
 
void loop()
{
  unsigned char new_data;
  	// pack bits as follows:
  	// indicator | direction (N/S/E/W) | position (cell 0 - 19)
  	// 1 bit | 2 bits | 5 bits
	
  	// test
  	unsigned char ind = 0;
  	unsigned char dir = 3;
  	unsigned char pos = 12;
	
  	// shift bits in order to pack bits, then or them together
  	new_data = ind << 7 | dir << 5 | pos;
  	// (0,3,12) should give 01101100 or 60 in decimal
  	Serial.println (new_data);
      setValue(new_data);
  	
}
```
 
 
### Receiving data at FPGA:
 
To receive the data at FPGA, we need a new module SPI:

```verilog 
module SPI (input sck, input mosi, input cs, output reg MAP);
  	reg [159:0] MAP;
  	reg [159:0] tmap;
  	reg [7:0] idx;
  	
  	initial begin
        	MAP <= 160'd0;
        	tmap <= 160'd0;
        	idx <= 8'd0;
        	end
  	
  	always @ (negedge cs) begin
        	idx <= 8'd0;
  	end
        	
  	always @ (posedge sck) begin
        	tmap[idx] <= mosi;
        	idx <= idx + 1;
  	end
  	
  	always @ (posedge cs) begin
        	MAP <= tmap;
  	end
  	
  	endmodule
```
 
When the cs (chip selected) goes down, the FPGA starts to receive data from the most line per positive clock cycle. For each cycle, the FPGA will receive a 8-bit data and stored it in MAP. Eventually the MAP will consist of 20 8-bit data for the robot being in all the positions in the maze, having a length of 160 bits and storing all the information of the vehicle when it is in the maze.

### Map Rendering with an FPGA

![Rendered 5x4 Map](/images/Lab4/map.jpg)

*The finished 5x4 map rendered with the robot (blue), explored areas (grey), and to be explored areas (white).*



The goal of this project is to be able to render the maze map using the FPGA's video output. The standard resolution for VGA is 640x480 pixels which divides nicely into 20x15 square tiles that are 32 pixels wide. Sadly, this didn't work out very well because the displays we are using have elongated pixels and the Y-dimension is shrunk. To fix this we extended the height from 32 to 38 to keep the rendered tiles square. Since each tile in the map is identical, we can breakdown this problem into two parts, first a module to render the data in a cell, for now this is just a color representing its state, and second a decoder that could convert X and Y coordinates to those relative to the cell to be rendered.

##### Tile Rendering

```verilog
`define X_SIZE 10'd32
`define Y_SIZE 10'd38
`define N_WALL 10'd5
`define S_WALL 10'd5
`define E_WALL 10'd5
`define W_WALL 10'd5
`define COL_WALL 	8'b11100000
`define COL_UNDF	8'b11100011
`define COL_RBT 8'b00000011
`define COL_VIS 8'b01101101
`define COL_UNV 8'b11111111

module TILE_RENDER (input [9:0] X, input [9:0] Y, input [9:0] XOFF, input [9:0] YOFF, input CLK, input [7:0] CELL, input [7:0] RBT, output reg [7:0] COL);
	wire NORTH;
	wire SOUTH;
	wire EAST;
	wire WEST;
	wire [1:0] COLOR;
	wire [1:0] TREASURE;
	wire ncnd;
	wire scnd;
	wire ecnd;
	wire wcnd;
	wire rcnd;
	wire icnd;
	wire [1:0] ront;
	wire rncnd;
	wire rscnd;
	wire recnd;
	wire rwcnd;
	
	assign NORTH = CELL[0];
	assign SOUTH = CELL[1];
	assign EAST = CELL[2];
	assign WEST = CELL[3];
	assign COLOR = CELL[5:4];
	assign TREASURE = CELL[7:6];
	assign rcnd = RBT[0];
	assign icnd = RBT[7];
	assign ront = RBT[6:5];
	assign rncnd = ront == 2'b00;
	assign rscnd = ront == 2'b01;
	assign recnd = ront == 2'b10;
	assign rwcnd = ront == 2'b11;
	
	wire [9:0] xrel;
	wire [9:0] yrel;
	
	assign xrel = X-XOFF;
	assign yrel = Y-YOFF;
	
	assign ncnd = yrel  < `N_WALL;
	assign scnd = yrel > `Y_SIZE-`S_WALL-1;
	assign ecnd = xrel > `X_SIZE-`E_WALL-1;
	assign wcnd = xrel < `W_WALL;
	
	always @ (posedge CLK)
	begin
		if(xrel >= 0 && xrel <= `X_SIZE && yrel >= 0 && yrel <= `Y_SIZE)
		begin
			if(rcnd)
				COL <= `COL_RBT;
			else if(COLOR == 2'b11)
				COL <= `COL_VIS;
			else if(COLOR == 2'b00)
				COL <= `COL_UNV;
			else
				COL <= `COL_UNDF;
		end
		else
			COL <= `COL_UNDF;
	end
endmodule
```

The code for the tile rendering is included above. It takes in pixel X and Y coordinates as well as the X and Y offsets to the current tile to be rendered. Additionally it takes in map data in terms of CELL and RBT to convey information about the current tile and the robot to be rendered. To see how this data is decoded and encoded look in the SPI communication section. Using the current pixel position and offsets we calculated tile relative coordinates that are always within X_SIZE and Y_SIZE of the tile. We then use the robot and cell data to pick which color we render to the screen which is an output of the module.



#####  Tile Decoding

```verilog
`define X_SIZE 10'd32
`define Y_SIZE 10'd38
`define X_OFFSET 10'd0
`define Y_OFFSET 10'd1

module TILE_DECODE (input [9:0] X, input [9:0] Y, input [167:0] MAP, input CLK, output reg [9:0] XOFF, output reg [9:0] YOFF, output reg [7:0] CELL, output reg [7:0] RBT);
	wire [4:0] rpos;
	wire [2:0] roth;
	assign rpos = MAP[164:160];
	assign roth = MAP[167:164];
	
	always @ (posedge CLK)
	begin
// row
		if(X >= (`X_SIZE*10'd0 + `X_OFFSET) && X < (`X_SIZE*(10'd0+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd0 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd0+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd0 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd0 + `Y_OFFSET);
			CELL <= MAP[7:0];
			if(rpos == 5'b00000)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
				
		end
		
		else if(X >= (`X_SIZE*10'd1 + `X_OFFSET) && X < (`X_SIZE*(10'd1+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd0 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd0+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd1 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd0 + `Y_OFFSET);
			CELL <= MAP[15:8];
			if(rpos == 5'b00001)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd2 + `X_OFFSET) && X < (`X_SIZE*(10'd2+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd0 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd0+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd2 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd0 + `Y_OFFSET);
			CELL <= MAP[23:16];
			if(rpos == 5'b00010)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd3 + `X_OFFSET) && X < (`X_SIZE*(10'd3+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd0 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd0+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd3 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd0 + `Y_OFFSET);
			CELL <= MAP[31:24];
			if(rpos == 5'b00011)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
	
/*..........CODE REMOVED....................*/
		
		else if(X >= (`X_SIZE*10'd3 + `X_OFFSET) && X < (`X_SIZE*(10'd3+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd4 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd4+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd3 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd4 + `Y_OFFSET);
			CELL <= MAP[159:152];
			if(rpos == 5'b10000)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else
		begin
			XOFF <= 10'b1111111111;
			YOFF <= 10'b1111111111;
			CELL <= 8'b00000000;
			RBT <= 8'b00000000;
		end
	end
	
endmodule
```

A truncated version of our cell decoder is shown above. For each tile, a set of discrete checks if performed that determines what tile the current pixel is in. The code then calculates the X and Y offsets of that tile and updates the CELL and RBT data from the MAP that gets wired to the tile renderer. The code follows a simple repetitive structure so while pretty long, the actual time to produce was short. Figure 1 shows the results of this process, this system is able to successfully decode the map input data and output the current state of each cell as a solid color. Additionally, the code was written with modularity in mind so it will be easy to add more tests to the renderer to decode walls and treasures in the future labs. 

[To Home Page](/index.md)
