`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/14/2020 02:44:18 PM
// Design Name: 
// Module Name: reduction_tb
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


module reduction_tb(

    );
    parameter N_TESTS  = 200;
    parameter MODULUS = 8;
    parameter BIT_WIDTH_IN = 32;
    parameter BIT_WIDTH_OUT = $clog2(MODULUS);
    reg CLK = 1;
    reg [BIT_WIDTH_IN-1:0] in = 0;
    wire [BIT_WIDTH_OUT-1:0] out;
    
    reduce #(BIT_WIDTH_IN,MODULUS,BIT_WIDTH_OUT) uut (
    .in(in),
    .out(out)
    );    
    
    integer errorflag = 0;
    initial begin
    for (integer i = 0; i < N_TESTS; i = i+1) begin
        #1
        CLK <= ~CLK;
        #1
        if(in % MODULUS != out) begin
            $display("Error with modulus: %d Input: %d", MODULUS, in);
            errorflag = 1;
        end
        CLK <= ~CLK;
        in <= $random % BIT_WIDTH_IN;  //random
    end
    
    if (errorflag == 0) begin
        $display("No errors (Modulus: %d Trials : %d)",MODULUS,N_TESTS);
    end
end
        
endmodule
