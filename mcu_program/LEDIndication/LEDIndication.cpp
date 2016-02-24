/*
 * LEDIndication.cpp
 * Проект рассчитан на тактирование внутренним 8МГц генератором
 * Created: 19.11.2014 11:27:10
 *  Author: prokopiy
 */ 

#define	tiny25

#define SYS_CLOCK 8000000
#define SYS_PERIOD 0.000000125	// 1/SYS_CLOCK


#include <avr/io.h>
#include <avr/interrupt.h>


int Status; // переменная состояния устройcтва
// Варианты состояний устройства
#define STATUS_OFF			0 // Если отключено
#define STATUS_ON			1 // Если включено
#define STATUS_TURNING_ON	2 // Если инициировано отключение
#define STATUS_TURNING_OFF	3 // Если инициировано отключение
#define STATUS_CHARGER_ON	4 // Подключена зарядка
#define STATUS_CHARGER_OFF	5 // Отключена зарядка

//Цвета в формате 0xRRGGBB
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

// Пауза
int64_t DelayCycles;
void Pause(float OnPeriodTime){
	DelayCycles = (OnPeriodTime/float(SYS_PERIOD))/400;
	while (DelayCycles > 0){DelayCycles -= 1;}  // Выдерживаем паузу
}

void SystemInit(void){
// Управление светодиодами ----- 
// Светодиоды подключены к порту B: LED_RED = B2(OC0A); LED_GREEN = B3(OC1A); LED_BLUE = B4(OC1B);
#ifdef	tiny2313
	DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB4);	// Настраиваем на выход
	DDRD |= (1<<DDD5);						// Настраиваем на выход
#endif

#ifdef tiny25
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB4);	// Настраиваем на выход
#endif
	
// Внешние прерывания ----------
// INT0 - ButtonInterrupt и INT1 - ChargerConnection (также могут быть привязаны к PCINT7..0)
// Кнопки подключены к порту D: PD2 - ButtonInterrupt(INT0), PD3 - ChargerConnection(INT1) 
#ifdef	tiny2313
	DDRD &= ~((1<<DDD2)|(1<<DDD3)); // Настраиваем PD2 и PD3 на вход
#endif
	
#ifdef tiny25
#define ButtonInterruptPinInput PINB2
#define ChargerConnectionPinInput PINB3
// Кнопки подключены к порту B PB2 - ButtonInterrupt(PCINT2), PB3 - ChargerConnection(PCINT3) 
	DDRB &= ~((1<<DDB2)|(1<<DDB3)); // Настраиваем PB2 и PB3 на вход
#endif

// Настройка внешних прерываний	
#ifdef	tiny2313	
	MCUCR |= (1<<ISC01)|(0<<ISC00);	/*Прерывание INT0 срабатывает по	(0<<ISC01)|(0<<ISC00) - низкому уровню 
																		(1<<ISC01)|(0<<ISC00) - по спадаещему фронту)*/
	MCUCR |= (1<<ISC11)|(0<<ISC10);	/*Прерывание INT1 срабатывает по	(0<<ISC11)|(0<<ISC10) - низкому уровню 
																		(1<<ISC11)|(0<<ISC10) - спадаещему фронту*/
	GIMSK |= (1<<INT1)|(1<<INT0);	// Разрешаем прерывания INT1 и INT0
#endif

#ifdef tiny25
	MCUCR |= (1<<ISC01); MCUCR &= ~(1<<ISC00); GIMSK |= (1<<INT0);	// Прерывание INT0
	PCMSK |= (1<<PCINT3); GIMSK |= (1<<PCIE);					// Прерывание PCINT3
#endif

	sei();				// Разрешение прерываний

	Status = STATUS_OFF;	// Сброс состояния устройства
}

void ButtonInterrupt(void){ 	// Прерывание кнопки включения/отключения
	// Запускаем таймер пропуска дребезга кнопки
}
void ChargerConnection(void){	// Обработка подключения зарядки
	
}
void ChargerDisConnection(void){	// Обработка отключения зарядки
	
}

// =========================================================

