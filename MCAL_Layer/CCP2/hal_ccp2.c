/* 
 * File:   hal_ccp2.h
 * Author: Ahmed Mohamed
 *
 * Created on 05 Junuary 2024, 11:11 ?
 */


#include "hal_ccp2.h"


#if CCP2_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
static void (*CCP2InterruptHandler) (void) = NULL;
#endif

static void ccp2_timer_select(ccp2_config_t * _ccp2_obj);


Std_ReturnType CCP2_Init(const ccp2_config_t *_ccp2){
 Std_ReturnType ret = E_OK;
    
    if(NULL == _ccp2)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /*First thing we will Disable CCP2 Module */
        CCP2_SET_MODE(CCP2_MODULE_DISABLED);
        
        /*CCP1 module Capture mode Initialization*/
        
        /*CCP1 module Capture mode Initialization*/
        if(CCP2_CAPTURE_MODE_SELECTED == _ccp2->ccp2_mode)
        {
            #if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_CAPTURE_MODE_SELECTED)
            
            switch(_ccp2->ccp2_mode_variant)
            {
                case CCP2_CAPTURE_MODE_1_FALLING_EDGE:
                   CCP2_SET_MODE(CCP2_CAPTURE_MODE_1_FALLING_EDGE);
                   break;
                case CCP2_CAPTURE_MODE_1_RISING_EDGE:
                   CCP2_SET_MODE(CCP2_CAPTURE_MODE_1_RISING_EDGE);
                   break;
                case CCP2_CAPTURE_MODE_4_RISING_EDGE:
                   CCP2_SET_MODE(CCP2_CAPTURE_MODE_4_RISING_EDGE);
                   break;
                case CCP2_CAPTURE_MODE_16_RISING_EDGE:
                   CCP2_SET_MODE(CCP2_CAPTURE_MODE_16_RISING_EDGE);
                   break;
                default: ret = E_NOT_OK;    /* Not supported variant */  
            }
            
            #endif 
            ccp2_timer_select(_ccp2);
        }

        
        
        /*CCP1 module Compare mode Initialization*/
        else if(CCP2_COMPARE_MODE_SELECTED == _ccp2->ccp2_mode)
            
        {
        #if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_COMPARE_MODE_SELECTED)        

            switch(_ccp2->ccp2_mode_variant)
            {
                case CCP2_COMPARE_MODE_TOGGLE_ON_MATCH:
                   CCP2_SET_MODE(CCP2_COMPARE_MODE_TOGGLE_ON_MATCH);
                   break;
                case CCP2_COMPARE_MODE_SET_PIN_LOW:
                   CCP2_SET_MODE(CCP2_COMPARE_MODE_SET_PIN_LOW);
                   break;
                case CCP2_COMPARE_MODE_SET_PIN_HIGH:
                   CCP2_SET_MODE(CCP2_COMPARE_MODE_SET_PIN_HIGH);
                   break;
                case CCP2_COMPARE_MODE_GEN_SW_INTERRUPT:
                   CCP2_SET_MODE(CCP2_COMPARE_MODE_GEN_SW_INTERRUPT);
                   break;
                case CCP2_COMPARE_MODE_SP_EVENT:
                   CCP2_SET_MODE(CCP2_COMPARE_MODE_SP_EVENT);
                   break;   
                default: ret = E_NOT_OK;  
            }
        #endif  
        }
       
        
        else if(CCP2_PWM_MODE_SELECTED == _ccp2->ccp2_mode)
        {
         #if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_PWM_MODE_SELECTED)        /*CCP2 module PWM mode Initialization*/

            if(CCP2_PWM_MODE == _ccp2->ccp2_mode)
                
                CCP2_SET_MODE(CCP2_PWM_MODE);
            else {/*Nothing*/}
            
            PR2 = (uint8)(((_XTAL_FREQ)/ ((_ccp2->PWM_Frequency) * (4.0) * (_ccp2->Postscale_value)
                         * (_ccp2->Prescale_value))) - 1) ;
           
            #endif  
        }
        
        else{/*Nothing*/}
      
         /*Initialize CCP2 pin */
         ret = gpio_pin_intialize(&(_ccp2->pin));
        
        /*Interrupt configuration*/   
    #if CCP2_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
        CCP2_InterruptEnable(); 
        CCP2_InterruptClearFlag();
        CCP2InterruptHandler = _ccp2->ccp2_InterruptHander;
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _ccp2->priority){
            /*High global interrupt enable*/
            INTERRUPT_globalInterruptHighEnable();
            CCP2_HighPriorityEnable();
        }
        else if (INTERRUPT_LOW_PRIORITY == _ccp2->priority) {
            /*Low global interrupt enable*/
            INTERRUPT_globalInterruptLowEnable();
            CCP2_LowPriorityEnable();
        }
        else {/*Nothing*/}
    #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralsInterruptEnable();
    #endif     
    #endif 

    }
    
    return ret;


}
Std_ReturnType CCP2_DeInit(const ccp2_config_t *_ccp2){

    Std_ReturnType ret = E_OK;
    
    if(NULL == _ccp2)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /*Disable CCP2 module*/
      CCP2_SET_MODE(CCP2_MODULE_DISABLED);
#if CCP2_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
      CCP2_InterruptDisable();
#endif      
       
    
    }
    
    return ret;




}
#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_CAPTURE_MODE_SELECTED)

