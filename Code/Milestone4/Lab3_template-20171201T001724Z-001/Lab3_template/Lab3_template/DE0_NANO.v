//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define ONE_SEC 25000000

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
    wire [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 
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
	 
	 reg [167:0] MAP;
	 wire [7:0] CELL;
	 wire [9:0] XOFF;
	 wire [9:0] YOFF;
	 wire [7:0] RBT;
	 //assign MAP = 168'b00010011_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111;	 

	 TILE_DECODE tcode(
		.X(PIXEL_COORD_X),
		.Y(PIXEL_COORD_Y),
		.MAP(MAP),
		.CLK(CLOCK_25),
		.XOFF(XOFF),
		.YOFF(YOFF),
		.CELL(CELL),
		.RBT(RBT)
	 );
	 
	 
	 TILE_RENDER trend(
		.X(PIXEL_COORD_X),
		.Y(PIXEL_COORD_Y),
		.XOFF(XOFF),
		.YOFF(YOFF),
		.CLK(CLOCK_25),
		.CELL(CELL),
		.RBT(RBT),
		.COL(PIXEL_COLOR)
	 );
	 
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 //assign PIXEL_COLOR = 8'b000_111_00; // Green
	 assign LED[0] = led_state;
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end // always @ (posedge CLOCK_50)
	
	 // Simple state machine to toggle LED0 every one second
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				led_state   <= 1'b0;
				led_counter <= 25'b0;
		  end
		  
		  if (led_counter == ONE_SEC) begin
				led_state   <= ~led_state;
				led_counter <= 25'b0;
		  end
		  else begin	
				led_state   <= led_state;
				led_counter <= led_counter + 25'b1;
		  end
		  
		  if(led_counter <= 25'd2500000)
				MAP <= 168'b00010011_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111;
		  else if (led_counter <= 25'd5000000)
				MAP <= 168'b00010010_00010111_00001011_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111;
		  else if (led_counter <= 25'd7500000)
				MAP <= 168'b00010001_00010111_01000011_00001011_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111;
		  else if (led_counter <= 25'd10000000)
				MAP <= 168'b00010000_00010111_01000011_00010011_00111011_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111;
		  else if (led_counter <= 25'd12500000)
				MAP <= 168'b10010000_00010111_01000011_00010011_00111011_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111_00001111_00001111_00001111_00111111;
		  else
				MAP <= MAP;
	 end
	 

endmodule
