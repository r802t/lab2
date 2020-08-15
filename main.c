//#include <stdio.h>
//#include <stdlib.h>
//#include <LPC17xx.h>
//#include <stdbool.h>

/*
int main(void){
#ifdef part1
	int currStat;
	LPC_GPIO2->FIODIR &= ~((1<<10)); //Set push button pin 10 to input
	LPC_GPIO1->FIODIR |= (1<<28); //Reset LEDs
	LPC_GPIO1->FIODIR |= (1<<29);
	LPC_GPIO1->FIODIR |= (1u<<31);
	LPC_GPIO2->FIODIR |= (0x1Fu<<2);
	while (1){
		currStat = LPC_GPIO2->FIOPIN & (1<<10);
		if (!currStat)
				LPC_GPIO2->FIOSET |= (1<<6); //turn on LED
		else 
				LPC_GPIO2->FIOCLR |= (1<<6); //turn off LED
	}
	
#endif
	
#ifdef part2
	LPC_GPIO1->FIODIR |= (0x06u<<20); //Set direction for joysticks
	char* dir = "center";
	char* pressed = "";
	while(1) {
		if ((LPC_GPIO1->FIOPIN & (1<<20)) == 0)
			dir = "Center";
		if ((LPC_GPIO1->FIOPIN & (1<<23)) == 0)
			dir = "North";
		if ((LPC_GPIO1->FIOPIN & (1<<24)) == 0)
			dir = "East";
		if ((LPC_GPIO1->FIOPIN & (1<<25)) == 0)
			dir = "South";
		if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0)
			dir = "West";					
		pressed = (LPC_GPIO1->FIOPIN & (1<<20))? "not pressed" : "pressed";
		printf("The joystick is %s, at %s\n", pressed, dir);
	}
#endif
	
#ifdef part3
	char input [4];//The maximum number that 8 LEDs can represent is 255 which is a 3-digit number
	scanf("%s",input);
	int num = atoi(input); //convert string to int
	int binary[8];
	//convert to binary array
	for (int i = 0; i<8; i++){
		binary[i]=num%2;
		num = num/2;
	}
	//Set dir
	LPC_GPIO1->FIODIR |= (0x0Bu<<28); 
	LPC_GPIO2->FIODIR |= (0x1Fu<<2);
	//Set LEDs
	LPC_GPIO1->FIOSET |= (binary[0]<<28); 
	LPC_GPIO1->FIOSET |= (binary[1]<<29); 
	LPC_GPIO1->FIOSET |= (binary[2]<<31); 
	LPC_GPIO2->FIOSET |= (binary[3]<<2);
	LPC_GPIO2->FIOSET |= (binary[4]<<3);
	LPC_GPIO2->FIOSET |= (binary[5]<<4);
	LPC_GPIO2->FIOSET |= (binary[6]<<5);
	LPC_GPIO2->FIOSET |= (binary[7]<<6);
#endif
	
#ifdef part4

	LPC_SC->PCONP |= (1<<12); //Power on the ADC
	LPC_PINCON->PINSEL1 &= ~(0x03 << 18);  // clear bits 18 and 19
	LPC_PINCON->PINSEL1 |= (0x01 << 18); //set bit 18 to 01
	
	LPC_ADC->ADCR = 0x0;
	LPC_ADC->ADCR |= 1 << 2; //Turn on P0.2
	LPC_ADC->ADCR |= 1 << 11; // 5Mhz = divisor +1
	LPC_ADC->ADCR |= 1 << 21; // Enable ADCR circuitry
	
	
	while(true){
		LPC_ADC->ADCR |= 1 << 24; // start conversion
		while ((LPC_ADC->ADGDR & (1<<31u)) == 0); //wait until conversion complete
		uint32_t result = ((LPC_ADC->ADGDR) >> 4) & 0xfff; // read bits 4-15 and mask it.
		result /= 1242;
		printf("Result: %d\n", result);
	}
	LPC_SC->PCONP |= (0x01<<12);
 LPC_PINCON->PINSEL1 &= ~(0x03 << 18);
 LPC_PINCON->PINSEL1 |= (0x01 <<18);

#endif
}
*/
/*
#include <stdio.h>
#include <cmsis_os2.h>
#include <LPC17xx.h>
#include <stdbool.h>

void setUpLEDs(){
	LPC_GPIO2->FIODIR &= ~((1<<10)); //set pushbutton as input
	LPC_GPIO1->FIODIR |= (1<<28); //Reset LEDs
	LPC_GPIO1->FIODIR |= (1<<29);
	LPC_GPIO1->FIODIR |= (1u<<31);
	LPC_GPIO2->FIODIR |= (0x1Fu<<2);
}

void showLED(int dir){
	LPC_GPIO1->FIOCLR |= (1<<28); //turn off LED
	LPC_GPIO1->FIOCLR |= (1<<29); 
	LPC_GPIO1->FIOCLR |= (1u<<31);
	switch (dir){
		case 0:
			LPC_GPIO2->FIOCLR |= (1<<3); //turn on LED
			break;
		case 1:
			LPC_GPIO1->FIOSET |= (1<<28); //turn on LED
			break;
		case 2:
			LPC_GPIO1->FIOSET |= (1<<29); //turn on LED
			break;
		case 3:
			LPC_GPIO1->FIOSET |= (1<<28); //turn on LED
			LPC_GPIO1->FIOSET |= (1<<29);
			break;
		case 4: 
			LPC_GPIO1->FIOSET |= (1u<<31); //turn on LED
			break;
		case 5:
			LPC_GPIO2->FIOSET |= (1<<3); //turn on LED
			break;
	}
}
void readLED (void *arg){
	int currStat;
	while (1){
		currStat = LPC_GPIO2->FIOPIN & (1<<10);
		if (!currStat)
				LPC_GPIO2->FIOSET |= (1<<6); //turn on LED
		else 
				LPC_GPIO2->FIOCLR |= (1<<6); //turn off LED
		osThreadYield();
	}
	
}

void readJoystick(void *arg){
	LPC_GPIO1->FIODIR |= (0x06u<<20); //Set direction for joysticks
	int dir;
	int pressed;
	while(1) {
		pressed = (LPC_GPIO1->FIOPIN & (1<<20))? 0 : 5; //Pressed
		showLED(pressed);
		if ((LPC_GPIO1->FIOPIN & (1<<23)) == 0)
			dir = 1; //North
		if ((LPC_GPIO1->FIOPIN & (1<<24)) == 0)
			dir = 2; //East
		if ((LPC_GPIO1->FIOPIN & (1<<25)) == 0)
			dir = 3; //South
		if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0)
			dir = 4; //West			
		showLED(dir);
		osThreadYield();
	}
}

void readADC(void *arg){
	LPC_SC->PCONP |= (0x01<<12);            // power on the ADC
	LPC_PINCON->PINSEL1 &= ~(0x03 << 18);   // clear bits 18 and 19 
	LPC_PINCON->PINSEL1 |= (0x01 <<18);     // set bit 18 to 01
	
	LPC_ADC->ADCR=0x0;											// reset ADCR
	LPC_ADC->ADCR |= (0x01<<2);             // set pin 2 to be analog input
	LPC_ADC->ADCR |= (0x04<<8);							// set clock dicisor to be 4
	LPC_ADC->ADCR |= (0x01<<21);						// enable ADCR circuitry

	while(true) {
		LPC_ADC->ADCR |= (0x01<<24);         // set bit to start conversion 
	
		while((LPC_ADC->ADGDR & 0x80000000) == 0);   //wait for conversion

		uint32_t ADC_Value = (LPC_ADC->ADGDR>>4)& (0xFFF) ; // extract bits 15...4 of LPC_ADC->ADGDR;
		double output_value= ADC_Value/1240.0;              // convert to output voltage 
		if(output_value>=3.5){
			printf("Output: %1.2f \n", 3.3000);               // max voltage 
		} else{
			printf("Output: %1.2f \n", output_value);
		}
		osThreadYield();
	}
}
int main (){
	osKernelInitialize();
	setUpLEDs();
	osThreadNew(readLED, NULL, NULL);
	osThreadNew(readJoystick, NULL, NULL);
	//osThreadNew(readADC, NULL, NULL);
	
	osKernelStart();
}
*/

