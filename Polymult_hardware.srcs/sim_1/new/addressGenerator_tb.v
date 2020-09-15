`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/29/2020 10:39:04 AM
// Design Name: 
// Module Name: addressGenerator_tb
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

module addressGenerator_tb(
    );
    
    parameter N_COEFF = 8;
    parameter ADDR_WIDTH = $clog2(N_COEFF);
    parameter BUTTERFLY_DELAY = 0;
    reg CLK = 0;
    wire [ADDR_WIDTH-1:0] readIndex0;
    wire [ADDR_WIDTH-1:0] readIndex1;
    wire [ADDR_WIDTH-1:0] writeIndex0;
    wire [ADDR_WIDTH-1:0] writeIndex1; 
    wire writeEn0;
    wire writeEn1;
    wire [ADDR_WIDTH-2:0] phi_index;
        

    
    addressGenerator #(N_COEFF,ADDR_WIDTH,BUTTERFLY_DELAY) uut(
    .CLK(CLK),
    .readIndex0(readIndex0),
    .readIndex1(readIndex1),
    .writeIndex0(writeIndex0),
    .writeIndex1(writeIndex1),
    .writeEn0(writeEn0),
    .writeEn1(writeEn1),
    .phi_index(phi_index)
    );
    

initial begin
    for (integer i = 0; i < 1024; i = i+1) begin
        #1
        CLK <= ~CLK;
        #1
        CLK <= ~CLK;
    end
end
endmodule
