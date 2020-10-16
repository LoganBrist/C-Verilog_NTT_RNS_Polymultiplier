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
    parameter MOD                  = 32'd4294967291,
    parameter integer IN_BW        = $clog2(MOD),               //Assumed, can be adjusted at instantiation
    parameter integer OUT_BW       = $clog2(MOD)                //Always equal to MOD bitwidth
)
(
    input wire  [IN_BW-1:0] A,
    input wire  [IN_BW-1:0] B,
    output wire [OUT_BW-1:0] Z 
);

    // Wires
    wire [OUT_BW-1:0] SUM;
    wire [IN_BW-1:0] SUB;
    wire SUB_CARRY;
    
    // Adder
    assign {SUB_CARRY,SUB} = A - B;
    
    // Subtractor
    assign SUM = SUB + MOD;
    
    // MUX
    assign Z = SUB_CARRY ? SUM : SUB;

endmodule