#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <cmsis_os2.h>
#include <math.h>

void setupLED(){
	// setup LEDs
	LPC_GPIO1->FIODIR |= (0x01<<28);
	LPC_GPIO1->FIODIR |= (0x01<<29);
	LPC_GPIO1->FIODIR |= (0x01u<<31);
	LPC_GPIO2->FIODIR |= (0x01<<2);
	LPC_GPIO2->FIODIR |= (0x01<<3);
	LPC_GPIO2->FIODIR |= (0x01<<4);
	LPC_GPIO2->FIODIR |= (0x01<<5);
	LPC_GPIO2->FIODIR |= (0x01<<6);
}

void clearLED(){
	LPC_GPIO1->FIOCLR |= (0x01<<28);
	LPC_GPIO1->FIOCLR |= (0x01<<29);
	LPC_GPIO1->FIOCLR |= (0x01u<<31);
	LPC_GPIO2->FIOCLR |= (0x01<<2);
	LPC_GPIO2->FIOCLR |= (0x01<<3);
	LPC_GPIO2->FIOCLR |= (0x01<<4);
	LPC_GPIO2->FIOCLR |= (0x01<<5);
	LPC_GPIO2->FIOCLR |= (0x01<<6);
}

void readJoystick(void *arg){
	while(true) {
		LPC_GPIO1->FIOCLR |= (0x01<<28);
		LPC_GPIO1->FIOCLR |= (0x01<<29);
		LPC_GPIO2->FIOCLR |= (0x01<<3);
		LPC_GPIO1->FIOCLR |= (0x01u<<31);
		
		if ((LPC_GPIO1->FIOPIN&(0x01<<20)) == 0 ){
				LPC_GPIO2->FIOSET |= (0x01<<3); 
			}
			
			//check direction
			if ((LPC_GPIO1->FIOPIN&(0x01<<23)) == 0 ){
				LPC_GPIO1->FIOSET |= (0x01<<28);
			} else if ((LPC_GPIO1->FIOPIN&(0x01<<24)) == 0 ){
				LPC_GPIO1->FIOSET |= (0x01<<29);
			} else if ((LPC_GPIO1->FIOPIN&(0x01<<26)) == 0 ){
				LPC_GPIO1->FIOSET |= (0x01u<<31);
			} else if ((LPC_GPIO1->FIOPIN&(0x01<<25)) == 0 ){
				LPC_GPIO1->FIOSET |= (0x01<<28);
				LPC_GPIO1->FIOSET |= (0x01<<29);
			}
		osThreadYield();
	}
}

