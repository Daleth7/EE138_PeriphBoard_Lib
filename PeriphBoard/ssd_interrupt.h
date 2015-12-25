#ifndef SSD_TIMER_INTERRUPTUS23878387777777________
#define SSD_TIMER_INTERRUPTUS23878387777777________

#include "extended_types.h"

#define DISPLAY_DIGIT_SIZE_MAX 4
    // Return the current period of the display timer in seconds
double display_period(void);
    // Stores the DISPLAY_DIGIT_SIZE_MAX digit
    //  number to display as individual digits in big endian format.
extern UINT8 display_number[DISPLAY_DIGIT_SIZE_MAX];
    // Default is to display decimal numbers. However,
    //  other number bases may be used. The maximum base
    //  that can be display is base 16.
void update_display_base(UINT8 new_base);
    // Updates display_number with the appropriate digits
void update_display(UINT32 num_to_display);
    // Set up power manager and general clock registers
    //  for timer associated with display
    //  Called by configure_display_timer()
void enable_display_tc_clocks(void);
void enable_display_timer(void);
void disable_display_timer(void);
    // Configure TC 7 as a 16-bit timer to generate interrupts upon overruns
    //  After configuring, call enable_display_timer().
    //  SAMD20 IO pins tied with TC7:
    //      - PA20
    //      - PA21
    //      - PB22
    //      - PB23
    //      - PB00
    //      - PB01
void configure_display_timer(void);

BOOLEAN__ display_interrupted(void);
void clear_display_interrupt(void);
    // ISR for display. Call within TC7_Handler()
void display_handler(UINT8 select);

#endif