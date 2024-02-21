/* 
 * File:   hal_ccp1.h
 * Author: Ahmed Mohamed
 *
 * Created on 05 Junuary 2024, 11:08 ?
 */

#ifndef HAL_CCP2_H
#define	HAL_CCP2_H


/* Section : Includes */
#include "ccp2_cfg.h"
#include "pic18f4620.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/hal_gpio.h"
#include "../Timer2/hal_timer2.h"
/* Section: --------------- Macro Declarations ------------------ */


/** 
 * @brief : CCPxM3:CCPxM0: CCPx Module Mode Select bits 
*/

/* 0000 = Capture/Compare/PWM disabled (resets CCPx module) */
#define CCP2_MODULE_DISABLED                 ((uint8) 0x00) 
/* 0010 = Compare mode, toggle output on match (CCPxIF bit is set) */
#define CCP2_COMPARE_MODE_TOGGLE_ON_MATCH     ((uint8)0x02)
/* 0100 = Capture mode, every falling edge  */ 
#define CCP2_CAPTURE_MODE_1_FALLING_EDGE      ((uint8)0x04)
/* 0101 = Capture mode, every rising edge   */
#define CCP2_CAPTURE_MODE_1_RISING_EDGE       ((uint8)0x05)
/* 0110 = Capture mode, every 4th rising edge */
#define CCP2_CAPTURE_MODE_4_RISING_EDGE       ((uint8)0x06)
/* 0111 = Capture mode, every 16th rising edge */
#define CCP2_CAPTURE_MODE_16_RISING_EDGE      ((uint8)0x07)
/* 1000 = Compare mode, initialize CCPx pin low; on compare match, force CCPx pin high (CCPxIF bit is set) */
#define CCP2_COMPARE_MODE_SET_PIN_LOW         ((uint8)0x08)
/* 1001 = Compare mode, initialize CCPx pin high; on compare match, force CCPx pin low (CCPxIF bit is set) */
#define CCP2_COMPARE_MODE_SET_PIN_HIGH        ((uint8)0x09)
/* 1010 = Compare mode, generate software interrupt on compare match */
#define CCP2_COMPARE_MODE_GEN_SW_INTERRUPT    ((uint8)0x0A)
/* 1011 = Compare mode, trigger special event; reset timer; CCPx match starts A/D conversion*/
#define CCP2_COMPARE_MODE_SP_EVENT            ((uint8)0x0B)
/* 11xx = PWM mode*/
#define CCP2_PWM_MODE                         ((uint8)0x0C)

/* CCP2 capture mode state */
#define CCP2_CAPTURE_MODE_READY                        0x01
#define CCP2_CAPTURE_MODE_NOT_READY                    0x00

/* CCP2 compare mode state */
#define CCP2_COMPARE_MODE_READY                        0x01
#define CCP2_COMPARE_MODE_NOT_READY                    0x00

/* Section:--------------- Macro Functions Declarations --------------- */
#define CCP2_SET_MODE(_CONFIG) (CCP2CONbits.CCP2M = _CONFIG)

/* Section: --------------- Data Type Declarations --------------- */
/*Timer2 prescaler values for CCP2*/
typedef enum {
    CCP2_TIMER2_PRESCALER_DIV_1  = 1,           /* 00 = 1:1   Prescale value */
    CCP2_TIMER2_PRESCALER_DIV_4  = 4,           /* 10 = 1:4   Prescale value */
    CCP2_TIMER2_PRESCALER_DIV_16 = 16,          /* 11 = 1:16  Prescale value */    
}ccp2_timer2_prescaler_select_t;

/*Timer2 postscaler values for CCP2*/
typedef enum {
    CCP2_TIMER2_POSTSCALER_DIV_1 = 1,    
    CCP2_TIMER2_POSTSCALER_DIV_2,          
    CCP2_TIMER2_POSTSCALER_DIV_3,         
    CCP2_TIMER2_POSTSCALER_DIV_4,
    CCP2_TIMER2_POSTSCALER_DIV_5,
    CCP2_TIMER2_POSTSCALER_DIV_6,
    CCP2_TIMER2_POSTSCALER_DIV_7,
    CCP2_TIMER2_POSTSCALER_DIV_8,
    CCP2_TIMER2_POSTSCALER_DIV_9,
    CCP2_TIMER2_POSTSCALER_DIV_10,
    CCP2_TIMER2_POSTSCALER_DIV_11,
    CCP2_TIMER2_POSTSCALER_DIV_12,
    CCP2_TIMER2_POSTSCALER_DIV_13,
    CCP2_TIMER2_POSTSCALER_DIV_14,
    CCP2_TIMER2_POSTSCALER_DIV_15,
    CCP2_TIMER2_POSTSCALER_DIV_16       
}ccp2_timer2_postscaler_select_t;
typedef enum {
    CCP2_CAPTURE_MODE_SELECTED = 0,    /*capture mode select*/
    CCP2_COMPARE_MODE_SELECTED,        /*compare mode select*/
    CCP2_PWM_MODE_SELECTED             /*PWM select*/
}ccp2_mode_t;

typedef enum {
    CCP1_CCP2_Timer1_ = 0,    /* Timer1 is the capture/compare clock source for the CCP modules*/     
    
    CCP1_Timer1_CCP2_Timer3_, /*Timer3 is the capture/compare clock source for CCP2;
                             Timer1 is the capture/compare clock source for CCP1*/       
    CCP1_CCP2_Timer3_         /*Timer3 is the capture/compare clock source for the CCP modules*/             
}ccp2_timer_t;

/**
 * 
 * @Summary      Defines the values to convert from 16-bit to 8-bit and vice versa
 * @Description  used to get 8-bit two values from 16-bit and also 8-bit two values are combined to get 16-bit 
 * 
 */
typedef union {
    
    struct 
    {
        uint8 ccp2_low;   
        uint8 ccp2_high;
    };
    
    struct 
    {
        uint16 ccp2_16bit;
    };
}CCP2_PERIOD_REG_T;

/* CCP2 module configuration Struct type */
typedef struct {
#if CCP2_INTERRPT_STATUS  == INTERRUPT_FEATURE_ENABLE  
void ( * ccp2_InterruptHander)(void);
Interrupt_priority_cfg priority;
#endif
pin_config_t pin;
ccp2_mode_t  ccp2_mode;
uint8        ccp2_mode_variant;
ccp2_timer_t ccp2_timer;
#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_PWM_MODE_SELECTED)
uint32 PWM_Frequency;
ccp2_timer2_prescaler_select_t Prescale_value;
ccp2_timer2_postscaler_select_t Postscale_value;


#endif


    
}ccp2_config_t;
/* Section: --------------- Function Declarations --------------- */
Std_ReturnType CCP2_Init(const ccp2_config_t *_ccp2);
Std_ReturnType CCP2_DeInit(const ccp2_config_t *_ccp2);

#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_CAPTURE_MODE_SELECTED)

Std_ReturnType CCP2_IsCapturedReady(uint8 *_capture_status);
Std_ReturnType CCP2_Capture_Mode_Read_value(uint16 *_captured_value);

#endif
#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_COMPARE_MODE_SELECTED)
 
Std_ReturnType CCP2_IsComppareCompete(uint8 *_compare_status);
Std_ReturnType CCP2_Compare_Mode_Set_value(uint16 _compare_value);

#endif

#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_PWM_MODE_SELECTED)

Std_ReturnType CCP2_PWM2_Set_Duty(const uint8 _duty);
Std_ReturnType CCP2_PWM2_Start(void);
Std_ReturnType CCP2_PWM2_Stop(void);
#endif

#endif	/* HAL_CCP2_H */

