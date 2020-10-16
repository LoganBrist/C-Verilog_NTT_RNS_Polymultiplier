`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2020 04:00:19 PM
// Design Name: 
// Module Name: MOD_SUB
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


module MOD_SUB #(
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
    wire SUB_CARRY;
    
    // Adder
    assign {SUB_CARRY,SUB} = A - B;
    
    // Subtractor
    assign SUM = SUB + M;
    
    // MUX
    assign Z = SUB_CARRY ? SUM : SUB;

endmodule