`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/03/2020 12:18:07 PM
// Design Name: 
// Module Name: phi_table
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


module phi_table #(
                   parameter integer BIT_WIDTH = 4,
                   parameter integer N_ELEMENTS = 64,
                   parameter integer ADDR_WIDTH = 6,
                   parameter PHI_FILENAME = "",
                   parameter integer w_n = 2,
                   parameter integer modulus = 11
                   )
                  (input wire [ADDR_WIDTH-1:0] phi_addr,
                   output wire [BIT_WIDTH-1:0] phi
                   );

    // Phi table    
    reg [BIT_WIDTH:0] phi_table [0:N_ELEMENTS];
    
    assign phi = phi_table[phi_addr];
    
    integer temp = 1;
    
    initial begin 
        if(PHI_FILENAME != "")
            $readmemh(PHI_FILENAME, phi_table);
        /*    
        else begin
            for (integer i = 0; i < N_ELEMENTS / 2; i = i + 1) begin
                phi_table[i] = temp;
                temp = (temp * w_n) % modulus; 
            end
         end
       */  
    end

endmodule
