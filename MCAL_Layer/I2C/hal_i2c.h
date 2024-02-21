/* 
 * File:   hal_i2c.h
 * Author: Ahmed Mohamed
 *
 * Created on 01 Feb, 2024, 03:26 ?
 */

#ifndef HAL_I2C_H
#define	HAL_I2C_H

/* Section : ------------Includes--------------- */

#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "pic18f4620.h"
#include "../device_config.h"
#include "../mcal_std_types.h"

/* Section: --------------Macro Declarations-------------- */

/*(A) SSPM3:SSPM0: Master Synchronous Serial Port Mode Select bits */

/*0110 = I2C Slave mode, 7-bit address*/
#define MSSP_I2C_SLAVE_MODE_7BIT_ADDRESS                       0x06
/*0111 = I2C Slave mode, 10-bit address*/
#define MSSP_I2C_SLAVE_MODE_10BIT_ADDRESS                      0x07
/*1000 = I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))*/
#define MSSP_I2C_MASTER_MODE_CLOCK_FOSC                        0x08
/*1011 = I2C Firmware Controlled Master mode (Slave Idle)*/
#define MSSP_I2C_MASTER_MODE_FIRMWARE_CONTROLLED               0x0B
/*1110 = I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled*/
#define MSSP_I2C_SLAVE_MODE_7BIT_ADDRESS_INTERRUPT_ENABLED     0x0E
/*1111 = I2C Slave mode, 10-bit address with Start and Stop bit interrupts enabled*/
#define MSSP_I2C_SLAVE_MODE_10BIT_ADDRESS_INTERRUPT_ENABLED    0x0F
/* Slew rate control enabled */
#define I2C_SLEW_RATE_ENABLED                                  0
/* Slew rate control disabled */
#define I2C_SLEW_RATE_DISABLED                                 1
/* SMBus control enabled */
#define I2C_SMBUS_ENABLED                                      1
/* SMBus control disabled */
#define I2C_SMBUS_DISABLED                                     0
/*  Indicates that the last byte received or transmitted was data */
#define I2C_LAST_BYTE_DATA                                     0
/*  Indicates that the last byte received or transmitted was address */
#define I2C_LAST_BYTE_ADDRESS                                  0
/*  Indicates that a Stop bit has been detected last */
#define I2C_STOP_BIT_DETECTED                                  1
/*  Stop bit was not detected last */
#define I2C_STOP_BIT_NOT_DETECTED                              0
/*  Indicates that a Start bit has been detected last */
#define I2C_START_BIT_DETECTED                                 1
/*  Start bit was not detected last */
#define I2C_START_BIT_NOT_DETECTED                             0
/* General call address enable*/
#define I2C_GENERAL_CALL_ENABLE                                1
/* General call address disable*/
#define I2C_GENERAL_CALL_DISABLE                               0
/* Select i2c mode master*/
#define MSSP_I2C_MASTER_MODE                                   1
/* Select i2c mode slave*/
#define MSSP_I2C_SLAVE_MODE                                    0 
/* ACKDT: Acknowledge Data bit (Master Receive mode only) */
#define MSSP_I2C_MASTER_SEND_ACK                               0
#define MSSP_I2C_MASTER_SEND_NOT_ACK                           1 
/* Section: --------------Macro Functions Declarations--------------- */

/**
 * @brief CKP: SCK Release Control bit
 * @note In Slave mode:
 * @note MSSP_ClockRelease() : Releases clock
 * @note MSSP_ClockHoldLow() : Holds clock low (clock stretch), used to ensure data setup time
 */
#define MSSP_ClockRelease()                         (SSPCON1bits.CKP = 1)
#define MSSP_ClockHoldLow()                         (SSPCON1bits.CKP = 0)

/**
 * @brief SSPEN: Master Synchronous Serial Port Enable bit
 * @note In Slave mode:
 * @note MSSP_I2c_Enable()  : Enables the serial port and configures the SDA and SCL pins as the serial port pins
 * @note MSSP_I2c_Disable() : Disables serial port and configures these pins as I/O port pins
 */
#define MSSP_I2c_Enable()                          (SSPCON1bits.SSPEN = 1)
#define MSSP_I2c_Disable()                         (SSPCON1bits.SSPEN = 0)

/**
 * @brief SMP: Slew Rate Control bit
 * @note In Master or Slave mode:
 * @note  MSSP_I2c_SlewRateDisable() : Disables serial port and configures these pins as I/O port pins
 * @note  MSSP_I2c_SlewRateEnable() : Enables the serial port and configures the SDA and SCL pins as the serial port pins
 */
#define MSSP_I2c_SlewRateDisable()                 (SSPSTATbits.SMP = 1)
#define MSSP_I2c_SlewRateEnable()                  (SSPSTATbits.SMP = 0)

/**
 * @brief CKE: SMBus Select bit
 * @note In Master or Slave mode:
 * @note  MSSP_I2c_SMBusEnable()   : Enable SMBus specific inputs
 * @note  MSSP_I2c_SMBusDisable()  : Disable SMBus specific inputs
 */
#define MSSP_I2c_SMBusEnable()                    (SSPSTATbits.CKE = 1)
#define MSSP_I2c_SMBusDisable()                   (SSPSTATbits.CKE = 0)

/**
 * @brief GCEN: General Call Enable bit (Slave mode only) 
 * @note In Slave mode:
 * @note  MSSP_I2c_GeneralCallEnable()   : Enables interrupt when a general call address (0000h) is received in the SSPSR
 * @note  MSSP_I2c_GeneralCallDisable()  : General call address disabled.
 */
