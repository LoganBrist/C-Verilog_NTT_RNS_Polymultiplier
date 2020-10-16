`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2020 04:56:24 PM
// Design Name: 
// Module Name: MOD_MULT
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


module MOD_MULT #(
    parameter integer CH_BW        = 32,                
    parameter MOD = 32'd4294967291,
    parameter k   = CH_BW*2,                           // Constant that needs to satisfy a condition. Determined in C++ code but also can be = CH_BW*2 for worst case scenario
    parameter [CH_BW*2:0] m   = (2**k) / MOD      // Constant derived from bitshift k
)
(
input wire  [CH_BW-1:0] A,
input wire  [CH_BW-1:0] B,
output wire [CH_BW-1:0] Z 
);

//////////////////////////////////////////////////////////////////////////

// Begin barrett multiplication
wire [CH_BW*2:0] PROD, RES;
wire [CH_BW*3:0] T, T_SHIFT;
wire [CH_BW*3+CH_BW:0] PROD2;

assign PROD    = A    * B;
assign T       = PROD * m; 
assign T_SHIFT = T >> k;
assign PROD2   = T_SHIFT * MOD;
assign RES     = PROD - PROD2;
assign Z = (RES >= MOD) ? (RES - MOD) : RES;

endmodule
