`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/10/2020 10:59:27 AM
// Design Name: 
// Module Name: butterfly_RNS
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


module butterfly_RNS
#(
parameter CH_BW        = 32,                           //RNS channel bitwidth
parameter N_CHANNELS   = 4,                            //Number of RNS channels
parameter RNS_BW = CH_BW * N_CHANNELS,                 //total RNS buswidth
parameter EXT_BW = CH_BW * (N_CHANNELS+1),                     //total RNS_EXT buswidth + m_r
parameter RNS     = {32'd4294967291, 32'd4294967279, 
                     32'd4294967231, 32'd4294967197},  //RNS channels
parameter RNS_EXT = {32'd4294967189, 32'd4294967161,
                     32'd4294967143, 32'd4294967111, 
                                     32'd4294967087}  //extended RNS channels with redundant channel 
)
(
input wire CLK,
input  wire [RNS_BW-1:0]  A,
input  wire [RNS_BW-1:0]  B,
output wire [RNS_BW-1:0]  Y,
output wire [RNS_BW-1:0]  Z
    );

   reg [RNS_BW-1:0] phitable [4:0]; //this is the phi table, specific per stage
   reg [7:0] phi_idx [5:0];    //this is repeated address order for the phi table
   
   initial begin 
       phitable[0] = 2;
       phitable[1] = 4;
       phitable[2] = 6;
       phitable[3] = 8;
       
       phi_idx[0] = 0;
       phi_idx[1] = 4;
       phi_idx[2] = 3;
   end
   
   //assign phi
    //Phi is stored in a ROM. Make a list of the binary addr vals in order of their call and hopefully there is
    //a pattern with this serial butterflies. Each stage alternates between several set addresses. Just have those saved locally as well
     assign phi = phitable[0];
     
   wire [RNS_BW-1:0] mult_result;
   
   //butterfly mod multiply by phi
   RNS_modmult #(CH_BW,N_CHANNELS,RNS_BW,EXT_BW,RNS,RNS_EXT) mm (CLK,B, phi, mult_result);

  //buterfly add and subtract
  RNS_ADD #(CH_BW,N_CHANNELS,RNS_BW,RNS) ra(A,mult_result,Y);
  RNS_SUB #(CH_BW,N_CHANNELS,RNS_BW,RNS) rs(A,mult_result,Z);


endmodule
