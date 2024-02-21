/* 
 * File:   hal_ccp1.h
 * Author: Ahmed Mohamed
 *
 * Created on 05 Junuary 2024, 11:11 ?
 */


#include "hal_ccp1.h"


#if CCP1_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
static void (*CCP1InterruptHandler) (void) = NULL;
#endif

static void ccp1_timer_select(const ccp1_config_t *_ccp1_obj);

Std_ReturnType CCP1_Init(const ccp1_config_t *_ccp1){
 Std_ReturnType ret = E_OK;
    
    if(NULL == _ccp1)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /*First thing we will Disable CCP1 Module */
        CCP1_SET_MODE(CCP1_MODULE_DISABLED);
        
        /*CCP1 module Capture mode Initialization*/
        
        /*CCP1 module Capture mode Initialization*/
        if(CCP1_CAPTURE_MODE_SELECTED == _ccp1->ccp1_mode)
        {
            #if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_CAPTURE_MODE_SELECTED)
            
            switch(_ccp1->ccp1_mode_variant)
            {
                case CCP1_CAPTURE_MODE_1_FALLING_EDGE:
                   CCP1_SET_MODE(CCP1_CAPTURE_MODE_1_FALLING_EDGE);
                   break;
                case CCP1_CAPTURE_MODE_1_RISING_EDGE:
                   CCP1_SET_MODE(CCP1_CAPTURE_MODE_1_RISING_EDGE);
                   break;
                case CCP1_CAPTURE_MODE_4_RISING_EDGE:
                   CCP1_SET_MODE(CCP1_CAPTURE_MODE_4_RISING_EDGE);
                   break;
                case CCP1_CAPTURE_MODE_16_RISING_EDGE:
                   CCP1_SET_MODE(CCP1_CAPTURE_MODE_16_RISING_EDGE);
                   break;
                default: ret = E_NOT_OK;    /* Not supported variant */  
            }
            
            #endif 
        }

        
        
        /*CCP1 module Compare mode Initialization*/
        else if(CCP1_COMPARE_MODE_SELECTED == _ccp1->ccp1_mode)
            
        {
        #if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_COMPARE_MODE_SELECTED)        

            switch(_ccp1->ccp1_mode_variant)
            {
                case CCP1_COMPARE_MODE_TOGGLE_ON_MATCH:
                   CCP1_SET_MODE(CCP1_COMPARE_MODE_TOGGLE_ON_MATCH);
                   break;
                case CCP1_COMPARE_MODE_SET_PIN_LOW:
                   CCP1_SET_MODE(CCP1_COMPARE_MODE_SET_PIN_LOW);
                   break;
                case CCP1_COMPARE_MODE_SET_PIN_HIGH:
                   CCP1_SET_MODE(CCP1_COMPARE_MODE_SET_PIN_HIGH);
                   break;
                case CCP1_COMPARE_MODE_GEN_SW_INTERRUPT:
                   CCP1_SET_MODE(CCP1_COMPARE_MODE_GEN_SW_INTERRUPT);
                   break;
                case CCP1_COMPARE_MODE_SP_EVENT:
                   CCP1_SET_MODE(CCP1_COMPARE_MODE_SP_EVENT);
                   break;   
                default: ret = E_NOT_OK;  
            }
        #endif 
            ccp1_timer_select(_ccp1);
        }
       
        
        else if(CCP1_PWM_MODE_SELECTED == _ccp1->ccp1_mode)
        {
         #if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_PWM_MODE_SELECTED)        /*CCP1 module PWM mode Initialization*/

            if(CCP1_PWM_MODE == _ccp1->ccp1_mode)
                
                CCP1_SET_MODE(CCP1_PWM_MODE);
            else {/*Nothing*/}
            
            PR2 = (uint8)(((_XTAL_FREQ)/ ((_ccp1->PWM_Frequency) * (4.0) * (_ccp1->Postscale_value)
                         * (_ccp1->Prescale_value))) - 1) ;
           
            #endif  
        }
        
        else{/*Nothing*/}
      
         /*Initialize CCP1 pin */
         ret = gpio_pin_intialize(&(_ccp1->pin));
        
        /*Interrupt configuration*/   
    #if CCP1_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
        CCP1_InterruptEnable(); 
        CCP1_InterruptClearFlag();
        CCP1InterruptHandler = _ccp1->ccp1_InterruptHander;
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _ccp1->priority){
            /*High global interrupt enable*/
            INTERRUPT_globalInterruptHighEnable();
            CCP1_HighPriorityEnable();
        }
        else if (INTERRUPT_LOW_PRIORITY == _ccp1->priority) {
            /*Low global interrupt enable*/
            INTERRUPT_globalInterruptLowEnable();
            CCP1_LowPriorityEnable();
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
Std_ReturnType CCP1_DeInit(const ccp1_config_t *_ccp1){

    Std_ReturnType ret = E_OK;
    
    if(NULL == _ccp1)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /*Disable CCP1 module*/
      CCP1_SET_MODE(CCP1_MODULE_DISABLED);
#if CCP1_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
      CCP1_InterruptDisable();
#endif      
       
    
    }
    
    return ret;




}
#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_CAPTURE_MODE_SELECTED)

