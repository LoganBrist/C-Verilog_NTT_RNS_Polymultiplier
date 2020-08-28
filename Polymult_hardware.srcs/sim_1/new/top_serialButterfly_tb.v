`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/25/2020 10:49:36 PM
// Design Name: 
// Module Name: top_serialButterfly_tb
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


module top_serialButterfly_tb(

    );
    parameter integer W_N            = 2;
    parameter integer MOD            = 673; //12289,
    parameter integer POLY_LENGTH    = 8;
    parameter integer BIT_WIDTH      = $clog2(MOD); //64,
    parameter integer ADDR_WIDTH      = $clog2(POLY_LENGTH);
    parameter A_FILENAME = "A.txt";
    parameter PHI_FILENAME = "phi.txt";
    parameter integer N_NTT_MODULES = 1;
    
    integer CYCLE = 0;
    reg CLK = 0;
    wire passData1;
    wire [BIT_WIDTH-1:0] b1_in1, b1_in2, b1_out1, b1_out2;
    wire passData2;
    wire [BIT_WIDTH-1:0] b2_in1, b2_in2, b2_out1, b2_out2;
    wire passData3;
    wire [BIT_WIDTH-1:0] b3_in1, b3_in2, b3_out1, b3_out2;
    
top_serialButterfly #(W_N, MOD,POLY_LENGTH,BIT_WIDTH, ADDR_WIDTH, A_FILENAME,PHI_FILENAME,N_NTT_MODULES)
    uut(
    .CLK(CLK),
    .passData1(passData1),
    .passData2(passData2), 
    .passData3( passData3),
    .b1_in1(b1_in1), .b1_in2(b1_in2), .b1_out1(b1_out1), .b1_out2(b1_out2),
    .b2_in1(b2_in1), .b2_in2(b2_in2), .b2_out1(b2_out1), .b2_out2(b2_out2),
    .b3_in1(b3_in1), .b3_in2(b3_in2), .b3_out1(b3_out1), .b3_out2(b3_out2)
    );  
    
  initial begin
  #1
    for (integer i = 0; i < 16; i = i+1) begin
        #1
        CLK <= ~CLK;
        CYCLE <= CYCLE + 1;
        #1
        CLK <= ~CLK;  
     end
  end
endmodule
