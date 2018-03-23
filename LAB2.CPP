#define COM3BASE 0x3E8
#define COM4BASE 0x2E8
#define TXDATA COM3BASE
#define LCR (COM3BASE+3)
#define LSR (COM3BASE+5)
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

void setup(void);
void send(int ch);
int get(void);
char *out;
char in,outchar;
int main(void)
{

	setup();
	puts("Send or Receive (s or r) : ");
	do{
		outchar = getche();
		if (outchar == 's' or outchar == 'S')
		{
			printf("\nPC1 : ");
			gets(out);
			for(int i=0;i<strlen(out);i++)
			{
				send(out[i]);
				if(outchar == 17)
				{
					return 0;
				}
			}

			printf("\nPC2 : ");
			while(1)
			{
				in = get();
				printf("%c",in);
				if(in == 17)
				{
					return 0;
				}
				if(in == '\0')
				{
					break;
				}
			}
		}
		else if(outchar == 'r' or outchar == 'R')
		{
		    printf("\nPC2 : ");
			while(1)
			{
				in = get();
				printf("%c",in);
				if(in == 17)
				{
					return 0;
				}
				if(in == '\0')
				{
					break;
				}
			}

			printf("\nPC1 : ");
			gets(out);
			for(int i=0;i<strlen(out);i++)
			{
				send(out[i]);
				if(outchar == 17)
				{
					return 0;
				}
			}

		}
	}while(outchar != 17);
	return 0;
}
void send(char ch)
{
	char s;
	do{
	s = inportb(LSR) & 0x40;
	}while(s !=0x40);
	outportb(TXDATA, ch);
}
void setup()
{
	outportb(LCR,0x80);
	outportb(TXDATA,0x0C);
	outportb(TXDATA+1,0x00);
	outportb(LCR,0x0A);
}
int get()
{
	int s;
	do
	{
	s = inportb(LSR) & 0x01;
	}while(s != 0x01);
	return ((int)inportb(TXDATA));
}
