`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/12/2020 09:26:37 AM
// Design Name: 
// Module Name: modmult
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


module modmult
#(
    parameter integer BIT_WIDTH_IN  = 16,
    parameter integer MODULUS       = 31,
    parameter integer BIT_WIDTH_OUT = $clog2(MODULUS)
 )
 (
    input wire [BIT_WIDTH_IN-1:0] A,
    input wire [BIT_WIDTH_IN-1:0] B,
    output reg [BIT_WIDTH_OUT-1:0] out
 );
    
    reg [BIT_WIDTH_IN * 2 - 1 : 0] reduced_product_in;
    wire [BIT_WIDTH_OUT-1 : 0]      reduced_product_out;
    
    reduce #(BIT_WIDTH_IN * 2, MODULUS) red1 (reduced_product_in, reduced_product_out);
    
    always @(*) begin
      reduced_product_in = A * B;
      out                = reduced_product_out; 
    end
endmodule
