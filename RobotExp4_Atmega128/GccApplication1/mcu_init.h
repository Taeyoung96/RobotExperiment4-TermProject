/*
* MCU_Init.h
*
* Created: 2019-04-20 오후 3:02:08
* Author: Administrator
*/

#ifndef _MCU_INIT_H__
#define _MCU_INIT_H__

//////////////////////////////////////////////////////////////////
//include 
//////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>



#define		sbi(PORTX,BitX)	PORTX|= (1<<BitX)	//비트 set	 명령 정의
#define		cbi(PORTX,BitX)	PORTX&=~(1<<BitX)	//비트 clear 명령 정의

//LS7366
#define SELECT_MDR0				0b00001000 // Select MDR0
#define SELECT_MDR1				0b00010000 // Select MDR1
#define SELECT_DTR				0b00011000 // Select DTR
#define SELECT_CNTR				0b00100000 // Select CNTR
#define SELECT_OTR				0b00101000 // Select OTR
#define SELECT_STR				0b00110000 // Select STR

#define CLR_REG					0b00000000 // CLR register
#define RD_REG					0b01000000 // RD register
#define WR_REG					0b10000000 // WR register
#define LOAD_REG				0b11000000 // LOAD register


#define NON_QUAD				0b00000000 // non-quadrature counter mode.
#define X1_QUAD					0b00000001 // x1 quadrature counter mode.
#define X2_QUAD					0b00000010 // x2 quadrature counter mode.
#define X4_QUAD					0b00000011 // x4 quadrature counter mode.
#define FREE_RUN				0b00000000 // free-running count mode.
#define SINGLE_CYCLE			0b00000100 // single-cycle count mode.
#define RANGE_LIMIT				0b00001000 // range-limit count mode.
#define MODULO_N				0b00001100 // modulo-n count mode.
#define DISABLE_INDEX			0b00000000 // disable index.
#define INDEX_AS_LOAD_CNTR		0b00010000 // configure index as the "loadCNTR" input(clears CNTR to 0).
#define INDEX_AS_RESET_CNTR		0b00100000 // configure index as the"reset CNTR" input(clears CNTR to 0).
#define INDEX_AS_LOAD_OTR		0b00110000 // configure index as the "loadOTR" input(transfer CNTR to OTR).
#define ASYCHRONOUS_INDEX		0b00000000 // Asynchronous index
#define SYNCHRONOUS_INDEX		0b01000000 // Synchoronous index
#define FILTER_CDF_1			0b00000000 // Filter clock division factor = 1
#define FILTER_CDF_2			0b10000000 // Filter clock division factor = 2

#define FOUR_BYTE_COUNT_MODE	0b00000000 // 4-byte counter mode
#define THREE_BYTE_COUNT_MODE	0b00000001 // 3-byte counter mode
#define TWO_BYTE_COUNT_MODE		0b00000010 // 2-byte counter mode
#define ENABLE_COUNTING			0b00000000 // Enable counting
#define DISABLE_COUNTING		0b00000100 // Disable counting
#define FLAG_ON_IDX				0b00010000 // FLAG on IDX (B4 of STR)
#define FLAG_ON_CMP				0b00100000 // FLAG on CMP (B5 of STR)
#define FLAG_ON_BW				0b01000000 // FLAG on BW (B6 of STR)
#define FLAG_ON_CY				0b10000000 // FLAG on CY (B7 of STR)


#define ICR_TOP	399
#define OCR_MAX 390
#define OCR_MIN 10


//////////////////////////////////////////////////////////////////
//Functions
//////////////////////////////////////////////////////////////////


void InitIO();
void InitExtInt();

void InitTimer0();
void InitTimer1();
void InitTimer2();
void InitTimer3();

void InitADC();
int GetADC(char ch);

void InitUart0();
void InitUart1();

void InitSPI();

void TransUart0(unsigned char data);
void TransUart1(unsigned char data);

void TransNumUart0(int num);
void SendShortUART0(int16_t num);
void TransNumUart1(int num);
void SendShortUART1(int16_t num);

unsigned char RecvUart0();
unsigned char RecvUart1();

void SPI_MasterSend(unsigned char data);
unsigned char SPI_MasterRecv(void);


//////////////////////////////////////////////////////////////////
//enum for external interrupt
//EICRA
enum{

	INT0_LOW_LEVEL,
	INT0_RESERVED,
	INT0_FALLING,
	INT0_RISING,
	
	INT1_LOW_LEVEL = 0x00,
	INT1_RESERVED = 0x04,
	INT1_FALLING = 0x08,
	INT1_RISING = 0x0C,
	
	INT2_LOW_LEVEL = 0x00,
	INT2_RESERVED = 0x10,
	INT2_FALLING = 0x20,
	INT2_RISING = 0x30,
	
	INT3_LOW_LEVEL = 0x00,
	INT3_RESERVED = 0x40,
	INT3_FALLING = 0x80,
	INT3_RISING = 0xC0
};

//EICRB
enum{

	INT4_LOW_LEVEL,
	INT4_TOGGLE,
	INT4_FALLING,
	INT4_RISING,
	
	INT5_LOW_LEVEL = 0x00,
	INT5_TOGGLE = 0x04,
	INT5_FALLING = 0x08,
	INT5_RISING = 0x0C,
	
	INT6_LOW_LEVEL = 0x00,
	INT6_TOGGLE = 0x10,
	INT6_FALLING = 0x20,
	INT6_RISING = 0x30,
	
	INT7_LOW_LEVEL = 0x00,
	INT7_TOGGLE = 0x40,
	INT7_FALLING = 0x80,
	INT7_RISING = 0xC0
};

