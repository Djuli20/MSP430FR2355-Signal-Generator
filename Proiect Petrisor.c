//******************************************************************************
//  MSP430FR235x Demo - SAC-L3, DAC Buffer Mode
//
//  Description: Configure SAC-L3 for DAC Buffer Mode. Use the 12 bit DAC to
//  output positive ramp. The OA is set in buffer mode to improve DAC output
//  drive strength. Internal 2.5V reference is selected as DAC reference.
//  Observe the output of OA0O pin with oscilloscope.
//  ACLK = n/a, MCLK = SMCLK = default DCODIV ~1MHz.
//
//                MSP430FR235x
//             -------------------
//         /|\|                         |
//          | |                         |
//          --|RST     P1.1->DAC12->OA0O|--> oscilloscope
//            |                         |
//            |                         |
//
//******************************************************************************
#include <msp430.h>

unsigned int DAC_data=0;
unsigned int i=0;
unsigned int frcvsemnal=0;
int counter=0;
#define DAC_Resolution 4095

int incrementareCounter=1;
int optiuneButon=1;
double amplitudine = 4095; // Amplitudinea maxima a semnalului (adica valoarea maxima a DAC-ului)
double frecventa = 1000; // Frecventa semnalului în Hz
int triggerfrq=1000;
int frecvang;

unsigned int semnal[100]={1024,1088,1152,1216,1279,1340,1401,1460,
                          1517,1573,1626,1677,1725,1770,1813,1852,
                          1889,1921,1951,1976,1998,2016,2030,2040,
                          2046,2048,2046,2040,2030,2016,1998,1976,
                          1951,1921,1889,1852,1813,1770,1725,1677,
                          1626,1573,1517,1460,1401,1340,1279,1216,
                          1152,1088,1024,960,896,832,769,708,
                          647,588,531,475,422,371,323,278,
                          235,196,159,127,97,72,50,32,
                          18,8,2,0,2,8,18,32,
                          50,72,97,127,159,196,235,278,
                          323,371,422,475,531,588,647,708,
                          769,832,896,960,1024};
unsigned int semnalInitial[100]={1024,1088,1152,1216,1279,1340,1401,1460,
                          1517,1573,1626,1677,1725,1770,1813,1852,
                          1889,1921,1951,1976,1998,2016,2030,2040,
                          2046,2048,2046,2040,2030,2016,1998,1976,
                          1951,1921,1889,1852,1813,1770,1725,1677,
                          1626,1573,1517,1460,1401,1340,1279,1216,
                          1152,1088,1024,960,896,832,769,708,
                          647,588,531,475,422,371,323,278,
                          235,196,159,127,97,72,50,32,
                          18,8,2,0,2,8,18,32,
                          50,72,97,127,159,196,235,278,
                          323,371,422,475,531,588,647,708,
                          769,832,896,960,1024};

int ordinSchimbareAmplitudine=1;
double ordin=0;
int PWMDutyCycle=2000;
//void modificareAmp(){
//    ordin+=0.2;
//    if(ordin>0.8){
//            ordin=0;
//    }
//    for(i=0;i<100;i++)
//    {
//        semnal[i]=semnalInitial[i];
//    }
//    for(i=0;i<100;i++)
//    {
//        if(i<50){
//            semnal[i]+=semnal[i]*ordin;
//        }
//
//        if(i>=50){
//            semnal[i]+=semnal[i]*ordin;
//        }
//
//    }
//
//
//}
//double procent= ordin/100;
//for(i=0;i<100;i++)
//{
//    semnal[i]=semnalInitial[i];
//}
//for(i=0;i<100;i++)
//{
//    semnal[i]+=semnal[i]*procent;
//
//}
//if(ordin>80){
//    ordin=0;
//}
//else{
//    ordin+=20;
//}

