`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/01/2020 10:34:00 PM
// Design Name: 
// Module Name: top_tb
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


module top_tb;
    parameter integer W_N             = 609;
    parameter integer  MOD            = 673;
    parameter  integer POLY_LENGTH    = 8;
    parameter  integer BIT_WIDTH      = $ceil($clog2(MOD)); //5
    parameter  integer ADDR_WIDTH      = $clog2(POLY_LENGTH); //3
    parameter A_FILENAME = "A.txt";
    parameter PHI_FILENAME = "phi.txt";
    parameter integer N_NTT_MODULES = 1;
    
    reg CLK = 0;
    reg DOUBLE_CLK = 0;
    /*wire [ADDR_WIDTH-1:0] gen_addr1, gen_addr2; */
    wire [ADDR_WIDTH-1:0] /*mem_readAddr,*/ mem_writeAddr;
    wire mem_writeEN;
    wire [BIT_WIDTH-1:0]  /*mem_dataOut,*/ mem_dataIn, bf_data_out1, bf_data_out2 , bf_data_in1, bf_data_in2, phi;   
    wire [ADDR_WIDTH-1:0] save_addr1, save_addr2; 
    reg [BIT_WIDTH-1:0] NTTRESULT [0:POLY_LENGTH-1];
    
    top     #(W_N, MOD,POLY_LENGTH, BIT_WIDTH, ADDR_WIDTH, A_FILENAME, PHI_FILENAME, N_NTT_MODULES) 
    uut      (.CLK(CLK),                   .DOUBLE_CLK(CLK), .mem_writeEN(mem_writeEN),
              /*.gen_addr1(gen_addr1),       .gen_addr2(gen_addr2),*/ 
             /* .mem_readAddr(mem_readAddr),*/ .mem_writeAddr(mem_writeAddr),
             /* .mem_dataOut(mem_dataOut),*/ .mem_dataIn(mem_dataIn),
              .bf_data_in1(bf_data_in1),   .bf_data_in2(bf_data_in2), .phi(phi),
              .bf_data_out1(bf_data_out1), .bf_data_out2(bf_data_out2), 
              .save_addr1(save_addr1),     .save_addr2(save_addr2));
    
    initial begin
       
        for (integer i = 0; i < 1024; i = i+1) begin
            #1
            CLK <= ~ CLK;
            DOUBLE_CLK <= ~DOUBLE_CLK;  
            NTTRESULT[mem_writeAddr] <= mem_dataIn;          
            #1
            DOUBLE_CLK <= ~DOUBLE_CLK;

    end
    end


endmodule

//current problem: every other data is being saved in memory. NTTRESULT 

//Input vector needs saved in bit reversed order!