#include "ssd.h"

#include "global_ports.h"

static PortGroup* bankA_ptr, *bankB_ptr;

void configure_ssd_ports(void){
    configure_global_ports();
    bankA_ptr = bankA;
    bankB_ptr = bankB;

        // Controls power to the keypad and SSDs. 0000 1111 0000
    bankA_ptr->DIR.reg |= 0x000000F0;

    bankB_ptr->DIR.reg
            // Controls which segment turn on. 0000 1111 1111
        |= 0x000000FF
            // Controls sign indicator. 0010 0000 0000
        |  0x00000200
        ;
    // Set high drive strength ?
    unsigned short i = 0;
    for(; i < 8; ++i){
        bankA_ptr->PINCFG[i].reg |= (1<<6);
    }
        // Turn off all dots
    bankB_ptr->OUT.reg &= ~0x10;
    bankB_ptr->OUT.reg |= 0x00000080;
}

void display_dig(UINT8 num, UINT8 select){
        // Active low logic
    bankB_ptr->OUT.reg |= 0x000000FF;
        // Provide power to one specific SSD
    bankA_ptr->OUT.reg |= 0x000000F0;   // Turn off first, then turn on later
    switch(num){
                //  GEF DCBA
        case 0: // 1100 0000
            bankB_ptr->OUT.reg &= 0xC0;
            break;
        case 1: // 1111 1001
            bankB_ptr->OUT.reg &= 0xF9;
            break;
        case 2: // 1010 0100
            bankB_ptr->OUT.reg &= 0xA4;
            break;
        case 3: // 1011 0000
            bankB_ptr->OUT.reg &= 0xB0;
            break;
        case 4: // 1001 1001
            bankB_ptr->OUT.reg &= 0x99;
            break;
        case 5: // 1001 0010
            bankB_ptr->OUT.reg &= 0x92;
            break;
        case 6: // 1000 0010
            bankB_ptr->OUT.reg &= 0x82;
            break;
        case 7: // 1111 1000
            bankB_ptr->OUT.reg &= 0xF8;
            break;
        case 8: // 1000 0000
            bankB_ptr->OUT.reg &= 0x80;
            break;
        case 9: // 1001 0000
            bankB_ptr->OUT.reg &= 0x90;
            break;
        case 10: // 1000 1000
            bankB_ptr->OUT.reg &= 0x88;
            break;
        case 11: // 1000 0011
            bankB_ptr->OUT.reg &= 0x83;
            break;
        case 12: // 1100 0110
            bankB_ptr->OUT.reg &= 0xC6;
            break;
        case 13: // 1010 0001
            bankB_ptr->OUT.reg &= 0xA1;
            break;
        case 14: // 1000 0110
            bankB_ptr->OUT.reg &= 0x86;
            break;
        case 15: // 1000 1110
            bankB_ptr->OUT.reg &= 0x8E;
            break;
        default:    // Non-hexadecimal digit or negative
            bankB_ptr->OUT.reg |= 0xFF;
            break;
    }
    bankA_ptr->OUT.reg &= ~(1 << (select + 4u));    // Turn on specific display
}

void display_dig_spec(
    UINT8 num, UINT8 select,
    BOOLEAN__ show_dot, BOOLEAN__ show_sign
){
        // Active low logic
    bankB_ptr->OUT.reg |= 0x000000FF;
        // Provide power to one specific SSD
    bankA_ptr->OUT.reg |= 0x000000F0;   // Turn off first, then turn on later
    switch(num){
                //  GEF DCBA
        case 0: // 0100 0000
            bankB_ptr->OUT.reg &= ~0xBF;
            break;
        case 1: // 0111 1001
            bankB_ptr->OUT.reg &= ~0x86;
            break;
        case 2: // 0010 0100
            bankB_ptr->OUT.reg &= ~0xDB;
            break;
        case 3: // 0011 0000
            bankB_ptr->OUT.reg &= ~0xCF;
            break;
        case 4: // 0001 1001
            bankB_ptr->OUT.reg &= ~0xE6;
            break;
        case 5: // 0001 0010
            bankB_ptr->OUT.reg &= ~0xED;
            break;
        case 6: // 0000 0010
            bankB_ptr->OUT.reg &= ~0xFD;
            break;
        case 7: // 0111 1000
            bankB_ptr->OUT.reg &= ~0x87;
            break;
        case 8: // 0000 0000
            bankB_ptr->OUT.reg &= ~0x7F;
            break;
        case 9: // 0001 0000
            bankB_ptr->OUT.reg &= ~0xEF;
            break;
        case 10: // 0000 1000
            bankB_ptr->OUT.reg &= ~0xF7;
            break;
        case 11: // 0000 0011
            bankB_ptr->OUT.reg &= ~0xFC;
            break;
        case 12: // 0100 0110
            bankB_ptr->OUT.reg &= ~0xB9;
            break;
        case 13: // 0010 0001
            bankB_ptr->OUT.reg &= ~0xDE;
            break;
        case 14: // 0000 0110
            bankB_ptr->OUT.reg &= ~0xF9;
            break;
        case 15: // 0000 1110
            bankB_ptr->OUT.reg &= ~0xF1;
            break;
        default:    // Non-hexadecimal digit or negative
            bankB_ptr->OUT.reg |= 0xFF;
            show_dot = FALSE__;
            break;
    }
    if(show_dot)    bankB_ptr->OUT.reg &= ~0x00000080;
    else            bankB_ptr->OUT.reg |=  0x00000080;
    if(show_sign)   bankB_ptr->OUT.reg &= ~0x00000200;
    else            bankB_ptr->OUT.reg |=  0x00000200;
    bankA_ptr->OUT.reg &= ~(1 << (select + 4u));    // Turn on specific display
}

void turn_off_ssd(void){
    bankB_ptr->OUT.reg |= 0x000000FF;   // Turn off segments
    bankA_ptr->OUT.reg |= 0x000000F0;   // Turn off power to ssd
}