void modificareAmplitudine(){
    PMMCTL0_H = PMMPW_H;                // Unlock the PMM registers
    if(ordinSchimbareAmplitudine==1){
        SAC0DAC=DACSREF_0+ DACLSEL_2 + DACIE ;

        ordinSchimbareAmplitudine=2;

    }else
    {
        if(ordinSchimbareAmplitudine==2){
            PMMCTL0_H = PMMPW_H;
            PMMCTL2 = INTREFEN | REFVSEL_2;           // Enable internal 2.5V reference
            while(!(PMMCTL2 & REFGENRDY));
            SAC0DAC=DACSREF_1+ DACLSEL_2 + DACIE ;
            SAC0DAC |= DACEN;
            ordinSchimbareAmplitudine++;
        }
        else{
            if(ordinSchimbareAmplitudine==3){
                PMMCTL0_H = PMMPW_H;
                PMMCTL2 = INTREFEN | REFVSEL_1;           // Enable internal 2.0V reference
                while(!(PMMCTL2 & REFGENRDY));
                SAC0DAC=DACSREF_1+ DACLSEL_2 + DACIE ;
                SAC0DAC |= DACEN;

                ordinSchimbareAmplitudine++;
            }
            else{
                PMMCTL0_H = PMMPW_H;
                PMMCTL2 = INTREFEN | REFVSEL_0;           // Enable internal 1.5V reference
                while(!(PMMCTL2 & REFGENRDY));
                SAC0DAC=DACSREF_1+ DACLSEL_2 + DACIE ;
                SAC0DAC |= DACEN;

                ordinSchimbareAmplitudine=1;
            }
        }
    }

}
void ajustareFrecventa(){
    if(frcvsemnal==0){
            // Use TB2.1 as DAC hardware trigger
             TB2CCR0 = 1200-1; // CLK= 20KHz f-semnal=200Hz                          // PWM Period/2
             TB2CCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
             TB2CCR1 = 600;                              // TBCCR1 PWM duty cycle
             TB2CTL = TBSSEL__SMCLK | MC_1 | TBCLR;     // SMCLK, up mode, clear TBR
        }
        if(frcvsemnal==1){       // Use TB2.1 as DAC hardware trigger
            TB2CCR0 = 480-1; // CLK= 50KHz f-semnal=500Hz                          // PWM Period/2
            TB2CCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
            TB2CCR1 = 240;                              // TBCCR1 PWM duty cycle
            TB2CTL = TBSSEL__SMCLK | MC_1 | TBCLR;     // SMCLK, up mode, clear TBR
        }
        if(frcvsemnal==2){        // Use TB2.1 as DAC hardware trigger
            // CLK= 100KHz f-semnal=1000Hz
            TB2CCR0 = 240-1;                           // PWM Period/2
            TB2CCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
            TB2CCR1 = 120;                              // TBCCR1 PWM duty cycle
            TB2CTL = TBSSEL__SMCLK | MC_1 | TBCLR;     // SMCLK, up mode, clear TBR
        }
}
void modificareFrecventa(){
    frcvsemnal++;     //0-> 200Hz, 1-> 500Hz, 2->1KHz
    if(frcvsemnal>2){
        frcvsemnal=0;
    }
    ajustareFrecventa();
//    if(frcvsemnal==0){
//        // Use TB2.1 as DAC hardware trigger
//         TB2CCR0 = 1200-1; // CLK= 20KHz f-semnal=200Hz                          // PWM Period/2
//         TB2CCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
//         TB2CCR1 = 600;                              // TBCCR1 PWM duty cycle
//         TB2CTL = TBSSEL__SMCLK | MC_1 | TBCLR;     // SMCLK, up mode, clear TBR
//    }
//    if(frcvsemnal==1){       // Use TB2.1 as DAC hardware trigger
//        TB2CCR0 = 480-1; // CLK= 50KHz f-semnal=500Hz                          // PWM Period/2
//        TB2CCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
//        TB2CCR1 = 240;                              // TBCCR1 PWM duty cycle
//        TB2CTL = TBSSEL__SMCLK | MC_1 | TBCLR;     // SMCLK, up mode, clear TBR
//    }
//    if(frcvsemnal==2){        // Use TB2.1 as DAC hardware trigger
//        // CLK= 100KHz f-semnal=1000Hz
//        TB2CCR0 = 240-1;                           // PWM Period/2
//        TB2CCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
//        TB2CCR1 = 120;                              // TBCCR1 PWM duty cycle
//        TB2CTL = TBSSEL__SMCLK | MC_1 | TBCLR;     // SMCLK, up mode, clear TBR
//    }

}
void modificareFrecventaEsantionare(){
    if(incrementareCounter==1){
        incrementareCounter=2;
    }
    if(incrementareCounter>8){
        incrementareCounter=1;
    }
    else{
        incrementareCounter+=2;
    }


}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watch dog timer
// configurare CS SMCLK=24MHz

  // Configure two FRAM waitstate as required by the device datasheet for MCLK
  // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
  FRCTL0 = FRCTLPW | NWAITS_2;

  __bis_SR_register(SCG0);                           // disable FLL
  CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
  CSCTL0 = 0;                                        // clear DCO and MOD registers
  CSCTL1 |= DCORSEL_7;                               // Set DCO = 24MHz
  CSCTL2 = FLLD_0 + 731;                             // DCOCLKDIV = 24MHz
  __delay_cycles(3);
  __bic_SR_register(SCG0);                           // enable FLL
  while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked

  CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;        // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                                     // default DCOCLKDIV as MCLK and SMCLK source

  P1DIR |= BIT0;       // set P1.0 as SMCLK  output
  P1SEL1 |= BIT0;                             // set ACLK and  SMCLK pin as second function

