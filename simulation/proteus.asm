
 RJMP PC+0x0013;
 RJMP PC+0x0053;
 RJMP PC+0x005C;
 RJMP PC+0x001E;
 RJMP PC+0x001D ;		Relative jump 
 RJMP PC+0x006D	;	Relative jump 
 RJMP PC+0x0062	;	Relative jump 
 RJMP PC+0x001A	;	Relative jump 
 RJMP PC+0x0019	;	Relative jump 
 RJMP PC+0x0018	;	Relative jump 
 RJMP PC+0x0017	;	Relative jump 
 RJMP PC+0x0016	;	Relative jump 
 RJMP PC+0x0015	;	Relative jump 
 RJMP PC+0x0014	;	Relative jump 
 RJMP PC+0x0013	;	Relative jump 
 RJMP PC+0x0012	;	Relative jump 
 RJMP PC+0x0011	;	Relative jump 
 RJMP PC+0x0010	;	Relative jump 
 RJMP PC+0x000F	;	Relative jump 
 CLR R1		;Clear Register 
 OUT 0x3F,R1	;	Out to I/O location 
 LDI R28,0xDF	;	Load immediate 
 OUT 0x3D,R28	;	Out to I/O location 
 LDI R18,0x00	;	Load immediate 
 LDI R26,0x60	;	Load immediate 
 LDI R27,0x00	;	Load immediate 
 RJMP PC+0x0002	;	Relative jump 
 ST X+,R1	;	Store indirect and postincrement 
 CPI R26,0x62	;	Compare with immediate 
 CPC R27,R18	;	Compare with carry 
;--- No source file -------------------------------------------------------------
 BRNE PC-0x03	;	Branch if not equal 
 RCALL PC+0x005D;		Relative call subroutine 
 RJMP PC+0x005E	;	Relative jump 
 RJMP PC-0x0021	;	Relative jump 
;--- D:\FreeTime\Biz\LED_Indication\LEDIndication\LEDIndication\Debug/.././LEDIndication.cpp 
 OUT 0x17,R1	;	Out to I/O location 
 LDI R24,0x1C	;	Load immediate 
 OUT 0x17,R24	;	Out to I/O location 
 OUT 0x18,R1	;	Out to I/O location 
 OUT 0x18,R24	;	Out to I/O location 
 OUT 0x11,R1	;	Out to I/O location 
 LDI R24,0x02	;	Load immediate 
 OUT 0x35,R24	;	Out to I/O location 
 LDI R24,0x08	;	Load immediate 
 OUT 0x35,R24	;	Out to I/O location 
 LDI R24,0xC0	;	Load immediate 
 OUT 0x3B,R24	;	Out to I/O location 
 IN R24,0x30	;	In from I/O location 
 ORI R24,0x03	;	Logical OR with immediate 
 OUT 0x30,R24	;	Out to I/O location 
 IN R24,0x30	;	In from I/O location 
 ORI R24,0x80	;	Logical OR with immediate 
 OUT 0x30,R24	;	Out to I/O location 
 IN R24,0x30	;	In from I/O location 
 ORI R24,0x20	;	Logical OR with immediate 
 OUT 0x30,R24	;	Out to I/O location 
 OUT 0x33,R1	;	Out to I/O location 
 OUT 0x36,R1	;	Out to I/O location 
 OUT 0x3C,R1	;	Out to I/O location 
 IN R24,0x39	;	In from I/O location 
 ORI R24,0x02	;	Logical OR with immediate 
;--- D:\FreeTime\Biz\LED_Indication\LEDIndication\LEDIndication\Debug/.././LEDIndication.cpp 
 OUT 0x39,R24	;	Out to I/O location 
 IN R24,0x2F	;	In from I/O location 
 ORI R24,0x03	;	Logical OR with immediate 
 OUT 0x2F,R24	;	Out to I/O location 
 IN R24,0x2F	;	In from I/O location 
 ORI R24,0x80	;	Logical OR with immediate 
 OUT 0x2F,R24	;	Out to I/O location 
 IN R24,0x2F	;	In from I/O location 
 ORI R24,0x20	;	Logical OR with immediate 
 OUT 0x2F,R24	;	Out to I/O location 
 OUT 0x2E,R1	;	Out to I/O location 
 OUT 0x2B,R1	;	Out to I/O location 
 OUT 0x2A,R1	;	Out to I/O location 
 OUT 0x29,R1	;	Out to I/O location 
 OUT 0x28,R1	;	Out to I/O location 
;--- D:\FreeTime\Biz\LED_Indication\LEDIndication\LEDIndication\Debug/.././LEDIndication.cpp 
 IN R24,0x39	;	In from I/O location 
 ORI R24,0x80	;	Logical OR with immediate 
 OUT 0x39,R24	;	Out to I/O location 
 SEI 		;Global Interrupt Enable 
 STS 0x0061,R1	;	Store direct to data space 
 STS 0x0060,R1	;	Store direct to data space 
 RET 		;Subroutine return 
 PUSH R1	;	Push register on stack 
 PUSH R0	;	Push register on stack 
 IN R0,0x3F	;	In from I/O location 
 PUSH R0	;	Push register on stack 
 CLR R1		;Clear Register 
 POP R0		;Pop register from stack 
 OUT 0x3F,R0	;	Out to I/O location 
 POP R0		;Pop register from stack 
;--- D:\FreeTime\Biz\LED_Indication\LEDIndication\LEDIndication\Debug/.././LEDIndication.cpp 
 POP R1		;Pop register from stack 
 RETI 		;Interrupt return 
 PUSH R1	;	Push register on stack 
 PUSH R0	;	Push register on stack 
 IN R0,0x3F	;	In from I/O location 
 PUSH R0	;	Push register on stack 
 CLR R1		;Clear Register 
 POP R0		;Pop register from stack 
 OUT 0x3F,R0	;	Out to I/O location 
 POP R0		;Pop register from stack 
 POP R1		;Pop register from stack 
 RETI 		;Interrupt return 
 PUSH R1	;	Push register on stack 
 PUSH R0	;	Push register on stack 
 IN R0,0x3F	;	In from I/O location 
;--- D:\FreeTime\Biz\LED_Indication\LEDIndication\LEDIndication\Debug/.././LEDIndication.cpp 
 PUSH R0	;	Push register on stack 
 CLR R1		;Clear Register 
 POP R0		;Pop register from stack 
 OUT 0x3F,R0	;	Out to I/O location 
 POP R0		;Pop register from stack 
 POP R1		;Pop register from stack 
 RETI 		;Interrupt return 
 PUSH R1	;	Push register on stack 
 PUSH R0	;	Push register on stack 
 IN R0,0x3F	;	In from I/O location 
 PUSH R0	;	Push register on stack 
 CLR R1		;Clear Register 
 POP R0		;Pop register from stack 
 OUT 0x3F,R0	;	Out to I/O location 
 POP R0		;Pop register from stack 
;--- D:\FreeTime\Biz\LED_Indication\LEDIndication\LEDIndication\Debug/.././LEDIndication.cpp 
 POP R1		;Pop register from stack 
 RETI 		;Interrupt return 
 RCALL PC-0x005A;		Relative call subroutine 
 RJMP PC-0x0000	;	Relative jump 
;--- No source file -------------------------------------------------------------
 CLI 		;Global Interrupt Disable 
 RJMP PC-0x0000	;	Relative jump 