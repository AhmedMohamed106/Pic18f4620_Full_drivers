/* 
 * File:   hal_timer1.h
 * Author: Ahmed Mohamed
 *
 * Created on 28 December 2023, 01:47 ?
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H
/*Section: Includes */
#include <xc.h>
#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/hal_gpio.h"
 
/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/**
 * @brief Configure Timer2 enables to work : Timer2 On/Off Control bit
 * @note TIMER2_Enable() : will enable timer2
 * @note TIMER2_Disable(): will disable timer2
 */
#define TIMER2_Enable()                   ( T2CONbits.TMR2ON= 1)
#define TIMER2_Disable()                  ( T2CONbits.TMR2ON= 0)


/* Section: Data Type Declarations */

/*prescaler values*/
typedef enum {
    TIMER2_PRESCALER_DIV_1 = 0,      /* 00 = 1:1   Prescale value */
    TIMER2_PRESCALER_DIV_4,          /* 10 = 1:4   Prescale value */
    TIMER2_PRESCALER_DIV_16,         /* 11 = 1:16   Prescale value */    
}timer2_prescaler_select_t;

/*postscaler values*/
typedef enum {
    TIMER2_POSTSCALER_DIV_1 = 0,    
    TIMER2_POSTSCALER_DIV_2,          
    TIMER2_POSTSCALER_DIV_3,         
    TIMER2_POSTSCALER_DIV_4,
    TIMER2_POSTSCALER_DIV_5,
    TIMER2_POSTSCALER_DIV_6,
    TIMER2_POSTSCALER_DIV_7,
    TIMER2_POSTSCALER_DIV_8,
    TIMER2_POSTSCALER_DIV_9,
    TIMER2_POSTSCALER_DIV_10,
    TIMER2_POSTSCALER_DIV_11,
    TIMER2_POSTSCALER_DIV_12,
    TIMER2_POSTSCALER_DIV_13,
    TIMER2_POSTSCALER_DIV_14,
    TIMER2_POSTSCALER_DIV_15,
    TIMER2_POSTSCALER_DIV_16       
}timer2_postscaler_select_t;

typedef struct {

#if TIMER2_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE    
    void (*Timer2_InterruptHandler)(void);
    Interrupt_priority_cfg  priority;
#endif    
    timer2_prescaler_select_t  Prescale_value;
    timer2_postscaler_select_t Postscale_value;
    uint8 timer2_preload_value;            
    
}timer2_config_t;

/* Section: Function Declarations */
Std_ReturnType Timer2_Init(const timer2_config_t *_timer);
Std_ReturnType Timer2_DeInit(const timer2_config_t *_timer);
Std_ReturnType Timer2_Write_Value(const timer2_config_t *_timer , uint8 _value);
Std_ReturnType Timer2_Read_Value(const timer2_config_t *_timer , uint8 *_value);



#endif	/* HAL_TIMER2_H */

