`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/24/2020 02:53:23 PM
// Design Name: 
// Module Name: butterfly
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

// Give three cycle headstart then produces one per cycle

//data1 is the left earlier index. data2 is the right later index 
module butterfly
  #(parameter integer BIT_WIDTH = 256,
    parameter integer MOD = 12289
    )
    (
    input   wire CLK,   
    input   wire [BIT_WIDTH-1:0]  data_in1,
    input   wire [BIT_WIDTH-1:0]  data_in2,
    
    output  reg [BIT_WIDTH-1:0]  data_out1,
    output  reg [BIT_WIDTH-1:0]  data_out2,
    input   wire [BIT_WIDTH-1:0]  phi 
    );
   
    //Intermediate buttefly registers
    reg [BIT_WIDTH-1:0] A_ff0 = 0, B_ff0 = 0, phi_ff0 = 0, A_ff1 = 0, B_ff1 = 0; 

    // RNS multiplier 
    wire [BIT_WIDTH*2-1:0] rns_out;
    RNS #(BIT_WIDTH) rns (B_ff0, phi_ff0, rns_out);
    //assign rns_out = B_ff0 * phi_ff0;
    
    //To be replaced with barrett or montgomery reduction and RNS multiplication 
     always @(*) begin
        //cycle 0
        A_ff0   = data_in1;
        B_ff0   = data_in2; 
        phi_ff0 = phi;
        
        //cycle 1
        A_ff1   =  A_ff0;
        B_ff1   = rns_out % MOD; //(B_ff0 * phi_ff0) % MOD //rns_out % MOD; 
          
        //cycle 2
        data_out2 = (A_ff1 + B_ff1) % MOD;       //switched 1 and 2 (erase when switched back)
        data_out1 = (A_ff1 + MOD - B_ff1) % MOD;
     end
    
endmodule
