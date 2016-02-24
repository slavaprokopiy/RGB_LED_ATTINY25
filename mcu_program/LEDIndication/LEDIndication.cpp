/*
 * LEDIndication.cpp
 * ������ ��������� �� ������������ ���������� 8��� �����������
 * Created: 19.11.2014 11:27:10
 *  Author: prokopiy
 */ 

#define	tiny25

#define SYS_CLOCK 8000000
#define SYS_PERIOD 0.000000125	// 1/SYS_CLOCK


#include <avr/io.h>
#include <avr/interrupt.h>


int Status; // ���������� ��������� ������c���
// �������� ��������� ����������
#define STATUS_OFF			0 // ���� ���������
#define STATUS_ON			1 // ���� ��������
#define STATUS_TURNING_ON	2 // ���� ������������ ����������
#define STATUS_TURNING_OFF	3 // ���� ������������ ����������
#define STATUS_CHARGER_ON	4 // ���������� �������
#define STATUS_CHARGER_OFF	5 // ��������� �������

//����� � ������� 0xRRGGBB
#define COLOR_GREEN		0x64A70B // R=100(0x64)	G=167(0xA7)	B=11(0x0B)	Pantone 369C; ??HEX = 0x83c05a
#define COLOR_RED		0xE4002B // R=228(0xE4)	G=0(0x00)	B=43(0x2B)	Pantone 185C; ??HEX = 0xd22d3a
#define COLOR_ORANGE	0xED8B00 // R=237(0XED)	G=139(0X8B)	B=0(0X00)	Pantone 144C; ??HEX = 0xed8b00	

//
#ifdef tiny2313
	#define PWM_RED		OCR0B
	#define PWM_GREEN	OCR0A
	#define PWM_BLUE	OCR1A
#endif
#ifdef tiny25
	#define PWM_RED		OCR1B
	#define PWM_GREEN	OCR0A
	#define PWM_BLUE	OCR0B
#endif

uint64_t DelayTime;

// �����
int64_t DelayCycles;
void Pause(float OnPeriodTime){
	DelayCycles = (OnPeriodTime/float(SYS_PERIOD))/400;
	while (DelayCycles > 0){DelayCycles -= 1;}  // ����������� �����
}

void SystemInit(void){
// ���������� ������������ ----- 
// ���������� ���������� � ����� B: LED_RED = B2(OC0A); LED_GREEN = B3(OC1A); LED_BLUE = B4(OC1B);
#ifdef	tiny2313
	DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB4);	// ����������� �� �����
	DDRD |= (1<<DDD5);						// ����������� �� �����
#endif

#ifdef tiny25
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB4);	// ����������� �� �����
#endif
	
// ������� ���������� ----------
// INT0 - ButtonInterrupt � INT1 - ChargerConnection (����� ����� ���� ��������� � PCINT7..0)
// ������ ���������� � ����� D: PD2 - ButtonInterrupt(INT0), PD3 - ChargerConnection(INT1) 
#ifdef	tiny2313
	DDRD &= ~((1<<DDD2)|(1<<DDD3)); // ����������� PD2 � PD3 �� ����
#endif
	
#ifdef tiny25
#define ButtonInterruptPinInput PINB2
#define ChargerConnectionPinInput PINB3
// ������ ���������� � ����� B PB2 - ButtonInterrupt(PCINT2), PB3 - ChargerConnection(PCINT3) 
	DDRB &= ~((1<<DDB2)|(1<<DDB3)); // ����������� PB2 � PB3 �� ����
#endif

// ��������� ������� ����������	
#ifdef	tiny2313	
	MCUCR |= (1<<ISC01)|(0<<ISC00);	/*���������� INT0 ����������� ��	(0<<ISC01)|(0<<ISC00) - ������� ������ 
																		(1<<ISC01)|(0<<ISC00) - �� ���������� ������)*/
	MCUCR |= (1<<ISC11)|(0<<ISC10);	/*���������� INT1 ����������� ��	(0<<ISC11)|(0<<ISC10) - ������� ������ 
																		(1<<ISC11)|(0<<ISC10) - ���������� ������*/
	GIMSK |= (1<<INT1)|(1<<INT0);	// ��������� ���������� INT1 � INT0
#endif

#ifdef tiny25
	MCUCR |= (1<<ISC01); MCUCR &= ~(1<<ISC00); GIMSK |= (1<<INT0);	// ���������� INT0
	PCMSK |= (1<<PCINT3); GIMSK |= (1<<PCIE);					// ���������� PCINT3
