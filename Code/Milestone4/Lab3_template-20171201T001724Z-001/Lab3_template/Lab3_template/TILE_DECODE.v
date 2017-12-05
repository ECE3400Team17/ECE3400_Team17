`define X_SIZE 10'd32
`define Y_SIZE 10'd38
`define X_OFFSET 10'd50
`define Y_OFFSET 10'd50

module TILE_DECODE (input [9:0] X, input [9:0] Y, input [167:0] MAP, input CLK, output reg [9:0] XOFF, output reg [9:0] YOFF, output reg [7:0] CELL, output reg [7:0] RBT);
	wire [4:0] rpos;
	wire [2:0] roth;
	assign rpos = MAP[164:160];
	assign roth = MAP[167:165];
	
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
	
// row
		else if(X >= (`X_SIZE*10'd0 + `X_OFFSET) && X < (`X_SIZE*(10'd0+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd1 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd1+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd0 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd1 + `Y_OFFSET);
			CELL <= MAP[39:32];
			if(rpos == 5'b00100)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
				
		end
			
		else if(X >= (`X_SIZE*10'd1 + `X_OFFSET) && X < (`X_SIZE*(10'd1+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd1 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd1+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd1 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd1 + `Y_OFFSET);
			CELL <= MAP[47:40];
			if(rpos == 5'b00101)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd2 + `X_OFFSET) && X < (`X_SIZE*(10'd2+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd1 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd1+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd2 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd1 + `Y_OFFSET);
			CELL <= MAP[55:48];
			if(rpos == 5'b00110)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd3 + `X_OFFSET) && X < (`X_SIZE*(10'd3+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd1 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd1+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd3 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd1 + `Y_OFFSET);
			CELL <= MAP[63:56];
			if(rpos == 5'b00111)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
// row
		else if(X >= (`X_SIZE*10'd0 + `X_OFFSET) && X < (`X_SIZE*(10'd0+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd2 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd2+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd0 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd2 + `Y_OFFSET);
			CELL <= MAP[71:64];
			if(rpos == 5'b01000)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
				
		end
			
		else if(X >= (`X_SIZE*10'd1 + `X_OFFSET) && X < (`X_SIZE*(10'd1+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd2 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd2+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd1 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd2 + `Y_OFFSET);
			CELL <= MAP[79:72];
			if(rpos == 5'b01001)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd2 + `X_OFFSET) && X < (`X_SIZE*(10'd2+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd2 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd2+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd2 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd2 + `Y_OFFSET);
			CELL <= MAP[87:80];
			if(rpos == 5'b01010)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd3 + `X_OFFSET) && X < (`X_SIZE*(10'd3+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd2 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd2+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd3 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd2 + `Y_OFFSET);
			CELL <= MAP[95:88];
			if(rpos == 5'b01011)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
// row
		else if(X >= (`X_SIZE*10'd0 + `X_OFFSET) && X < (`X_SIZE*(10'd0+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd3 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd3+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd0 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd3 + `Y_OFFSET);
			CELL <= MAP[103:96];
			if(rpos == 5'b01100)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
				
		end
			
		else if(X >= (`X_SIZE*10'd1 + `X_OFFSET) && X < (`X_SIZE*(10'd1+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd3 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd3+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd1 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd3 + `Y_OFFSET);
			CELL <= MAP[111:104];
			if(rpos == 5'b01101)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd2 + `X_OFFSET) && X < (`X_SIZE*(10'd2+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd3 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd3+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd2 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd3 + `Y_OFFSET);
			CELL <= MAP[119:112];
			if(rpos == 5'b01110)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd3 + `X_OFFSET) && X < (`X_SIZE*(10'd3+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd3 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd3+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd3 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd3 + `Y_OFFSET);
			CELL <= MAP[127:120];
			if(rpos == 5'b01111)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
// row
		else if(X >= (`X_SIZE*10'd0 + `X_OFFSET) && X < (`X_SIZE*(10'd0+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd4 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd4+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd0 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd4 + `Y_OFFSET);
			CELL <= MAP[135:128];
			if(rpos == 5'b10000)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
				
		end
			
		else if(X >= (`X_SIZE*10'd1 + `X_OFFSET) && X < (`X_SIZE*(10'd1+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd4 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd4+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd1 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd4 + `Y_OFFSET);
			CELL <= MAP[143:136];
			if(rpos == 5'b10001)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd2 + `X_OFFSET) && X < (`X_SIZE*(10'd2+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd4 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd4+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd2 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd4 + `Y_OFFSET);
			CELL <= MAP[151:144];
			if(rpos == 5'b10010)
				RBT <= {roth,5'b11111};
			else
				RBT <= {roth,5'b00000};
		end
		
		else if(X >= (`X_SIZE*10'd3 + `X_OFFSET) && X < (`X_SIZE*(10'd3+10'd1) + `X_OFFSET) && Y >= (`Y_SIZE*10'd4 + `Y_OFFSET) && Y < (`Y_SIZE*(10'd4+10'd1) + `Y_OFFSET))
		begin 
			XOFF <= (`X_SIZE*10'd3 + `X_OFFSET);
			YOFF <= (`Y_SIZE*10'd4 + `Y_OFFSET);
			CELL <= MAP[159:152];
			if(rpos == 5'b10011)
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