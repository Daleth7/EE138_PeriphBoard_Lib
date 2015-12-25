#include "motor.h"

#include "global_ports.h"
#include "clock_manager_utilities.h"
#include "extended_types.h"

static TcCount8*    mot_timer;
static PortGroup*   bankA_ptr;

#define MOTOR_DUTY_MARGIN 10
#define MOTOR_U_PIN 23
#define MOTOR_V_PIN 22

UINT8 motor_duty_max(void)
    {return mot_timer->PER.reg - MOTOR_DUTY_MARGIN;}

UINT8 motor_duty_min(void)
    {return MOTOR_DUTY_MARGIN;}

UINT8 motor_duty_cur(void)
    {return mot_timer->CC[0].reg;}

UINT8 motor_duty_cur_percent(void)
    {return (mot_timer->PER.reg*100u)/mot_timer->PER.reg;}

float motor_duty_max_f(void)
    {return (float)(mot_timer->PER.reg - MOTOR_DUTY_MARGIN)/mot_timer->PER.reg;}

float motor_duty_min_f(void)
    {return (float)(MOTOR_DUTY_MARGIN)/mot_timer->PER.reg;}

float motor_duty_cur_percent_f(void)
    {return (float)(mot_timer->PER.reg)/mot_timer->PER.reg;}

void configure_motor_ports(void){
    mot_timer = timer4_8;
    bankA_ptr = bankA;

        // Set up timer pin to use the timer
    bankA_ptr->PINCFG[MOTOR_U_PIN].reg |= 0x1;            // Enable multiplexing
    bankA_ptr->PINCFG[MOTOR_V_PIN].reg |= 0x1;            // Enable multiplexing

    bankA_ptr->PMUX[(MOTOR_U_PIN-1)/2].reg |= 0x5 << 4u;    // Select function F (timer)
    bankA_ptr->PMUX[MOTOR_V_PIN/2].reg |= 0x5;    // Select function F (timer)
}

void enable_motor_clocks(void)
{
    enable_clock(
        12u,    // PM_APBCMASK is in the 12 position
        0x15    // ID for TC4 is 0x15  (see table 14-2)
        );
}

void configure_motor_pwm(void)
{
    configure_motor_pwm_custom(
        0x1,                    // Set presynchronizer to prescaled clock
        0x0,                    // Prescale clock by 1
        0x2,                    // Select the Normal PWM waveform
        200,
        100                     // Start with 50% duty cycle
        );
}

void configure_motor_pwm_custom(
    UINT8 presync_src, UINT8 prescale, UINT8 wave_mode,
    UINT8 period, UINT8 duty
    )
{
    configure_motor_ports();
    enable_motor_clocks();

        // To mess with registers, disable timer first
    disable_motor_pwm();

    mot_timer->CTRLA.reg |= 
          (presync_src << 12u)  // Set presynchronizer to prescaled clock
        | (prescale << 8u)      // Prescale clock by 1
        | (0x1 << 2u)           // Start in 8-bit mode
        | (wave_mode << 5u);    // Select the Normal PWM waveform
    mot_timer->PER.reg = period;
    if(
        period > MOTOR_DUTY_MARGIN &&
        duty > MOTOR_DUTY_MARGIN &&
        duty < (period-MOTOR_DUTY_MARGIN)
        ){
        mot_timer->CC[0].reg = mot_timer->CC[1].reg = duty;
    } else {
        mot_timer->CC[0].reg = mot_timer->CC[1].reg = period/2;
    }

    while(mot_timer->STATUS.reg & (1 << 7u));    // Synchronize before proceeding

    mot_timer->CTRLC.reg |= 0x1;    // Invert the waveform from one of the outputs
    mot_timer->CTRLA.reg |= 1 << 1u;    // Re-enable the timer
}

void disable_motor_pwm(void)
{
    mot_timer->CTRLA.reg &= ~(1 << 1u);
    while(mot_timer->STATUS.reg & (1 << 7u));    // Synchronize before proceeding
}

void motor_set_duty(UINT8 new_duty){
    if(
        new_duty < MOTOR_DUTY_MARGIN ||
        new_duty > (mot_timer->PER.reg-MOTOR_DUTY_MARGIN)
    )   return;
    mot_timer->CC[1].reg = mot_timer->CC[0].reg = new_duty;
}

void motor_set_duty_f(float new_duty){
    if(
        new_duty < 0.0f ||
        new_duty > 1.0f
    )   return;
    mot_timer->CC[1].reg = mot_timer->CC[0].reg = new_duty*mot_timer->PER.reg;
}

void motor_step_duty(INT16 duty_change){
    mot_timer->CC[0].reg += duty_change;
    mot_timer->CC[1].reg = mot_timer->CC[0].reg;
}

BOOLEAN__ step_toward8_basic(UINT8 target, UINT32 delay){
    return step_toward8(target, &mot_timer->CC[0].reg, &mot_timer->CC[1].reg, delay);
}

BOOLEAN__ step_toward8(
    UINT8 target,
    UINT8 volatile * reg1, UINT8 volatile * reg2,
    UINT32 delay
    )
{
    if(target > *reg1)       ++(*reg1);
    else if(target < *reg1)  --(*reg1);
    *reg2 = *reg1;
    delay_ms(delay);
    return target == *reg1;
}

void slow_motor_step(UINT32 target){
    while(!step_toward8(target, &mot_timer->CC[0].reg, &mot_timer->CC[1].reg, 10));
}