#include "mcu_init.h"
#include "dataType.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define dt_current 0.0005	// 0.5ms
#define dt_velocity 0.005	// 5ms
#define dt_position 0.05	// 50ms

#define Kcp 0.827		// current p gain	
#define Kci 2.2117e+03	// current i gain	
#define Kac 0.403		// current anti wind up (1/3Kcp)=0.403

#define Kvp 1.505		//  velocity p gain		
#define Kvi 40.0		//  velocity i gain	
#define Kas 0.96445		//  velocity anti wind up

#define Kpp 12.5664		//  position p gain		
#define Kpd 0.1		    //  position d gain		

#define Kt 0.0683	    //  역기전력 상수


volatile int32_t g_Cnt, g_preCnt;

volatile double g_Pdes = 0., g_Ppre=0.;
volatile double g_Pcur=0., g_Pre_Pcur=0.;
volatile double g_Perr=0.;
volatile double g_Perr_old = 0.;
volatile double g_Perr_det = 0.;

volatile double g_Vcur=0., g_Vpre=0;
volatile double g_Vdes = 0.0;
volatile double g_Verr=0.;
volatile double g_Vlimit = 1.;
volatile double g_Verr_sum=0.;


volatile double g_Ccur=0.;
volatile double g_Cdes=0.;
volatile double g_Cerr=0.;
volatile double g_Cerr_sum=0.;
volatile double g_Climit = 1.;

volatile double g_ADC;
volatile int g_SendFlag = 0;
volatile int g_Direction;

volatile int g_cur_control = 0;
volatile double g_vel_control=0.;
volatile double g_pos_control=0.;
volatile unsigned char g_TimerCnt;

volatile unsigned char checkSize;
volatile unsigned char g_buf[256], g_BufWriteCnt, g_BufReadCnt;

volatile Packet_t g_PacketBuffer;
volatile unsigned char g_PacketMode;
volatile unsigned char g_ID = 1;



//// SetDuty 설정 ////
void SetDutyCW(double v){
	
	while(TCNT1  == 0);

	int ocr = v * (200. / 24.) + 200;
	
	if(ocr > OCR_MAX)	ocr = OCR_MAX;
	else if(ocr < OCR_MIN)	ocr = OCR_MIN;
	//OCR1A = OCR1B = ocr;
	
	OCR1A = OCR3B = ocr + 8;		//1 H
	OCR1B = OCR3A = ocr - 8;		//1 L
}

//// LS7366  ////
void InitLS7366(){
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_MDR0 | WR_REG);
	SPI_MasterSend(X4_QUAD | FREE_RUN | DISABLE_INDEX | SYNCHRONOUS_INDEX |FILTER_CDF_1);
	PORTB = 0x01;
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_MDR1 | WR_REG);
	SPI_MasterSend(FOUR_BYTE_COUNT_MODE | ENABLE_COUNTING);
	PORTB = 0x01;
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_CNTR | CLR_REG);
	PORTB = 0x01;
}


//// ADC 설정 ////
int getADC(char ch){

	ADMUX = (ADMUX & 0xf0) + ch;
	ADCSRA |= 0x40;
	while(!(ADCSRA & 0x10));
	return ADC;
}


ISR(USART0_RX_vect){

	g_buf[g_BufWriteCnt++] = UDR0;
}



//// 제어기는 무조건 여기에 코딩(함수 사용 o) ////

