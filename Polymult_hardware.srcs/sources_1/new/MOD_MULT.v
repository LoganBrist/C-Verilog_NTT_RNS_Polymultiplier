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
    parameter MOD                  = 32'd4294967291,
    parameter integer IN_BW         = $clog2(MOD),                // Assumed, can be adjusted at instantiation. (HAVE TO ASSIGNING VALUE IF MODULI ARE NOT EQUAL WIDTHS) 
    parameter integer OUT_BW        = $clog2(MOD),                // Always equal to MOD bitwidth
    parameter k   = OUT_BW*2,                                     // MOD_BW*2 for worst case scenario. Can be determined with C++ code.
    parameter [OUT_BW*2:0] m   = (2**k) / MOD                     // Constant derived from bitshift k
)
(
input wire  [IN_BW-1:0] A,
input wire  [IN_BW-1:0] B,
output wire [OUT_BW-1:0] Z 
);

//////////////////////////////////////////////////////////////////////////

// Begin barrett multiplication
wire [IN_BW*2:0] PROD, RES;
wire [(IN_BW+OUT_BW)*2:0] T;
wire [(IN_BW+OUT_BW)*2-k:0] T_SHIFT;
wire [(IN_BW+OUT_BW)*2-k+OUT_BW:0] PROD2;

assign PROD    = A    * B;
assign T       = PROD * m; 
assign T_SHIFT = T >> k;
assign PROD2   = T_SHIFT * MOD;

assign RES     = PROD - PROD2;
assign Z = (RES >= MOD) ? (RES - MOD) : RES;

endmodule
