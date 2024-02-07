#include"GPIO.h"
#include<stdint.h>
//B0,B1,B3 are inputs


volatile uint32_t input=0;
void EXTI0_IRQHandler()
{
	if(EXTI->PR&0x01)
	{
		input=(input&~0x01)|(GPIOB->IDR&0x01);//west

	}
	EXTI->PR|=0x01;
}
void EXTI1_IRQHandler()
{
	if(EXTI->PR&0x02)
	{
		input=(input&~0x02)|(GPIOB->IDR&0x02);//south
	
	}
	EXTI->PR|=0x02;
}
void EXTI15_10_IRQHandler()
{
	if(EXTI->PR&0x1000)
		{
		input=(input&~0x04)|((GPIOB->IDR&0x01000)>>10);//pesidan
	}
	EXTI->PR|=0x01000;
}



struct state{
    uint32_t PortA_Val;
    uint32_t PortB_Val;
    uint32_t Wait;
    uint32_t Next[8];
};

typedef const struct state state_t;
#define allred 0
#define goS 1
#define waitS 2
#define allredS 3
#define goW 4
#define waitW 5
#define allredW 6
#define goP 7
#define redP1 8
#define offP1 9
#define redP2 10
#define offP2 11
#define redP3 12
#define offP3 13
#define stopP 14
state_t FSM[15] = {
	{0x24,0x01,0, {allred, goW, goS, goS, goP, goP, goS, goS}}, //0
	{0x21,0x01,10, {waitS, waitS, goS, waitS, waitS, waitS, waitS, waitS}}, //1
	{0x22,0x01,5, {allredS, allredS, allredS, allredS, allredS, allredS, allredS, allredS}}, //2
	{0x24,0x01,0, {allredS, goW, goS, goW, goP, goP, goP, goP}}, //3
	{0x0C,0x01,10, {waitW, goW, waitW, waitW, waitW, waitW, waitW, waitW}}, //4
	{0x14,0x01,5, {allredW, allredW, allredW, allredW, allredW, allredW, allredW, allredW}}, //5
	{0x24,0x01,0, {allredW, goW, goS, goS, goP, goP, goS, goS}}, //6
	{0x24,0x02,10, {redP1, redP1, redP1, redP1, goP, redP1, redP1, redP1}}, //7
	{0x24,0x01,1, {offP1, offP1, offP1, offP1, offP1, offP1, offP1, offP1}}, //8
	{0x24,0x00,1, {redP2, redP2, redP2, redP2, redP2, redP2, redP2, redP2}}, //9
	{0x24,0x01,1, {offP2, offP2, offP2, offP2, offP2, offP2, offP2, offP2}}, //10
	{0x24,0x00,1, {redP3, redP3, redP3, redP3, redP3, redP3, redP3, redP3}}, //11
	{0x24,0x01,1, {offP3, offP3, offP3, offP3, offP3, offP3, offP3, offP3}}, //12
	{0x24,0x00,1, {stopP, stopP, stopP, stopP, stopP, stopP, stopP, stopP}}, //13
	{0x24,0x01,0, {stopP, goW, goS, goW, goP, goW, goS, goW}}, //14
};

int main()
{
	SysTick_Init();
  RCC->APB2ENR=0x0D;
	GPIOA->CRL=0x00222222;//A0-5:output for South-West leds respectively
	GPIOB->CRH=0x82202;//b10-B11:output for walk leds ; B0,B1,B12: input for west,south,walk
	GPIOB->CRL=0x22200088;
	GPIOB->ODR=0x00;
	GPIOA->CRH=0x0220;
	uint32_t cs=allredS;
	//Configure Exti
	AFIO->EXTICR1=0x011;
	AFIO->EXTICR4=0x01;
	EXTI->IMR|=0x01003;
	EXTI->RTSR|=0x01003;
	EXTI->FTSR|=0x01003;
	
	NVIC_Enable_Peripheral_Interrupt(EXTI0);
	NVIC_Enable_Peripheral_Interrupt(EXTI1);
	NVIC_Enable_Peripheral_Interrupt(EXTI12);
	NVIC_Set_Peripheral_Priority(EXTI0,1);
	NVIC_Set_Peripheral_Priority(EXTI1,1);
	NVIC_Set_Peripheral_Priority(EXTI12,1);
	//main loop
	lcd_initialize();
	while(1)
	{
		GPIOA->ODR=(GPIOA->ODR&(~0x3f))|FSM[cs].PortA_Val;
		GPIOB->ODR=(GPIOB->ODR&(~0xC00))|(FSM[cs].PortB_Val<<10);
		delay1s(FSM[cs].Wait);
		if(cs==goS || cs==goW || cs==goP)
		{
			int i=11;
			while(input==0x4 && cs==goP)
			{
				  char c[5];
		      sprintf(c,"%d",i);
		      lcd_printf(c);
		      delay_us(7400);
		      i++;
			}
			while(input==0x2 && cs==goS)
			{
				  char c[5];
		      sprintf(c,"%d",i);
		      lcd_printf(c);
		      delay_us(7400);
		      i++;				
			};
			while(input==0x1 && cs==goW)
			{
				  char c[5];
		      sprintf(c,"%d",i);
		      lcd_printf(c);
		      delay_us(7400);
		      i++;				
			};
		}
		cs=FSM[cs].Next[input];
	}
}