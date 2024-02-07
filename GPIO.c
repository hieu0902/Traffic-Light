#include<stdio.h>
#include<stdint.h>
#include"GPIO.h"
void NVIC_Set_Peripheral_Priority(int32_t IRQn, uint32_t priority)
{
	NVIC->IP[IRQn]=(priority<<4)&0xFF;
}
void NVIC_Enable_Peripheral_Interrupt(int32_t IRQn)
{
	NVIC->ISER[IRQn>>5]|=(1<<(IRQn&0x1F));
}
//IRQn div 32->thu tu thanh ghi tuong ung dich phai la chia 2
//IRQ mod 32->offset tuong ung lay and vs ca 32

void SysTick_Init()
{
	SysTick->CTRL=0;
	SysTick->LOAD=0x01C20-0x01;//7200-1
	SysTick->VAL=0;
	SysTick->CTRL=0x05;
}
void delay_us(uint32_t time)//100us-> delay time*100us
{
	SysTick->VAL=0;
	while(time>0)
	{
		if(SysTick->CTRL&0x010000)//->value of countflag 
			time--;
	}
}
void lcd_pulse()
{
	eset();
	delay_us(2);
	eclear();
	delay_us(2);
}
void lcd_sendcommand(uint32_t cmd)
{
	commandport&=~(0x1<<rs);
	dataport=(dataport&~0x1E0)|((cmd&0xF0)<<1);
	lcd_pulse();//400us
	dataport=(dataport&~0x1E0)|((cmd&0x0F)<<5);
	lcd_pulse();//400us
	commandport|=(0x01<<rs);
}
void lcd_senddata(uint32_t data)
{
	commandport|=(0x01<<rs);
	dataport=(dataport&~0x1E0)|((data&0xF0)<<1);
	lcd_pulse();
	dataport=(dataport&~0x1E0)|((data&0x0F)<<5);
	lcd_pulse();
}
void lcd_initialize()
{
	delay_us(400);
	lcd_sendcommand(0x30);
	delay_us(45);
	lcd_sendcommand(0x30);
	delay_us(2);
	lcd_sendcommand(0x30);
	delay_us(2);
	lcd_sendcommand(0x20);
	delay_us(1);
	lcd_sendcommand(0x20);
	delay_us(1);
	lcd_sendcommand(0x08);
	delay_us(1);
	lcd_sendcommand(0x01);
	delay_us(20);
	lcd_sendcommand(0x06); //cursor to right
	delay_us(1);
	lcd_sendcommand(0x0f); //display on, cursor on, blink on
	delay_us(1);
}
void lcd_printf(char c[])
{
	int i=0;
	delay_us(1);//100us
	lcd_sendcommand(0x01);//800us
	delay_us(20);//2ms
	lcd_sendcommand(0x06);//800us
	delay_us(1);//100us
	lcd_sendcommand(0x0f);//800us
	delay_us(1);//100us
	while(c[i]!='\0')
		{
			lcd_senddata(c[i]);//800us perletter
			i++;
		}
}
void delay1s(uint32_t time)
{
	char c[5];
	int i=0;
	if(time==0)
	{
		sprintf(c,"%d",i);
		lcd_printf(c);
		delay_us(2000);
	}
	while(i<time)
	{
		sprintf(c,"%d",i);
		lcd_printf(c);
		delay_us(7400);
		i++;
	}
	sprintf(c,"%d",time);
	lcd_printf(c);
	delay_us(5000);
}

