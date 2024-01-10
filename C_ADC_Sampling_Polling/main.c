#include <msp430.h> 

unsigned int ADC_Value;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Set up ports
	P1DIR |= BIT0;
	P6DIR |= BIT6;

	P1SEL1 |= BIT2;             // Choose analog function for P1.2
	P1SEL0 |= BIT2;             // 11 = Analog

	PM5CTL0 &= ~LOCKLPM5;

	// Configure the ADC
	ADCCTL0 &=  ~ADCSHT;        // Set conv clock cycles 16 (01)
	ADCCTL0 |= ADCSHT_2;
	ADCCTL0 |= ADCON;           // Turn on ADC

	ADCCTL1 |= ADCSSEL_2;       // Choose SMCLK
	ADCCTL1 |= ADCSHP;          // Sample signal source = sampling timer

	ADCCTL2 &= ~ADCRES;         // Clear resolution
	ADCCTL2 |= ADCRES_2;        // 12-bit resolution

	ADCMCTL0 |= ADCINCH_2;      // ADC input = A2 (P1.2

	// Main
	while(1){
	    ADCCTL0 |= ADCENC | ADCSC;  // Enable and start conv

	    while((ADCIFG & ADCIFG0) == 0){}

	    ADC_Value = ADCMEM0;

	    if(ADC_Value > 3613){
	        P1OUT |= BIT0;
	        P6OUT &= ~BIT6;
	    }else{
	        P1OUT &= ~BIT0;
	        P6OUT |= BIT6;
	    }
	}
}
