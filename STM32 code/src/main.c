#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"


int LEDs_state[10];
// uint32_t GPIO_state = 0;
uint16_t GPIO_Pin_Table[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4,
		 GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9};

char all_received[50] = "";
char all_received_it[50] = "";
int i = 0;

//printf in USART
void send_char(char c)
{
 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 USART_SendData(USART2, c);
}

int __io_putchar(int c)
{
 if (c=='\n')
 send_char('\r');
 send_char(c);
 return c;
}


// USART
void USART_send(char* s) {
	while(*s) {
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, *s++);
	}
}

void USART_receive(char* received_msg) {
		char c = USART_ReceiveData(USART2);
		all_received_it[i++] = c;
		//debug - if usart get some data => led
		//if(c == "0")
		printf("%c", c);
		// end debug
		int len = strlen(received_msg);
		received_msg[len] = c;
		//received_msg[len+1] = '\0';
		printf("received_msg: %s\n", received_msg);

}

void change_LED_state(int LED_number, int state) {
	LEDs_state[LED_number] = state;
}

void update_LEDs_state(char* update_str) {
	int update_str_len = strlen(update_str);
	for(int i=0; i<update_str_len;) {
		char c_LED_number = update_str[i];
		int LED_number = c_LED_number - '0';
		++i;
		char c_state = update_str[i];
		int state = c_state - '0';
		++i;
		printf("update_leds_state: led_number %d, state %d\n", LED_number, state);
		change_LED_state(LED_number, state);
	}
}

void change_GPIO_state() {
	for(int i=0; i<10; i++) {
		// 1 is to do task, 0 is done task or none task
		if(LEDs_state[i] == 1) {
			printf("change_GPIO_state SET: pin %d, led state %d\n", i, LEDs_state[i]);
			GPIO_SetBits(GPIOC, GPIO_Pin_Table[i]);
		}
		else {
			printf("change_GPIO_state RESET: pin %d, led state %d\n", i, LEDs_state[i]);
			GPIO_ResetBits(GPIOC, GPIO_Pin_Table[i]);
		}

		// something special? generalization to one GPIO line?
		// function: void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
		// uint32_t led_state = 0;
		// led_state |= (LEDs_state[i] << 1);
		// GPIO_state |= led_state;
	}
}


void save_msg(char* received_msg) {
	int len_received = strlen(received_msg);
	int len_all = strlen(all_received);
	for (int i=0; i<len_received; i++) {
		all_received[len_all] = received_msg[i];
		len_all++;
	}
	all_received[len_all] = '\0';
}

int main(void) {

	//GPIO
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);

	//USART
	USART_InitTypeDef uart;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// GPIO needed in USART
	// TX: output line - PA2
	// RX: input line - PA3
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;  // push-pull alternative function
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
			 GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpio);

	// USART
	USART_StructInit(&uart);
	uart.USART_BaudRate = 9600;
	USART_Init(USART2, &uart);

	USART_Cmd(USART2, ENABLE);

	for(int i=0; i <10; i++)
		LEDs_state[i] = 0;

	printf("START\n");

	char received_msg[20] = "";

	while(1) {
		if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
			GPIO_SetBits(GPIOC, GPIO_Pin_9);
			USART_receive(received_msg);
			if (strlen(received_msg) % 2 == 0) {
				if(strlen(received_msg) != 0) {
					update_LEDs_state(received_msg);
					save_msg(received_msg);
					printf("SAVED: %s\n", all_received);
					printf("SAVED: %s\n", all_received_it);
					change_GPIO_state();
					memset(received_msg, 0, sizeof received_msg);
				}
			}
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		}
	}
}
