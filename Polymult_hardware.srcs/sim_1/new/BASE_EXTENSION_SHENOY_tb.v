`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/17/2020 11:17:59 PM
// Design Name: 
// Module Name: BASE_EXTENSION_SHENOY_tb
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


module BASE_EXTENSION_SHENOY_tb(
);

        parameter N_TESTS = 1000;
        
        parameter CH_BW      = 32;
        parameter N_CHANNELS = 4;
        parameter RNS_BW     = CH_BW * N_CHANNELS;                 //total RNS buswidth 
        parameter EXT_BW     = CH_BW * (N_CHANNELS + 1);                     //total RNS_EXT buswidth + m_r
        parameter RNS        = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197};
        parameter RNS_EXT    = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087};
        
        /*
        parameter [EXT_BW-1:0] D2_J_INV_RED_J_LIST =  'h0000000100000001000000010000000100000001;
        parameter [EXT_BW-1:0] D2_J_INV_RED_R_LIST =  'h0000000100000001000000010000000100000001;
        parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h00000001;
        parameter [EXT_BW*N_CHANNELS-1:0] D2_J0_RED_I_LIST = {'h0000000100000001000000010000000100000001, 'h0000000200000002000000020000000200000002, 'h0000000200000002000000020000000200000002, 'h0000000300000003000000030000000300000003};
        parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h00000001000000010000000100000001;
        */
        
        parameter [EXT_BW-1:0] D2_J_INV_RED_J_LIST =  'hC9C4644BEB441A2C8C96E0C1BE609F1A58CC8CA2;
        parameter [EXT_BW-1:0] D2_J_INV_RED_R_LIST =  'h00018480000217800002C3A000067320000CD2C0;
        parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h6C7E999A;
        parameter D2_J0_RED_I_LIST = {'h00007620000085E000011EE00005E218, 'h000489900004FFB000086D3000106288, 'h0009DC80000ABB40001095C0001B0700, 'h001A7480001C58400027AEC000375F00, 'h002E680000314E800041BE0000561000};
        parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h150E1CC00E767B0001D9BB0000146880;
        
        //RESULTS ARE SAME AS C++. EXCEPT SOME PARAMETER (LIKELY D2_J_RED_I) IS NOT BEING SAVED IN TEXT CORRECTLY. COME BACK TO THIS LATER BUT SYNTHESIS SHOULD BE FINE.
        
        // Inputs and outputs
        reg CLK = 0;
        reg [EXT_BW-1:0] A = {32'd0,32'd0,32'd0,32'd0,32'd0};
        wire [RNS_BW-1:0] Z;
        
        
    BASE_EXTENSION_SHENOY #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT,D2_J_INV_RED_J_LIST,D2_J_INV_RED_R_LIST,D2_INV_RED_R_LIST,D2_J0_RED_I_LIST,D2_RED_I_LIST)  bex (
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
              A <= {i,i,i,i,i}; 
           end
        end
    endmodule        

