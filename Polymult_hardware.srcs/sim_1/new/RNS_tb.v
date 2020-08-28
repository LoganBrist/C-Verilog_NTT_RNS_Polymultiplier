`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/16/2020 10:46:56 AM
// Design Name: 
// Module Name: RNS_tb
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


module RNS_tb(

    );
    
     reg CLK = 0;
     parameter integer MODULUS = 673;
     parameter integer NUM_BW  = $clog2(MODULUS);
     parameter integer RES_BW = 5;
     parameter integer N_RES  = 6;
     reg [NUM_BW - 1:0] number_in1;
     reg [NUM_BW - 1:0] number_in2;
     wire [RES_BW-1:0] res1A,res1B,mult;
     //wire [RES_BW-1:0] res1,res2,res3,res4,res5,res6;
     //wire [RES_BW * N_RES - 1 : 0] fc1_out,fc2_out, rc_in;
     wire [NUM_BW*2-1:0] number_out;
    
    //assign {res1,res2,res3,res4,res5,res6} = fc1_out;
    
    RNS #(MODULUS) uut 
    (
    .number_in1(number_in1),
    .number_in2(number_in2),
    .res1A(res1A),
    .res1B(res1B),
     .mult(mult),
    /*
    .fc1_out(fc1_out),
    .fc2_out(fc2_out),
    .rc_in(rc_in), */
    .number_out(number_out)
    );
    
    initial begin
    for (integer i = 0; i < 200; i = i+1) begin
        number_in1 <= $random % NUM_BW;
        number_in2 <= $random % NUM_BW;
        
        #1
        CLK <= ~CLK;
        if (number_out == number_in1 * number_in2) begin
            $display("%d == %d * %d  correct", number_out, number_in1, number_in2);
        end
        else begin
            $display("%d != %d * %d  error", number_out, number_in1, number_in2);
        end
        
        #1
        CLK <= ~CLK;
        
    end
end
   
endmodule
