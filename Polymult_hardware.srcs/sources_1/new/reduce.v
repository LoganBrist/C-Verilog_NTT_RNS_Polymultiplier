`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/12/2020 09:36:45 AM
// Design Name: 
// Module Name: reduce
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

//will be replaced with barrett or montgomery reduction
module reduce
#(
    parameter integer BIT_WIDTH_IN  = 32,    
    parameter integer MODULUS       = 12289,
    parameter integer BIT_WIDTH_OUT = $clog2(MODULUS)
 )
 (
    input wire [BIT_WIDTH_IN -1:0] in,
    output reg [BIT_WIDTH_OUT-1:0] out
 );
 
    always @(*) begin
        out = in % MODULUS;
    end
endmodule
