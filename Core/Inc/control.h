/*
 * control.h
 *
 *  Created on: Apr 27, 2023
 *      Author: Jing Chen
 */
/*
 * [IMPORTANT：what param in this header file you can change]
 *
 * 1. motor driver pin (INA, INB, PWM for VNH5019)(PHASE, PWM for DRV8874)
 *
 * 2. encoder and pwm timer channel
 * (timer9 is toxic)
 *
 * 3. PID gain (KP, KI, KD)
 * */
#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "main.h"

// [!!! IMPORTANT !!!] choose the board used
//#define VNH5019
//#define DRV8874
#define G2_18V17

#ifdef VNH5019
#define M1_INA_Pin GPIOB
#define M1_INA_GPIO_port GPIO_PIN_1
#define M1_INB_Pin GPIOB
#define M1_INB_GPIO_port GPIO_PIN_2

#define M2_INA_Pin GPIOC
#define M2_INA_GPIO_port GPIO_PIN_2
#define M2_INB_Pin GPIOC
#define M2_INB_GPIO_port GPIO_PIN_3

#define M3_INA_Pin GPIOA
#define M3_INA_GPIO_port GPIO_PIN_10
#define M3_INB_Pin GPIOA
#define M3_INB_GPIO_port GPIO_PIN_11
#endif
#ifdef DRV8874
#define M1_PHASE_Pin GPIOB
#define M1_PHASE_GPIO_port GPIO_PIN_5

#define M2_PHASE_Pin GPIOC
#define M2_PHASE_GPIO_port GPIO_PIN_13

#define M3_PHASE_Pin GPIOC
#define M3_PHASE_GPIO_port GPIO_PIN_7
#endif

#ifdef G2_18V17
#define M1_DIR_Pin GPIOD
#define M1_DIR_GPIO_port GPIO_PIN_8

#define M2_DIR_Pin GPIOB
#define M2_DIR_GPIO_port GPIO_PIN_13

#define M3_DIR_Pin GPIOE
#define M3_DIR_GPIO_port GPIO_PIN_3

#define M4_DIR_Pin GPIOC
#define M4_DIR_GPIO_port GPIO_PIN_13
#endif

/*encoder and pwm timer/channel define*/
#define M1_Encoder_timer htim2
#define M1_Encoder_timerchannel TIM_CHANNEL_1 | TIM_CHANNEL_2
#define M1_Encoder_dir -1
#define M1_PWM_timer htim12
#define M1_PWM_timerchannel TIM_CHANNEL_1

#define M2_Encoder_timer htim5
#define M2_Encoder_timerchannel TIM_CHANNEL_1 | TIM_CHANNEL_2
#define M2_Encoder_dir -1
#define M2_PWM_timer htim12
#define M2_PWM_timerchannel TIM_CHANNEL_2

#define M3_Encoder_timer htim3
#define M3_Encoder_timerchannel TIM_CHANNEL_1 | TIM_CHANNEL_2
#define M3_Encoder_dir 1
#define M3_PWM_timer htim15
#define M3_PWM_timerchannel TIM_CHANNEL_1

#define M4_Encoder_timer htim4
#define M4_Encoder_timerchannel TIM_CHANNEL_1 | TIM_CHANNEL_2
#define M4_Encoder_dir -1
#define M4_PWM_timer htim15
#define M4_PWM_timerchannel TIM_CHANNEL_2


#define Encoder_Interrupt_timer htim23
//#define control_period 0.01 // frequency = 84M/(13+1)/(60000) = 100HZ
extern double control_period;

/* Hardware Info */
#define encoder_resolution 512
#define speed_reduction_ratio (36 / 1.125) // GBN50-3D: 36 // GBN50-2D: 12

#define wheel_radius 0.05075
#define chassis_radius 0.3

extern double radius_error_a;
extern double radius_error_b;
extern double radius_error_c;
extern double radius_error_d;
extern double radius_error_chassis;

extern double linearvelocity_x;
extern double linearvelocity_y;
extern double angularvelocity;

/* PID gain param : only PI control*/
#define robot1
#ifdef robot1
#define M1_KP 1.5
#define M1_KI 50
#define M1_KD 0.0

#define M2_KP 3
#define M2_KI 30
#define M2_KD 0.0

#define M3_KP 10
#define M3_KI 30
#define M3_KD 0.0

#define M4_KP 5.8
#define M4_KI 60
#define M4_KD 0.0
#endif

/* PID control param define */
extern double limit_integral;
extern double pwm_arr;


typedef struct
{
		double Kp, Ki, Kd;

		int16_t CountNum;
		double rps, rps_before;
		double goal, err;
		double propotional, integral, differential;
		double duty;

#ifdef VNH5019
		GPIO_TypeDef *INA_pin_type;
		uint16_t INA_pin_Num;
		GPIO_TypeDef *INB_pin_type;
		uint16_t INB_pin_Num;
#endif
#ifdef DRV8874
		GPIO_TypeDef *PHASE_pin_type;
		uint16_t PHASE_pin_Num;
#endif
#ifdef G2_18V17
		GPIO_TypeDef *DIR_pin_type;
		uint16_t DIR_pin_Num;
#endif

		TIM_HandleTypeDef encoder_timer;
		uint32_t encoder_timer_channel;
		int encoder_dir;
		TIM_HandleTypeDef pwm_timer;
		uint32_t pwm_timer_channel;

}PID_Control;

extern PID_Control WheelA;
extern PID_Control WheelB;
extern PID_Control WheelC;
extern PID_Control WheelD;

extern int i;
extern double sssss[1000];

void Control_Init();
void Pid_Param_Init(PID_Control *Wheel_, double kp, double ki, double kd);
void Hardware_Info_Init();
void Control_Timer_Init();
void PID_Controller(PID_Control *Wheel_);
void Forward_Kinematics(double x, double y, double w);
void Inverse_Kinematics(PID_Control *WheelA_, PID_Control *WheelB_, PID_Control *WheelC_, PID_Control *WheelD_);
void Stop_Chasis();

#ifdef VNH5019
void Motor_Driver_Init(PID_Control *Wheel_,
		GPIO_TypeDef *INA_pin_type_, uint16_t INA_pin_num_,
		GPIO_TypeDef *INB_pin_type_, uint16_t INB_pin_num_,
		TIM_HandleTypeDef encoder_timer_, uint32_t encoder_timer_channel_, int encoder_dir_,
		TIM_HandleTypeDef pwm_timer_, uint32_t pwm_timer_channel_);
#endif
#ifdef DRV8874
void Motor_Driver_Init(PID_Control *Wheel_,
		GPIO_TypeDef *PHASE_pin_type_, uint16_t PHASE_pin_num_,
		TIM_HandleTypeDef encoder_timer_, uint32_t encoder_timer_channel_, int encoder_dir_,
		TIM_HandleTypeDef pwm_timer_, uint32_t pwm_timer_channel_);
#endif
#ifdef G2_18V17
void Motor_Driver_Init(PID_Control *Wheel_,
		GPIO_TypeDef *DIR_pin_type, uint16_t DIR_pin_num_,
		TIM_HandleTypeDef encoder_timer_, uint32_t encoder_timer_channel_, int encoder_dir_,
		TIM_HandleTypeDef pwm_timer_, uint32_t pwm_timer_channel_);
#endif

#endif /* INC_CONTROL_H_ */