Std_ReturnType CCP1_IsCapturedReady(uint8 *_capture_status){
    Std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _capture_status)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(CCP1_CAPTURE_MODE_READY ==PIR1bits.CCP1IF )
        {
            *_capture_status = CCP1_CAPTURE_MODE_READY;
            CCP1_InterruptClearFlag();
        }
        
        else{_capture_status = CCP1_CAPTURE_MODE_NOT_READY;}  
   
        ret = E_OK;
    }
    
    return ret;

}
Std_ReturnType CCP1_Capture_Mode_Read_value(uint16 *_captured_value){

    Std_ReturnType ret = E_NOT_OK;
    CCP1_PERIOD_REG_T Temp_value = {.ccp1_low = 0 , .ccp1_high = 0};
    
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
#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_COMPARE_MODE_SELECTED)
 
Std_ReturnType CCP1_IsComppareCompete(uint8 *_compare_status){
    Std_ReturnType ret = E_OK;
    
    if(NULL == _compare_status)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
    
        if(CCP1_COMPARE_MODE_READY ==PIR1bits.CCP1IF )
        {
            *_compare_status = CCP1_COMPARE_MODE_READY;
            CCP1_InterruptClearFlag();
        }
        
        else{_compare_status = CCP1_COMPARE_MODE_NOT_READY;}  
   
        ret = E_OK;
    }
    
    return ret;


}
Std_ReturnType CCP1_Compare_Mode_Set_value(uint16 _compare_value){
  Std_ReturnType ret = E_NOT_OK;

  CCP1_PERIOD_REG_T Temp_value = {.ccp1_low = 0 , .ccp1_high = 0};

  Temp_value.ccp1_16bit = _compare_value;
  
  CCPR1L = Temp_value.ccp1_low;
  CCPR1H = Temp_value.ccp1_high;
  ret = E_OK;
  


  return ret;
}

#endif

#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_PWM_MODE_SELECTED)

Std_ReturnType CCP1_PWM_Set_Duty(const uint8 _duty){

    Std_ReturnType ret = E_OK;
    uint16 l_duty_temp = ZERO_INIT;
    l_duty_temp = (uint16)((4*(PR2+1)) * (_duty / 100.0));
    CCP1CONbits.DC1B = (uint8)(l_duty_temp  & (0x0003));  /* Get the 2 - LSB in DC1B in CCP1CON register*/
    CCPR1L = (uint8)(l_duty_temp>>2);                    /*store the rest of the duty temp in CCPR1L register*/



  return ret;


}
Std_ReturnType CCP1_PWM1_Start(void){

    Std_ReturnType ret = E_NOT_OK;
    CCP1_SET_MODE(CCP1_PWM_MODE);
    ret = E_OK;
    return ret;


}
Std_ReturnType CCP1_PWM1_Stop(void){

    CCP1_SET_MODE(CCP1_MODULE_DISABLED);
     Std_ReturnType ret = E_OK;



  return ret;


}



#endif
static void ccp1_timer_select(const ccp1_config_t *_ccp1_obj)
{
    if(CCP1_CCP2_Timer1 == _ccp1_obj->ccp1_timer)
    {
         /* Timer1 is the capture/compare clock source for the CCP modules*/
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_Timer1_CCP2_Timer3 == _ccp1_obj->ccp1_timer)
    {
        /*Timer3 is the capture/compare clock source for CCP2;
        Timer1 is the capture/compare clock source for CCP1*/  
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    
     else if(CCP1_CCP2_Timer3 == _ccp1_obj->ccp1_timer)
    {
        /*Timer3 is the capture/compare clock source for the CCP modules*/
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
     else {/*Nothing*/}
}

void CCP1_ISR(void){

    /* Clear flag of the interrupt*/
    CCP1_InterruptClearFlag();

    if(CCP1InterruptHandler)
        
    {CCP1InterruptHandler();}

}