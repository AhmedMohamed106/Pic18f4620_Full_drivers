/* 
 * File:   mcal_internal_interrupt.h
 * Author: Ahmed mohamed
 *
 * Created on 09 dec 2023, 04:22 ?
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H


/* section: Includes */
#include "mcal_interrupt_config.h"

/* Section : Includes */

/* Section: Macro Declarations */
#if ADC_INTERRUPT_STATUS == ADC_INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the ADC module */
#define ADC_InterruptEnable()   (PIE1bits.ADIE = 1)
/* this routine disables the interrupt for the ADC module */
#define ADC_InterruptDisable()   (PIE1bits.ADIE = 0)
/*this routine  clears the flag of ADC module */
#define ADC_InterruptClearFlag()  (PIR1bits.ADIF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the ADC priority as high priority*/
#define ADC_HighPriorityEnable()  (IPR1bits.ADIP = 1)
/*this routine configure the ADC priority as low priority*/
#define ADC_LowPriorityEnable()  (IPR1bits.ADIP = 0)
#endif

#endif

#if TIMER0_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the Timer0 module */
#define TIMER0_InterruptEnable()    (INTCONbits.TMR0IE = 1)
/* this routine disables the interrupt for the Timer0 module */
#define TIMER0_InterruptDisable()   (INTCONbits.TMR0IE = 0)
/*this routine  clears the flag of ADC module */
#define TIMER0_InterruptClearFlag() (INTCONbits.TMR0IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the Timer0 priority as high priority*/
#define TIMER0_HighPriorityEnable()  (INTCON2bits.TMR0IP = 1)
/*this routine configure the Timer0 priority as low priority*/
#define TIMER0_LowPriorityEnable()   (INTCON2bits.TMR0IP = 0)
#endif
#endif

#if TIMER1_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the Timer1 module */
#define TIMER1_InterruptEnable()    (PIE1bits.TMR1IE = 1)
/* this routine disables the interrupt for the Timer1 module */
#define TIMER1_InterruptDisable()   (PIE1bits.TMR1IE = 0)
/*this routine  clears the flag of ADC module */
#define TIMER1_InterruptClearFlag() (PIR1bits.TMR1IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the Timer1 priority as high priority*/
#define TIMER1_HighPriorityEnable()  (IPR1bits.TMR1IP = 1)
/*this routine configure the Timer1 priority as low priority*/
#define TIMER1_LowPriorityEnable()   (IPR1bits.TMR1IP = 0)
#endif
#endif


#if TIMER2_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the Timer2 module */
#define TIMER2_InterruptEnable()    (PIE1bits.TMR2IE = 1)
/* this routine disables the interrupt for the Timer2 module */
#define TIMER2_InterruptDisable()   (PIE1bits.TMR2IE = 0)
/*this routine  clears the flag of Timer2 module */
#define TIMER2_InterruptClearFlag() (PIR1bits.TMR2IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the Timer2 priority as high priority*/
#define TIMER2_HighPriorityEnable()  (IPR1bits.TMR2IP = 1)
/*this routine configure the Timer2 priority as low priority*/
#define TIMER2_LowPriorityEnable()   (IPR1bits.TMR2IP = 0)
#endif
#endif

#if TIMER3_INTERRUPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the Timer3 module */
#define TIMER3_InterruptEnable()    (PIE2bits.TMR3IE = 1)
/* this routine disables the interrupt for the Timer3 module */
#define TIMER3_InterruptDisable()   (PIE2bits.TMR3IE = 0)
/*this routine  clears the flag of Timer3 module */
#define TIMER3_InterruptClearFlag() (PIR2bits.TMR3IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the Timer3 priority as high priority*/
#define TIMER3_HighPriorityEnable()  (IPR2bits.TMR3IP = 1)
/*this routine configure the Timer3 priority as low priority*/
#define TIMER3_LowPriorityEnable()   (IPR2bits.TMR3IP = 0)
#endif
#endif


#if CCP1_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the CCP1 module */
#define CCP1_InterruptEnable()    (PIE1bits.CCP1IE = 1)
/* this routine disables the interrupt for the CCP1 module */
#define CCP1_InterruptDisable()   (PIE1bits.CCP1IE = 0)
/*this routine  clears the flag of CCP1 module */
#define CCP1_InterruptClearFlag() (PIR1bits.CCP1IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the CCP1 priority as high priority*/
#define CCP1_HighPriorityEnable()  (IPR1bits.CCP1IP = 1)
/*this routine configure the CCP1 priority as low priority*/
#define CCP1_LowPriorityEnable()   (IPR1bits.CCP1IP = 0)
#endif
#endif