//configurare DAC -> out P1.1
  P1SEL0 |= BIT1;                           // Select P1.1 as OA0O function
  P1SEL1 |= BIT1;                           // OA is used as buffer for DAC

// configurare buton P2.3

  P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
  P2REN |= BIT3;                          // P2.3 pull-up register enable
  P2IES |= BIT3;                          // P2.3 Hi/Low edge
  P2IE |= BIT3;                           // P2.3 interrupt enabled
  P2IFG &= ~BIT3;                         // P2.3 IFG cleared

  // configuram buton P4.1

  P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
  P4REN |= BIT1;                          // P2.3 pull-up register enable
  P4IES |= BIT1;                          // P2.3 Hi/Low edge
  P4IE |= BIT1;                           // P2.3 interrupt enabled
  P4IFG &= ~BIT1;                         // P2.3 IFG cleared

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

  // Configure reference module
  PMMCTL0_H = PMMPW_H;                      // Unlock the PMM registers
  PMMCTL2 = INTREFEN | REFVSEL_0;           // Enable internal 2V reference
  while(!(PMMCTL2 & REFGENRDY));            // Poll till internal reference settles

  SAC0DAC = DACSREF_0 + DACLSEL_2 + DACIE;  // Select int Vref as DAC reference
  SAC0DAT = semnal[0];                       // Initial DAC data
 // SAC0DACSTS=DACIFG;// clear intreupt
  SAC0DAC |= DACEN;                         // Enable DAC

 // SAC0DAT = semnal[0]; // write sample

  SAC0OA = NMUXEN + PMUXEN + PSEL_1 + NSEL_1;//Select positive and negative pin input
  SAC0OA |= OAPM_0;                            // Select low speed and low power mode
  SAC0PGA = MSEL_1;                          // Set OA as buffer mode
  SAC0OA |= SACEN + OAEN;                    // Enable SAC and OA

 // Use TB2.1 as DAC hardware trigger
  TB2CCR0 = 120-1;                           // PWM Period/2
  TB2CCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
  TB2CCR1 = 60;                              // TBCCR1 PWM duty cycle
  TB2CTL = TBSSEL__SMCLK | MC_1 | TBCLR;     // SMCLK, up mode, clear TBR

  __bis_SR_register(GIE);        // Enter LPM3, Enable Interrupt
  //__bis_SR_register(LPM3_bits + GIE);

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SAC0_SAC2_VECTOR
__interrupt void SAC0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SAC0_SAC2_VECTOR))) SAC0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(SAC0IV,SACIV_4))
  {
    case SACIV_0: break;
    case SACIV_2: break;
    case SACIV_4:
      //  DAC_data++;
      //  DAC_data &= 0xFFF;
       // SAC0DACSTS=0;
        counter+=incrementareCounter;

        if(counter>=99)
            counter=0;

        SAC0DAT = semnal[counter];

       // SAC0DACSTS=DACIFG;

        break;
    default: break;
  }
}


// Port 2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
    P2IFG &= ~BIT3;                         // Clear P1.3 IFG
    if(optiuneButon<=3)
    {
        optiuneButon++;
    }
    else
    {
        optiuneButon=1;
    }
}

// Port 4 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT4_VECTOR))) Port_4 (void)
#else
#error Compiler not supported!
#endif
{
    P4IFG &= ~BIT1;                         // Clear P1.3 IFG
    switch(optiuneButon){
    case 1:
        modificareFrecventa();break;
    case 2:
        modificareAmplitudine();break;
    case 3:
        modificareFrecventaEsantionare();break;
    default:
        break;

    }
}
