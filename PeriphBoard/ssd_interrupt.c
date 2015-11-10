#include "ssd_interrupt.h"

#include "global_ports.h"
#include "clock_manager_utilities.h"
#include "ssd.h"

static TcCount16* disp_timer;
UINT8 display_number[DISPLAY_DIGIT_SIZE_MAX] = {-1, -1, -1, -1};
static UINT8 display_base = 10;

void update_display_base(UINT8 new_base){
    if(new_base > 1 && new_base < 17)   display_base = new_base;
}

void update_display(UINT32 num_to_display){
    UINT32 ten_factor = display_base;
    for(UINT8 i = DISPLAY_DIGIT_SIZE_MAX; i > 0; --i, ten_factor *= display_base){
        display_number[i-1] = (num_to_display%ten_factor)/(ten_factor/display_base);
    }
        // Replace leading zeros with blanks
    for(UINT8 i = 0; i < DISPLAY_DIGIT_SIZE_MAX-1 && display_number[i] == 0; ++i){
        display_number[i] = -1;
    }
}

void enable_display_tc_clocks(void){
    enable_clock(
        15u,    // PM_APBCMASK is in the 15 position
        0x16    // ID for TC7 is 0x16  (see table 14-2)
        );
}

void enable_display_timer(void){
    while(disp_timer->STATUS.reg & (1 << 7u));    // Synchronize before proceeding
    disp_timer->CTRLA.reg |= 1 << 1u;    // Re-enable the timer
}

void disable_display_timer(void){
    disp_timer->CTRLA.reg &= ~(1 << 1u);    // Disable the timer
    while(disp_timer->STATUS.reg & (1 << 7u));    // Synchronize before proceeding
}

    // In a future implementation, possibly write an event-driven system
    // Possibly rewrite for control over brightness
void configure_display_timer(void){
    disp_timer = timer7_16; // Use the one of the count structures within the union

    enable_display_tc_clocks();
    disable_display_timer();

        // Set up timer 7 settings
    disp_timer->CTRLA.reg |=
          (0x1 << 12u)  // Set presynchronizer to prescaled clock
        | (0x4 << 8u)   // Prescale clock by 16
        | (0x0 << 2u)   // Start in 16-bit mode
        | (0x1 << 5u)   // Select the Match Frequncy waveform generator
                        //  Allow control over refresh speed and brightness
        ;
    disp_timer->CC[0].reg = 0xF0;

        // Set up timer 7 interrupt
    NVIC->ISER[0] |= 1 << 20u;
    disp_timer->INTENSET.reg |= 1 << 4u;
    disp_timer->INTFLAG.reg |= 0x1 << 4u;
}

void display_handler(void){
    static UINT8 dig = 0;
    if(disp_timer->INTFLAG.reg & (0x1 << 4u)){
        display_dig(display_number[DISPLAY_DIGIT_SIZE_MAX-1-dig], dig, FALSE__, FALSE__);
        dig = (dig == DISPLAY_DIGIT_SIZE_MAX-1) ? 0 : (dig+1);
        disp_timer->INTFLAG.reg |= 0x1;
    }
}

void TC7_Handler(void){
    display_handler();
}
