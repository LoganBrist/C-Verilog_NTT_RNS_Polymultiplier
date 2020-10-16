`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2020 11:07:21 PM
// Design Name: 
// Module Name: REDUCE
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


module REDUCE #(
    parameter integer BW_IN        = 32,       //must be less than 2x the modulus BW         
    parameter MOD = 32'd4294967291,
    parameter BW_MOD = $clog2(MOD),
    parameter k   = BW_MOD*2,                           // Constant that needs to satisfy a condition. Determined in C++ code but also can be = CH_BW*2 for worst case scenario
    parameter [BW_MOD*2:0] m   = (2**k) / MOD      // Constant derived from bitshift k
)
(
input wire  [BW_IN-1:0] A,
output wire [BW_MOD-1:0] Z 
);

//////////////////////////////////////////////////////////////////////////

// Begin barrett multiplication
wire [BW_MOD+BW_IN:0] T;
assign T       = A * m; 

wire [BW_MOD+BW_IN-k:0] T_SHIFT;
assign T_SHIFT = T >> k;

wire [(BW_MOD+BW_IN-k)+BW_MOD:0] PROD2;
assign PROD2   = T_SHIFT * MOD;

wire [BW_MOD:0] RES;
assign RES     = A - PROD2;

assign Z = (RES >= MOD) ? (RES - MOD) : RES;

endmodule
