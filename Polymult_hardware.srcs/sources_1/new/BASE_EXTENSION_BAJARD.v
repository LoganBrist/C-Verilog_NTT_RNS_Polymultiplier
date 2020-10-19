`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/06/2020 05:11:40 PM
// Design Name: 
// Module Name: BASE_EXTENSION_BAJARD
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

//TOTAL DELAY: 2
module BASE_EXTENSION_BAJARD
    #(
    parameter CH_BW      = 32,
    parameter N_CHANNELS = 4,
    parameter RNS_BW     = CH_BW * N_CHANNELS,                 //total RNS buswidth 
    parameter EXT_BW     = CH_BW * (N_CHANNELS + 1),                     //total RNS_EXT buswidth + m_r
    parameter RNS        = {32'd4294967291, 32'd4294967279, 32'd4294967231, 32'd4294967197},
    parameter RNS_EXT    = {32'd4294967189, 32'd4294967161, 32'd4294967143, 32'd4294967111, 32'd4294967087},
    parameter [RNS_BW*(N_CHANNELS+1)-1:0] D1_I_RED_J_LIST     = {'h00007620000085E000011EE00005E218,'h000489900004FFB000086D3000106288,'h0009DC80000ABB40001095C0001B0700,'h001A7480001C58400027AEC000375F00,'h002E680000314E800041BE0000561000},
    parameter D1_I_INV_RED_I_LIST = 'hC91E9F844C3BA9F47DFD52886CA863AD
    )
    (
    input wire [RNS_BW-1:0] A,
    output wire[EXT_BW-1:0] Z,
    input wire CLK
    );
    
   //TOTAL DELAY: 4
   //IO delay 
   wire [RNS_BW-1:0] A_ff;
   delay #(1, RNS_BW) d0 (CLK, A, A_ff); 

   
   // Constant memory
   reg [RNS_BW-1:0] D1_I_INV_RED_I [0:0];
   reg [RNS_BW-1:0] D1_I_RED_J [0:N_CHANNELS+1]; 
   
  integer j; 
  initial begin
    // for j outbound channels load data
    for(j = 0; j < N_CHANNELS + 1; j = j + 1) begin
        D1_I_RED_J[j] = D1_I_RED_J_LIST[RNS_BW*j +: RNS_BW];
    end
    
    //Assign other parameter 
    D1_I_INV_RED_I [0] = D1_I_INV_RED_I_LIST; 
  end 
  
  //Step 1 - Calculate sigma (1 cycles)
  wire [RNS_BW-1:0] sigma;
  RNS_MULT #(CH_BW, N_CHANNELS, RNS_BW, RNS) m(A_ff, D1_I_INV_RED_I [0], sigma); 
  
  
  
  //step 1b. Delay
// wire [RNS_BW-1:0] sigma_ff;
//delay #(1, RNS_BW) d0 (CLK, sigma, sigma_ff); 
  
  //Step 2 - Accumulate t (i additions, j channels) has 1 delay
  wire [EXT_BW-1:0] out;
  
  genvar j_ch;
  generate 
        // MAC in parallel for each j  
        for (j_ch = 0; j_ch < N_CHANNELS + 1; j_ch = j_ch + 1) begin
              RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS_EXT[CH_BW*j_ch +: CH_BW]) mac0(sigma, D1_I_RED_J[j_ch], out[CH_BW*j_ch +: CH_BW],CLK);      
        end      
  endgenerate 

  assign Z = out;
      
endmodule   

 /*  
 // NON PARAMETERIZED VERSION     
    //Precomputed tables 
    reg [RNS_BW-1:0] D1_I_INV_RED_I  = 'hC91E9F844C3BA9F47DFD52886CA863AD;
     
    reg [RNS_BW-1:0] D1_I_RED_J0     = 'h00007620000489900009DC80001A7480;
    reg [RNS_BW-1:0] D1_I_RED_J1     = 'h002E6800000085E00004FFB0000ABB40;
    reg [RNS_BW-1:0] D1_I_RED_J2     = 'h001C584000314E8000011EE000086D30;
    reg [RNS_BW-1:0] D1_I_RED_J3     = 'h001095C00027AEC00041BE000005E218;
    reg [RNS_BW-1:0] D1_I_RED_J4     = 'h00106288001B070000375F0000561000;
    

    //wire [RNS_BW-1:0] D1_I_INV_RED_I; //reg [RNS_BW-1:0] D1_I_INV_RED_I [N_CHANNELS-1:0];            
    //wire [RNS_BW-1:0] D1_I_RED_0, D1_I_RED_1, D1_I_RED_2, D1_I_RED_3, D1_I_RED_4;               
    //ROM #(RNS_BW, 1, FILENAME_D1_I_INV_RED_I, 1) rom0 (0, D1_I_INV_RED_I);
    //ROM #(RNS_BW, N_CHANNELS+1,     FILENAME_D1_I_RED_J) rom1 (0, D1_I_RED_0);  
    //ROM #(RNS_BW, N_CHANNELS+1,     FILENAME_D1_I_RED_J) rom2 (1, D1_I_RED_1);  
    //ROM #(RNS_BW, N_CHANNELS+1,     FILENAME_D1_I_RED_J) rom3 (2, D1_I_RED_2); 
    //ROM #(RNS_BW, N_CHANNELS+1,     FILENAME_D1_I_RED_J) rom4 (3, D1_I_RED_3);  
    //ROM #(RNS_BW, N_CHANNELS+1,     FILENAME_D1_I_RED_J) rom5 (4, D1_I_RED_4);
    
    //Step 1 - Calculate sigma (1 cycles)
    wire [RNS_BW-1:0] sigma;
    RNS_MULT #(CH_BW, N_CHANNELS, RNS_BW, RNS) m(A, D1_I_INV_RED_I, sigma); 
    
    //Step 2 - Accumulate t (i additions, j channels)
    wire [CH_BW-1:0] out0, out1, out2, out3, out4;
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS_EXT[CH_BW*0 +: CH_BW]) mac0(sigma, D1_I_RED_J0, out0);
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS_EXT[CH_BW*1 +: CH_BW]) mac1(sigma, D1_I_RED_J1, out1);
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS_EXT[CH_BW*2 +: CH_BW]) mac2(sigma, D1_I_RED_J2, out2);
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS_EXT[CH_BW*3 +: CH_BW]) mac3(sigma, D1_I_RED_J3, out3);
    RNS_MAC #(CH_BW, N_CHANNELS, RNS_BW, RNS_EXT[CH_BW*4 +: CH_BW]) mac4(sigma, D1_I_RED_J4, out4);
  
  
    assign Z = {out0,out1,out2,out3,out4};
   
   
endmodule
*/ 
        