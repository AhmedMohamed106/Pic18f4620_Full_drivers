/* 
 * File:   hal_timer3.h
 * Author: Ahmed Mohamed
 *
 * Created on 20 December 2023, 01:47 ?
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H
/*Section: Includes */
#include <xc.h>
#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/hal_gpio.h"
 
/* Section: Macro Declarations */


//timer 1 modes timer | counter
#define TIMER3_TIMER_MODE            0x00U
#define TIMER3_COUNTER_MODE          0x01U

/*timer 3 sync and async modes */
#define TIMER3_ASYNC_COUNTER_MODE    0x01U
#define TIMER3_SYNC_COUNTER_MODE     0x00U
/*Timer 1 oscillator*/
#define TIMER1_OSC_ENABLE            0x01U
#define TIMER1_OSC_DISABLE          0x00U
/* 16-Bit Read/Write Mode Enable bit */
#define TIMER3_RW_16BIT_MODE         0x01U
#define TIMER3_RW_8BIT_MODE          0x00U


/* Section: Macro Functions Declarations */

/**
 * @brief Configure Timer1 enables to work : Timer3 On/Off Control bit
 * @note TIMER3_Enable() :   will enable timer3
 * @note TIMER3_Disable():   will disable timer3
 */
#define TIMER3_Enable()                   ( T3CONbits.TMR3ON = 1)
#define TIMER3_Disable()                  ( T3CONbits.TMR3ON = 0)

/**
 * @brief Configure Timer3 Synchronization Mode : Timer3 External Clock Input Synchronization Select
 * @note TIMER3_Async_CounterModeEnable() :   Do not synchronize external clock input 
 * @note TIMER3_Sync_CounterModeEnable()  :   Synchronize external clock input
 */
#define TIMER3_Async_CounterModeEnable()  (T3CONbits.T3SYNC = 1)
#define TIMER3_Sync_CounterModeEnable()   (T3CONbits.T3SYNC = 0)

/**
 * @brief Configure Timer1 Oscillator
 * @note TIMER1_Async_CounterModeEnable() :   Timer1 oscillator is enabled  
 * @note TIMER1_Sync_CounterModeEnable()  :   Timer1 oscillator is shut off
 */
#define TIMER1_Osc_Enable()               (T1CONbits.T1OSCEN = 1)
#define TIMER1_Osc_Disable()              (T1CONbits.T1OSCEN = 0)

/**
 * @brief 16-Bit Read/Write Mode Enable bit
 * @note  TIMER3_8bitModeEnable() :   Enables register read/write of Timer1 in two 8-bit operations
 * @note  TIMER3_16bitModeEnable():   Enables register read/write of TImer1 in one 16-bit operation
 */

#define TIMER3_8bitModeEnable()          (T3CONbits.RD16 = 0)
#define TIMER3_16bitModeEnable()         (T3CONbits.RD16 = 1)

/**
 * @brief Configure Timer3 Clock source Internal or External : Timer3 Clock Source Select bit
 * @note TIMER3_TimerMode() :    External cock source (Transition on T1CKI pin) 
 * @note TIMER3_CounterMode():   Internal instruction cycle clock source (CLK1)
 */
#define TIMER3_TimerModeEnable()         (T3CONbits.TMR3CS = 0)
#define TIMER3_CounterModeEnable()       (T3CONbits.TMR3CS = 1)

/*CCP module and will be configured in it  */

/* Section: Data Type Declarations */

/*prescaler values*/
typedef enum {
    TIMER3_PRESCALER_DIV_1 = 0,      /* 00 = 1:1   Prescale value */
    TIMER3_PRESCALER_DIV_2,          /* 01 = 1:2   Prescale value */
    TIMER3_PRESCALER_DIV_4,          /* 10 = 1:4   Prescale value */
    TIMER3_PRESCALER_DIV_8,          /* 11 = 1:8   Prescale value */    
}timer3_prescaler_select_t;

typedef struct {

#if TIMER3_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE    
    void (*Timer3_InterruptHandler)(void);
    Interrupt_priority_cfg  priority;
#endif    
    timer3_prescaler_select_t Prescale_value;
    uint16 timer3_preload_value;             /* preloaded value */
    uint8  timer3_mode                :1;    /* timer3 as timer or counter */
    uint8  timer3_counter_mode        :1;    /* sync or async */
    uint8  timer3_re_wr_mode          :1;    /* 8-bit or 16-bit read/write */
    uint8  timer3_reserved            :5; 
    
    
}timer3_config_t;

/* Section: Function Declarations */
Std_ReturnType Timer3_Init(const timer3_config_t *_timer);
Std_ReturnType Timer3_DeInit(const timer3_config_t *_timer);
Std_ReturnType Timer3_Write_Value(const timer3_config_t *_timer , uint16 _value);
Std_ReturnType Timer3_Read_Value(const timer3_config_t *_timer , uint16 *_value);



#endif	/* HAL_TIMER3_H */

