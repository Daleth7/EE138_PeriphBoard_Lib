#include "clock_manager_utilities.h"

#include "global_ports.h"

void enable_clock_gen_src_sel(
    uint32_t pm_bit_pos,
    uint32_t gen_clk_id,
    uint32_t gen_clk_src
){
    configure_global_ports();

    // PM_APBCMASK is in the pm_bit_pos position
    power_manager->APBCMASK.reg |= (1 << pm_bit_pos);
     
    gen_clk->CLKCTRL.reg =  //  Setup in the CLKCTRL register
          (gen_clk_src<<8)  //  General clock source
        | gen_clk_id;       //  Clock selection ID(see table 14-2)
    gen_clk->CLKCTRL.reg |= 0x1u << 14;    // enable it.
}

void enable_clock(uint32_t pm_bit_pos, uint32_t gen_clk_id){
    enable_clock_gen_src_sel(pm_bit_pos, gen_clk_id, 0);
}