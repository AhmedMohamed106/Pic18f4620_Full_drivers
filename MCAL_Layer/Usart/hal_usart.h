/* 
 * File:   hal_usart.h
 * Author: Ahmed Mohamed
 *
 * Created on 16 junuary 2024, 02:14 ?
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/* Section : -------------Includes -----------------------*/
#include "hal_usart_cfg.h"
#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* Section: --------------Macro Declarations-------------- */

/* Represent the mode EUSART will work on */
#define EUSART_ASYNC_MODE             1     
#define EUSART_SYNC_MODE              0
#define EUSART_MODE             EUSART_ASYNC_MODE
/* EUSART transmit enable*/
#define EUSART_ASYNC_TX_ENABLE                  1
#define EUSART_ASYNC_TX_DISABLE                 0
/* EUSART interrupt transmit enable*/
#define EUSART_ASYNC_TX_INTERRUPT_ENABLE        1
#define EUSART_ASYNC_TX_INTERRUPT_DISABLE       0
/*EUSART 9th-Bit transmit enable*/
#define EUSART_ASYNC_9BIT_TX_ENABLE             1
#define EUSART_ASYNC_9BIT_TX_DISABLE            0
/* EUSART receive enable */
#define EUSART_ASYNC_RX_ENABLE        1
#define EUSART_ASYNC_RX_DISABLE       0
/* EUSART interrupt transmit enable*/
#define EUSART_ASYNC_RX_INTERRUPT_ENABLE        1
#define EUSART_ASYNC_RX_INTERRUPT_DISABLE       0
/*EUSART 9th-Bit receive enable*/
#define EUSART_ASYNC_9BIT_RX_ENABLE   1
#define EUSART_ASYNC_9BIT_RX_DISABLE  0
/*Framing Error detecting */
#define EUSART_FRAMING_ERROR_DETECTED 1
#define EUSART_FRAMING_ERROR_CEARED   0
/* Overrun Error detecting */
#define EUSART_OVERRUN_ERROR_DETECTED 1
#define EUSART_OVERRUN_ERROR_CEARED   0
/* Section: -------------Macro Functions Declarations ---------------*/

/**
 * @brief BRGH: High Baud Rate Select bit
 * @note EUSART_Async_HighSpeed() :  High speed
 * @note  EUSART_Async_LowSpeed() :  Low speed
 */
#define EUSART_Async_HighSpeed() (TXSTAbits.BRGH = 1)
#define EUSART_Async_LowSpeed()  (TXSTAbits.BRGH = 0)

/**
 * @brief BRG16: 16-Bit Baud Rate Register Enable bit
 * @note  EUSART_16Bit_BaudRate() :  16-bit Baud Rate Generator ? SPBRGH and SPBRG
 * @note  EUSART_8Bit_BaudRate() :  8-bit Baud Rate Generator ? SPBRG only (Compatible mode), SPBRGH value ignored
 */
#define EUSART_16Bit_BaudRate() (BAUDCONbits.BRG16 = 1)
#define EUSART_8Bit_BaudRate()  (BAUDCONbits.BRG16 = 0)



/**
 * @brief SYNC: EUSART Mode Select bit
 * @note EUSART_AsyncModeEnable() :  Asynchronous mode
 * @note  EUSART_SyncModeEnable() :  Synchronous mode
 */
#define EUSART_AsyncModeEnable()  (TXSTAbits.SYNC = 0)
#define EUSART_SyncModeEnable()   (TXSTAbits.SYNC = 1)

/**
 * @brief Serial Port Enable bit
 * @note  EUSART_Enable()   :  Master mode (clock generated internally from BRG)
 * @note  EUSART_Disable()  :  Slave mode (clock from external source)
 */
#define EUSART_Enable()      (RCSTAbits.SPEN = 1)
#define EUSART_Disable()     (RCSTAbits.SPEN = 0)

/**
 * @brief CSRC: Clock Source Select bit
 * @note  Asynchronous mode: Don?t care. Work in synchronous mode only
 * @note  EUSART_MasterMode() :  Master mode (clock generated internally from BRG)
 * @note  EUSART_SlaveMode()  :  Slave mode (clock from external source)
 */
#define EUSART_MasterMode()    (TXSTAbits.CSRC = 1)
#define EUSART_SlaveMode()     (TXSTAbits.CSRC = 0)

/**
 * @brief SENDB: Send Break Character bit
 * @note  synchronous mode: Don?t care. Work in Asynchronous mode only
 * @note  EUSART_SyncBreakSend()       : Send Sync Break on next transmission (cleared by hardware upon completion
 * @note  EUSART_SyncBreakCompleted()  : Sync Break transmission completed
 */
#define EUSART_SyncBreakSend()        (TXSTAbits.SENDB = 1)
#define EUSART_SyncBreakCompleted()   (TXSTAbits.SENDB = 0)

/* Transmit configuration : TXSTA */

