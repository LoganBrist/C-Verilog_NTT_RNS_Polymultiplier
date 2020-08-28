`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/06/2020 05:49:37 PM
// Design Name: 
// Module Name: MAC_CONTROLLER
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

module MAC_CONTROLLER
    #(
    parameter BIT_WIDTH = 32,
    parameter ADDR_WIDTH = 32
    )
    (
    input  wire CLK,
    input  wire [ADDR_WIDTH-1:0] input_addr1,
    input  wire [ADDR_WIDTH-1:0] input_addr2,
    input  wire [BIT_WIDTH-1:0]  mem_data1,
    input  wire [BIT_WIDTH-1:0]  mem_data2,
    input  wire run_flag,
    input  wire reset,
    output reg [BIT_WIDTH-1:0]  output_data
    );
 /*   
    //detect execute_flag edge
    reg was_not_running = 0;
    reg was_running     = 1;
    
    //addr registers
    reg [ADDR_WIDTH-1:0] addr_counter1 = 0;
    reg [ADDR_WIDTH-1:0] addr_counter2 = 0;
    wire [ADDR_WIDTH-1:0] next_addr1, next_addr2; 
    
    assign next_addr1 = addr_counter1 + 1;
    assign next_addr2 = addr_counter2 + 1;
    
    always @(posedge CLK) begin
        //See if MAC was running on the last cycle
        was_not_running <= !run_flag;
        was_running     <= run_flag;
        
        //Set addr pointers if not running
        if (run_flag && was_not_running) begin
            addr_counter1 <= input_addr1;
            addr_counter2 <= input_addr2;
            output_data   <= 0;
        end
        
        //Increment points if running
        else if(run_flag && was_running) begin
            addr_counter1     <= addr_counter1 + 1;
            addr_counter2     <= addr_counter2 + 1; 
            output_data       <= mem1[next_addr_counter1] * mem2[next_addr_counter2];   
        end
    end
    */
endmodule


