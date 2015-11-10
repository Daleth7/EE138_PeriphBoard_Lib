#ifndef POWER_MANAGER_AND_GENERAL322536475CLOCKIMPLEMENT_SHARED_HELPERS23228
#define POWER_MANAGER_AND_GENERAL322536475CLOCKIMPLEMENT_SHARED_HELPERS23228

#include <stdint.h>

    // Set up registers for power managing and the general clock
    //  pm_bit_pos:     Bit position of the clock in APBCMASK (see page 129)
    //  gen_clk_id:     General clock selection id (see pages 97 and 98)
    //  gen_clk_src:    General clock generator select (see pages 96 and 97)
void enable_clock_gen_src_sel(uint32_t pm_bit_pos, uint32_t gen_clk_id, uint32_t gen_clk_src);
void enable_clock(uint32_t pm_bit_pos, uint32_t gen_clk_id);

#endif