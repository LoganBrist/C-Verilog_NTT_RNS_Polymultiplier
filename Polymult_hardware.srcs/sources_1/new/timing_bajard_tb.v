`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/16/2020 01:42:21 AM
// Design Name: 
// Module Name: timing_bajard_tb
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


module timing_bajard_tb#(
        parameter CH_BW      = 32,
        parameter N_CHANNELS = 4,
        parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
        parameter EXT_BW     = CH_BW * (N_CHANNELS + 1),                     //total RNS_EXT buswidth + m_r
        parameter RNS        = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},
        parameter RNS_EXT    = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087}
    )(
    input wire [RNS_BW-1:0] A,
    output reg [EXT_BW-1:0] Z_reg,
    input wire CLK
    );
    
    reg  [RNS_BW-1:0] A_reg;
    wire [EXT_BW-1:0] Z;
    
    
    always @ (posedge CLK) begin
        A_reg  <= A;
        Z_reg  <= Z;
    end
        

    // instantiate RNS module    
    BASE_EXTENSION_BAJARD #(CH_BW, N_CHANNELS, RNS_BW, EXT_BW, RNS, RNS_EXT)  bex (
        .A(A_reg),
        .Z(Z),
        .CLK(CLK)
     );
     
endmodule
