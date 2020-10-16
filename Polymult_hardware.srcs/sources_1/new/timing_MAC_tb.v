`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2020 11:38:34 PM
// Design Name: 
// Module Name: timing_MAC_tb
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


module timing_MAC_tb#(
    parameter integer CH_BW        = 32,                                                                    //RNS channel bitwidth
    parameter integer N_CHANNELS   = 5,                                                                     //Number of RNS channels, stay constant for test
    parameter RNS_BW               = CH_BW * N_CHANNELS,                                                    //total RNS buswidth
    parameter RNS_MOD              = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197,32'd4294967189}// 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087}     //RNS channels
    )(
    input  wire  [RNS_BW-1:0] A,
    input  wire  [RNS_BW-1:0] B,
    output reg   [RNS_BW-1:0] Z_reg,
    input wire CLK
    );
    
    reg [RNS_BW-1:0] A_reg;
    reg [RNS_BW-1:0] B_reg;
    wire [RNS_BW-1:0] Z;
    
    
    always @ (posedge CLK) begin
        A_reg <= A;
        B_reg <= B;
        Z_reg  <= Z;
    end
        

    // instantiate RNS module    
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS_MOD)  mac (
      .A(A_reg),
      .B(B_reg),
      .Z(Z)
     );
     
 endmodule
