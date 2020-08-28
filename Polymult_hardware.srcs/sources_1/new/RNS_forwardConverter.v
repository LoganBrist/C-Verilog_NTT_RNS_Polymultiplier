module RNS_forwardConverter
    #(
     parameter integer N_MOD = 4,
     parameter integer MOD_BW = 32,
     parameter integer BIT_WIDTH = 64,
     parameter [N_MOD*MOD_BW - 1 : 0] MODULI  = {32'd7, 32'd11, 32'd17, 32'd19}
    )
    (
    input  wire [BIT_WIDTH-1:0] number,
    output wire [N_MOD * MOD_BW - 1 : 0] residues
    );
    
    //create reduction units
    genvar i;
 
    generate 
        for (i = 0; i < N_MOD; i = i + 1) begin
            reduce #(BIT_WIDTH, MODULI[(i+1) * MOD_BW - 1: i * MOD_BW]) red (number, res);
            assign residues[(i+1) * MOD_BW - 1: i * MOD_BW] = res;
        end
    endgenerate
    

endmodule