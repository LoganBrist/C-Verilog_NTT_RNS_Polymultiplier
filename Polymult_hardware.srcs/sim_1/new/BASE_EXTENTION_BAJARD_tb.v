`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/15/2020 03:46:44 PM
// Design Name: 
// Module Name: BASE_EXTENTION_BAJARD_tb
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


module BASE_EXTENTION_BAJARD_tb(

    );
    
    parameter N_TESTS = 1000;
    
    parameter CH_BW      = 32;
    parameter N_CHANNELS = 4;
    parameter RNS_BW     = CH_BW * N_CHANNELS;                 //total RNS buswidth 
    parameter EXT_BW     = CH_BW * (N_CHANNELS + 1);                     //total RNS_EXT buswidth + m_r
    parameter RNS        = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197};
    parameter RNS_EXT    = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087};
    
    //For actual extension
    parameter [RNS_BW*(N_CHANNELS+1)-1:0] D1_I_RED_J_LIST     =  {'h00007620000085E000011EE00005E218,'h000489900004FFB000086D3000106288,'h0009DC80000ABB40001095C0001B0700,'h001A7480001C58400027AEC000375F00,'h002E680000314E800041BE0000561000};
    parameter D1_I_INV_RED_I_LIST = 'hC91E9F844C3BA9F47DFD52886CA863AD;
    /*
    // For unit contants test. (With inputs == 1, sigma = A and MAC result = sigma + sigma +... = (N_CHANNEL * A)
    parameter [RNS_BW*(N_CHANNELS+1)-1:0] D1_I_RED_J_LIST     = {'h00000001000000010000000100000001,'h00000001000000010000000100000001,'h00000001000000010000000100000001,'h00000001000000010000000100000001,'h00000001000000010000000100000001};
    parameter D1_I_INV_RED_I_LIST = 'h00000001000000010000000100000001;
    */
    
    
    reg CLK = 0;
    //reg [RNS_BW-1:0] A = {32'd0,32'd0,32'd0,32'd0};
    reg [RNS_BW-1:0] A = {32'hFFFFFE63,32'hFFFFFE6F,32'hFFFFFE9F,32'hFFFFFEC1};
    wire [EXT_BW-1:0] Z;
    
    
    BASE_EXTENSION_BAJARD #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT,D1_I_RED_J_LIST,D1_I_INV_RED_I_LIST) uut (
    .A(A),
    .Z(Z),
    .CLK(CLK)
    );
    
    initial begin
      for (integer i = 0; i < N_TESTS; i = i+1) begin
          #1
          CLK <= ~CLK;
          #1
          CLK <= ~CLK; 
          A <= A + {1,1,1,1}; //{i,i,i,i}; 
       end
    end
endmodule
