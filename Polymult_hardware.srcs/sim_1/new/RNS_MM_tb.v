`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/18/2020 08:06:25 PM
// Design Name: 
// Module Name: RNS_MM_tb
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


module RNS_MM_tb(

    );
    
    
    parameter N_TESTS  = 2000;
    
    parameter CH_BW        = 32;                                 //RNS channel bitwidth
    parameter N_CHANNELS   = 4;                                  //Number of RNS channels
    parameter RNS_BW = CH_BW * N_CHANNELS;                       //total RNS buswidth
    parameter EXT_BW = CH_BW * (N_CHANNELS+1);                     //total RNS_EXT buswidth + m_r
    parameter RNS     = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197};  //RNS channels
    parameter RNS_EXT = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087};  //extended RNS channels with redundant channel 
    parameter TOTAL_RNS = {RNS,RNS_EXT}; //{32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197,32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087};
    parameter TOTAL_CH = N_CHANNELS*2 + 1;
    parameter TOTAL_BW = RNS_BW + EXT_BW;
    
    reg CLK = 0;
    reg [TOTAL_BW-1:0] A,B;
    wire [TOTAL_BW-1:0] Z;
    
    
 RNS_modmult #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW , RNS, RNS_EXT, TOTAL_RNS, TOTAL_CH, TOTAL_BW) MODMULT (CLK, A, B, Z); 
    
    initial begin
    for (integer i = 0; i < N_TESTS; i = i+1) begin
        #1
        CLK <= ~CLK;
        #1
        CLK <= ~CLK;
        A <= {i,i,i,i,i,i,i,i,i}; //$random % TOTAL_BW;  //random
        B <= {i,i,i,i,i,i,i,i,i};  //random
    end
end 
endmodule