//EIMSK
enum{

	INT0_ENABLE = 0x01,
	INT1_ENABLE = 0x02,
	INT2_ENABLE = 0x04,
	INT3_ENABLE = 0x08,
	INT4_ENABLE = 0x10,
	INT5_ENABLE = 0x20,
	INT6_ENABLE = 0x40,
	INT7_ENABLE = 0x80	
};



//////////////////////////////////////////////////////////////////
//enum for timer0 //2021 2학기 수요일 로실 
//TCCR0
enum{
	
	TIMER0_WGM_NORMAL = 0x00,
	TIMER0_WGM_PWM = 0x08,
	TIMER0_WGM_CTC = 0x40,
	TIMER0_WGM_FAST_PWM	= 0x48
};
enum{

	TIMER0_COM_NORMAL = 0x00,
	TIMER0_COM_TOGGLE_OC0 = 0x10,
	TIMER0_COM_RESERVED_OC0 = 0x10,
	TIMER0_COM_CLEAR_OC0 = 0x20,
	TIMER0_COM_SET_OC0 = 0x30
};
enum{

	TIMER0_CS_NO_CLOCK_SOURCE,
	TIMER0_CS_PRESCALER_1,
	TIMER0_CS_PRESCALER_8,
	TIMER0_CS_PRESCALER_32,
	TIMER0_CS_PRESCALER_64,
	TIMER0_CS_PRESCALER_128,
	TIMER0_CS_PRESCALER_256,
	TIMER0_CS_PRESCALER_1024
};


//////////////////////////////////////////////////////////////////
//enum for timer2
//TCCR2
enum{
	
	TIMER2_WGM_NORMAL = 0x00,
	TIMER2_WGM_PWM = 0x08,
	TIMER2_WGM_CTC = 0x40,
	TIMER2_WGM_FAST_PWM	= 0x48
};
enum{

	TIMER2_COM_NORMAL = 0x00,
	TIMER2_COM_TOGGLE_OC0 = 0x10,
	TIMER2_COM_RESERVED_OC0 = 0x10,
	TIMER2_COM_CLEAR_OC0 = 0x20,
	TIMER2_COM_SET_OC0 = 0x30
};
enum{

	TIMER2_CS_NO_CLOCK_SOURCE,
	TIMER2_CS_PRESCALER_1,
	TIMER2_CS_PRESCALER_8,
	TIMER2_CS_PRESCALER_64,
	TIMER2_CS_PRESCALER_256,
	TIMER2_CS_PRESCALER_1024,
	TIMER2_CS_EXTERNAL_CLOCK_T2_FALLING,
	TIMER2_CS_EXTERNAL_CLOCK_T2_RISING
};


//////////////////////////////////////////////////////////////////
//enum for uart
//UCSRA
enum{

	USART_MULTI_PROCESSOR_MODE = 0x01,
	USART_DOUBLE_SPEED = 0x02,
	USART_PARITY_ERROR = 0x04,
	USART_DATA_OVERRUN = 0x08,
	USART_FRAME_ERROR = 0x10,
	USART_UDR_EMPTY = 0x20,
	USART_TRANS_COMPLETE = 0x40,
	USART_RECV_COMPLETE = 0x80
};

//UCSRB
enum{
	USART_CHAR_SIZE_9BIT_B = 0x04,
	USART_TRANS_ENABLE = 0x08,
	USART_RECV_ENABLE = 0x10,
	USART_UDR_EMPTY_INTERRUPT_ENABLE = 0x20,
	USART_TRANS_INTERRUPT_ENABLE = 0x40,
	USART_RECV_INTERRUPT_ENABLE = 0x80
};

//UCSRC
enum{

	USART_SYNC_MODE	= 0x40,
	USART_PARITY_NONE = 0x00,
	USART_PARITY_EVEN = 0x20,
	USART_PARITY_ODD = 0x30,
	USART_STOP_1BIT	= 0x00,
	USART_STOP_2BIT = 0x08,
	USART_CHAR_SIZE_5BIT = 0x00,
	USART_CHAR_SIZE_6BIT = 0x02,
	USART_CHAR_SIZE_7BIT = 0x04,
	USART_CHAR_SIZE_8BIT = 0x06,
	USART_CHAR_SIZE_9BIT_C = 0x06,
	USART_SYNC_CLOCK_RISING = 0x00,
	USART_SYNC_CLOCK_FALLING = 0x01
};

//UBRRL
enum{

	USART_9600BPS = 103,
	USART_115200BPS = 8	
};


//////////////////////////////////////////////////////////////////
//enum for adc
//ADMUX
enum{

	ADC_REF_SELECTION_AREF = 0x00,
	ADC_REF_SELECTION_AVCC = 0x04,
	ADC_REF_SELECTION_INTERNAL = 0xC,
	ADC_ADJUST_RIGHT = 0x00,
	ADC_ADJUST_LEFT = 0x20	
};

enum{

	ADC_MUX_CH0,
	ADC_MUX_CH1,
	ADC_MUX_CH2,
	ADC_MUX_CH3,
	ADC_MUX_CH4,
	ADC_MUX_CH5,
	ADC_MUX_CH6,
	ADC_MUX_CH7	
};

//ADCSRA
enum{

	ADC_INTERRUPT_ENABLE = 0x08,
	ADC_FREE_RUNNING = 0x20,
	ADC_CONVERTION_START = 0x40,
	ADC_ENABLE = 0x80	
};
enum{
	ADC_PRESCALER_2,
	ADC_PRESCALER_4 = 2,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128
};

#endif