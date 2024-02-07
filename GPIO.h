#include<stdio.h>
#include<stdint.h>
enum IRQn_Type
{
	EXTI0=6,
	EXTI1=7,
	EXTI2=8,
	EXTI3=9,
	EXTI4=10,
	EXTI12=40
};
typedef struct
{
	volatile uint32_t CR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t APB1RSTR;
  volatile uint32_t AHBENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t APB1ENR;
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
}RCC_Typedef;
typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile const uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
}GPIO_Typedef;
typedef struct
{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	uint32_t RESERVED;
	volatile uint32_t MAPR2;
}AFIO_Typedef;

typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_Typedef;
typedef struct
{
  volatile uint32_t ISER[3];               
        uint32_t RESERVED0[29];
  volatile uint32_t ICER[3];               
        uint32_t RESERVED1[29];
  volatile uint32_t ISPR[3];               
        uint32_t RESERVED2[29];
  volatile uint32_t ICPR[3];               
        uint32_t RESERVED3[29];
  volatile uint32_t IABR[3];               
        uint32_t RESERVED4[61];
  volatile uint8_t  IP[81];
        uint8_t RESERVEDBYTE[3];	
        uint32_t RESERVED5[695];
  volatile  uint32_t STIR;             
} NVIC_Typedef;
typedef struct
{
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile const uint32_t CALIB;
}SysTick_Typedef;
#define GPIOA ((GPIO_Typedef*) 0x40010800)
#define GPIOB ((GPIO_Typedef*) 0x40010C00)
#define GPIOC ((GPIO_Typedef*) 0x40011000)
#define GPIOD ((GPIO_Typedef*) 0x40011400)
#define GPIOE ((GPIO_Typedef*) 0x40011800)
#define GPIOF ((GPIO_Typedef*) 0x40011C00)
#define GPIOG ((GPIO_Typedef*) 0x40012000)

#define AFIO ((AFIO_Typedef*) 0x40010000)

#define EXTI ((EXTI_Typedef*) 0x40010400)

#define SysTick ((SysTick_Typedef*) 0xE000E010)

#define NVIC ((NVIC_Typedef*) 0xE000E100)

#define RCC ((RCC_Typedef*)0x40021000) //RCC_Typedef* RCC=0x40021000
void NVIC_Set_Peripheral_Priority(int32_t IRQn, uint32_t priority);
void NVIC_Enable_Peripheral_Interrupt(int32_t IRQn);

void delay_us(uint32_t time);
void SysTick_Init();
#define commandport GPIOA->ODR
#define rs 10
#define e 9
#define dataport GPIOB->ODR
#define eset() GPIOA->BSRR|=(1<<e)
#define eclear() GPIOA->BRR|=(1<<e)
void lcd_pulse();
void lcd_sendcommand(uint32_t cmd);
void lcd_senddata(uint32_t data);
void lcd_initialize();
void lcd_printf(char c[]);
void delay1s(uint32_t time);