/**
 * @brief TXEN: Transmit Enable bit
 * @note  EUSART_TransmitEnable()   :  Transmit enabled
 * @note  EUSART_TransmitDisable()  :  Transmit disabled
 */
#define EUSART_TransmitEnable()    (TXSTAbits.TXEN = 1)
#define EUSART_TransmitDisable()   (TXSTAbits.TXEN = 0)

/**
 * @brief TX9: 9-Bit Transmit Enable bit 
 * @note  EUSART_9Bit_TransmitEnable()   : Selects 9-bit transmission
 * @note  EUSART_9Bit_TransmitDisable()  : Selects 8-bit transmission
 */
#define EUSART_9Bit_TransmitEnable()    (TXSTAbits.TX9 = 1)
#define EUSART_9Bit_TransmitDisable()   (TXSTAbits.TX9 = 0)

/* Receive configuration : RCSTA */

/**
 * @brief RX9: 9-Bit Transmit Enable bit 
 * @note  EUSART_9Bit_ReceiveEnable()   : Selects 9-bit  reception 
 * @note  EUSART_9Bit_ReceiveDisable()  : Selects 8-bit  reception 
 */
#define EUSART_9Bit_ReceiveEnable()    (RCSTAbits.RX9 = 1)
#define EUSART_9Bit_ReceiveDisable()   (RCSTAbits.RX9 = 0)

/**
 * @brief CREN: Continuous Receive Enable bit 
 * @note Asynchronous mode and synchronous mode
 * @note  EUSART_ReceiveEnable()   :  Enables receiver 
 * @note  EUSART_ReceiveDisable()  :  Disables receiver 
 */
#define EUSART_ReceiveEnable()    (RCSTAbits.CREN = 1)
#define EUSART_ReceiveDisable()   (RCSTAbits.CREN = 0)

/**
 * @brief SREN: Single Receive Enable bit 
 * @note  Synchronous mode ? Master:
 * @note  EUSART_SingleReceiveEnable()   :  Enables single receive 
 * @note  EUSART_SingleReceiveDisable()  :  Disables single receive 
 */
#define EUSART_SingleReceiveEnable()    (RCSTAbits.SREN = 1)
#define EUSART_SingleReceiveDisable()   (RCSTAbits.SREN = 0)


/* Section: -----------Data Type Declarations------------ */
typedef enum {
    EUSART_ASYNC_8BIT_LOW_SPEED,
    EUSART_ASYNC_8BIT_HIGH_SPEED,
    EUSART_ASYNC_16BIT_LOW_SPEED,        
    EUSART_ASYNC_16BIT_HIGH_SPEED,
    EUSART_SYNC_8BIT,
    EUSART_SYNC_16BIT        
}baudRate_gen_t;

typedef struct {
    uint8 transmit_enable        : 1;
    uint8 usart_9bit_tx_enable   : 1;
    uint8 tx_interrupt_enable    : 1; 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
    Interrupt_priority_cfg tx_priority
#endif    
    uint8 reserved               : 5;
    
}usart_tx_cfg_t;

typedef struct {
    uint8 recievce_enable        : 1;
    uint8 usart_9bit_rx_enable   : 1;
    uint8 rx_interrupt_enable    : 1; 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
    Interrupt_priority_cfg rx_priority   
#endif
    
    uint8 reserved               : 5;
    
}usart_rx_cfg_t;

typedef struct {
  usart_tx_cfg_t tx_cfg;
  usart_rx_cfg_t rx_cfg;
  uint16 baudrate;
  baudRate_gen_t  baudRate_gen;
#if EUSART_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
  void (*EUSART_DefaultTxInterruptHandler)    (void);
  void (*EUSART_DefaultRxInterruptHandler)    (void);
  void (*EUSART_FramingErrorInterruptHandler) (void);
  void (*EUSART_OverrunErrorInterruptHandler) (void);
#endif
  
}usart_t;
/* Section: -----------Function Declarations -------------*/
/**
 * 
 * @param eusart
 * @return 
 */
Std_ReturnType EUSART_Async_Init(const usart_t *eusart);
/**
 * 
 * @param eusart
 * @return 
 */
Std_ReturnType EUSART_Async_DeInit(const usart_t *eusart);
/**
 * 
 * @param eusart
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_Async_ReadByteBlocking(uint8 *_data);
/**
 * 
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_Async_ReadByteNonBlocking(uint8 *_data);

Std_ReturnType EUSART_Async_Rx_Restart(void);
/**
 * 
 * @param eusart
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_Async_WriteByteBlocking(uint8 _data);

/**
 * 
 * @param _data
 * @param str_lengh
 * @return 
 */
Std_ReturnType EUSART_Async_WriteStringBlocking(uint8 *_data , uint16 str_length);

Std_ReturnType EUSART_Async_WriteByteNonBlocking(uint8 _data);
Std_ReturnType EUSART_Async_WriteStringNonBlocking(uint8 *_data , uint16 str_length);


#endif	/* HAL_USART_H */

