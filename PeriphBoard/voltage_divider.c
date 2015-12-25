#include "voltage_divider.h"

#include "global_ports.h"

#define DIVIDER_SOURCE_PIN   13

static PortGroup* bankA_ptr;

void configure_voltage_divider_ports(void){
    bankA_ptr = bankA;
    bankA_ptr->DIR.reg |= (1 << DIVIDER_SOURCE_PIN);
    bankA_ptr->OUT.reg |= (1 << DIVIDER_SOURCE_PIN);            // Set high
}

void turn_on_divider(void){
    bankA_ptr->OUT.reg |= (1 << DIVIDER_SOURCE_PIN);
}

void turn_off_divider(void){
    bankA_ptr->OUT.reg &= ~(1 << DIVIDER_SOURCE_PIN);
}