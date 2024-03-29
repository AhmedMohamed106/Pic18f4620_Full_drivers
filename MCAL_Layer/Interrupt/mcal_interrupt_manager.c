/* 
 * File:   mcal_interrupt_manager.c
 * Author: Ahmed mohamed
 *
 * Created on 09 dec 2023, 04:24 ?
 */
#include "mcal_interrupt_manager.h"
#include "mcal_external_interrupt.h"
#include "mcal_internal_interrupt.h"

static volatile uint8 RB4_Flag = 1, RB5_Flag = 1, RB6_Flag = 1, RB7_Flag = 1;


#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE

void __interrupt() InterruptManagerHigh(void){
  if((INTERRUPT_ENABLE == INTCONbits.INT0IE)&&(INTERRUPT_OCCUR == INTCONbits.INT0IF))
    {INT0_ISR();}
    
    else {/*Nothing*/}
  
  if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF))
    {
        INT2_ISR(); /* External Interrupt 2 */
    }
    else{ /* Nothing */ }
    /* ============ INTx External Interrupt End ============ */
 /* ============ PortB External On Change Interrupt Start ============ */
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB4 == GPIO_HIGH) && (RB4_Flag == 1)){
        RB4_Flag = 0;
        RB4_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB4 == GPIO_LOW) && (RB4_Flag == 0)){
        RB4_Flag = 1;
        RB4_ISR(1);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB5 == GPIO_HIGH) && (RB5_Flag == 1)){
        RB5_Flag = 0;
        RB5_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB5 == GPIO_LOW) && (RB5_Flag == 0)){
        RB5_Flag = 1;
        RB5_ISR(1);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB6 == GPIO_HIGH) && (RB6_Flag == 1)){
        RB6_Flag = 0;
        RB6_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB6 == GPIO_LOW) && (RB6_Flag == 0)){
        RB6_Flag = 1;
        RB6_ISR(1);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB7 == GPIO_HIGH) && (RB7_Flag == 1)){
        RB7_Flag = 0;
        RB7_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB7 == GPIO_LOW) && (RB7_Flag == 0)){
        RB7_Flag = 1;
        RB7_ISR(1);
    }
    else{ /* Nothing */ }
    /* ============ PortB External On Change Interrupt End ============ */
    

}

