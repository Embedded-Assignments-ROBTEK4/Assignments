#include "../headers/SPI.h"
void setup_spi(void)
{
	//Enable the module for SSI3
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R3;
	//SSI3 is on port D, so we enable that port.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	
	__asm__("NOP");
	__asm__("NOP");
	
	  GPIO_PORTD_AFSEL_R |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;           // enable alt funct on PD3-0
		//Configure pins as SSI
		GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & ~(GPIO_PCTL_PD0_M | GPIO_PCTL_PD1_M |
													GPIO_PCTL_PD2_M | GPIO_PCTL_PD3_M)) |
  												GPIO_PCTL_PD0_SSI3CLK | GPIO_PCTL_PD1_SSI3FSS |
  												GPIO_PCTL_PD2_SSI3RX | GPIO_PCTL_PD3_SSI3TX;

	  GPIO_PORTD_DEN_R |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;             // enable digital I/O on PD3-0
	  
	  //Set drive strenght to 4 milliamp
	  GPIO_PORTD_DR4R_R |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
	  //Set pins to open drain
	  GPIO_PORTD_ODR_R |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
	  
	  GPIO_PORTD_AMSEL_R &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);          // disable analog functionality on PD3-0

		//disable SSI
		SSI3_CR1_R &= ~SSI_CR1_SSE;
		
		//Set device as master
		SSI3_CR1_R &=~SSI_CR1_MS;
		
		//Use 16Mhz internal OSCILATOR
		SSI3_CC_R = SSI_CC_CS_PIOSC;
		
		//Set first clock divisor
		SSI3_CPSR_R = 254;
		
		//Set second clock divisor
		SSI3_CR0_R &= ~SSI_CR0_SCR_M;
		SSI3_CR0_R |= 255 << 8;
		
		
		//Data is captured on the first clock edge transition.
		//This setting only applies in Freescale SPI Format.
		SSI3_CR0_R &= ~SSI_CR0_SPH;
		
		//steady state Low value is placed on the SSInClk pin. when data is not transfered
		SSI3_CR0_R &= ~SSI_CR0_SPO;
		
		//Select Freescale SPI Format
		SSI3_CR0_R &= ~SSI_CR0_FRF_MOTO;
		SSI3_CR0_R |= SSI_CR0_FRF_MOTO;
		
		
		//Select 8 bit datasize
		SSI3_CR0_R &= ~SSI_CR0_DSS_M;
		SSI3_CR0_R |= SSI_CR0_DSS_4;
		
		//Enable SSI
		SSI3_CR1_R |= SSI_CR1_SSE;
				
}

void spi3_write_data(INT8U data) //blocking
{
	while(!(SSI3_SR_R & SSI_SR_TNF)); //While buffer is full, wait
	SSI3_DR_R = 0xF & data;
}

INT8U spi3_read_data(void) //blocking
{
	while(!(SSI3_SR_R & SSI_SR_RNE)); //while buffer is empty, wait
	return SSI3_DR_R & 0xF;
}


