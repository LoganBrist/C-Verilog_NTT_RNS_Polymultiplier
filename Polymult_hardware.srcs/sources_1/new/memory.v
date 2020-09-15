`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/31/2020 07:32:49 AM
// Design Name: 
// Module Name: memory
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

// This has two reads and two write per cycle. Needs to read and write two for the butterfly
module memory
  #(
     parameter integer BIT_WIDTH = 3, 
     parameter integer SIZE = 1024,
     parameter INITIALIZATION_FILENAME = "",
     parameter integer ADDR_WIDTH = $clog2(SIZE)
     )
   
    (
    input  wire [ADDR_WIDTH-1:0] readAddr,
    input  wire [ADDR_WIDTH-1:0] writeAddr,
    input  wire [BIT_WIDTH-1:0] writeData,
    input  wire                 writeEn,
    output wire [BIT_WIDTH-1:0] readData,
    input  wire          CLK
    );
   

   reg [BIT_WIDTH-1:0]         mem[SIZE-1:0];

   //load given filename
   initial begin 
    if(INITIALIZATION_FILENAME != "")
     $readmemh(INITIALIZATION_FILENAME, mem);
   end
     
   assign readData  = mem[readAddr];

   always @(posedge CLK) begin
     if(writeEn)
       mem[writeAddr]  <= writeData;
    end
    
    
    //the read write addresses have unequal parity, make two block memories saving each
endmodule
