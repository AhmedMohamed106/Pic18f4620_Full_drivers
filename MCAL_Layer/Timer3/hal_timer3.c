/* 
 * File:   hal_timer0.c
 * Author: Ahmed Mohamed
 *
 * Created on 20 December 2023, 01:47 ?
 */
#include "hal_timer3.h"

#if TIMER3_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
static void (*Timer3InterruptHandler) (void) = NULL;
#endif
static uint16 timer3_preload = ZERO_INIT;

static inline void Timer3_Prescaler_Config(const timer3_config_t * _timer);
static inline void Timer3_Register_Size_Config(const timer3_config_t * _timer);
static inline void Timer3_Mode_Select(const timer3_config_t * _timer);





/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType Timer3_Init(const timer3_config_t *_timer){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
        /*Disable the Timer3 module */
        TIMER3_Disable();
        /*timer3 prescaler configuration*/
        Timer3_Prescaler_Config(_timer);
        /*timer1 timer/counter mode selection*/
        Timer3_Mode_Select(_timer);
        /*timer3 Register mode : 8-Bit mode or 16-Bit mode*/
        //Timer3_Register_Size_Config(_timer);
        TMR3H = ((_timer->timer3_preload_value) >> 8);
        TMR3L = (uint8)(_timer->timer3_preload_value);
        timer3_preload = _timer->timer3_preload_value;
    #if TIMER3_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptEnable(); 
        TIMER3_InterruptClearFlag();
        Timer3InterruptHandler = _timer->Timer3_InterruptHandler;
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            /*High global interrupt enable*/
            INTERRUPT_globalInterruptHighEnable();
            TIMER3_HighPriorityEnable();
        }
        else if (INTERRUPT_LOW_PRIORITY == _timer->priority) {
            /*Low global interrupt enable*/
            INTERRUPT_globalInterruptLowEnable();
            TIMER3_LowPriorityEnable();
        }
        else {/*Nothing*/}
    #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralsInterruptEnable();
    #endif     
    #endif 
        TIMER3_Enable();
        ret = E_OK;
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType Timer3_DeInit(const timer3_config_t *_timer){ 
Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
      /*Disable timer 1*/
        TIMER3_Disable();
     #if TIMER3_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptDisable();
     #endif
    }
    return ret;
}
/**
 * 
 * @param _timer
 * @param _value
 * @return 
 */
Std_ReturnType Timer3_Write_Value(const timer3_config_t *_timer , uint16 _value){ 
Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
        
        TMR3H = ((_value) >> 8);
        TMR3L = (uint8)(_value);
        ret = E_OK;
    
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @param _value
 * @return 
 */
Std_ReturnType Timer3_Read_Value(const timer3_config_t *_timer , uint16 *_value){ 
Std_ReturnType ret = E_NOT_OK;
uint8 l_timer = ZERO_INIT , h_timer = ZERO_INIT;
    if(NULL== _timer || NULL== _value)
    {
        ret = E_NOT_OK;
    }

    else
    {
       l_timer = TMR3L;  
       h_timer = TMR3H;
       *_value = (uint16)((h_timer << 8) + l_timer);
       ret = E_OK;
      
        
    }
    return ret;
}

static inline void Timer3_Prescaler_Config(const timer3_config_t * _timer){

    T3CONbits.T3CKPS = _timer->Prescale_value;
}

static inline void Timer3_Mode_Select(const timer3_config_t * _timer){
  if(TIMER3_TIMER_MODE == _timer->timer3_mode)
    {
        TIMER3_TimerModeEnable();
    }
    else if(TIMER3_COUNTER_MODE == _timer->timer3_mode)
    {
        TIMER3_CounterModeEnable();
        if(TIMER3_ASYNC_COUNTER_MODE == _timer->timer3_counter_mode)
        {
            TIMER3_Async_CounterModeEnable();
        }
        else if (TIMER3_SYNC_COUNTER_MODE == _timer->timer3_counter_mode)
        {
            TIMER3_Sync_CounterModeEnable();
        }
        
        else {/*Nothing*/}
    }
  
    else {/*Nothing*/}   
}
static inline void Timer3_Register_Size_Config(const timer3_config_t * _timer)
{
    if(TIMER3_RW_16BIT_MODE == _timer->timer3_re_wr_mode)
    {
        TIMER3_16bitModeEnable();
    }
    else if (TIMER3_RW_8BIT_MODE == _timer->timer3_re_wr_mode)
    {
        TIMER3_8bitModeEnable();
    }
    else
    {
        TIMER3_16bitModeEnable();
    }
   
}

void TIMER3_ISR(void){

    /*Clear the interrupt flag*/
    TIMER3_InterruptClearFlag();
    TMR3H = ((timer3_preload) >> 8);
    TMR3L = (uint8)(timer3_preload);
    
    
    /*execute some mcal layer code */
    
    /*configure the callack function*/
    if(Timer3InterruptHandler)
    {Timer3InterruptHandler();}

}
