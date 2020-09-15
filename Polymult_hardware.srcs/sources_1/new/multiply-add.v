`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/25/2020 11:32:01 AM
// Design Name: 
// Module Name: multiply-add
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


module MOD_MAC#(
    parameter BW      = 32,
    parameter MOD     = 13
    )
    (
    input  wire CLK,
    input  wire [BW-1:0]  A,
    input  wire [BW-1:0]  B,
    input  wire reset,
    output reg  [BW-1:0]  Z_out = 0
    );
   
   wire [2*BW-1:0] a;
   wire [BW-1:0]   b, result;
   wire [BW:0]     c;
   
   assign a      = A * B;
   assign b      = a % MOD;
   assign c      = b + Z_out;
   assign result = c % MOD;

    always @(posedge CLK) begin
        if(reset)
            Z_out <= 0;
        else 
            Z_out <= result;
    end
endmodule
