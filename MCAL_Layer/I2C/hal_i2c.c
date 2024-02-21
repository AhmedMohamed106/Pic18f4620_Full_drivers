/* 
 * File:   hal_i2c.h
 * Author: Ahmed Mohamed
 *
 * Created on 01 Feb, 2024, 03:26 ?
 */


#include "hal_i2c.h"

/* Data type declaration */
static void (* MSSP_I2cMaster_receiver_overflow ) (void) = NULL;
#if MSSP_I2C_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
static void (* MSSP_I2cMasterInterruptHandler )   (void) = NULL;
#if MSSP_I2C_BUS_COLLISION_STATUS == INTERRUPT_FEATURE_ENABLE
static void (* MSSP_i2c_report_write_collision )  (void) = NULL;
#endif
#endif

/* Function declaration */
static void inline Master_Mode_Clk_Configuration(const mssp_i2c_t * i2c_obj);
static void inline MSSP_I2c_Interrupt_Configuration(const mssp_i2c_t * i2c_obj);

/**
 * 
 * @param i2c_obj
 * @return 
 */

Std_ReturnType MSSP_I2c_Init(const mssp_i2c_t * i2c_obj){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Disable MSSP module */
        MSSP_I2c_Disable();
        /* MSSP I2c select mode */   
        if(MSSP_I2C_MASTER_MODE == i2c_obj->i2c_cfg.i2c_mode)
        {
            /* MSSP Master mode configuration */
            /* MSSP Master mode clock configuration */
            Master_Mode_Clk_Configuration(i2c_obj);

        }

        else if (MSSP_I2C_SLAVE_MODE == i2c_obj->i2c_cfg.i2c_mode)
        {
            /* MSSP Slave mode configuration */
            /* MSSP Slave mode General call configuration */
            if(I2C_GENERAL_CALL_ENABLE == i2c_obj->i2c_cfg.i2c_general_call)
            {
                MSSP_I2c_GeneralCallEnable();
            }
            else if(I2C_GENERAL_CALL_DISABLE == i2c_obj->i2c_cfg.i2c_general_call)
            {
                MSSP_I2c_GeneralCallDisable();
            }
            else {/* Nothing */}
           
            /* Clear the right collision detect*/
            SSPCON1bits.WCOL =0;  /* No Collision */
            /* Clear the receive overflow indicator */
            SSPCON1bits.SSPOV = 0;
            /* Release the clock*/
            SSPCON1bits.CKP = 1;
            /* Assign the I2C Slave address*/
            SSPADD = i2c_obj->i2c_slave_Address; 
            /* I2c Slave mode*/
            SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
        }
        else {/* Nothing */}
        /* MSSP I2C GPIO configuration */
        TRISCbits.TRISC3 = 1;                /* Configure SCL -> input pin */
        TRISCbits.TRISC4 = 1;                /* Configure SDA -> input pin */     
        /* MSSP slew rate configuration */
        if(I2C_SLEW_RATE_ENABLED == i2c_obj->i2c_cfg.i2c_slew_rate)
        { MSSP_I2c_SlewRateEnable();}
        else if(I2C_SLEW_RATE_DISABLED == i2c_obj->i2c_cfg.i2c_slew_rate)
        { MSSP_I2c_SlewRateDisable();}
        else {/* Nothing */}
        /* MSSP SMBus configuration */
        if(I2C_SMBUS_ENABLED == i2c_obj->i2c_cfg.i2c_SMBus_Control)
        { MSSP_I2c_SMBusEnable();}
        else if(I2C_SMBUS_DISABLED == i2c_obj->i2c_cfg.i2c_SMBus_Control)
        { MSSP_I2c_SlewRateDisable();}
        else {/* Nothing */}
        MSSP_I2cMaster_receiver_overflow = i2c_obj->Master_receiver_overflow;

        
        /* Interrupt Configuration*/
#if MSSP_I2C_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE

        MSSP_I2c_Interrupt_Configuration(i2c_obj);
#endif        
        /* Enable MSSP module*/
        MSSP_I2c_Enable();
        ret = E_OK;
    }

    return ret;

}
/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType MSSP_I2c_DeInit(const mssp_i2c_t * i2c_obj){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /*Disable MSSP module */
         MSSP_I2c_Disable();
        /* Interrupt Configuration*/
#if MSSP_I2C_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
      MSSP_I2c_MasterInterruptDisable();
#if MSSP_I2C_BUS_COLLISION_STATUS == INTERRUPT_FEATURE_ENABLE      
      MSSP_I2c_ColisionInterruptDisable();
#endif      
#endif
        ret = E_OK;
    }
    return ret;
}
/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType MSSP_I2c_Master_Send_Start(const mssp_i2c_t * i2c_obj){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Initiates Start Condition on SDA and SCL Lines  */
        MSSP_I2c_StartConditionEnable();
        /* wait for the completion of start condition */
       while(SSPCON2bits.SEN);
        /* Clear the MSSP Interrupt Flag ->SSPIF  = 0 */
       PIR1bits.SSPIF = 0;
        /* Report the start condition detection*/
       if(I2C_START_BIT_DETECTED == SSPSTATbits.S)
       {
           ret = E_OK;
       }
       else {ret = E_NOT_OK;}
       
    }

    return ret;


}
/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType MSSP_I2c_Master_Send_Repeated_Start(const mssp_i2c_t * i2c_obj){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
      /* Initiates Repeated start Condition on SDA and SCL Lines  */
        MSSP_I2c_ReStartConditionEnable();
        /* wait for the completion of Repeated start condition */
       while(SSPCON2bits.PEN);
        /* Clear the MSSP Interrupt Flag ->SSPIF  = 0 */
       PIR1bits.SSPIF = 0;
    }

    return ret;
}
/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType MSSP_I2c_Master_Send_Stop(const mssp_i2c_t * i2c_obj){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj)
    {
        ret = E_NOT_OK;
    }
    else
    { 
        /* Initiates Stop Condition on SDA and SCL Lines  */
        MSSP_I2c_StopConditionEnable();
        /* wait for the completion of Stop condition */
       while(SSPCON2bits.PEN);
        /* Clear the MSSP Interrupt Flag ->SSPIF  = 0 */
       PIR1bits.SSPIF = 0;
        /* Report the Stop condition detection*/
       if(I2C_STOP_BIT_DETECTED == SSPSTATbits.P)
       {
           ret = E_OK;
       }
       else {ret = E_NOT_OK;}
    }

    return ret;
 

}
/**
 * 
 * @param i2c_obj
 * @param i2c_Value
 * @return 
 */
