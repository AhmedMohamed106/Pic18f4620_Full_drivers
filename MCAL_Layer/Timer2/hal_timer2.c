/* 
 * File:   hal_timer0.c
 * Author: Ahmed Mohamed
 *
 * Created on 20 December 2023, 01:47 ?
 */
#include "hal_timer2.h"

#if TIMER2_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
static void (*Timer2InterruptHandler) (void) = NULL;
#endif
static uint8 timer2_preload = ZERO_INIT;

static inline void Timer2_Pre_post_scaler_Config(const timer2_config_t * _timer);



/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType Timer2_Init(const timer2_config_t *_timer){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
        /*Disable the Timer2 module */
        TIMER2_Disable();
        /*timer1 prescaler configuration*/
        Timer2_Pre_post_scaler_Config(_timer);
        TMR2 = (uint8)(_timer->timer2_preload_value);
        timer2_preload = _timer->timer2_preload_value;
    #if TIMER2_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptEnable(); 
        TIMER2_InterruptClearFlag();
        Timer2InterruptHandler = _timer->Timer2_InterruptHandler;
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            /*High global interrupt enable*/
            INTERRUPT_globalInterruptHighEnable();
            TIMER2_HighPriorityEnable();
        }
        else if (INTERRUPT_LOW_PRIORITY == _timer->priority) {
            /*Low global interrupt enable*/
            INTERRUPT_globalInterruptLowEnable();
            TIMER2_LowPriorityEnable();
        }
        else {/*Nothing*/}
    #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralsInterruptEnable();
    #endif     
    #endif 
        TIMER2_Enable();
        ret = E_OK;
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType Timer2_DeInit(const timer2_config_t *_timer){ 
Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }

    else
    {
      /*Disable timer 1*/
        TIMER2_Disable();
     #if TIMER1_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptDisable();
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
Std_ReturnType Timer2_Write_Value(const timer2_config_t *_timer , uint8 _value){ 
Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR2 = (uint8)(_value);
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
Std_ReturnType Timer2_Read_Value(const timer2_config_t *_timer , uint8 *_value){ 
Std_ReturnType ret = E_NOT_OK;
    if(NULL== _timer || NULL== _value)
    {
        ret = E_NOT_OK;
    }

    else
    {

       *_value = TMR2;
       ret = E_OK;
      
        
    }
    return ret;
}

static inline void Timer2_Pre_post_scaler_Config(const timer2_config_t * _timer){

    T2CONbits.T2CKPS  = _timer->Prescale_value;
    T2CONbits.TOUTPS  = _timer->Postscale_value;
}




void TIMER2_ISR(void){

    /*Clear the interrupt flag*/
    TIMER2_InterruptClearFlag();
    TMR2 = (uint8)(timer2_preload);
    
    
    /*execute some mcal layer code */
    
    /*configure the callack function*/
    if(Timer2InterruptHandler)
    {Timer2InterruptHandler();}

}
