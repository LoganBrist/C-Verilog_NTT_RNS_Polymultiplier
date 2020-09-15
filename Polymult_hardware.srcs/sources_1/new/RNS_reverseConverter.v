`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/11/2020 11:20:23 AM
// Design Name: 
// Module Name: RNS_reverseConverter
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


module RNS_reverseConverter
    #(
     parameter integer RES_BW = 5,
     parameter integer DYNAMIC_RANGE = 16 * 17 * 19 * 21 * 23 * 25,
     parameter integer NUM_BW = $clog2(DYNAMIC_RANGE), 
     parameter [NUM_BW-1:0] WEIGHT1  = 26'd3900225,
     parameter [NUM_BW-1:0] WEIGHT2  = 26'd18354000,
     parameter [NUM_BW-1:0] WEIGHT3  = 26'd42697200,
     parameter [NUM_BW-1:0] WEIGHT4  = 26'd11886400,
     parameter [NUM_BW-1:0] WEIGHT5  = 26'd37984800,
     parameter [NUM_BW-1:0] WEIGHT6  = 26'd9984576
    )
    (
    input wire [RES_BW-1: 0] residue1,
    input wire [RES_BW-1: 0] residue2,
    input wire [RES_BW-1: 0] residue3,
    input wire [RES_BW-1: 0] residue4,
    input wire [RES_BW-1: 0] residue5,
    input wire [RES_BW-1: 0] residue6,
    
    output wire [NUM_BW-1:0] number
    );
    

     assign prod1 = (residue1 * WEIGHT1) % DYNAMIC_RANGE;
     assign prod2 = (prod1 + residue2 * WEIGHT2) % DYNAMIC_RANGE;
     assign prod3 = (prod2 + residue3 * WEIGHT3) % DYNAMIC_RANGE;
     assign prod4 = (prod3 + residue4 * WEIGHT4) % DYNAMIC_RANGE;
     assign prod5 = (prod4 + residue5 * WEIGHT5) % DYNAMIC_RANGE;
     assign prod6 = (prod5 + residue6 * WEIGHT6) % DYNAMIC_RANGE;
               
     assign number = prod6;//;(prod1 + prod2 + prod3 + prod4 + prod5 + prod6) % DYNAMIC_RANGE;
     
endmodule
/*
C++ code
   for (int i = 0; i < n_moduli; i++) {
        ret_val += weights[i] * num_RNS[i];
        ret_val %= dR;
*/  