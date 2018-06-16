//Lab4 file sender
#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
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
	setup_serial();
	sender();
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

void sender(){
	int inchar,Aframe,c,frame = 0;
	clock_t start,stop;
	char timeout;
	char *namefile;
	FILE *fp;
	//filename
	printf("\n\nSend file : ");
	gets(namefile);
		for(int i = 0; i<strlen(namefile); i++){
			send_character(namefile[i]);
		}
		send_character('\0');
	fp = fopen(namefile,"r");
	//Read form file
	c = fgetc(fp);
	inchar = get_character();
	if(inchar == '\s'){
		while(1)
		{
			if(feof(fp)){
				send_character('\q');
				break;
			}
			printf("\n\nSend frame : %d",frame);
			printf("\nData : %c",c);
			send_character(c);
			printf("\nTimeout : ");

			timeout = getche();
			if(timeout == 't'){
                send_character('t');
                continue;
			}
            Aframe = get_character();
			if(Aframe == 'a' || Aframe == 'A'){
				if(frame == 0){
				frame++;
				}
				else{
				frame--;
				}
				printf("\nReceive ACK%d",frame);
				c = fgetc(fp);
				}

			}
		}
	}
	fclose(fp);
}