Std_ReturnType MSSP_I2c_Master_Write_Blocking(const mssp_i2c_t * i2c_obj , uint8 i2c_Value , uint8 *_ack){

    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj || NULL == _ack)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Write data to register : SSPBUF */
        while(SSPSTATbits.BF);
        SSPBUF = i2c_Value;
        /* wait transmission to be done */
        while (!PIR1bits.SSPIF); 
        /* Clear the MSSP Interrupt Flag ->SSPIF  = 0 */
       PIR1bits.SSPIF = 0;
       /* Report ACK from Slave*/
       if(0 == SSPCON2bits.ACKSTAT)
       {
           *_ack = 0;   /* Ack was done*/
          
       }
       else if (1 == SSPCON2bits.ACKSTAT)
       {
           *_ack = 1;    /* Not Ack*/
          
       }
       else {/* Nothing */}
       
        ret = E_OK;
    }

    return ret;
}
/**
 * 
 * @param i2c_obj
 * @param ack
 * @param i2c_Value
 * @return 
 */
Std_ReturnType MSSP_I2c_Master_Read_Blocking(const mssp_i2c_t * i2c_obj ,uint8 ack ,uint8 * i2c_Value){


    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj || NULL == i2c_Value)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Master mode receive enable*/
        MSSP_I2c_MasterReceiveEnable();
        /* wait for buffer full flag  : complete receiving byte */
         while(!SSPSTATbits.BF);
        /* copy the data register to buffer variable*/
         *i2c_Value = SSPBUF;
        /* send ack or non ack after read*/
         if(MSSP_I2C_MASTER_SEND_ACK == ack)
         {
             
            SSPCON2bits.ACKDT = 0;   /* Send Acknowledge*/
            MSSP_I2c_AckEnable();    /* Enable Acknowledge sequence*/
         
         }
         
         else if (MSSP_I2C_MASTER_SEND_NOT_ACK == ack)
         {
             SSPCON2bits.ACKDT = 1;   /* Send Not Acknowledge*/
             MSSP_I2c_AckEnable();    /* Enable Acknowledge sequence*/
         }
         
         else { /* Nothing  */}
    
        ret = E_OK;
    }

    return ret;


}
static void inline Master_Mode_Clk_Configuration(const mssp_i2c_t * i2c_obj){
    
    SSPCON1bits.SSPM =  i2c_obj->i2c_cfg.i2c_mode_cfg;    
    SSPADD = (uint8)((_XTAL_FREQ / (4.0  * (i2c_obj->i2c_clk))) - 1);
} 
static void inline MSSP_I2c_Interrupt_Configuration(const mssp_i2c_t * i2c_obj){

           
#if MSSP_I2C_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
        MSSP_I2c_MasterInterruptEnable(); 
        MSSP_I2c_MasterInterruptClearFlag();
#if MSSP_I2C_BUS_COLLISION_STATUS == INTERRUPT_FEATURE_ENABLE
        MSSP_I2c_ColisionInterruptEnable();
        MSSP_I2c_ColisionInterruptClearFlag();
        MSSP_i2c_report_write_collision = i2c_obj->i2c_report_write_collision;

#endif        
        MSSP_I2cMasterInterruptHandler = i2c_obj->Default_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelsEnable();
            if(INTERRUPT_HIGH_PRIORITY == i2c_obj->priority){
                /*High global interrupt enable*/
                INTERRUPT_globalInterruptHighEnable();
                MSSP_I2c_MasterHighPriorityEnable();
            }
            else if (INTERRUPT_LOW_PRIORITY == i2c_obj->priority) {
                /*Low global interrupt enable*/
                INTERRUPT_globalInterruptLowEnable();
                MSSP_I2c_MasterLowPriorityEnable();
            }
            else {/*Nothing*/}
#if MSSP_I2C_BUS_COLLISION_STATUS == INTERRUPT_FEATURE_ENABLE
            if(INTERRUPT_HIGH_PRIORITY == i2c_obj->Collision_priority){
                /*High global interrupt enable*/
                INTERRUPT_globalInterruptHighEnable();
                MSSP_I2c_ColisionHighPriorityEnable();
            }
            else if (INTERRUPT_LOW_PRIORITY == i2c_obj->Collision_priority) {
                /*Low global interrupt enable*/
                INTERRUPT_globalInterruptLowEnable();
                MSSP_I2c_ColisionLowPriorityEnable();
            }
            else {/*Nothing*/}       
#endif            
#else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralsInterruptEnable();
#endif     
#endif

    
} 
/* ISRS*/
void MSSP_I2C_ISR(void){
    /* Clear Flag*/
    MSSP_I2c_MasterInterruptClearFlag();
    /* Execute any hardware code*/
    /* call back function*/
    if(MSSP_I2cMasterInterruptHandler)
    {MSSP_I2cMasterInterruptHandler();}
    

    else {/* Nothing*/}   
}
void MSSP_I2C_COL_ISR(void){
    /* Clear Flag*/
    MSSP_I2c_ColisionInterruptClearFlag();
    /* Execute any hardware code*/
    /* call back function*/
    if(MSSP_i2c_report_write_collision)
    {MSSP_i2c_report_write_collision();}
    else {/* Nothing*/}
}