// Управление таймерами ====================================
#ifdef tiny2313
	#define Timer0CLKOn		TCCR0B |= (1<<CS00); TCCR0B &= ~((1<<CS02)|(1<<CS01))		// Включаем тактирование Таймера0
	#define Timer0CLKOff	TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00))	// Отключаем тактирование Таймера0
	#define Timer1CLKOn		TCCR1B (1<<CS00); TCCR1B &= ~((1<<CS02)|(1<<CS01))		// Включаем тактирование Таймера1	
	#define Timer1CLKOff	TCCR1B &= ~((0<<CS02)|(1<<CS01)|(1<<CS00))		// Отключаем тактирование Таймера1
	
	#define Timer0ModeFastPWM	TCCR0A |= (1<<WGM01)|(1<<WGM00)			// Режим работы таймера 0:
	#define Timer1ModeFastPWM	TCCR1A |= (0<<WGM11)|(1<<WGM10); TCCR1B |= (0<<WGM13)|(1<<WGM12) // Режим работы таймера1 - Fast PWM 8-bit
	#define Timer0ModeNormal	TCCR0A &= ~((1<<WGM01)|(1<<WGM00))
	#define Timer1ModeNormal	TCCR1A &= ~((1<<WGM11)|(1<<WGM10));TCCR1B &= ~((1<<WGM13)|(1<<WGM12))
	
	#define OC0AModeClearOnCompareMatch	TCCR0A |= (1<<COM0A1)|(0<<COM0A0) // Режим работы вывода OC0A
	#define OC0AModeNormal				TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0))
	
	#define OC0BModeClearOnCompareMatch	TCCR0A |= (1<<COM0B1); TCCR0A &= ~(1<<COM0B0); TCCR1 &= ~((1<<COM1A1)|(1<<COM1A0)) // Режим работы вывода OC0B
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
// Функции управления светодиодами =========================
// Включение выводов управления светодиодами
void LEDPinsOn(void){
	OC0AModeClearOnCompareMatch;	// Режим работы вывода OC0A
	OC0BModeClearOnCompareMatch;	// Режим работы вывода OC0B
	OC1BModeClearOnCompareMatch;	// Режим работы вывода OC1B
	// Переводим таймеры в режим работы FastPWM
	Timer0ModeFastPWM;
	Timer1ModeFastPWM;
	Timer0CLKOn;	// Включаем тактирование Таймера0
	Timer1CLKOn;	// Включаем тактирование Таймера1
}

// Отключение выводов управления светодиодами
void LEDPinsOff(void){
	//Переводим таймеры в нормальный режим работы
	Timer0ModeNormal;	// Режим работы таймера0
	Timer1ModeNormal;	// Режим работы таймера1
	Timer0CLKOff;
	Timer1CLKOff;
	OC0AModeNormal;	// Режим работы вывода OC0A
	OC0BModeNormal;	// Режим работы вывода OC0B
	OC1AModeNormal;	// Режим работы вывода OC1A
	// (0<<COM0A1)|(0<<COM0A0) - Normal port operation, OC0A disconnected.
	OC1BModeNormal;	// Режим работы вывода OC1B
	LedPortReset;	// Сбрасываем в "0"
	CountersReset;
}

// Установка регистров сравнения ШИМ в соответствии с указанным цветом
void SetColorPWM(int32_t Color){
	//Переводим таймеры в нормальный режим работы, для установки значений регистров сравнения
	Timer0ModeNormal;
	Timer1ModeNormal;
	PWM_RED		= int((Color>>16)	& 0x0000FF);
	PWM_GREEN	= int((Color>>8)	& 0x0000FF);
	PWM_BLUE	= int(Color			& 0x0000FF);
	Timer0ModeFastPWM;
	Timer1ModeFastPWM;
}


// Включение цвета на указанное время
void LedPulse(int32_t Color, float OnPeriodTime){ // Функция однократного включения-отключения светодиода с указанием времени свечения
	
	LEDPinsOff();		// Отключаем светодиоды
	SetColorPWM(Color);	// Настраиваем ШИМ на заданный цвет
	LEDPinsOn();		// Включаем светодиоды
	Pause(OnPeriodTime);
	LEDPinsOff();		// Отключаем светодиоды
}

// Включение режима плавного мигания
void LedSmooth(int32_t Color, int64_t OnPeriodTime){ // Функция однократного плавного включения-отключения с указанием периода
	SetColorPWM(Color);// Настраиваем ШИМ на заданный цвет
}
// ==========================================================


// Функции включения/отключения =============================
void TurningDeviceOn(void){ // Действия при включении устройства
	
	for(int i=0;i<2;i++){ 			// Счетчик морганий
		LedPulse(COLOR_GREEN,1000);	// Подаем импульс зеленого цвета
		DelayTime = 1000;				
		while (DelayTime > 0){DelayTime--;}  // Выдерживаем паузу
	}
	//!d LedSmooth(COLOR_ORANGE, 2000);			// Плавное мигание оранжевым цветом
	
}
void TurningDeviceOff(void){ // Действия при выключении устройства
	LedPulse(COLOR_RED,2000);// Подаем импульс красного цвета
}
// ==========================================================