#endif

	sei();				// ���������� ����������

	Status = STATUS_OFF;	// ����� ��������� ����������
}

void ButtonInterrupt(void){ 	// ���������� ������ ���������/����������
	// ��������� ������ �������� �������� ������
}
void ChargerConnection(void){	// ��������� ����������� �������
	
}
void ChargerDisConnection(void){	// ��������� ���������� �������
	
}

// =========================================================

// ���������� ��������� ====================================
#ifdef tiny2313
	#define Timer0CLKOn		TCCR0B |= (1<<CS00); TCCR0B &= ~((1<<CS02)|(1<<CS01))		// �������� ������������ �������0
	#define Timer0CLKOff	TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00))	// ��������� ������������ �������0
	#define Timer1CLKOn		TCCR1B (1<<CS00); TCCR1B &= ~((1<<CS02)|(1<<CS01))		// �������� ������������ �������1	
	#define Timer1CLKOff	TCCR1B &= ~((0<<CS02)|(1<<CS01)|(1<<CS00))		// ��������� ������������ �������1
	
	#define Timer0ModeFastPWM	TCCR0A |= (1<<WGM01)|(1<<WGM00)			// ����� ������ ������� 0:
	#define Timer1ModeFastPWM	TCCR1A |= (0<<WGM11)|(1<<WGM10); TCCR1B |= (0<<WGM13)|(1<<WGM12) // ����� ������ �������1 - Fast PWM 8-bit
	#define Timer0ModeNormal	TCCR0A &= ~((1<<WGM01)|(1<<WGM00))
	#define Timer1ModeNormal	TCCR1A &= ~((1<<WGM11)|(1<<WGM10));TCCR1B &= ~((1<<WGM13)|(1<<WGM12))
	
	#define OC0AModeClearOnCompareMatch	TCCR0A |= (1<<COM0A1)|(0<<COM0A0) // ����� ������ ������ OC0A
	#define OC0AModeNormal				TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0))
	
	#define OC0BModeClearOnCompareMatch	TCCR0A |= (1<<COM0B1); TCCR0A &= ~(1<<COM0B0); TCCR1 &= ~((1<<COM1A1)|(1<<COM1A0)) // ����� ������ ������ OC0B
	#define OC0BModeNormal				TCCR0A &= ~((1<<COM0B1)|(1<<COM0B0))
	
#define OC1AModeClearOnCompareMatch	GTCCR |= 1<<PWM1B; TCCR1A |= (1<<COM0A1)|(0<<COM0A0)
	#define OC1AModeNormal				TCCR1A &= ~((1<<COM0A1)|(1<<COM0A0))
	#define OC1BModeClearOnCompareMatch	GTCCR |= 1<<PWM1B; TCCR1 |= (1<<COM1B1); GTCCR &= ~(1<<COM1B0)
	#define OC1BModeNormal				GTCCR &= ~((1<<COM1B1)|(1<<COM1B0))
	
	#define LedPortReset	PORTB &= ~((1<<DDB0)|(1<<DDB1)|(1<<DDB4));PORTD &= ~(1<<DDD5)
	
	#define CountersReset	TCNT0 = 0xFF; TCNT1 = 0xFF
#endif

#ifdef tiny25
	#define Timer0CLKOn			TCCR0B |= (1<<CS00); TCCR0B &= ~((1<<CS02)|(1<<CS01))
	#define Timer0CLKOff		TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00))
	#define Timer0ModeFastPWM	TCCR0A |= (1<<WGM01)|(1<<WGM00)
	#define Timer0ModeNormal	TCCR0A &= ~((1<<WGM01)|(1<<WGM00))
	#define OC0AModeClearOnCompareMatch	TCCR0A |= 1<<COM0A1; TCCR0A &= ~(1<<COM0A0)
	#define OC0AModeNormal				TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0))
	#define OC0BModeClearOnCompareMatch	TCCR0A |= 1<<COM0B1; TCCR0A &= ~(1<<COM0B0)
	#define OC0BModeNormal				TCCR0A &= ~((1<<COM0B1)|(1<<COM0B0))
		
	#define Timer1CLKOn			PLLCSR &= ~(1<<PCKE); TCCR1 |= (1<<CS10); TCCR1 &= ~((1<<CS13)|(1<<CS12)|(1<<CS11))
	#define Timer1CLKOff		TCCR1 &= ~((1<<CS13)|(1<<CS12)|(1<<CS11)|(1<<CS10))
	#define Timer1ModeFastPWM	TCCR1 |= (1<<PWM1A); GTCCR |= (1<<PWM1B)
	#define Timer1ModeNormal	TCCR1 &= ~(1<<PWM1A); GTCCR &= ~(1<<PWM1B)
	
	#define OC1AModeClearOnCompareMatch TCCR1 |= (1<<COM1A1); TCCR1 &= ~(1<<COM1A0)
	#define OC1AModeNormal				TCCR1  &= ~((1<<COM1A1)|(1<<COM1A0))
	#define OC1BModeClearOnCompareMatch GTCCR |= (1<<COM1B1); GTCCR &= ~(0<<COM1B0)
	#define OC1BModeNormal				GTCCR  &= ~((1<<COM1B1)|(1<<COM1B0))
	
	#define LedPortReset	PORTB &= ~((1<<DDB0)|(1<<DDB1)|(1<<DDB4))
	#define CountersReset	TCNT0 = 0xFF; TCNT1 = 0xFF