ISR(TIMER0_OVF_vect){
			
	TCNT0 = 256 - 125;		//0.5ms	 2000HZ
	
	//Read LS7366
	int32_t cnt;
	
	PORTC = 0x01;
	
	g_ADC = getADC(0);
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_OTR | LOAD_REG);
	PORTB = 0x01;
			
	PORTB = 0x00;
	SPI_MasterSend(SELECT_OTR | RD_REG);
	cnt = SPI_MasterRecv();		cnt = cnt<< 8;
	cnt |= SPI_MasterRecv();	cnt = cnt<< 8;
	cnt |= SPI_MasterRecv();	cnt = cnt<< 8;
	cnt |= SPI_MasterRecv();
	PORTB = 0x01;
	g_Cnt = -cnt;		//누적 pulse 값 반화
	
	PORTC = 0x03;
	
	g_Pcur = (double)(g_Cnt / (4096. * 81.)) * 2 * M_PI;	// 단위 [rad]
	
	//g_Pcur = (double)(g_Cnt * M_PI / 165888.);
	
	//TO DO
	if((g_TimerCnt % 100) == 0){	//위치제어기		50ms
		
		g_TimerCnt = 0;
		g_Perr = (g_Pdes - g_Pcur);
		
		g_Perr_det = (g_Perr - g_Perr_old)/dt_position;
		g_Perr_old = g_Perr;	// 이전 값 저장
		
		g_pos_control = g_Perr*Kpp + g_Perr_det*Kpd;
		
		
		if(g_Vlimit >= 0)
		{
			if(g_pos_control >= g_Vlimit)
			{
				
				g_pos_control = g_Vlimit;
			}else if(g_pos_control <= -1*g_Vlimit)
			{
				g_pos_control = -1*g_Vlimit;
			}
			
		}
		else
		{
			if(g_pos_control <= g_Vlimit)
			{
				g_pos_control = g_Vlimit;
			}
			else if(g_pos_control >= -1*g_Vlimit)
			{
				g_pos_control = -1*g_Vlimit;
			}
		}
		
	}
	
	if((g_TimerCnt % 10) == 0){		//속도제어기		5ms
		
		g_Vcur = (double)(g_Pcur - g_Pre_Pcur) / 0.005;		//위치 미분 해서 속도를 얻어냄
		g_Pre_Pcur = (double)g_Pcur;	//위치 이전 값 저장
		
		g_Vdes = g_pos_control;	//[rad/s]
		//g_Vdes = g_Vlimit;			//mfc에서 목표값 줄때 사용함
		//g_Vdes = 0.5;
		
		
		
		g_Verr = (double)(g_Vdes - g_Vcur);	//속도 에러값
		g_Verr_sum += (double)g_Verr;
		
		g_vel_control = (double)(g_Verr * Kvp + g_Verr_sum* Kvi*dt_velocity);
		

		if(g_vel_control >= g_Climit)
		{
			g_Verr_sum -= (double)((g_vel_control - g_Climit)*Kas);
			g_vel_control = g_Climit;
			
		}else if(g_vel_control <= -g_Climit)
		{
			g_Verr_sum -= (double)((g_vel_control + g_Climit)*Kas);
			g_vel_control = -g_Climit;
		}
		
		/*		if(g_vel_control >= 2.08)
		{
		g_Verr_sum -= (double)((g_vel_control - 2.08)*Kas);
		g_vel_control = 2.08;
		
		}else if(g_vel_control <= -2.08)
		{
		g_Verr_sum -= (double)((g_vel_control + 2.08)*Kas);
		g_vel_control = -2.08;
		}*/
		
	}
	
	
	g_TimerCnt++;
	
	//전류제어기 0.5ms
	
	//g_Cdes = -0.2;	// 0.1을 0.2로
	
	g_Cdes = g_vel_control;		//속도 제어기 출력을 전류제어 des
	//g_Cdes=g_Climit;			//mfc에서 목표값 줄때 사용하였음
	

	g_Ccur = -( ((g_ADC / 1024. * 5.) - 2.5) * 10.);	
	g_Cerr = (double)(g_Cdes - g_Ccur);
	
	g_Cerr_sum += (double)g_Cerr;
	


	g_cur_control = (double)(g_Cerr * Kcp + g_Cerr_sum * Kci* dt_current);
	
	g_cur_control += (double)(g_Vcur * Kt); //역기전력 보상
	//cur_control += g_Vcur * 0.0683;
	
	//전류제어기 Antiwind up
	
	if(g_cur_control >= 24)
	{
		g_Cerr_sum -= (double)(g_cur_control - 24)*Kac;
		g_cur_control = 24.;
	}
	else if(g_cur_control <= -24)
	{
		
		g_Cerr_sum -= (double)(g_cur_control + 24)*Kac;
		g_cur_control = -24.;
	}
	
	
	// 전류제어기 끝
	
	
	SetDutyCW(g_cur_control);		//duty비 설정
	
	
	/////////////////////////////////////////
	
	g_SendFlag++;

}



