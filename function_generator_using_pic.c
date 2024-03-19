/* Program to generate waves of different frequencies
 File name:   Signal Generator Version:    1.0 
 Author:      Akanksha Gupta 
 Company:     Coventry University 
 Date:        November 2023 
 Program function: This program generate different waveforms like triangle, ramp up sawtooth, ramp down sawtooth
 square and sine wave for different frequencies 
 The following configure operational parameters of the PIC    */
 
 #pragma config OSC = HS      //set osc mode to HS
 #pragma config WDT=OFF       //Watch Dog Timer disabled
 #pragma config LVP=OFF       //Low Voltage Programming option OFF
 #pragma config MCLRE = ON   //Master Clear Pin ON
 
 // Include the folowing headers
 #include <p18f4520.h>        //  Device used is the PICF4520
 #include <delays.h>          // Include Delays
 #include <usart.h>           //for communication with virtual terminal
 #include <timers.h>          // for configuring timer

 // Define the number of samples per cycle for each waveform
 #define SAWTOOTH_SAMPLES 100
 #define TRIANGLE_SAMPLES 200
 #define SINE_SAMPLES 96
 
 // Define the amplitude of the waveforms
 #define AMPLITUDE 127
 
 #define  CS PORTCbits.RC0    // Port bit C0 is used as the Chip Select pin CS
   
  //define functions 
 void DAC(unsigned int);
 void generateTriangleWave(int);
 void generateRampUpSawtoothWave(int);
 void generateRampDownSawtoothWave(int);
 void generateSquareWave(int);
 void generateSineWave(int);
 void generatePulseWave(int);
 
 //initialize variables
 unsigned char waveform = 0;
 unsigned char selectedFrequency = 0;
 unsigned int time_delay = 0;
 unsigned int time_count = 0;
 unsigned int temp = 0;
 void main()
 {  
 TRISCbits.RC0 = 0;        // RC0 is configured as an output port for CS
 TRISCbits.RC3 = 0;        //make the SCK pin as output
 TRISCbits.RC5 = 0;        //make the SDO pin as output
 TRISCbits.RC6 = 0;        // make the TX pin a digital output     
 TRISCbits.RC7 = 1;        // make the RX pin a digital input 
 PORTC = 0;                // Initialise Ports  
 SSPSTAT = 0xC0;           //Status Register SSPSTAT=11000000  
            
 
 /* Open the USART for communication and set the following operational parameters: 
 Set USART Parameters Baud rate TX & RX interrupts disabled, asynchronous operation, 8 data bits,
 Continuous reception, High Baud rate, calculated value for 9600 is 129 decimal */ 
 OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE
 & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 129); 
 
  OpenTimer0(TIMER_INT_OFF &        // Disable the interruptions            */
                    T0_16BIT & 	    // 16-bit configuration of the timer    */
               T0_SOURCE_INT & 	    // Use the internal clock               */
                 T0_PS_1_256);	    // No pre-scaler  */
		 
 
  putrsUSART( "Signal Generator \r\n " );  // Send the string message 
  while(BusyUSART()) ;    // wait till USART is busy
        
 while(1)
 {   
     putrsUSART( "Select a waveform to generate: \r\n " ); //for selecting the waveform
     putrsUSART( "1 - Triangle Wave \r\n " );
     putrsUSART( "2 - Ramp up Sawtooth Wave \r\n " );
     putrsUSART( "3 - Ramp down Sawtooth Wave \r\n " );
     putrsUSART( "4 - Square Wave \r\n " );
     putrsUSART( "5 - Sine Wave \r\n " );
     putrsUSART( "6 - Pulse Wave \r\n " );
     while(BusyUSART()) ;    // wait till USART is busy
     while(!DataRdyUSART()) ;   //get character from terminal     
  
     while(getcUSART() < '1' || getcUSART() > '6'){  //check for valid input
        putrsUSART( "Enter a valid number: \r " );
        while(!DataRdyUSART()) ;     
     } 
     
     waveform = getcUSART();  // save the entered character
     putcUSART(waveform);
     putrsUSART("\r\n");
 
     putrsUSART( "Select a frequency: \r\n " ); //frequency selection
     putrsUSART( "1 - 2 Hz \r\n " );
     putrsUSART( "2 - 5 Hz \r\n " );
     putrsUSART( "3 - 10 Hz \r\n " );
     putrsUSART( "4 - 20 Hz \r\n " );
     putrsUSART( "5 - 50 Hz \r\n " );
     putrsUSART( "6 - 100 Hz \r\n " );
     while(!DataRdyUSART()) ;   //get character from terminal 
     
     while(getcUSART() < '1' || getcUSART() > '6'){ //check for valid character
     putrsUSART( "Enter a valid number: \r " );
     while(!DataRdyUSART()) ;
     }
     selectedFrequency = getcUSART(); //save the entered frequency
     putcUSART(selectedFrequency);
     putrsUSART("\r\n");

      
     switch(selectedFrequency){   // assigning the time delay to generate the appropriate frequency
	    case '1': 
		      time_delay = 200;  // for frequency = 2 Hz
		      time_count = 10000;
		      break;
	    case '2': 			// for frequency = 5 Hz
	              time_delay = 85; 
		      time_count = 4550;
	              break;
            case '3': 			// for frequency = 10 Hz
	              time_delay = 45;
		      time_count = 2220;
	              break;
            case '4': 			// for frequency = 20 Hz
	              time_delay = 20;
		      time_count = 800;
		      break;
	    case '5': 			// for frequency = 50 Hz
	              time_delay = 8;
		      time_count = 200;
		      break;
	    case '6': 			// // for frequency = 100 Hz
	              time_delay = 4;
		      time_count = 10;
		      break;
	    
	 }
	 
	  
	 SSPCON1=0x20;       //Enables serial port pins & set the SPI clock as clock = FOSC/4  
	 putrsUSART( "Generating signal....\r\n " );
	 putrsUSART( "Press any key to select new signal....\r\n " );
	
	
        switch(waveform){                                     //generate the signal for selected waveform and frequency
	    case '1': generateTriangleWave(time_count);   
		      break;
	    case '2': generateRampUpSawtoothWave(time_count);
		      break;
	    case '3': generateRampDownSawtoothWave(time_count);
		      break;
	    case '4': generateSquareWave(time_delay); 
		      break;
	    case '5': generateSineWave(time_count);
		      break;
            case '6': generatePulseWave(time_delay);
	              break;
	    
       }  
      
      temp = getcUSART();         // temporary variable to ignore the pressed key for exiting  
             
 } 
 } 
 
 
 // Function to generate a ramp up sawtooth wave