#endif

// =========================================================
// ������� ���������� ������������ =========================
// ��������� ������� ���������� ������������
void LEDPinsOn(void){
	OC0AModeClearOnCompareMatch;	// ����� ������ ������ OC0A
	OC0BModeClearOnCompareMatch;	// ����� ������ ������ OC0B
	OC1BModeClearOnCompareMatch;	// ����� ������ ������ OC1B
	// ��������� ������� � ����� ������ FastPWM
	Timer0ModeFastPWM;
	Timer1ModeFastPWM;
	Timer0CLKOn;	// �������� ������������ �������0
	Timer1CLKOn;	// �������� ������������ �������1
}

// ���������� ������� ���������� ������������
void LEDPinsOff(void){
	//��������� ������� � ���������� ����� ������
	Timer0ModeNormal;	// ����� ������ �������0
	Timer1ModeNormal;	// ����� ������ �������1
	Timer0CLKOff;
	Timer1CLKOff;
	OC0AModeNormal;	// ����� ������ ������ OC0A
	OC0BModeNormal;	// ����� ������ ������ OC0B
	OC1AModeNormal;	// ����� ������ ������ OC1A
	// (0<<COM0A1)|(0<<COM0A0) - Normal port operation, OC0A disconnected.
	OC1BModeNormal;	// ����� ������ ������ OC1B
	LedPortReset;	// ���������� � "0"
	CountersReset;
}

// ��������� ��������� ��������� ��� � ������������ � ��������� ������
void SetColorPWM(int32_t Color){
	//��������� ������� � ���������� ����� ������, ��� ��������� �������� ��������� ���������
	Timer0ModeNormal;
	Timer1ModeNormal;
	PWM_RED		= int((Color>>16)	& 0x0000FF);
	PWM_GREEN	= int((Color>>8)	& 0x0000FF);
	PWM_BLUE	= int(Color			& 0x0000FF);
	Timer0ModeFastPWM;
	Timer1ModeFastPWM;
}


// ��������� ����� �� ��������� �����
void LedPulse(int32_t Color, float OnPeriodTime){ // ������� ������������ ���������-���������� ���������� � ��������� ������� ��������
	
	LEDPinsOff();		// ��������� ����������
	SetColorPWM(Color);	// ����������� ��� �� �������� ����
	LEDPinsOn();		// �������� ����������
	Pause(OnPeriodTime);
	LEDPinsOff();		// ��������� ����������
}

// ��������� ������ �������� �������
void LedSmooth(int32_t Color, int64_t OnPeriodTime){ // ������� ������������ �������� ���������-���������� � ��������� �������
	SetColorPWM(Color);// ����������� ��� �� �������� ����
}
// ==========================================================


// ������� ���������/���������� =============================
void TurningDeviceOn(void){ // �������� ��� ��������� ����������
	
	for(int i=0;i<2;i++){ 			// ������� ��������
		LedPulse(COLOR_GREEN,1000);	// ������ ������� �������� �����
		DelayTime = 1000;				
		while (DelayTime > 0){DelayTime--;}  // ����������� �����
	}
	//!d LedSmooth(COLOR_ORANGE, 2000);			// ������� ������� ��������� ������
	
}
void TurningDeviceOff(void){ // �������� ��� ���������� ����������
	LedPulse(COLOR_RED,2000);// ������ ������� �������� �����
}
// ==========================================================

