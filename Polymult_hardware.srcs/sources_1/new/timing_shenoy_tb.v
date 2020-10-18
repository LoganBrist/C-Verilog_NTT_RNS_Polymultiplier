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
        
        parameter [EXT_BW-1:0] D2_J_INV_RED_J_LIST =  'h0000000100000001000000010000000100000001,
        parameter [EXT_BW-1:0] D2_J_INV_RED_R_LIST =  'h0000000100000001000000010000000100000001,
        parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h00000001,
        parameter [RNS_BW*N_CHANNELS-1:0] D2_J0_RED_I_LIST = {'h00000001000000010000000100000001, 'h00000001000000010000000100000001, 'h00000001000000010000000100000001, 'h00000001000000010000000100000001},
        parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h00000001000000010000000100000001
        /*
        parameter [EXT_BW-1:0] D2_J_INV_RED_J_LIST =  'hC9C4644BEB441A2C8C96E0C1BE609F1A58CC8CA2,
        parameter [EXT_BW-1:0] D2_J_INV_RED_R_LIST =  'h00018480000217800002C3A000067320000CD2C0,
        parameter [CH_BW-1:0]  D2_INV_RED_R_LIST   =  'h6C7E999A,
        parameter [RNS_BW*N_CHANNELS-1:0] D2_J0_RED_I_LIST = {'hFFCB27DBFFD6DC6FFFF4B83FFFFD728D, 'hFFD6899BFFE09F6FFFF93B3FFFFF6E7D, 'hFFDB948BFFE4C68FFFFA9D9FFFFF9EDD, 'hFFE20E0BFFE9F60FFFFC0D1FFFFFC2DD},
        parameter [RNS_BW-1:0] D2_RED_I_LIST = 'h150E1CC00E767B0001D9BB0000146880
        */
    )(
    input wire [EXT_BW-1:0] A,
    output reg [RNS_BW-1:0] Z_reg,
    input wire CLK
    );
    
    reg  [RNS_BW-1:0] A_reg;
    wire [EXT_BW-1:0] Z;
    
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