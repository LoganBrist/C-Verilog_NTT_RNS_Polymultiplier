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


module memory_tb(

    );
     parameter integer BIT_WIDTH    = 3;
     parameter integer POLY_LENGTH  = 8; 
     parameter integer ADDR_WIDTH   = $clog2(POLY_LENGTH);
     parameter FILENAME = "A.txt";  
              
    reg          CLK        = 0;
    reg       DOUBLE_CLK    = 0;
    
 //4 port input


 wire [ADDR_WIDTH-1:0] mem_readAddr1_in, mem_readAddr2_in;
  wire [BIT_WIDTH-1:0]  mem_Data1_out, mem_Data2_out;
 wire [ADDR_WIDTH-1:0]  mem_writeAddr1_in, mem_writeAddr2_in;
 wire [BIT_WIDTH-1:0] mem_writeData1_in, mem_writeData2_in;
 wire  mem_writeEn1_in, mem_writeEn2_in;
 
assign mem_writeData1_in = mem_Data1_out + 1;
assign mem_writeData2_in = mem_Data2_out + 1;

 //memory connections
 wire [ADDR_WIDTH-1:0] mem_readAddrIn, mem_writeAddrIn;
 wire [BIT_WIDTH-1:0]  mem_DataIn, mem_DataOut;
 wire mem_writeEn; 
 
  wire [BIT_WIDTH-1:0] ag_PhiAddrOut;
 
 //address generator
 addressGenerator #(POLY_LENGTH) 
 ag (CLK,  mem_readAddr1_in,  mem_readAddr2_in, mem_writeAddr1_in, mem_writeAddr2_in, mem_writeEn1_in, mem_writeEn2_in, ag_PhiAddrOut); 

//memory multiplexers
  mem_4to2_input #(BIT_WIDTH, ADDR_WIDTH) mem_in 
   (mem_readAddr1_in, mem_readAddr2_in, mem_writeAddr1_in, mem_writeAddr2_in, 
    mem_writeData1_in, mem_writeData2_in, mem_writeEn1_in, mem_writeEn2_in,
    mem_readAddrIn, mem_writeAddrIn, mem_DataIn, mem_writeEn,
    CLK, DOUBLE_CLK
    );
    
  memory #(BIT_WIDTH,POLY_LENGTH,FILENAME, ADDR_WIDTH) mem
  (mem_readAddrIn, mem_writeAddrIn, mem_DataIn, mem_writeEn, mem_DataOut, DOUBLE_CLK
  );
    
  mem_4to2_output #(BIT_WIDTH,ADDR_WIDTH) mem_out
   ( mem_DataOut, mem_Data1_out, mem_Data2_out, CLK, DOUBLE_CLK
    ); 
    
    
    initial begin
        for (integer i = 0; i < 20; i = i + 1) begin
            #1
            CLK <= ~ CLK;
            DOUBLE_CLK <= ~DOUBLE_CLK;      
            #1
            DOUBLE_CLK <= ~DOUBLE_CLK;

    end
 end    

endmodule
