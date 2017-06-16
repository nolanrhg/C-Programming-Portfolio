#ifndef SERVO_H
#define SERVO_H

void servo_push(void);
void rotate_servo1(uint16_t ccr);
void rotate_servo2(uint16_t ccr);
void home_servo1(void);
void home_servo2(void);
void servo1_signal_off(void);
void servo2_signal_off(void);

#endif