void __interrupt(low_priority) InterruptManagerLow(void){

if((INTERRUPT_ENABLE == INTCON3bits.INT1E) && (INTERRUPT_OCCUR == INTCON3bits.INT1F))
    {
        INT1_ISR(); /* External Interrupt 1 */
    }
    else{ /* Nothing */ }

}  
#else
void __interrupt() InterruptManager(void){

    /* ============ INTx External Interrupt Start ============ */
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE)&&(INTERRUPT_OCCUR == INTCONbits.INT0IF))
    {INT0_ISR();}
    
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == INTCON3bits.INT1E) && (INTERRUPT_OCCUR == INTCON3bits.INT1F))
    {
        INT1_ISR(); /* External Interrupt 1 */
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF))
    {
        INT2_ISR(); /* External Interrupt 2 */
    }
    else{ /* Nothing */ }
    /* ============ INTx External Interrupt End ============ */
    
     /* ============ PortB External On Change Interrupt Start ============ */
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB4 == GPIO_HIGH) && (RB4_Flag == 1)){
        RB4_Flag = 0;
        RB4_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB4 == GPIO_LOW) && (RB4_Flag == 0)){
        RB4_Flag = 1;
        RB4_ISR(1);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB5 == GPIO_HIGH) && (RB5_Flag == 1)){
        RB5_Flag = 0;
        RB5_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB5 == GPIO_LOW) && (RB5_Flag == 0)){
        RB5_Flag = 1;
        RB5_ISR(1);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB6 == GPIO_HIGH) && (RB6_Flag == 1)){
        RB6_Flag = 0;
        RB6_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB6 == GPIO_LOW) && (RB6_Flag == 0)){
        RB6_Flag = 1;
        RB6_ISR(1);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB7 == GPIO_HIGH) && (RB7_Flag == 1)){
        RB7_Flag = 0;
        RB7_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && 
       (PORTBbits.RB7 == GPIO_LOW) && (RB7_Flag == 0)){
        //EXT_RBx_InterruptClearFlag();
        RB7_Flag = 1;
        RB7_ISR(1);
    }
    else{ /* Nothing */ }


    /* ============ PortB External On Change Interrupt End ============ */
    
    /* ============ ADC Interrupt Start ============ */
    if((INTERRUPT_ENABLE == PIE1bits.ADIE)&&(INTERRUPT_OCCUR == PIR1bits.ADIF)){
    
        ADC_InterruptClearFlag();
        ADC_ISR();
    }
    else{/*Nothing*/}
        /* ============ ADC Interrupt End ============ */

        /* ============  TIMER0 Interrupt Start ============ */
    
    if((INTERRUPT_ENABLE == INTCONbits.T0IE)&&(INTERRUPT_OCCUR == INTCONbits.T0IF)){
    
        TIMER0_ISR();
    }
    else{/*Nothing*/}
            /* ============  TIMER0 Interrupt End ============ */

            /* ============  TIMER1 Interrupt Start ============ */

    if((INTERRUPT_ENABLE == PIE1bits.TMR1IE)&&(INTERRUPT_OCCUR == PIR1bits.TMR1IF)){
    
        TIMER1_ISR();
    }
    else{/*Nothing*/}
                /* ============  TIMER1 Interrupt End ============ */

                /* ============  TIMER2 Interrupt Start ============ */

    if((INTERRUPT_ENABLE == PIE1bits.TMR2IE)&&(INTERRUPT_OCCUR == PIR1bits.TMR2IF)){
    
        TIMER2_ISR();
    }
    else{/*Nothing*/}
                /* ============  TIMER2 Interrupt End ============ */

    
                /* ============  TIMER3 Interrupt Start ============ */

    if((INTERRUPT_ENABLE == PIE2bits.TMR3IE)&&(INTERRUPT_OCCUR == PIR2bits.TMR3IF)){
    
        TIMER3_ISR();
    }
    else{/*Nothing*/}
                    /* ============  TIMER3 Interrupt End ============ */

                    /* ============   CCP1 Interrupt Start ============ */

     if((INTERRUPT_ENABLE == PIE1bits.CCP1IE)&&(INTERRUPT_OCCUR == PIR1bits.CCP1IF)){
    
        CCP1_ISR();
    }
    else{/*Nothing*/}
                        /* ============   CCP1 Interrupt End ============ */

                        /* ============   CCP2 Interrupt Start ============ */

     if((INTERRUPT_ENABLE == PIE2bits.CCP2IE)&&(INTERRUPT_OCCUR == PIR2bits.CCP2IF)){
    
        CCP2_ISR();
    }
    else{/*Nothing*/}
                            /* ============   CCP2 Interrupt End ============ */

                            /* ============   EUSART Interrupt Start ============ */

     if((INTERRUPT_ENABLE == PIE1bits.TXIE)&&(INTERRUPT_OCCUR == PIR1bits.TXIF)){
    
       Tx_ISR();
    }
    else{/*Nothing*/}
    
    if((INTERRUPT_ENABLE == PIE1bits.RCIE)&&(INTERRUPT_OCCUR == PIR1bits.RCIF)){
    
       Rx_ISR();
    }
    else{/*Nothing*/}
                            /* ============   EUSART Interrupt End ============ */
    
                            /* ============   MSSP I2C Interrupt Start ============ */

    if((INTERRUPT_ENABLE == PIE1bits.SSPIE)&&(INTERRUPT_OCCUR == PIR1bits.SSPIF)){
    
       MSSP_I2C_ISR();
    }
    else{/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE2bits.BCLIE)&&(INTERRUPT_OCCUR == PIR2bits.BCLIF)){
    
       MSSP_I2C_COL_ISR();
    }
    else{/*Nothing*/}
                                /* ============   MSSP I2C Interrupt End ============ */

}


#endif

