`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/14/2020 02:44:45 PM
// Design Name: 
// Module Name: modmult_tb
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


module modmult_tb(

    );
    
    
    parameter N_TESTS  = 2000;
    parameter MODULUS = 31;
    parameter BIT_WIDTH_IN = 10;
    parameter BIT_WIDTH_OUT = $clog2(MODULUS);
    reg CLK = 1;
    reg [BIT_WIDTH_IN-1:0] A = 0, B = 0;
    wire [BIT_WIDTH_OUT-1:0] out;
    
    modmult #(BIT_WIDTH_IN,MODULUS,BIT_WIDTH_OUT) uut (
    .A(A),
    .B(B),
    .out(out)
    );    
    
    integer errorflag = 0;
    initial begin
    for (integer i = 0; i < N_TESTS; i = i+1) begin
        #1
        CLK <= ~CLK;
        #1
        if((A * B) % MODULUS != out) begin
            $display("Error with modulus: %d Input1: %d Input2: %d", MODULUS, A, B);
            errorflag = 1;
        end
        CLK <= ~CLK;
        A <= $random % BIT_WIDTH_IN;  //random
        B <= $random % BIT_WIDTH_IN;  //random
    end
    
    if (errorflag == 0) begin
        $display("No errors (Modulus: %d Trials : %d)",MODULUS,N_TESTS);
    end
end
        
endmodule
