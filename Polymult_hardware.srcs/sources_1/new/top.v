`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/29/2020 04:31:07 PM
// Design Name: 
// Module Name: top
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



module top #(
    parameter integer W_N            = 2,
    parameter integer MOD            = 673, //12289,
    parameter integer POLY_LENGTH    = 8,
    parameter integer BIT_WIDTH      = $clog2(MOD), //64,
    parameter integer ADDR_WIDTH      = $clog2(POLY_LENGTH),
    parameter A_FILENAME = "A.txt",
    parameter PHI_FILENAME = "phi.txt",
    parameter integer N_NTT_MODULES = 1
    )
    (
    input wire CLK,
    input wire  DOUBLE_CLK, 
    output wire mem_writeEN,
    //output wire [ADDR_WIDTH-1:0] gen_addr1, gen_addr2,
    output wire [ADDR_WIDTH-1:0] /*mem_readAddr,*/ mem_writeAddr,
    output wire [BIT_WIDTH-1:0] /*mem_dataOut,*/ mem_dataIn,
    output wire [BIT_WIDTH-1:0] bf_data_in1, bf_data_in2, phi,
    output wire [BIT_WIDTH-1:0] bf_data_out1, bf_data_out2,
    output wire [ADDR_WIDTH-1:0] save_addr1, save_addr2
    );  
    // top module output to testbench
/*assign gen_addr1 = ag_AddrOut;
assign gen_addr2 = ag_AddrOut2;
*/
assign mem_writeEN = mem_writeEn;
assign bf_data_in1  = bf_inA;
assign  bf_data_in2 = bf_inB;
assign phi = phitable_out;
assign save_addr1 = ag_WriteAddrOut;//mem_writeAddr1_in;
assign save_addr2 =  ag_WriteAddrOut2; //mem_writeAddr2_in;
assign  bf_data_out1 = bf_outA;
assign  bf_data_out2 = bf_outB;    
   /*assign mem_readAddr = mem_readAddrIn;*/
   assign mem_writeAddr = mem_writeAddrIn;
   /*assign mem_dataOut  = mem_DataOut;*/
   assign mem_dataIn   = mem_DataIn;
   
 //NTT index generator  
 wire [ADDR_WIDTH-1:0] ag_AddrOut, ag_AddrOut2, ag_WriteAddrOut, ag_WriteAddrOut2;
 wire [ADDR_WIDTH-2:0] ag_PhiAddrOut;
 wire ag_writeEn, ag_writeEn2;
 
 addressGenerator #(POLY_LENGTH) 
 ag (CLK, ag_AddrOut, ag_AddrOut2, ag_WriteAddrOut, ag_WriteAddrOut2, ag_writeEn, ag_writeEn2, ag_PhiAddrOut); 

//twiddle factors
 wire [ADDR_WIDTH-2:0] phitable_in;
 wire [BIT_WIDTH-1:0]  phitable_out;
  
  phi_table #(BIT_WIDTH, POLY_LENGTH / 2, ADDR_WIDTH - 1, PHI_FILENAME, W_N, MOD) 
  p(phitable_in, phitable_out);
 
 //NTT butterflies   
 wire [BIT_WIDTH-1:0] bf_inA, bf_inB, bf_outA, bf_outB, bf_inPHI; 
  
 butterfly #(BIT_WIDTH,MOD)
 bf (CLK, bf_inA, bf_inB, bf_outA, bf_outB, bf_inPHI); 
              
 //half clock speed for memory
 wire [ADDR_WIDTH-1:0] mem_readAddrIn, mem_writeAddrIn, mem_readAddr1_in, mem_readAddr2_in, mem_writeAddr1_in, mem_writeAddr2_in;
 wire [BIT_WIDTH-1:0] mem_DataIn, mem_DataOut, mem_Data1_out, mem_Data2_out, mem_writeData1_in, mem_writeData2_in;
 wire mem_writeEn, mem_writeEn1_in, mem_writeEn2_in;
 
 //memory multiplexers
  mem_4to2_input #(BIT_WIDTH, ADDR_WIDTH) mem_in 
   (mem_readAddr1_in, mem_readAddr2_in, mem_writeAddr1_in, mem_writeAddr2_in, 
    mem_writeData1_in, mem_writeData2_in, mem_writeEn1_in, mem_writeEn2_in,
    mem_readAddrIn, mem_writeAddrIn, mem_DataIn, mem_writeEn,
    CLK, DOUBLE_CLK
    );
    
  memory #(BIT_WIDTH,POLY_LENGTH,A_FILENAME, ADDR_WIDTH) mem
  (mem_readAddrIn, mem_writeAddrIn, mem_DataIn, mem_writeEn, mem_DataOut, DOUBLE_CLK
  );
    
  mem_4to2_output #(BIT_WIDTH,ADDR_WIDTH) mem_out
   ( mem_DataOut, mem_Data1_out, mem_Data2_out, CLK, DOUBLE_CLK
    ); 
//////////////////////////////////////////////////////////////////////////////////////////
// Connections 
//////////////////////////////////////////////////////////////////////////////////////////
//phi table I/O 
assign phitable_in  = ag_PhiAddrOut;
assign bf_inPHI     = phitable_out;  

//connect memory output to butterfly
assign bf_inA = mem_Data1_out;
assign bf_inB = mem_Data2_out; 

//multiplex address generator and butterfly into memory 
assign mem_writeEn1_in   = ag_writeEn;
assign mem_writeEn2_in   = ag_writeEn2;
assign mem_readAddr1_in  = ag_AddrOut;
assign mem_readAddr2_in  = ag_AddrOut2;
assign mem_writeAddr1_in = ag_WriteAddrOut;
assign mem_writeAddr2_in = ag_WriteAddrOut2;
assign mem_writeData1_in = bf_outA;
assign mem_writeData2_in = bf_outB;

endmodule

  
// Multi port memory and/or scheduling is difficult! For now, get an example running at half speed
// by splitting butterfly read and write into two cycles. 