Std_ReturnType CCP2_IsCapturedReady(uint8 *_capture_status){
    Std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _capture_status)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP2_CAPTURE_MODE_READY ==PIR1bits.CCP1IF )
        {
            *_capture_status = CCP2_CAPTURE_MODE_READY;
            CCP2_InterruptClearFlag();
        }
        
        else{_capture_status = CCP2_CAPTURE_MODE_NOT_READY;}  
   
        ret = E_OK;
    }
    
    return ret;

}
Std_ReturnType CCP2_Capture_Mode_Read_value(uint16 *_captured_value){

    Std_ReturnType ret = E_NOT_OK;
    CCP2_PERIOD_REG_T Temp_value = {.ccp1_low = 0 , .ccp1_high = 0};
    
    if(NULL == _captured_value)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        Temp_value.ccp1_low = CCPR1L;
        Temp_value.ccp1_high = CCPR1H;
        *_captured_value = Temp_value.ccp1_16bit;
    
        ret = E_OK;
    }
    
    return ret;




}

#endif
#if (CCP1_CFG_SELECTED_MODE == CCP2_CFG_COMPARE_MODE_SELECTED)
 
Std_ReturnType CCP2_IsComppareCompete(uint8 *_compare_status){
    Std_ReturnType ret = E_OK;
    
    if(NULL == _compare_status)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
    
        if(CCP2_COMPARE_MODE_READY ==PIR2bits.CCP2IF )
        {
            *_compare_status = CCP2_COMPARE_MODE_READY;
            CCP2_InterruptClearFlag();
        }
        
        else{_compare_status = CCP2_COMPARE_MODE_NOT_READY;}  
   
        ret = E_OK;
    }
    
    return ret;


}
Std_ReturnType CCP2_Compare_Mode_Set_value(uint16 _compare_value){
  Std_ReturnType ret = E_NOT_OK;

  CCP2_PERIOD_REG_T Temp_value = {.ccp2_low = 0 , .ccp2_high = 0};

  Temp_value.ccp2_16bit = _compare_value;
  
  CCPR2L = Temp_value.ccp2_low;
  CCPR2H = Temp_value.ccp2_high;
  ret = E_OK;
  


  return ret;
}

#endif

#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_PWM_MODE_SELECTED)

Std_ReturnType CCP2_PWM2_Set_Duty(const uint8 _duty){

    Std_ReturnType ret = E_OK;
    uint16 l_duty_temp = ZERO_INIT;
    l_duty_temp = (uint16)((4*(PR2+1)) * (_duty / 100.0));
    CCP2CONbits.DC2B = (uint8)(l_duty_temp  & (0x0003));  /* Get the 2 - LSB in DC1B in CCP2CON register*/
    CCPR2L = (uint8)(l_duty_temp>>2);                    /*store the rest of the duty temp in CCPR1L register*/



  return ret;


}
Std_ReturnType CCP2_PWM2_Start(void){

    Std_ReturnType ret = E_NOT_OK;
    CCP2_SET_MODE(CCP2_PWM_MODE);
    ret = E_OK;
    return ret;


}

Std_ReturnType CCP2_PWM1_Stop(void){

    CCP2_SET_MODE(CCP2_MODULE_DISABLED);
     Std_ReturnType ret = E_OK;



  return ret;


}



#endif

static void ccp2_timer_select(ccp2_config_t * _ccp2_obj){

    if(CCP1_CCP2_Timer1_ == _ccp2_obj->ccp2_timer)
    {
      /* Timer1 is the capture/compare clock source for the CCP modules*/
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_Timer1_CCP2_Timer3_ == _ccp2_obj->ccp2_timer)
    {
        /*Timer3 is the capture/compare clock source for CCP2;
        Timer1 is the capture/compare clock source for CCP1*/  
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    
     else if(CCP1_CCP2_Timer3_ == _ccp2_obj->ccp2_timer)
    {
        /*Timer3 is the capture/compare clock source for the CCP modules*/
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
     else {/*Nothing*/}
}



void CCP2_ISR(void){

    /* Clear flag of the interrupt*/
    CCP2_InterruptClearFlag();

    if(CCP2InterruptHandler)
        
    {CCP2InterruptHandler();}

}