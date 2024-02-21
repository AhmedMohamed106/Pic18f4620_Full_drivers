/* 
 * File:   hal_usart.h
 * Author: Ahmed Mohamed
 *
 * Created on 16 junuary 2024, 02:14 ?
 */

#include "hal_usart.h"
#if EUSART_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
static void (*EUSART_TxInterruptHandler)  (void) = NULL;
static void (*EUSART_RxInterruptHandler)  (void) = NULL; 
static void (*EUSART_FrameErrorHandler)   (void) = NULL; 
static void (*EUSART_OverrunErrorHandler) (void) = NULL; 
#endif
/* Helper functions*/
static void EUSART_TxInit(const usart_t *eusart);
static void EUSART_RxInit(const usart_t *eusart);
static void EUSART_BaudRate_Calculation(const usart_t *eusart);
/**
 * 
 * @param eusart
 * @return 
 */
Std_ReturnType EUSART_Async_Init(const usart_t *eusart){

    Std_ReturnType ret = E_NOT_OK;
    
    if(NULL == eusart)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /*Disable EUSART module*/
        EUSART_Disable();
        /* TRISC<7> bit must be set (= 1) */
        TRISCbits.RC7 = 1;
        /* TRISC<6> bit must be set (= 1) */
        TRISCbits.RC6 = 1;
        /*Configure baud rate */
        EUSART_BaudRate_Calculation(eusart);
        /*Initialize Transmit */
        if(EUSART_ASYNC_TX_ENABLE == eusart->tx_cfg.transmit_enable)
        {
            EUSART_TxInit(eusart);
        }
        /*Initialize receive*/
        if(EUSART_ASYNC_RX_ENABLE == eusart->rx_cfg.recievce_enable)
        {
           EUSART_RxInit(eusart);
        }
        
        /*Enable EUSART module*/
         EUSART_Enable();
            ret = E_OK;
        }
    return ret;

}
/**
 * 
 * @param eusart
 * @return 
 */
Std_ReturnType EUSART_Async_DeInit(const usart_t *eusart){

Std_ReturnType ret = E_NOT_OK;
    
    if(NULL == eusart)
    {
        ret = E_NOT_OK;
    }
    
    else
    {
    
         /*Disable EUSART module*/
        EUSART_Disable();
#if EUSART_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
        
#endif           
    
        ret = E_OK;
    }
    return ret;



}
/**
 * 
 * @param eusart
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_Async_ReadByteBlocking(uint8 *_data){

    Std_ReturnType ret = E_NOT_OK; 
    if(NULL == _data)
    {
        ret = E_NOT_OK;
    }
    else
    {
        while(!(PIR1bits.RCIF));
        *_data = RCREG;
        ret = E_OK;
    }
    return ret;
}
/**
 * 
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_Async_ReadByteNonBlocking(uint8 *_data){

    Std_ReturnType ret = E_NOT_OK;
    
    if(1 == PIR1bits.RCIF)
    {
        
    *_data = RCREG;
    ret = E_OK;
    }
    else  if(0 == PIR1bits.RCIF)
    {
        
    ret = E_NOT_OK;
    }

    return ret;
}

/**
 * 
 * @param eusart
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_Async_WriteByteBlocking(uint8 _data){

    Std_ReturnType ret = E_OK;
        /* check TRMT : shift register is empty */
        while(!(TXSTAbits.TRMT));
#if EUSART_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
        EUSART_TxInterruptEnable();
#endif        
        TXREG = _data;  
        
        ret = E_OK;
    return ret;



}

/**
 * 
 * @restart rx  
 */
Std_ReturnType EUSART_Async_Rx_Restart(void){

    Std_ReturnType ret = E_OK;
    
    EUSART_ReceiveDisable();    /* Disable the receiver*/
    EUSART_ReceiveEnable();     /* Enable the receiver*/
    return ret;
}


Std_ReturnType EUSART_Async_WriteStringBlocking(uint8 *_data , uint16 str_length){

    Std_ReturnType ret = E_NOT_OK;
    
    uint8 counter_char = ZERO_INIT;
  
    for (counter_char = ZERO_INIT; counter_char < str_length; counter_char++) {
       EUSART_Async_WriteByteBlocking(_data[counter_char]);

    }
    ret = E_OK;
    return ret;
}


Std_ReturnType EUSART_Async_WriteByteNonBlocking(uint8 _data){

     Std_ReturnType ret = E_OK;
     
    if(1 == PIR1bits.TXIF)
    {
        NOP();
        NOP();
        TXREG = _data;  
        ret = E_OK;
    }
     
    else 
    {    
    ret = E_NOT_OK;
    }
     
   
     
    return ret;

}
Std_ReturnType EUSART_Async_WriteStringNonBlocking(uint8 *_data , uint16 str_length){

     Std_ReturnType ret = E_NOT_OK;
    
    uint8 counter_char = ZERO_INIT;
  
    for (counter_char = ZERO_INIT; counter_char < str_length; counter_char++) {
       EUSART_Async_WriteByteNonBlocking(_data[counter_char]);

    }
    ret = E_OK;
    return ret;

}