#if CCP2_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the CCP2 module */
#define CCP2_InterruptEnable()    (PIE2bits.CCP2IE = 1)
/* this routine disables the interrupt for the CCP2 module */
#define CCP2_InterruptDisable()   (PIE2bits.CCP2IE = 0)
/*this routine  clears the flag of CCP2 module */
#define CCP2_InterruptClearFlag() (PIR2bits.CCP2IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the CCP2 priority as high priority*/
#define CCP2_HighPriorityEnable()  (IPR2bits.CCP2IP = 1)
/*this routine configure the CCP2 priority as low priority*/
#define CCP2_LowPriorityEnable()   (IPR2bits.CCP2IP = 0)
#endif
#endif


#if EUSART_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the EUSART TX module */
#define EUSART_TxInterruptEnable()    (PIE1bits.TXIE = 1)
/* this routine disables the interrupt for the EUSART TX  module */
#define EUSART_TxInterruptDisable()   (PIE1bits.TXIE = 0)
/*this routine  clears the flag of EUSART TX  module */
#define EUSART_TxInterruptFlag()      (PIR1bits.TXIF)
/* this routine enables the interrupt for the EUSART RX module */
#define EUSART_RxInterruptEnable()    (PIE1bits.RCIE = 1)
/* this routine disables the interrupt for the EUSART RX  module */
#define EUSART_RxInterruptDisable()   (PIE1bits.RCIE = 0)
/*this routine  clears the flag of EUSART RX  module */
#define EUSART_RxInterruptFlag()      (PIR1bits.RCIF)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*this routine configure the EUSART Tx priority as high priority*/
#define EUSART_TxHighPriorityEnable()  (IPR1bits.TXIP = 1)
/*this routine configure the  EUSART Tx priority as low priority*/
#define EUSART_TxLowPriorityEnable()   (IPR1bits.TXIP = 0)

/*this routine configure the EUSART Rx priority as high priority*/
#define EUSART_RxHighPriorityEnable()  (IPR1bits.RCIP = 1)
/*this routine configure the EUSART Rx priority as low priority*/
#define EUSART_RxLowPriorityEnable()   (IPR1bits.RCIP = 0)
#endif
#endif


#if MSSP_I2C_INTERRPT_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the I2c module */
#define MSSP_I2c_MasterInterruptEnable()           (PIE1bits.SSPIE = 1)
/* this routine disables the interrupt for the I2c module */
#define MSSP_I2c_MasterInterruptDisable()          (PIE1bits.SSPIE = 0)
/* this routine  clears the flag of the I2c module */
#define MSSP_I2c_MasterInterruptClearFlag()        (PIR1bits.SSPIF = 0)
#if MSSP_I2C_BUS_COLLISION_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine enables the interrupt for the I2c module */
#define MSSP_I2c_ColisionInterruptEnable()         (PIE2bits.BCLIE = 1)
/* this routine disables the interrupt for the I2c module */
#define MSSP_I2c_ColisionInterruptDisable()        (PIE2bits.BCLIE = 0)
/* this routine  clears the flag of the I2c module */
#define MSSP_I2c_ColisionInterruptClearFlag()      (PIR2bits.BCLIF = 0)
#endif
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/* this routine configure the I2c module priority as high priority*/
#define MSSP_I2c_MasterHighPriorityEnable()  (IPR1bits.SSPIP = 1)
/* this routine configure the I2c module priority as low priority*/
#define MSSP_I2c_MasterLowPriorityEnable()   (IPR1bits.SSPIP = 0)
#if MSSP_I2C_BUS_COLLISION_STATUS == INTERRUPT_FEATURE_ENABLE
/* this routine configure the I2c module priority as high priority*/
#define MSSP_I2c_ColisionHighPriorityEnable()  (IPR2bits.BCLIP = 1)
/ *this routine configure the the I2c module as low priority*/
#define MSSP_I2c_ColisionLowPriorityEnable()   (IPR2bits.BCLIP = 0)
#endif        
#endif
#endif


/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */

/* Section: Function Declarations */

#endif	/* MCAL_INTERNAL_INTERRUPT_H */

