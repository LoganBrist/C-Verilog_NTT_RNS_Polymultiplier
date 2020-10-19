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
        
        
        //parameter [EXT_BW-1:0] D2_J_INV_RED_J_LIST =  'h0000000100000001000000010000000100000001;
        //parameter [EXT_BW-1:0] D2_J_INV_RED_R_LIST =  'h0000000100000001000000010000000100000001;
        //parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h00000001;
        //parameter [EXT_BW*N_CHANNELS-1:0] D2_J0_RED_I_LIST = {'h00000001_00000001_00000001_00000001_00000001, 'h00000001_00000001_00000001_00000001_00000001, 'h00000001_00000001_00000001_00000001_00000001, 'h00000001_00000001_00000001_00000001_00000001};
        //parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h00000001000000010000000100000001;
        
        
        parameter [RNS_BW-1:0] D2_J_INV_RED_J_LIST =  'hC9C4644B_EB441A2C_8C96E0C1_BE609F1A;
        parameter [RNS_BW-1:0] D2_J_RED_R_LIST =  'h00018480_00021780_0002C3A0_00067320;
        parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h6C7E999A;
        parameter D2_J_RED_I_LIST = {'hFFCB27DB_FFD6899B_FFDB948B_FFE20E0B, 'hFFD6DC6F_FFE09F6F_FFE4C68F_FFE9F60F, 'hFFF4B83F_FFF93B3F_FFFA9D9F_FFFC0D1F, 'hFFFD728D_FFFF6E7D_FFFF9EDD_FFFFC2DD};
        parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h150E1CC0_0E767B00_01D9BB00_00146880;
        
        //RESULTS ARE SAME AS C++. EXCEPT SOME PARAMETER (LIKELY D2_J_RED_I) IS NOT BEING SAVED IN TEXT CORRECTLY. COME BACK TO THIS LATER BUT SYNTHESIS SHOULD BE FINE.
        
        // Inputs and outputs
        reg CLK = 0;
        reg [EXT_BW-1:0] A = {32'd0,32'd0,32'd0,32'd0,32'd0};
        wire [RNS_BW-1:0] Z;
        
        
    BASE_EXTENSION_SHENOY #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT,D2_J_INV_RED_J_LIST,D2_J_RED_R_LIST,D2_INV_RED_R_LIST,D2_J_RED_I_LIST,D2_RED_I_LIST)  bex (
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

