`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/29/2020 04:31:07 PM
// Design Name: 
// Module Name: top
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



module top_serialButterfly #(
    parameter integer W_N            = 2,
    parameter integer MOD            = 673, //12289,
    parameter integer POLY_LENGTH    = 8,
    parameter integer BIT_WIDTH      = $clog2(MOD), //64,
    parameter integer ADDR_WIDTH      = $clog2(POLY_LENGTH),
    parameter A_FILENAME = "A.txt",
    parameter PHI_FILENAME = "phi.txt",
    parameter integer N_NTT_MODULES = 1
    )
    (
    input wire CLK,
    output wire passData1, passData2, passData3,
    output wire [BIT_WIDTH-1:0] b1_in1, b1_in2, b1_out1, b1_out2,
    output wire [BIT_WIDTH-1:0] b2_in1, b2_in2, b2_out1, b2_out2,
    output wire [BIT_WIDTH-1:0] b3_in1, b3_in2, b3_out1, b3_out2
    );  
   
   assign {passData1,passData2,passData3} = bypassEN;
   assign b1_in2  = data_to_b1; 
   assign b2_in2  = b1_to_b2; 
   assign b3_in2  = b2_to_b3;
   
   assign b1_out2 = b1_to_b2;
   assign b2_out2 = b2_to_b3;
   assign b3_out2 = b3_to_mem;

   
//twiddle factors
 wire [ADDR_WIDTH-2:0] phitable_in;
 wire [BIT_WIDTH-1:0]  phitable_out;
  
  phi_table #(BIT_WIDTH, POLY_LENGTH / 2, ADDR_WIDTH - 1, PHI_FILENAME, W_N, MOD) 
  p(phitable_in, phitable_out);
 
 //NTT butterflies   
 wire [BIT_WIDTH-1:0] data_to_b1, b1_to_b2, b2_to_b3, b3_to_mem, bf_inPHI1, bf_inPHI2, bf_inPHI3;
  
 //read input only once 
 always @(posedge CLK) begin
    if (mem_readAddrIn < POLY_LENGTH-1) begin
        mem_readAddrIn <= mem_readAddrIn + 1;
    end
    else begin
        mem_readAddrIn <= mem_readAddrIn;
    end
 end 
 
 //control signal for bypass or butterfly operation
 wire [2:0] bypassEN;
 bypassControl bc (CLK, bypassEN);
 
 butterfly_single #(BIT_WIDTH,MOD,POLY_LENGTH/2) bf1 (CLK, data_to_b1, b1_in1,  b1_out1,  b1_to_b2, bf_inPHI1, bypassEN[0]); 
 butterfly_single #(BIT_WIDTH,MOD,POLY_LENGTH/2) bf2 (CLK,   b1_to_b2, b2_in1,  b2_out1,  b2_to_b3, bf_inPHI2, bypassEN[1]);              
 butterfly_single #(BIT_WIDTH,MOD,POLY_LENGTH/2) bf3 (CLK,   b2_to_b3, b3_in1,  b3_out1, b3_to_mem, bf_inPHI3, bypassEN[2]);
     
  reg [ADDR_WIDTH-1:0] mem_readAddrIn = 0;
  reg [ADDR_WIDTH-1:0] mem_writeAddrIn = 0;
  wire [BIT_WIDTH-1:0] mem_DataIn, mem_DataOut;
  reg mem_writeEn = 0;
  
      
  memory #(BIT_WIDTH,POLY_LENGTH,A_FILENAME, ADDR_WIDTH) mem
  (mem_readAddrIn, mem_writeAddrIn, mem_DataIn, mem_writeEn, mem_DataOut,CLK
  );
    
//////////////////////////////////////////////////////////////////////////////////////////
// Connections 
//////////////////////////////////////////////////////////////////////////////////////////
//phi table I/O 
assign phitable_in  = 1;
assign bf_inPHI1     = phitable_out;  
assign bf_inPHI2     = phitable_out;  
assign bf_inPHI3     = phitable_out;  

//connect memory output to butterfly
assign data_to_b1 = mem_DataOut;
assign mem_DataIn = b3_to_mem; 
endmodule

  
// Multi port memory and/or scheduling is difficult! For now, get an example running at half speed
// by splitting butterfly read and write into two cycles. 
