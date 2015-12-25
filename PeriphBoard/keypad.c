#include "global_ports.h"
#include "keypad.h"

static PortGroup* key_bankA;

void configure_keypad_ports(void){
    configure_global_ports();
    key_bankA = bankA;
        // Controls power to the keypad and SSDs. 0000 1111 0000
    key_bankA->DIR.reg |= 0x000000F0;

        // For reading input from keypad. 1111 0000 0000 0000 0000 
        //  Active high logic for input.
    key_bankA->DIR.reg &= ~0x000F0000;
    UINT8 i;
    for(i = 16; i < 20; ++i){
            // Enable input (bit 1).
            //  Note that the pins are externally pulled low,
            //  so disable pull up.
        key_bankA->PINCFG[i].reg = PORT_PINCFG_INEN;
    }
}

void check_key(UINT8* row_dest, UINT8* col_dest, UINT8 row_to_check){
    // Provide power to one specific row
    key_bankA->OUT.reg |= 0x000000F0;
    key_bankA->OUT.reg &= ~(1u << (4u + row_to_check));
        // Extract the four bits we're interested in from
        //   the keypad.
    if(!IS_NULL(col_dest))  *col_dest = debounce_keypress();
    if(!IS_NULL(row_dest))  *row_dest = row_to_check;
}

UINT8 debounce_keypress(void){
    // Triggered the instant the first key press is detected
    //  Returns the resulting hex number

    UINT8 toreturn = (key_bankA->IN.reg >> 16u) & 0xF;

        // Check if more than one button in a row was pressed.
        //  If so, checking for glitches is no longer important.
    UINT32 counter = 0x0;
    BOOLEAN__ already_on = FALSE__;
    for(; counter < 4; ++counter){
        if(already_on & (toreturn >> counter))  return toreturn;
        else    already_on = (toreturn >> counter) & 0x1;
    }

    if(!toreturn)   return 0x10;
#define MAX_JITTER  5
#define MAX_JITTER2 2500
#define RELEASE_LIM 11250

    // First, read up to MAX_JITTER times to swallow spikes as button is
    //  pressed. If no key press was detected in this time, the noise is
    //  not from a button press.
    for(counter = 0x0; counter < MAX_JITTER; ++counter){
        if(!((key_bankA->IN.reg >> 16u) & 0xF))    return 0x10;
    }

    // Now swallow the spikes as the button is released. Do not exit
    //  until the spikes are no longer detected after MAX_JITTER reads.
    //  If the user is holding down the button, release manually based
    //  on RELEASE_LIM.
    volatile UINT32 release = 0x0;
    for(
        counter = 0x0;
        counter < MAX_JITTER2 && release < RELEASE_LIM;
        ++counter, ++release
    ){
        if((key_bankA->IN.reg >> 16u) & 0xF)    counter = 0x0;
    }

    return toreturn;

#undef MAX_JITTER
#undef MAX_JITTER2
#undef RELEASE_LIM
}