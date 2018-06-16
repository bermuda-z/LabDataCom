//Lab3 serial chat
#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include<string.h>
#define COM3BASE1 0x3E8
#define COM3BASE2 0x3E8
#define TXDATA COM3BASE1
#define LCR (COM3BASE1+3)
#define LSR (COM3BASE2+5)

void setup_serial(void);
void send_character(char ch);
int get_character(void);
void sender(void);
void receiver(void);

int main(void){
	char status;
	setup_serial();
	puts("Send or Recieve");
	status = getch();
	printf("%c", status);
	if(status == 's' || status == 'S'){
		sender();
	}
	else if(status == 'r' || status == 'R'){
		receiver();
	}
	return(0);
}

void setup_serial(void){
	outportb(LCR, 0x80);
	outportb(TXDATA, 0x0C);
	outportb(TXDATA+1, 0x00);
	outportb(LCR, 0x0A);
}

void send_character(char ch){
	char status;
	do{
		status = inportb(LSR) & 0x40;
	}while(status!=0x40);
	outportb(TXDATA, (char)ch);
}

int get_character(void){
	int status;
	do{
		status = inportb(LSR) & 0x01;

	}while(status!=0x01);
	return((int)inportb(TXDATA));
}

void sender(void){
	int inchar;
	char *outchar;
	do{
		printf("\nPc1: ");

		gets(outchar);
		for(int i = 0; i<strlen(outchar); i++){
			send_character(outchar[i]);
			if(outchar[i] == 17){
				return;
			}
		}
		send_character('\0');
		printf("\nPc2: ");
		//inchar = get_character();
		//printf("%c\n", inchar);
		while(1){
			inchar = get_character();
			if(inchar == 17){
				return;
			}
			printf("%c", inchar);
			if(inchar == '\0'){
				printf(" \n");
				break;
			}

		}
	}while(1);

}

void receiver(void){
	int inchar;
	char *outchar;
	do{
		printf("\nPc1: ");
		while(1){
			inchar = get_character();
			if(inchar == 17){
				return;
			}
			printf("%c", inchar);
			if(inchar == '\0'){
				printf(" \n");
				break;
			}

		}
		printf("\nPc2: ");
		//scanf("%[\n]", outchar);
		//outchar = getch();
		//send_character(outchar);
		gets(outchar);
		for(int i = 0; i<strlen(outchar); i++){
			send_character(outchar[i]);
			if(outchar[i] == 17){
				return;
			}
		}
		send_character('\0');
	}while(inchar!=4);

}

