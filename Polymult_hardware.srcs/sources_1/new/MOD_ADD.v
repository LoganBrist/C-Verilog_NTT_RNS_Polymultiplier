`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2020 12:38:48 PM
// Design Name: 
// Module Name: MOD_ADD
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


module MOD_ADD 
#(
parameter MOD                  = 32'd4294967291,
parameter integer IN_BW        = $clog2(MOD),               // Assumed, can be adjusted at instantiation. (HAVE TO ASSIGNING VALUE IF INPUT AND MODULUS ARE NOT EQUAL WIDTHS) 
parameter integer OUT_BW       = $clog2(MOD)                // Always equal to MOD bitwidth

)
(
input wire  [IN_BW-1:0] A,
input wire  [IN_BW-1:0] B,
output wire [OUT_BW-1:0] Z 
);

// Wires
wire [IN_BW-1:0] SUM;
wire [IN_BW-1:0] SUB;
wire SUM_CARRY;
wire SUB_CARRY;

// Adder
assign {SUM_CARRY,SUM} = A + B;

// Subtractor
assign {SUB_CARRY,SUB} = SUM - MOD;

// Select 
assign SEL = !(!SUM_CARRY & SUB_CARRY);

// MUX
assign Z = SEL ? SUB : SUM;


/*
// Version with MOD passed as input
#(
parameter integer CH_BW        = 32                //RNS channel bitwidth
)
(
input wire  [CH_BW-1:0] A,
input wire  [CH_BW-1:0] B,
input wire  [CH_BW-1:0] M,
output wire [CH_BW-1:0] Z 
);

// Wires
wire [CH_BW-1:0] SUM;
wire [CH_BW-1:0] SUB;
wire SUM_CARRY;
wire SUB_CARRY;

// Adder
assign {SUM_CARRY,SUM} = A + B;

// Subtractor
assign {SUB_CARRY,SUB} = SUM - M;

// Select 
assign SEL = !(!SUM_CARRY & SUB_CARRY);

// MUX
assign Z = SEL ? SUB : SUM;
*/



endmodule