#define MSSP_I2c_GeneralCallEnable()                    (SSPCON2bits.GCEN = 1)
#define MSSP_I2c_GeneralCallDisable()                   (SSPCON2bits.GCEN = 0)

/**
 * @brief RCEN: Receive Enable bit (Master mode only) 
 * @note In Master mode:
 * @note  MSSP_I2c_MasterReceiveEnable()   : Enables Receive mode for I2C 
 * @note  MSSP_I2c_MasterReceiveDisable()  : Receive Idle
 */
#define MSSP_I2c_MasterReceiveEnable()                    (SSPCON2bits.RCEN = 1)
#define MSSP_I2c_MasterReceiveDisable()                   (SSPCON2bits.RCEN = 0)

/**
 * @brief SEN: Start Condition Enable/Stretch Enable bit 
 * @note In Master mode:
 *             1 = Initiates Start condition on SDA and SCL pins. Automatically cleared by hardware.
 *             0 = Start condition Idle
 * @note In Save mode:
 *              1 = Clock stretching is enabled for both slave transmit and slave receive (stretch enabled)
 *              0 = Clock stretching is disabled
 * @note  MSSP_I2c_StartConditionEnable()   : Enables start  condition / stretch I2C 
 * @note  MSSP_I2c_StartConditionDisable()  : Disables Start condition
 */
#define MSSP_I2c_StartConditionEnable()                    (SSPCON2bits.SEN = 1)
#define MSSP_I2c_StartConditionDisable()                   (SSPCON2bits.SEN = 0)

/**
 * @brief RSEN: Repeated Start Condition Enable bit (Master mode only) 
 * @note In Master mode:
 * @note  MSSP_I2c_ReStartConditionEnable()   :  Initiates Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware. 
 * @note  MSSP_I2c_ReStartConditionDisable()  :  Repeated Start condition Idle
 */
#define MSSP_I2c_ReStartConditionEnable()                    (SSPCON2bits.RSEN = 1)
#define MSSP_I2c_ReStartConditionDisable()                   (SSPCON2bits.RSEN = 0)


/**
 * @brief PEN: Stop Condition Enable bit (Master mode only) 
 * @note In Master mode
 * @note  MSSP_I2c_StartConditionEnable()   : Initiates Stop condition on SDA and SCL pins. Automatically cleared by hardware 
 * @note  MSSP_I2c_StartConditionDisable()  : Stop condition Idle
 */
#define MSSP_I2c_StopConditionEnable()                    (SSPCON2bits.PEN = 1)
#define MSSP_I2c_StopConditionDisable()                   (SSPCON2bits.PEN = 0)


/**
 * @brief ACKEN: Acknowledge Sequence Enable bit (Master Receive mode only)
 * @note In Master mode
 * @note  MSSP_I2c_AckEnable()   : Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. Automatically cleared by hardware.
 * @note  MSSP_I2c_AckDisable()  : Stop condition Idle
 */
#define MSSP_I2c_AckEnable()                    (SSPCON2bits.ACKEN = 1)
#define MSSP_I2c_AckDisable()                   (SSPCON2bits.ACKEN = 0)

/* Section: --------------Data Type Declarations--------------------- */

/*I2c configure*/
typedef struct {

    
    
    uint8 i2c_mode_cfg;            /* Configure the mode specifications of i2c */
    uint8 i2c_mode            :1;   /*MAster or slave mode*/
    uint8 i2c_slew_rate       :1;
    uint8 i2c_SMBus_Control   :1;
    uint8 i2c_Master_rec_mode :1;
    uint8 i2c_general_call    :1;
    uint8 reserved            :3;                    


}i2c_Config_t;

/*I2c Data type */
typedef struct {
   
    uint32 i2c_clk;                            /*Master Clock Frequency */
    i2c_Config_t i2c_cfg;                      /* basic configurations of i2c*/
    uint8 i2c_slave_Address;                   /* Slave address of the device*/
    void (* Master_receiver_overflow)(void);   /* Receiver overflow indicator */
#if MSSP_I2C_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
     void (* Default_InterruptHandler) (void);
    Interrupt_priority_cfg priority;
#if MSSP_I2C_BUS_COLLISION_STATUS == INTERRUPT_FEATURE_ENABLE
    void (*i2c_report_write_collision)(void);  /*Write collision indicator*/
    Interrupt_priority_cfg Collision_priority;
#endif      
#endif    

}mssp_i2c_t;

/* Section: ---------------Function Declarations---------------------- */
Std_ReturnType MSSP_I2c_Init(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2c_DeInit(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2c_Master_Send_Start(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2c_Master_Send_Repeated_Start(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2c_Master_Send_Stop(const mssp_i2c_t * i2c_obj);
Std_ReturnType MSSP_I2c_Master_Write_Blocking(const mssp_i2c_t * i2c_obj , uint8 i2c_Value ,uint8 *_ack);
Std_ReturnType MSSP_I2c_Master_Write_NonBlocking(const mssp_i2c_t * i2c_obj , uint8 i2c_Value ,uint8 *_ack);
Std_ReturnType MSSP_I2c_Master_Read_Blocking(const mssp_i2c_t * i2c_obj ,uint8 ack ,uint8 * i2c_Value);
Std_ReturnType MSSP_I2c_Master_Read_NonBlocking(const mssp_i2c_t * i2c_obj ,uint8 ack ,uint8 * i2c_Value);

#endif /* HAL_I2C_H */