void readADC(void *arg){
	LPC_SC->PCONP |= (0x01<<12);
	LPC_PINCON->PINSEL1 &= ~(0x03 << 18);
	LPC_PINCON->PINSEL1 |= (0x01 <<18);
	
	LPC_ADC->ADCR=0x0;
	LPC_ADC->ADCR |= (0x01<<2);
	LPC_ADC->ADCR |= (0x04<<8);
	LPC_ADC->ADCR |= (0x01<<21);

	while(true) {
		LPC_ADC->ADCR |= (0x01<<24);
	
		while((LPC_ADC->ADGDR & 0x80000000) == 0); 

		uint32_t ADC_Value = (LPC_ADC->ADGDR>>4)& (0xFFF) ; // extract bits 15...4 of LPC_ADC->ADGDR;

		double output_value= ADC_Value/1240.0;
		if(output_value>=3.5){
			printf("ADC Output: %1.3f \n", 3.3000);
		} else{
			printf("ADC Output: %1.3f \n", output_value);		
		}
		osThreadYield();
	}
}

/* button down and up->LED on, button down and up->LED off
bool isLEDon=false;
void readPushbutton(void *arg){
	while(true) {
		// read pushbutton
		if ((LPC_GPIO2->FIOPIN&(0x01<<10)) == 0 ){
			if(!isLEDon){
				LPC_GPIO2->FIOSET |= (0x01<<6);
				isLEDon=true;
			} else {
				LPC_GPIO2->FIOCLR |= (0x01<<6);
				isLEDon=false;
			}
		}
		osThreadYield();
	}
}
*/

// button down->LED on, button up->LED off
void readPushbutton (void *arg){
	int currStat;
	while (1){
		currStat = LPC_GPIO2->FIOPIN & (1<<10);
		if (!currStat)
				LPC_GPIO2->FIOSET |= (1<<6); //turn on LED
		else 
				LPC_GPIO2->FIOCLR |= (1<<6); //turn off LED
		osThreadYield();
	}
}


int main(){
	setupLED();
	clearLED();
	osKernelInitialize();
	osThreadNew(readJoystick, NULL, NULL);
	osThreadNew(readADC, NULL, NULL);
	osThreadNew(readPushbutton, NULL, NULL);
	osKernelStart();
}








