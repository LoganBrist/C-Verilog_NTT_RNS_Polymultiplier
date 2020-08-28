`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/10/2020 03:03:14 PM
// Design Name: 
// Module Name: NTT_RNS_top
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


module top_NTT_RNS
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
input wire [RNS_BW-1:0] data_in,
output wire [RNS_BW-1:0] data_out
);
    
    wire [RNS_BW-1:0] A1,A2,A3,B1,B2,B3,Y1,Y2,Y3,Z1,Z2,Z3;
    assign B1 = data_in;  
   ///////////  
    // FIFO1 outputs
    wire 	[255+1:0]	data_count1;
    wire 	[127+1:0]	data_count2;
    wire 	[63+1:0]	data_count3;
	wire  empty1, full1, almst_empty1, almst_full1, err1, empty2, full2, almst_empty2, almst_full2, err2,
	      empty3, full3, almst_empty3, almst_full3, err3;
	      
	//FIFO1 inputs
	reg n_reset = 0; //always 1 (at CLK)
	reg wr_en   = 0;   //always 1 (at CLK)
	reg rd_en = 0;   //switches to 1 at negedge of bypass
	////////////////
	 
    FIFO #(256, RNS_BW) fifo1 (A1, data_count1, empty1, full1, almst_empty1, almst_full1, err1, Y1, wr_en, rd_en, n_reset, CLK);  
    butterfly_RNS #(CH_BW,N_CHANNELS,RNS_BW,EXT_BW,RNS,RNS_EXT) bf1 (CLK,A1,B1,Y1,Z1);
    assign B2 = Z1;

    /*
    FIFO #(128, RNS_BW) fifo2 (A2, data_count2,empty2, full2, almst_empty2, almst_full2, err2, Y2, wr_en, rd_en, n_reset, CLK);  
    butterfly_RNS #(CH_BW,N_CHANNELS,RNS_BW,EXT_BW,RNS,RNS_EXT) bf2 (CLK,A2,B2,Y2,Z2);
    assign B3 = Z2;
    
    FIFO #(64, RNS_BW) fifo3 (A3, data_count3,empty3, full3, almst_empty3, almst_full3, err3, Y3, wr_en, rd_en, n_reset, CLK);  
    butterfly_RNS #(CH_BW,N_CHANNELS,RNS_BW,EXT_BW,RNS,RNS_EXT) bf3 (CLK,A3,B3,Y3,Z3);
    
    assign data_out = Z3;
    */
     
    assign data_out = Z1;

endmodule
