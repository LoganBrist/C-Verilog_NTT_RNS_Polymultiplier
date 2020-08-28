// Multiplexes 4port ram into 2 port ram
module mem_4to2_input
  #(
     parameter integer BIT_WIDTH = 4, 
     parameter integer ADDR_WIDTH = 6
     )
   (
   //double port inputs
    input  wire [ADDR_WIDTH-1:0] readAddr1,
    input  wire [ADDR_WIDTH-1:0] readAddr2,
    input  wire [ADDR_WIDTH-1:0] writeAddr1,
    input  wire [ADDR_WIDTH-1:0]  writeAddr2,
    input  wire [BIT_WIDTH-1:0]  writeData1,
    input  wire [BIT_WIDTH-1:0]  writeData2,
    input  wire                  writeEn1,
    input  wire                  writeEn2,

    // single port outputs
    output  reg [ADDR_WIDTH-1:0] readAddr_out,
    output  reg [ADDR_WIDTH-1:0] writeAddr_out,
    output  reg [BIT_WIDTH-1:0]  writeData_out,
    output  reg                  writeEn_out = 0,
    input  wire          CLK,
    input  wire          DOUBLE_CLK
    );
    
    reg [BIT_WIDTH-1:0]  writeData1_ff, writeData2_ff;
    reg [ADDR_WIDTH-1:0] writeAddr1_ff, writeAddr2_ff;
    reg writeEn2_ff;

    //when CLK is zero, the buttefly results are correct.
    // combinationally send write Addr1 and data1 to memory while CLK == 0.
    // save write Addr2 and data2 at posedge CLK. combinationally send when CLK == 1.
    
    
    always @(posedge CLK) begin
    //writeData1_ff <= writeData1;  //both registers needed so butterfly doesn't change it after the clock edge 
    writeData2_ff <= writeData2;
    //writeAddr1_ff <= writeAddr1;  //both needed because writing has to occur the cycle after
    writeAddr2_ff <= writeAddr2;
    end
    always @(*) begin
        readAddr_out  = (CLK == 1) ? readAddr1  : readAddr2;
        writeAddr_out = (CLK == 0) ? writeAddr1 : writeAddr2_ff; //0
        writeData_out = (CLK == 0) ? writeData1 : writeData2_ff; //0
        writeEn_out   = (CLK == 1) ? writeEn1   : writeEn2;
    end
    
    endmodule
    
    
    // all 1 values need to be directly routed. All 2 values need registers and directly routed on the next clock