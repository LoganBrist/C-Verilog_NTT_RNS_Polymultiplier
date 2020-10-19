`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/16/2020 01:56:53 AM
// Design Name: 
// Module Name: timing_shenoy_tb
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


module timing_shenoy_tb#(
        parameter CH_BW      = 32,
        parameter N_CHANNELS = 4,
        parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
        parameter EXT_BW     = CH_BW * (N_CHANNELS + 1),                     //total RNS_EXT buswidth + m_r
        parameter RNS        = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},
        parameter RNS_EXT    = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},
        
        /*
        parameter [EXT_BW-1:0] D2_J_INV_RED_J_LIST =  'h0000000100000001000000010000000100000001,
        parameter [EXT_BW-1:0] D2_J_INV_RED_R_LIST =  'h0000000100000001000000010000000100000001,
        parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h00000001,
        parameter [EXT_BW*N_CHANNELS-1:0] D2_J0_RED_I_LIST = {'h0000000100000001000000010000000100000001, 'h0000000100000001000000010000000100000001, 'h0000000100000001000000010000000100000001, 'h0000000100000001000000010000000100000001},
        parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h00000001000000010000000100000001
        */
        
        parameter [EXT_BW-1:0] D2_J_INV_RED_J_LIST =  'hC9C4644B_EB441A2C_8C96E0C1_BE609F1A_58CC8CA2,
        parameter [EXT_BW-1:0] D2_J_INV_RED_R_LIST =  'h00018480_00021780_0002C3A0_00067320_000CD2C0,
        parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h6C7E999A,
        parameter D2_J0_RED_I_LIST = {'hFFCB27DB_FFD6899B_FFDB948B_FFE20E0B_FFE6562B, 'hFFD6DC6F_FFE09F6F_FFE4C68F_FFE9F60F_FFED85AF, 'hFFF4B83F_FFF93B3F_FFFA9D9F_FFFC0D1F_FFFDC8BF, 'hFFFD728D_FFFF6E7D_FFFF9EDD_FFFFC2DD_000138C0},
        parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h150E1CC0_0E767B00_01D9BB00_00146880
        
    )(
    input wire [EXT_BW-1:0] A,
    output reg [RNS_BW-1:0] Z_reg,
    input wire CLK
    );
    
    reg  [EXT_BW-1:0] A_reg;
    wire [RNS_BW-1:0] Z;
    
    always @ (posedge CLK) begin
        A_reg  <= A;
        Z_reg  <= Z;
    end
        
    // instantiate RNS module    
    BASE_EXTENSION_SHENOY #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT,D2_J_INV_RED_J_LIST,D2_J_INV_RED_R_LIST,D2_INV_RED_R_LIST,D2_J0_RED_I_LIST,D2_RED_I_LIST)  bex (
        .A(A_reg),
        .Z(Z),
        .CLK(CLK)
     );
     
endmodule