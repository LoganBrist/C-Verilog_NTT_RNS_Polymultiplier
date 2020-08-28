`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/04/2020 10:55:48 AM
// Design Name: 
// Module Name: memory_tb
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


module memory_4port_tb(

    );
    
     parameter integer bW = 3; 
     parameter integer eC = 8;
     parameter INITIALIZATION_FILENAME = "A.txt";
     parameter integer aW=$clog2(eC);
     
    reg          CLK = 0;
    wire [aW-1:0] readAddr;
    wire [aW-1:0] readAddr2;
    wire [bW-1:0] readData;
    wire [bW-1:0] readData2;
     
    wire [aW-1:0] writeAddr;
    wire [aW-1:0] writeAddr2;
    wire [bW-1:0] writeData;
    wire [bW-1:0] writeData2;
    wire          writeEn;  
    wire          writeEn2;
    
    
    
    memory_4port #(bW,eC,INITIALIZATION_FILENAME,aW) uut(
    .readAddr(readAddr),
    .writeAddr(writeAddr),
    .writeData(writeData),
    .writeEn(writeEn),
    .readData(readData),
    .CLK(CLK),
    .readAddr2(readAddr2),
    .writeAddr2(writeAddr2),
    .writeData2(writeData2),
    .writeEn2(writeEn2),
    .readData2(readData2)
    );
    
    wire [aW-2:0] phi_index;
    addressGenerator #(eC, aW, 3) ad(CLK, readAddr, readAddr2, writeAddr, writeAddr2, writeEn,writeEn2,phi_index); 
     
    initial begin
    for (integer i = 0; i < 1024; i = i+1) begin
        #1
        CLK <= ~CLK;
        #1
        CLK <= ~CLK;
        //readAddr <= i;
        //readAddr2 <= i + 1;
    end
end
endmodule
