`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/24/2020 04:31:05 PM
// Design Name: 
// Module Name: butterfly_tb
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


module butterfly_tb;
    parameter integer BIT_WIDTH = 64;
    parameter integer MOD = 673;
    parameter integer ADDR_WIDTH = 8;
    reg CLK = 1; 
    reg [BIT_WIDTH-1:0]  data_in2 = 0;
    wire [BIT_WIDTH-1:0]  data_in1;
    wire  [BIT_WIDTH-1:0]  data_out1, data_out2;
    reg [BIT_WIDTH-1:0]  phi = 2;
    wire bypassEN;
    wire [6:0] flags;
    
    //bypass control for first butterfly
    wire [2:0] bpEN;
    bypassControl bc (CLK, bpEN);
    assign bypassEN = bpEN[0];
    
// Unit under test
butterfly_single #(BIT_WIDTH, MOD, 4) uut(
    .CLK(CLK),
    .data_in2(data_in2),
    .data_in1(data_in1),
    .data_out1(data_out1),
    .data_out2(data_out2),
    .phi(phi),
    .bypassEN(bypassEN),
    .flags(flags)
    );
    
integer k = 0;
integer max = 1024;

initial begin
    for (k = 0; k < max; k = k+1) begin
        #1
        CLK <= ~CLK;
        #1
        CLK <= ~CLK;
        
        data_in2 <= (data_in2 + 1) % 16;
    end
end

endmodule
