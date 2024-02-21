/* 
 * File:   hal_timer1.h
 * Author: Ahmed Mohamed
 *
 * Created on 20 December 2023, 01:47 ?
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H
/*Section: Includes */
#include <xc.h>
#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/hal_gpio.h"
 
/* Section: Macro Declarations */


//timer1 modes timer | counter
#define TIMER1_TIMER_MODE           0x00U
#define TIMER1_COUNTER_MODE         0x01U

/*timer 1 sync and async modes */
#define TIMER1_ASYNC_COUNTER_MODE   0x01U
#define TIMER1_SYNC_COUNTER_MODE    0x00U
/*Timer1 oscillator*/
#define TIMER1_OSC_ENABLE           0x01U
#define TIMER1_OSC_DISABLE          0x00U
/* 16-Bit Read/Write Mode Enable bit */
#define TIMER1_RW_16BIT_MODE        0x01U
#define TIMER1_RW_8BIT_MODE         0x00U


/* Section: Macro Functions Declarations */

/**
 * @brief Configure Timer1 enables to work : Timer1 On/Off Control bit
 * @note TIMER1_Enable() : will enable timer1
 * @note TIMER1_Disable(): will disable timer1
 */
#define TIMER1_Enable()                   ( T1CONbits.TMR1ON = 1)
#define TIMER1_Disable()                  ( T1CONbits.TMR1ON = 0)

/**
 * @brief Configure Timer1 Synchronization Mode : Timer1 External Clock Input Synchronization Select
 * @note TIMER1_Async_CounterModeEnable() : Do not synchronize external clock input 
 * @note TIMER1_Sync_CounterModeEnable()  : Synchronize external clock input
 */
#define TIMER1_Async_CounterModeEnable()  (T1CONbits.T1SYNC = 1)
#define TIMER1_Sync_CounterModeEnable()   (T1CONbits.T1SYNC = 0)

/**
 * @brief Configure Timer1 Oscillator
 * @note TIMER1_Async_CounterModeEnable() :  Timer1 oscillator is enabled  
 * @note TIMER1_Sync_CounterModeEnable()  :  Timer1 oscillator is shut off
 */
#define TIMER1_Osc_Enable()               (T1CONbits.T1OSCEN = 1)
#define TIMER1_Osc_Disable()              (T1CONbits.T1OSCEN = 0)

/**
 * @brief 16-Bit Read/Write Mode Enable bit
 * @note  TIMER1_8bitModeEnable() : Enables register read/write of Timer1 in two 8-bit operations
 * @note  TIMER1_16bitModeEnable(): Enables register read/write of TImer1 in one 16-bit operation
 */

#define TIMER1_8bitModeEnable()          (T1CONbits.RD16 = 0)
#define TIMER1_16bitModeEnable()         (T1CONbits.RD16 = 1)

/**
 * @brief Configure Timer1 Clock source Internal or External : Timer0 Clock Source Select bit
 * @note TIMER1_TimerMode() :  External cock source (Transition on T1CKI pin) 
 * @note TIMER1_CounterMode():   Internal instruction cycle clock source (CLKO)
 */
#define TIMER1_TimerModeEnable()         (T1CONbits.TMR1CS = 0)
#define TIMER1_CounterModeEnable()       (T1CONbits.TMR1CS = 1)

/**/
#define TIMER1_SystemClkStatus()         (T1CONbits.T1RUN)  

/* Section: Data Type Declarations */

/*prescaler values*/
typedef enum {
    TIMER1_PRESCALER_DIV_1 = 0,      /* 00 = 1:1   Prescale value */
    TIMER1_PRESCALER_DIV_2,          /* 01 = 1:2   Prescale value */
    TIMER1_PRESCALER_DIV_4,          /* 10 = 1:4   Prescale value */
    TIMER1_PRESCALER_DIV_8,          /* 11 = 1:8   Prescale value */    
}timer1_prescaler_select_t;

typedef struct {

#if TIMER1_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE    
    void (*Timer1_InterruptHandler)(void);
    Interrupt_priority_cfg  priority;
#endif    
    timer1_prescaler_select_t Prescale_value;
    uint16 timer1_preload_value;             /* preloaded value */
    uint8  timer1_osc_cfg             :1;    /* timer1 oscillator status  */
    uint8  timer1_mode                :1;    /* timer1 as timer or counter */
    uint8  timer1_counter_mode        :1;    /* sync or async */
    uint8  timer1_re_wr_mode          :1;    /* 8-bit or 16-bit read/write */
    uint8  timer1_reserved            :4; 
    
    
}timer1_config_t;

/* Section: Function Declarations */
Std_ReturnType Timer1_Init(const timer1_config_t *_timer);
Std_ReturnType Timer1_DeInit(const timer1_config_t *_timer);
Std_ReturnType Timer1_Write_Value(const timer1_config_t *_timer , uint16 _value);
Std_ReturnType Timer1_Read_Value(const timer1_config_t *_timer , uint16 *_value);



#endif	/* HAL_TIMER0_H */