void generateRampUpSawtoothWave(int timeCount) {
  int i = 0;
  
   while(!DataRdyUSART() ){
      for (i = 0; i < SAWTOOTH_SAMPLES; i++) {
       DAC( i * AMPLITUDE / SAWTOOTH_SAMPLES);
       WriteTimer0(0);
       while(ReadTimer0() < 2*timeCount); 
     }
  }
}

 // Function to generate a ramp down sawtooth wave
void generateRampDownSawtoothWave(int timeCount) {
  int i = 0;
  
  while(!DataRdyUSART()){
     for (i = SAWTOOTH_SAMPLES; i > 0; i--) {
     DAC( i * AMPLITUDE / SAWTOOTH_SAMPLES);
     WriteTimer0(0);
     while(ReadTimer0() < 2*timeCount);
     }
  }
}
 

 // Function to generate a triangular wave
void generateTriangleWave(int timeCount) {
  int i = 0;
  while(!DataRdyUSART() ){
  
     for(i = 0; i < TRIANGLE_SAMPLES / 2; i++) {
       DAC(i * AMPLITUDE / TRIANGLE_SAMPLES);
       WriteTimer0(0);
       while(ReadTimer0() < timeCount);
    }

    for( i = TRIANGLE_SAMPLES / 2; i < TRIANGLE_SAMPLES; i++) {
      DAC((TRIANGLE_SAMPLES - i) * AMPLITUDE / TRIANGLE_SAMPLES);
       WriteTimer0(0);
       while(ReadTimer0() < timeCount);
     }
  }
}

// Function to generate a square wave
void generateSquareWave(int timeDelay) {
  int timePeriod = timeDelay/2;  //for two samples (duty cycle 50%)
 

  while(!DataRdyUSART() ) {
    DAC(255);
    Delay10KTCYx(timePeriod);
   
    DAC(00);
    Delay10KTCYx(timePeriod);
    
  }

}

// Function to generate a pulse wave
void generatePulseWave(int timeDelay) {
  int timePeriod = timeDelay * 30/100;  //for pulse with duty cycle 30%
  while(!DataRdyUSART() ) {
    DAC(255);
    Delay10KTCYx(timePeriod);
   
    DAC(00);
    Delay10KTCYx(timeDelay - timePeriod);
    
  }
}

// Function to generate a sine wave using a lookup table
void generateSineWave(int timeCount) {
  int i = 0;

  const unsigned char sine_wave_constant[] = {127, 135, 143, 151, 159, 156, 166, 174, 181, 188, 195, 202, 214, 220, 225, 230, 234, 
                                               238, 242, 245, 248, 252, 253, 254, 254, 253, 252, 250, 248, 245, 242, 238, 230, 225, 220,
					       214, 208, 202, 195, 188, 181, 174, 166, 159, 151, 145, 135, 127, 119, 111, 103, 95, 88, 80, 
					       73, 66, 59, 52, 46, 40, 34, 29, 24, 16, 12, 9, 6, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 9, 12, 16, 20,
					       24, 29, 34, 40, 46, 52, 59, 66, 73, 80, 88, 95, 103, 111, 119};

  
  while(!DataRdyUSART()){

     for (i = 0; i < SINE_SAMPLES; i++) {
       DAC(sine_wave_constant[i]);
       WriteTimer0(0);
       while(ReadTimer0() < 2*timeCount);
       
     }
  }
}
 
 
 void DAC(unsigned int data)
 {   
    unsigned int c ;   
    unsigned int lower_bits;   
    unsigned int upper_bits;   
    c = ((data+1)*16) -1;                  // here we obtain 12 bit data
    upper_bits = c/256;                    // obtain the upper 4 bits
    upper_bits = (48) | upper_bits;        // append 0011 to the above 4 bits 
    lower_bits = 255 & c;                  //now obtain the lower 8 bits by bitwise AND operation with 11111111
    CS = 0;                                // Chip Select RC0 Low
    SSPBUF = upper_bits;                   // sending the upper 8 bits first serially    
    while(!SSPSTATbits.BF);                // wait until the upper 8 bits are sent by checking BF flag
    SSPBUF = lower_bits;                   // sending the lower 8 bits serially
    while(!SSPSTATbits.BF);                // wait until the lower 8 bits are sent 
    CS =1;                                 // Chip Select RC0 Low
 }
 

 