`define X_SIZE 10'd34
`define Y_SIZE 10'd38
`define ZERO 10'd0
`define N_WALL 10'd5
`define S_WALL 10'd5
`define E_WALL 10'd5
`define W_WALL 10'd5
`define COL_WALL 	8'b11111100
`define COL_UNDF	8'b11100011
`define COL_RBT 8'b11100011
`define COL_VIS 8'b01101101
`define COL_UNV 8'b11111111
`define COL_BCK 8'b01101101
`define COL_ERR 8'b11110011
`define RED 8'b11100000
`define GREEN 8'b00011100
`define BLUE 8'b00000011
`define WHITE 8'b11111111
`define BLACK 8'b00000000

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
	
	wire [9:0] xrel;
	wire [9:0] yrel;
	
	assign xrel = X-XOFF;
	assign yrel = Y-YOFF;
	
	
	always @ (posedge CLK)
	begin
		if(XOFF == 10'b1111111111 && YOFF == 10'b1111111111)
		begin
			COL <= `BLACK;
		end
		else if(xrel >= 0 && xrel < `X_SIZE && yrel >= 0 && yrel < `Y_SIZE)
		begin
			// northwest corner
			if(xrel >= `ZERO && xrel < `W_WALL && yrel >= `ZERO && yrel < `N_WALL && `ZERO)
				COL <= `COL_WALL;
			
			// north wall
			else if(xrel >= `W_WALL && xrel < `X_SIZE-`E_WALL && yrel >= `ZERO && yrel < `N_WALL && NORTH)
				COL <= `COL_WALL;
			
			// norhteast corner
			else if(xrel >= `x_SIZE-`E_WALL && xrel < `X_SIZE && yrel >= `ZERO && yrel < `N_WALL && `ZERO)
				COL <= `COL_WALL;
			
			// west wall
			else if(xrel >= `ZERO && xrel < `W_WALL && yrel >= `N_WALL && yrel < `Y_SIZE-`S_WALL && WEST)
				COL <= `COL_WALL;
			
			// east wall
			else if(xrel >= `X_SIZE-`E_WALL && xrel < `X_SIZE && yrel >= `N_WALL && yrel < `Y_SIZE-`S_WALL && EAST)
				COL <= `COL_WALL;
			
			// southeast corner
			else if(xrel >= `ZERO && xrel < `W_WALL && yrel >= `Y_SIZE-`S_WALL && yrel < `Y_SIZE && `ZERO)
				COL <= `COL_WALL;
			
			// south wall
			else if(xrel >= `W_WALL && xrel < `X_SIZE-`E_WALL && yrel >= `Y_SIZE-`S_WALL && yrel < `Y_SIZE && SOUTH)
				COL <= `COL_WALL;
			
			// southwest corner
			else if(xrel >= `X_SIZE-`E_WALL && xrel < `X_SIZE && yrel >= `Y_SIZE-`S_WALL && yrel < `Y_SIZE && `ZERO)
				COL <= `COL_WALL;
			
			// background color
			else
				if(TREASURE == 2'b01)
					COL <= `RED;
				else if(TREASURE == 2'b10)
					COL <= `GREEN;
				else if(TREASURE == 2'b11)
					COL <= `BLUE;
				else if(COLOR == 2'b01)
					COL <= `COL_VIS;
				else if(rcnd)
					COL <= `COL_RBT;
				else
					COL <= `WHITE;
		end
		else
			COL <= `COL_UNDF;
	end
endmodule