static void EUSART_TxInit(const usart_t *eusart){

    /* Enable transmit module */
    EUSART_TransmitEnable();
     EUSART_TxInterruptHandler = eusart->EUSART_DefaultTxInterruptHandler;
    if(EUSART_ASYNC_TX_INTERRUPT_ENABLE == eusart->tx_cfg.tx_interrupt_enable)
    {
#if EUSART_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
    EUSART_TxInterruptEnable();
   
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == eusart->tx_cfg.tx_priority){
        /*High global interrupt enable*/
        INTERRUPT_globalInterruptHighEnable();
        EUSART_TxHighPriorityEnable();
    }
    else if (INTERRUPT_LOW_PRIORITY == eusart->tx_cfg.tx_priority) {
        /*Low global interrupt enable*/
        INTERRUPT_globalInterruptLowEnable();
        EUSART_TxLowPriorityEnable();
    }
    else {/*Nothing*/}
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralsInterruptEnable();
#endif     
#endif 
     }
 
     if(EUSART_ASYNC_9BIT_TX_ENABLE == eusart->tx_cfg.usart_9bit_tx_enable)
     {
         EUSART_9Bit_TransmitEnable();
     }
     else if(EUSART_ASYNC_9BIT_TX_DISABLE == eusart->tx_cfg.usart_9bit_tx_enable)
     {
         EUSART_9Bit_TransmitDisable();
     }
     else {/* Nothing */}
   
}
static void EUSART_RxInit(const usart_t *eusart){
   /* Enable receive module */
    EUSART_ReceiveEnable();
     EUSART_RxInterruptHandler  = eusart->EUSART_DefaultRxInterruptHandler;
     EUSART_FrameErrorHandler   = eusart->EUSART_FramingErrorInterruptHandler;
     EUSART_OverrunErrorHandler = eusart->EUSART_OverrunErrorInterruptHandler;
        if(EUSART_ASYNC_RX_INTERRUPT_ENABLE == eusart->rx_cfg.rx_interrupt_enable){

#if EUSART_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
    EUSART_RxInterruptEnable();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == eusart->rx_cfg.rx_priority){
        /*High global interrupt enable*/
        INTERRUPT_globalInterruptHighEnable();
        EUSART_RxHighPriorityEnable();
    }
    else if (INTERRUPT_LOW_PRIORITY == eusart->rx_cfg.rx_priority) {
        /*Low global interrupt enable*/
        INTERRUPT_globalInterruptLowEnable();
        EUSART_RxLowPriorityEnable();
    }
    else {/*Nothing*/}
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralsInterruptEnable();
#endif     
#endif 
        }
     if(EUSART_ASYNC_9BIT_RX_ENABLE == eusart->rx_cfg.usart_9bit_rx_enable)
     {
         EUSART_9Bit_ReceiveEnable();
     }
     else if(EUSART_ASYNC_9BIT_RX_DISABLE == eusart->rx_cfg.usart_9bit_rx_enable)
     {
         EUSART_9Bit_ReceiveDisable();
     }
     else {/* Nothing */}
   
   
}


static void EUSART_BaudRate_Calculation(const usart_t *eusart){

    uint16 temp = 0;
     switch(eusart->baudRate_gen)
    {
        case EUSART_ASYNC_8BIT_LOW_SPEED: 
             EUSART_AsyncModeEnable();
             EUSART_Async_LowSpeed();
             EUSART_8Bit_BaudRate();
             SPBRG = (uint8)(((_XTAL_FREQ) / ((float)(eusart->baudrate)*64)) - 1);
             break;
        case EUSART_ASYNC_8BIT_HIGH_SPEED: 
             EUSART_AsyncModeEnable();
             EUSART_Async_HighSpeed();
             EUSART_8Bit_BaudRate();
             SPBRG = (uint8)(((_XTAL_FREQ) / ((float)(eusart->baudrate)*16)) - 1);
             break; 
        case EUSART_ASYNC_16BIT_LOW_SPEED: 
             EUSART_AsyncModeEnable();
             EUSART_Async_LowSpeed();
             EUSART_16Bit_BaudRate();
             temp = (uint16)(((_XTAL_FREQ) / ((float)(eusart->baudrate)*16)) - 1);
             SPBRG  = (uint8)temp;
             SPBRGH = (uint8)(temp>>8);
             break;
        case EUSART_ASYNC_16BIT_HIGH_SPEED: 
             EUSART_AsyncModeEnable();
             EUSART_Async_HighSpeed();
             EUSART_16Bit_BaudRate();
             temp = (uint16)(((_XTAL_FREQ) / ((float)(eusart->baudrate)*4)) - 1);
             SPBRG  = (uint8)temp;
             SPBRGH = (uint8)(temp>>8);
             break;
        case EUSART_SYNC_8BIT: 
             EUSART_SyncModeEnable();
             EUSART_8Bit_BaudRate();
             SPBRG = (uint8)(((_XTAL_FREQ) / ((float)(eusart->baudrate)*4)) - 1);
             break;
        case EUSART_SYNC_16BIT: 
             EUSART_SyncModeEnable();
             EUSART_16Bit_BaudRate();
             temp = (uint16)(((_XTAL_FREQ) / ((float)(eusart->baudrate)*4)) - 1);
             SPBRG  = (uint8)temp;
             SPBRGH = (uint8)(temp>>8);
             break;
         default: ;      
    }       
}

void Tx_ISR(void){

    /* implement some HW code */
    EUSART_TxInterruptDisable();
    if(EUSART_TxInterruptHandler)
    {EUSART_TxInterruptHandler();}

}
void Rx_ISR(void){

    /* implement some HW code */
    
    /*check if rx handler is null or  not */
    if(EUSART_RxInterruptHandler)
    {EUSART_RxInterruptHandler();}
    /* check if framing error  handler is null or  not */    
    if(EUSART_FrameErrorHandler)
    {EUSART_FrameErrorHandler();}
    /* check if overrun error  handler is null or  not */        
    if(EUSART_OverrunErrorHandler)
    {EUSART_OverrunErrorHandler();}

}

