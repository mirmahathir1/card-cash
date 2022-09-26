/*
 * GccApplication9.c
 *
 * Created: 7/29/2019 6:59:06 PM
 * Author : mdash
 */ 
#define F_CPU 1000000UL

#define D4 eS_PORTC4
#define D5 eS_PORTC5
#define D6 eS_PORTC6
#define D7 eS_PORTC7
#define RS eS_PORTC0
#define EN eS_PORTC2

#define KEY_PRT PORTA
#define KEY_DDR	DDRA
#define KEY_PIN	PINA

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "keypad.h"
#include "lcd.h" 
#include "UART.h"
#include "TRANSACTION_TYPES.h"

char name[20];
char password[5];
char tempPassword[4];
int amount;
char amountStr[10];
uint8_t transactioType;

void delay(){
	for(uint8_t i=0;i<255;i++)
		for(uint8_t j=0;j<255;j++)
			for(uint16_t k=0;k<500;k++);
}

//this function take a 4 char password and store it to tempPassword
void takePassword(uint8_t base){
	
	uint8_t i=0;
	
	while(i<4){
		tempPassword[i]=keyfind();
		
		if(tempPassword[i]!='*' && tempPassword[i]!='#'){
			i++;
			Lcd4_Write_Char('*'); //adding a char
		}
		else if(i!=0 && tempPassword[i]=='*'){
			i--; //deleting last character.
			
			//adjusting cursor
			Lcd4_Set_Cursor(2,i+base);
			Lcd4_Write_Char(' ');
			Lcd4_Set_Cursor(2,i+base);
		}
	}
}

//this function take password and match with agent's password
uint8_t takeAndMatchAgentPassword(){
	char p[]="123A";
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Agent Password:");
	Lcd4_Set_Cursor(2,0);
		
	takePassword(0);
		
	// checking whether password is matched
	for(int i=0;i<4;i++){
		if(tempPassword[i]!=p[i])
		return 0;
	}
		
	return 1;
}

// this function take password and it with user password 
// if user have no password then ignore taking password
uint8_t takeAndMatchPassword(){
	if(transactioType==TRANSACTION_WITH_PASSWORD){
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("Password:");
		takePassword(9);
		
		// checking whether password is matched
		for(int i=0;i<4;i++){
			if(tempPassword[i]!=password[i])
				return 0;
		}
		
	}
	return 1;
}

// this function take transaction amount for both cash out and cash in
void takeAmount(){
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("                ");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Amount:");
	
	amountStr[0]='-'; //by default cash out
	
	char c;
	uint8_t i=1;
	uint8_t base=7;
	
	while(1){
		c=keyfind();
		if(c>='0' && c<='9'){
			amountStr[i]=c;
			Lcd4_Write_Char(c);
			i++;
		}
		else if(i==1 && c=='#'){   // for cash in 
			amountStr[0]='+';
			Lcd4_Write_Char(c);
		}
		else if(i==1 && c=='A'){  // for skiping the transaction
			amountStr[0]='0';
			amountStr[1]='\0';
			break;
		}
		else if(c=='A'){
			amountStr[i]='\0';
			break;
		}
		else if(c=='*'){
			i--; //deleting last character.
			
			//adjusting cursor
			if(i==0){
				amountStr[0]='-';
				i=1;
			}
			else{
				Lcd4_Set_Cursor(2,i-1+base);
				Lcd4_Write_Char(' ');
				Lcd4_Set_Cursor(2,i-1+base);
			}
		}
	}
}

// for cash out if value of cash out money greater 
// than his money then show an error

uint8_t isValidAmount(){ 
	if(amountStr[0]=='+')
		return 1;
	else{
		int a=atoi(amountStr+1);
		
		if(amount>a)
			return 1;
	}
	
	return 0;
}

int main(void)
{
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	DDRC = 0xFF;
	
	UART_init(9600);
	
	Lcd4_Init();
	Lcd4_Clear();
	
	stdout = fdevopen(UART_send, NULL);
	stdin = fdevopen(NULL, UART_receive);
	
	while(1)
	{
		
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("  Punch a Card");
		
		scanf("%d%s%s%d",&transactioType,&name,&password,&amount);
		
		if(transactioType==INVALID_RFID){
			printf("0");
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("  Invalid RFID");
			continue;
		}
		else if(transactioType==SERVER_NOT_CONNECTED){
			printf("0");
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("  SERVER DOWN");
			continue;
		}
		
		//printing name
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String(name);
		Lcd4_Write_Char(' ');
		sprintf(amountStr,"%d",amount);
		Lcd4_Write_String(amountStr);
		//amountStr[0]='\0';
		
		
		//taking password and matching with client's password
		if(!takeAndMatchPassword()){
			printf("0");
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(" Wrong password");
			continue;
		}
		
		//taking amount of money for cash in or cash out
		takeAmount();
		
		if(isValidAmount()){
			if(amountStr[0]=='+' && !takeAndMatchAgentPassword()){
				printf("0");
				
				Lcd4_Clear();
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String(" Wrong Password");
			}
			else{
				printf("%s",amountStr);
				
				Lcd4_Clear();
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String(" Last: Succeed");
			}
			
		}
			
		else{
			printf("0");
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Not Enough Money");
		}
		
	}
}
