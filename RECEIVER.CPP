//Lab4 file receiver
#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include<string.h>
#define COM3BASE1 0x3E8
#define COM3BASE2 0x3E8
#define TXDATA COM3BASE1
#define LCR (COM3BASE1+3)
#define LSR (COM3BASE2+5)
//#include<graphics.h>
void setup_serial(void);
void send_character(char ch);
int get_character(void);
void sender(void);
void receiver(void);

int main(void){
	//int gd=DETECT,gm;
	//initgraph(&gd,&gm,"C:\\BIN\\BGI");
	setup_serial();
	receiver();
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
	outportb(TXDATA, ch);
}
int get_character(void){
	int status;
	do{
		status = inportb(LSR) & 0x01;

	}while(status!=0x01);
	return((int)inportb(TXDATA));
}

void receiver(void){
	int inchar,Aframe,Rframe = 0,i=0,n=0;
	char *namefiles1,*namefiles2,*namefiler,name[30];
	FILE *fp;

	//namefiles

	printf("\n\nSender Send : ");
	while(1){
		inchar = get_character();
		namefiles1[i] = inchar;
		i++;
		if(inchar == '\0'){
			break;
		}
	}

	gets(namefiles2);
	printf("Save as : ");
	scanf("%s",&name);
	send_character('\s');

	fp = fopen(name,"w");
	//check namefile
	if(!strcmp(namefiles1,namefiles2)){
	//receive frame
	do{
			if(inchar == '\q')
			{
				break;
			}
			printf("\nReceive frame : %d",Rframe);
			//data
			printf("\nData : ");
			inchar = get_character();
			printf("%c", inchar);

			//Action frame
			printf("\nAction frame : ");
			Aframe = getche();
				if(Aframe == 'a' || Aframe == 'A'){
					if(n == 0){
					if(Rframe == 0){
						Rframe++;
					}else{
						Rframe--;
					}
					//write file
					fputc(inchar,fp);
					printf("\nReceived & Send ACK%d\n",Rframe);
					send_character(Aframe);
					}else{
						n=0;
						if(Rframe == 0){
							Rframe++;
						}else{
							Rframe--;
						}
						printf("\nReject & Send ACK%d\n",Rframe);
						send_character(Aframe);
					}
				}
				}else if(Aframe == 'n' || Aframe == 'N'){
					printf("\nReceived & Sleep\n");
					while(1)
					{       Aframe = get_character();
						if(Aframe == 't')
						{
							break;
						}
					}
					n++;
				}else if(Aframe == 'r' || Aframe == 'R'){
					printf("\nReject & Sleep\n");
					while(1)
					{       Aframe = get_character();
						if(Aframe == 't')
						{
							break;
						}
					}
				}else{
					send_character(Aframe);
				}

		}while(1);
	}
	fclose(fp);
}