// ����������� ���������� ==============================================
#ifdef tiny2313
// ���������� INT0 - ButtonInterrupt
ISR (INT0_vect){
	//! TODO - ����������� debounce
	switch(Status){
		case STATUS_OFF:				// ���� ��������� � ����������� ���������
		Status = STATUS_TURNING_ON;	// ����������
		break;
		
		case STATUS_ON:					// ���� ��������� �� ���������� ���������
		Status = STATUS_TURNING_OFF;// �����������
		break;
	}
}
// ���������� INT1 - ChargerConnection
ISR (INT1_vect){
	//! TODO - ����������� debounce
	
	// ���������� �� ������������ ������
	Status = STATUS_CHARGER_ON;
	//!t //��������������� ���������� �� ��������� �����
	
	//���������� �� ���������� ������
	//!d Status = STATUS_CHARGER_OFF;
	//!t //��������������� ���������� �� ����������� �����
}
#endif

#ifdef tiny25
// ���������� PCINT2 - ButtonInterrupt
int ButtonInterruptFlag = 0;
ISR(INT0_vect){
	LedPulse(COLOR_RED,1);
	// ��������������� ���������� �� ����������� ����� - � attiny25 ����������
	/*if(((PINB & (1<<ButtonInterruptPinInput))>>ButtonInterruptPinInput) == 0){// ���� ��� � ������ ������
		ButtonInterruptFlag = 1;// ������ ���� ����������
		Pause(0.3);	// Debounce-�������� 0.3 �������
	}
	else{ButtonInterruptFlag = 0;} // ���������� ���� ����������*/
}
// ���������� PCINT3 - ChargerConnection
ISR(PCINT0_vect){
	LedPulse(COLOR_GREEN,5);
	/*
	//! TODO - ����������� debounce
	
	// ���������� �� ������������ ������
	Status = STATUS_CHARGER_ON;
	//!t //��������������� ���������� �� ��������� �����
	
	//���������� �� ���������� ������
	//!d Status = STATUS_CHARGER_OFF;
	//!t //��������������� ���������� �� ����������� �����*/
}
#endif

// ���������� ������������ �������0
ISR (TIMER0_OVF_vect){	
	/*cli();
	OCR0A += 1; //������� ��������� A
	OCR0B += 1; //������� ��������� B
	sei();*/
}
// ���������� ������������ �������1
ISR (TIMER1_OVF_vect){ 
	/*cli();
	OCR1A += 1; //������� ��������� A
	if(OCR1A>0xFF){OCR1A = 0;}
	OCR1B += 1; //������� ��������� B
	if(OCR1B>0xFF){OCR1B = 0;}
	sei();*/
}
//=======================================================================




uint32_t ButtonDelayCounter = 0;
#define A 10000
// ������� ���� =============================================
int main(void){
	SystemInit();
	while(1){
		/*LedPulse(COLOR_GREEN,2);
		Pause(2);
		
		LedPulse(COLOR_RED,1);
		Pause(2);
		
		LedPulse(COLOR_ORANGE,0.5);
		Pause(2);*/
		if(ButtonInterruptFlag){ // ���� ���� ���������� �� ������
			ButtonDelayCounter++;	// ����������� ��������
									// ���� �������� ���������
			if(ButtonDelayCounter == A){
				ButtonDelayCounter = 0;
				switch(Status){
					case STATUS_OFF:				// ���� ��������� � ����������� ���������
					Status = STATUS_TURNING_ON;	// ����������
					break;
					
					case STATUS_ON:					// ���� ��������� �� ���������� ���������
					Status = STATUS_TURNING_OFF;// �����������
					break;
				}
			}
		}
		switch(Status){			// ��������� ��������� ����������
			case STATUS_OFF:	// ���� ���������
			;
			break;
			
			case STATUS_TURNING_ON:	// ���� ������������ ���������
			TurningDeviceOn();	// ��������� ���������
			Status = STATUS_ON;	// �������� ���� ��������� ����������
			break;

			case STATUS_ON:		// ���� ��������
			break;

			case STATUS_TURNING_OFF:	// ���� ������������ ����������
			TurningDeviceOff();			// ��������� ����������
			Status = STATUS_OFF;		// �������� ���� ��������� ����������
			;
			break;
			
			case STATUS_CHARGER_ON:	// ���������� �������
				ChargerConnection();
				//!t Status = ?
			break;

			case STATUS_CHARGER_OFF:	// ��������� �������
				ChargerDisConnection();
				Status = STATUS_TURNING_OFF;
			break;
			
			default:
			break;
		}

		//	GoToSleep();
	}
}
// ==========================================================