int main(void){
	
	Packet_t packet;
	packet.data.header[0] = packet.data.header[1] = packet.data.header[2] = packet.data.header[3] = 0xFE;
	
	InitIO();
	
	//Uart
	InitUart0();
	
	//SPI
	InitSPI();
	
	//Timer
	InitTimer0();
	InitTimer1();
	InitTimer3();


	TCNT1 = TCNT3 = 0;
	SetDutyCW(0.);
	
	//ADC
	InitADC();
	
	//LS7366
	InitLS7366();
	
	TCNT0 = 256 - 125;
	sei();

	unsigned char check = 0;
	
	//// Packet 통신 설정 ////
    while (1) {
		for(;g_BufReadCnt != g_BufWriteCnt; g_BufReadCnt++){
			
			switch(g_PacketMode){
			case 0:
				
				if (g_buf[g_BufReadCnt] == 0xFF) {
					checkSize++;
					if (checkSize == 4) {
						g_PacketMode = 1;
					}
				}
				else {
					checkSize = 0;
				}
				break;
				
			case 1:

				g_PacketBuffer.buffer[checkSize++] = g_buf[g_BufReadCnt];
				
				if (checkSize == 8) {
					if(g_PacketBuffer.data.id == g_ID){

						g_PacketMode = 2;
					}
					else{
						g_PacketMode = 0;
						checkSize = 0;
					}
				}

				break;
			
			case 2:
				
				g_PacketBuffer.buffer[checkSize++] = g_buf[g_BufReadCnt];
				check += g_buf[g_BufReadCnt];
				
				if (checkSize == g_PacketBuffer.data.size) {

					if(check == g_PacketBuffer.data.check){

						switch(g_PacketBuffer.data.mode){

							case 2:
							g_Pdes = g_PacketBuffer.data.pos / 1000.;
							g_Vlimit = g_PacketBuffer.data.velo / 1000.;
							g_Climit = g_PacketBuffer.data.cur / 1000.;
							break;
							}
					}
					
					check = 0;
					g_PacketMode = 0;
					checkSize = 0;
				}
				else if(checkSize > g_PacketBuffer.data.size || checkSize > sizeof(Packet_t)) {
					TransUart0('f');
					check = 0;
					g_PacketMode = 0;
					checkSize = 0;
				}
			}
		}

		if(g_SendFlag > 19){	//19){ TODO : 100하니까 잘됐음
			g_SendFlag = 0;			

				
			packet.data.id = g_ID;
			packet.data.size = sizeof(Packet_data_t);
			packet.data.mode = 3;
			packet.data.check = 0;
			
			
			////   Current 값 보냄     ////
			packet.data.pos = g_Pcur * 1000;// g_Pcur * 1000;	//(rad 값)*1000  -> ODE에서 rad 값이 degree로 표현됨
			packet.data.velo = g_Vcur * 1000;
			packet.data.cur = g_Ccur * 1000;		//display하는거는 torque가 맞으나 실제 내부 값은 current값이 맞음 즉 g_Climit은 전류값 [A]
			
			/////////////////////////////////////
			
			for (int i = 8; i < sizeof(Packet_t); i++)
				packet.data.check += packet.buffer[i];
				
			
			for(int i=0; i<packet.data.size; i++){
				TransUart0(packet.buffer[i]);
			}
			
		}
	}
		
}

