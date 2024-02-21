/* 
 * File:   hal_timer0.c
 * Author: Ahmed Mohamed
 *
 * Created on 20 December 2023, 01:47 ?
 */
#include "hal_timer1.h"

#if TIMER1_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
static void (*Timer1InterruptHandler) (void) = NULL;
#endif
static uint16 timer1_preload = ZERO_INIT;

static inline void Timer1_Prescaler_Config(const timer1_config_t * _timer);
static inline void Timer1_Register_Size_Config(const timer1_config_t * _timer);
static inline void Timer1_Mode_Select(const timer1_config_t * _timer);





/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType Timer1_Init(const timer1_config_t *_timer){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
        /*Disable the Timer1 module */
        TIMER1_Disable();
        /*timer1 prescaler configuration*/
        Timer1_Prescaler_Config(_timer);
        /*timer1 timer/counter mode selection*/
        Timer1_Mode_Select(_timer);
        /*timer1 Register mode : 8-Bit mode or 16-Bit mode*/
        //Timer1_Register_Size_Config(_timer);
        TMR1H = ((_timer->timer1_preload_value) >> 8);
        TMR1L = (uint8)(_timer->timer1_preload_value);
        timer1_preload = _timer->timer1_preload_value;
    #if TIMER1_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable(); 
        TIMER1_InterruptClearFlag();
        Timer1InterruptHandler = _timer->Timer1_InterruptHandler;
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            /*High global interrupt enable*/
            INTERRUPT_globalInterruptHighEnable();
            TIMER1_HighPriorityEnable();
        }
        else if (INTERRUPT_LOW_PRIORITY == _timer->priority) {
            /*Low global interrupt enable*/
            INTERRUPT_globalInterruptLowEnable();
            TIMER1_LowPriorityEnable();
        }
        else {/*Nothing*/}
    #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralsInterruptEnable();
    #endif     
    #endif 
        TIMER1_Enable();
        ret = E_OK;
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType Timer1_DeInit(const timer1_config_t *_timer){ 
Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
      /*Disable timer 1*/
        TIMER1_Disable();
     #if TIMER1_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
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
Std_ReturnType Timer1_Write_Value(const timer1_config_t *_timer , uint16 _value){ 
Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
        
        TMR1H = ((_value) >> 8);
        TMR1L = (uint8)(_value);
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
Std_ReturnType Timer1_Read_Value(const timer1_config_t *_timer , uint16 *_value){ 
Std_ReturnType ret = E_NOT_OK;
uint8 l_timer = ZERO_INIT , h_timer = ZERO_INIT;
    if(NULL== _timer || NULL== _value)
    {
        ret = E_NOT_OK;
    }

    else
    {
       l_timer = TMR1L;  
       h_timer = TMR1H;
       *_value = (uint16)((h_timer << 8) + l_timer);
       ret = E_OK;
      
        
    }
    return ret;
}

static inline void Timer1_Prescaler_Config(const timer1_config_t * _timer){

    T1CONbits.T1CKPS = _timer->Prescale_value;
}

static inline void Timer1_Mode_Select(const timer1_config_t * _timer){
  if(TIMER1_TIMER_MODE == _timer->timer1_mode)
    {
        TIMER1_TimerModeEnable();
    }
    else if(TIMER1_COUNTER_MODE == _timer->timer1_mode)
    {
        TIMER1_CounterModeEnable();
        if(TIMER1_ASYNC_COUNTER_MODE == _timer->timer1_counter_mode)
        {
            TIMER1_Async_CounterModeEnable();
        }
        else if (TIMER1_SYNC_COUNTER_MODE == _timer->timer1_counter_mode)
        {
            TIMER1_Sync_CounterModeEnable();
        }
        
        else {/*Nothing*/}
    }
  
    else {/*Nothing*/}   
}
static inline void Timer1_Register_Size_Config(const timer1_config_t * _timer)
{
    if(TIMER1_RW_16BIT_MODE == _timer->timer1_re_wr_mode)
    {
        TIMER1_16bitModeEnable();
    }
    else if (TIMER1_RW_8BIT_MODE == _timer->timer1_re_wr_mode)
    {
        TIMER1_8bitModeEnable();
    }
    else
    {
        TIMER1_16bitModeEnable();
    }
   
}

void TIMER1_ISR(void){

    /*Clear the interrupt flag*/
    TIMER1_InterruptClearFlag();
    TMR1H = ((timer1_preload) >> 8);
    TMR1L = (uint8)(timer1_preload);
    
    
    /*execute some mcal layer code */
    
    /*configure the callack function*/
    if(Timer1InterruptHandler)
    {Timer1InterruptHandler();}

}
