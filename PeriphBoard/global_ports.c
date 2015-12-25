#include "global_ports.h"

    // Ports
Port* port = NULL;
PortGroup* bankA = NULL, *bankB = NULL;

    // ADC and DAC
Adc* adc = NULL;
Dac* dac = NULL;

    // Timers
    // timerX refers to TcX while timerX_Y refers to TcX Y-bit counter
Tc* timer2_set = NULL, *timer4_set = NULL, *timer6_set = NULL, *timer7_set = NULL;
    // Be careful with TcCount instances since they are part of a union in Tc
TcCount8* timer2_8 = NULL;
TcCount16* timer2_16 = NULL;
TcCount8* timer4_8 = NULL;
TcCount8* timer6_8 = NULL;
TcCount8* timer7_8 = NULL;
TcCount16* timer7_16 = NULL;

Pm* power_manager = NULL;
Gclk* gen_clk = NULL;

    // Assign valid memory addresses to each pointer.
    //  Always call this before dereferencing any of the global pointers.
void configure_global_ports(void){
    port = (Port*)(PORT);
    bankA = (PortGroup*)(&port->Group[0]);
    bankB = (PortGroup*)(&port->Group[1]);

    adc = (Adc*)(ADC);
    dac = (Dac*)(DAC);

    timer2_set = (Tc*)(TC2);
    timer2_8 = (TcCount8*)(&timer2_set->COUNT8);

    timer4_set = (Tc*)(TC4);
    timer4_8 = (TcCount8*)(&timer4_set->COUNT8);
    timer2_16 = (TcCount16*)(&timer2_set->COUNT16);

    timer6_set = (Tc*)(TC6);
    timer6_8 = (TcCount8*)(&timer6_set->COUNT8);

    timer7_set = (Tc*)(TC7);
    timer7_8 = (TcCount8*)(&timer7_set->COUNT8);
    timer7_16 = (TcCount16*)(&timer7_set->COUNT16);

    power_manager = (Pm*)(PM);
    gen_clk = (Gclk*)(GCLK);
}