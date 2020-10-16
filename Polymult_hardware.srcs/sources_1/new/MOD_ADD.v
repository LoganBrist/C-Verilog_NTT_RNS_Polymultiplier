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


module MOD_ADD #(
parameter integer CH_BW        = 32                //RNS channel bitwidth
)
(
input wire  [CH_BW-1:0] A,
input wire  [CH_BW-1:0] B,
input wire  [CH_BW-1:0] M,
output wire [CH_BW-1:0] Z 
);

/*
reg [CH_BW:0] SUM = 0;
always @(*) begin
    SUM <= A + B;
    if (SUM < M) begin
        Z <= SUM;
    end
    else begin
        Z <= SUM - M;
    end
end
*/

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




endmodule
