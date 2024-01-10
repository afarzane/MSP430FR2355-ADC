#include <msp430.h> 

unsigned int ADC_Value;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Set up ports
	P1DIR |= BIT0;
	P6DIR |= BIT6;              // Set up LEDs

	P1SEL1 |= BIT2;             // Configure P1.2 as Analog function
	P1SEL0 |= BIT2;             // 11

    // Configure ADC
    ADCCTL0 &= ~ADCSHT;         // Conversion cycles = 16
    ADCCTL0 |= ADCSHT_2;        // ADCSHT = 10
    ADCCTL0 |= ADCON;           // Turn on ADC

    ADCCTL1 |= ADCSSEL_2;       // SMCLK
    ADCCTL1 |= ADCSHP;          // Use ADC timer to trigger conversion

    ADCCTL2 &= ~ADCRES;         // Set resolution to 12-bit
    ADCCTL2 |= ADCRES_2;        // 10

    ADCMCTL0 |= ADCINCH_2;       // Select A2

    // Set up IRQ
    ADCIE |= ADCIE0;            // Conversion complete IRQ, local enable


    // Main
    while(1){
        ADCCTL0 |= ADCENC | ADCSC;          // Start conversion
        __bis_SR_register(GIE | LPM0_bits); // enable maskables and put into Low-Power Mode 0
    }

}

// Interrupt Service Routine
#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void){

    __bic_SR_register_on_exit(LPM0_bits);
    ADC_Value = ADCMEM0;        // When read, flags are cleared

    if(ADC_Value > 3613){
        P1OUT |= BIT0;
        P6OUT &= ~BIT6;
    }else{
        P1OUT &= ~BIT0;
        P6OUT |= BIT6;
    }

    ADCCTL0 |= ADCENC | ADCSC;      // Start next conversion
}
