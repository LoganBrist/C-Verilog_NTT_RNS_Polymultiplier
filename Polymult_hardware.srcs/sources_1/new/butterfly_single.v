`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/24/2020 02:53:23 PM
// Design Name: 
// Module Name: butterfly
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

// Give three cycle headstart then produces one per cycle

//data1 is the left earlier index. data2 is the right later index 
module butterfly_single
  #(parameter integer BIT_WIDTH = 256,
    parameter integer MOD = 12289,
    parameter integer MEM_SIZE = 128,
    parameter integer ADDR_W = $clog2(MEM_SIZE) + 1
    )
    (
    input   wire CLK,   
    input   wire [BIT_WIDTH-1:0]  data_in2,
    output  wire [BIT_WIDTH-1:0]  data_in1,
    output  reg  [BIT_WIDTH-1:0]  data_out1, data_out2,
    input   wire [BIT_WIDTH-1:0]  phi,
    input   wire bypassEN,
    output wire [6:0] flags
    );
    
    // FIFO outputs
    wire    [BIT_WIDTH-1:0] fifo_out;
    wire 	[ADDR_W		:	0]				data_count;
	wire  									empty, full, almst_empty, almst_full, err;
	
	//FIFO inputs
	wire   [BIT_WIDTH-1:0] fifo_in;
	reg n_reset = 0; //always 1 (at CLK)
	reg wr_en   = 0;   //always 1 (at CLK)
	reg rd_en = 0;   //switches to 1 at negedge of bypass

    assign flags = {n_reset,wr_en,rd_en,empty,full,almst_full,err};
    	
	//problem: FIFO outputs zero at all times. Set up test signals to determine if
	// data is being filled or if data is just not being read.
	
	//data is popped indefinitely once the first bypass stage is over
	always @(negedge bypassEN) begin
	rd_en <= 1;
	end
	
	
	always @(posedge CLK) begin
	   n_reset <= 1;
	   wr_en   <= 1;
	end
	
    FIFO #(ADDR_W, BIT_WIDTH) 	
		fifo	(fifo_out, data_count,empty, full, almst_empty, almst_full, err,
			     fifo_in, wr_en, rd_en, n_reset, CLK);

        
    //Intermediate buttefly registers
    reg [BIT_WIDTH-1:0] A_ff0 = 0, B_ff0 = 0, phi_ff0 = 0, A_ff1 = 0, B_ff1 = 0; 
    
    assign  fifo_in = data_out1;
    assign data_in1 = fifo_out;
    
     always @(*) begin                 
        if (bypassEN) begin
           data_out1  <= data_in2;
           data_out2  <= data_in1; 
        end
        
        else begin
            data_out1 <= data_in2 + 10;
            data_out2 <= data_in1 + 10;
        end
     end
     
endmodule