// Обработчики прерываний ==============================================
#ifdef tiny2313
// Прерывание INT0 - ButtonInterrupt
ISR (INT0_vect){
	//! TODO - реализовать debounce
	switch(Status){
		case STATUS_OFF:				// Если находимся в выключенном состоянии
		Status = STATUS_TURNING_ON;	// Включаемся
		break;
		
		case STATUS_ON:					// Если находимся во включенном состоянии
		Status = STATUS_TURNING_OFF;// Выключаемся
		break;
	}
}
// Прерывание INT1 - ChargerConnection
ISR (INT1_vect){
	//! TODO - реализовать debounce
	
	// Прерывание по нарастающему фронту
	Status = STATUS_CHARGER_ON;
	//!t //Перенастраиваем прерывание на спадающий фронт
	
	//Прерывание по спадающему фронту
	//!d Status = STATUS_CHARGER_OFF;
	//!t //Перенастраиваем прерывание на нарастающий фронт
}
#endif

#ifdef tiny25
// Прерывание PCINT2 - ButtonInterrupt
int ButtonInterruptFlag = 0;
ISR(INT0_vect){
	LedPulse(COLOR_RED,1);
	// Перенастраиваем прерывание на нарастающий фронт - в attiny25 недоступно
	/*if(((PINB & (1<<ButtonInterruptPinInput))>>ButtonInterruptPinInput) == 0){// Если пин в нижнем уровне
		ButtonInterruptFlag = 1;// Ставим флаг прерывания
		Pause(0.3);	// Debounce-задержка 0.3 секунды
	}
	else{ButtonInterruptFlag = 0;} // Сбрасываем флаг прерывания*/
}
// Прерывание PCINT3 - ChargerConnection
ISR(PCINT0_vect){
	LedPulse(COLOR_GREEN,5);
	/*
	//! TODO - реализовать debounce
	
	// Прерывание по нарастающему фронту
	Status = STATUS_CHARGER_ON;
	//!t //Перенастраиваем прерывание на спадающий фронт
	
	//Прерывание по спадающему фронту
	//!d Status = STATUS_CHARGER_OFF;
	//!t //Перенастраиваем прерывание на нарастающий фронт*/
}
#endif

// Прерывание переполнения таймера0
ISR (TIMER0_OVF_vect){	
	/*cli();
	OCR0A += 1; //Регистр сравнения A
	OCR0B += 1; //Регистр сравнения B
	sei();*/
}
// Прерывание переполнения таймера1
ISR (TIMER1_OVF_vect){ 
	/*cli();
	OCR1A += 1; //Регистр сравнения A
	if(OCR1A>0xFF){OCR1A = 0;}
	OCR1B += 1; //Регистр сравнения B
	if(OCR1B>0xFF){OCR1B = 0;}
	sei();*/
}
//=======================================================================




uint32_t ButtonDelayCounter = 0;
#define A 10000
// Главный цикл =============================================
int main(void){
	SystemInit();
	while(1){
		/*LedPulse(COLOR_GREEN,2);
		Pause(2);
		
		LedPulse(COLOR_RED,1);
		Pause(2);
		
		LedPulse(COLOR_ORANGE,0.5);
		Pause(2);*/
		if(ButtonInterruptFlag){ // Если было прерывание по кнопке
			ButtonDelayCounter++;	// Отсчитываем задержку
									// Если задержка выдержана
			if(ButtonDelayCounter == A){
				ButtonDelayCounter = 0;
				switch(Status){
					case STATUS_OFF:				// Если находимся в выключенном состоянии
					Status = STATUS_TURNING_ON;	// Включаемся
					break;
					
					case STATUS_ON:					// Если находимся во включенном состоянии
					Status = STATUS_TURNING_OFF;// Выключаемся
					break;
				}
			}
		}
		switch(Status){			// Проверяем состояние устройства
			case STATUS_OFF:	// Если отключено
			;
			break;
			
			case STATUS_TURNING_ON:	// Если инициировано включение
			TurningDeviceOn();	// Реализуем включение
			Status = STATUS_ON;	// Изменяем флаг состояния устройства
			break;

			case STATUS_ON:		// Если включено
			break;

			case STATUS_TURNING_OFF:	// Если инициировано отключение
			TurningDeviceOff();			// Реализуем отключение
			Status = STATUS_OFF;		// Изменяем флаг состояния устройства
			;
			break;
			
			case STATUS_CHARGER_ON:	// Подключена зарядка
				ChargerConnection();
				//!t Status = ?
			break;

			case STATUS_CHARGER_OFF:	// Отключена зарядка
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

