#ifndef MO238437843TOR_HEADER_SO3458935FAR_W5432ITHOUT_INTE3498590RRUPTS_____
#define MO238437843TOR_HEADER_SO3458935FAR_W5432ITHOUT_INTE3498590RRUPTS_____

#include "extended_types.h"

    // Informational functions
UINT8 motor_duty_max(void);
UINT8 motor_duty_min(void);
UINT8 motor_duty_cur(void);
UINT8 motor_duty_cur_percent(void);
float motor_duty_max_f(void);
float motor_duty_min_f(void);
float motor_duty_cur_percent_f(void);

    // Configuration functions
void configure_motor_ports(void);
void enable_motor_clocks(void);
void configure_motor_pwm(void);
void configure_motor_pwm_custom(
    UINT8 presync_src, UINT8 prescale, UINT8 wave_mode,
    UINT8 period, UINT8 duty
    );
void disable_motor_pwm(void);

    // Speed modification functions
void motor_set_duty(UINT8 new_duty);
void motor_set_duty_f(float new_duty);
void motor_step_duty(INT16 duty_change);
BOOLEAN__ step_toward8_basic(UINT8 target, UINT32 delay);
BOOLEAN__ step_toward8(
    UINT8 target,
    UINT8 volatile * reg1, UINT8 volatile * reg2,
    UINT32 delay
    );
void slow_motor_step(UINT32 target);

#endif