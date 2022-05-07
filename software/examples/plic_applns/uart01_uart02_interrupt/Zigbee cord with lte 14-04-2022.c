/***************************************************************************
 * Project           			:  shakti devt board
 * Name of the file	     		:  uart1_uart2_interrupt.c
 * Created date			        :  31.03.2022
 * Brief Description of file             :  Transmits data from UART1 - UART2 & vice versa.
 * Name of Author    	                :  E. Kotteeswaran
 * Email ID                              :  kottee.1@gmail.com

 Copyright (C) 2019  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

 ***************************************************************************/


#include <stdint.h>
#include <stddef.h>
#include "log.h"
#include "uart.h"
#include "pinmux.h"
#include "traps.h"
#include "platform.h"
#include "plic_driver.h"
#include<stdio.h>
#include<string.h>


#define FIXEDOFFSET 14
#define MQ2 0
#define MQ7 1
#define LM75 2
enum location{start,lenbyte1,lenbyte0,frametype, frameid,addrbase, addr16bit0=13, addr16bit1, broadcast, options, rfdatabase};



unsigned int uart0_prev_rcv_isr_count_flag = 0;
unsigned int uart1_prev_rcv_isr_count_flag = 0;
unsigned int uart2_prev_rcv_isr_count_flag = 0;

unsigned int uart0_prev_tx_isr_count_flag = 0;
unsigned int uart1_prev_tx_isr_count_flag = 0;
unsigned int uart2_prev_tx_isr_count_flag = 0;
#if 1
	uint8_t uart1_buf[] = {"abcdefghijklmnopqrstuvwxyz"};
	uint8_t uart2_buf[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
#else
	uint8_t uart1_buf[] = {"abcdef"};
	uint8_t uart2_buf[] = {"ABCDEF"};
#endif


#define UART_INST uart_instance[2]
#define USE_INTERRUPT
#define MAX_OUTPUT_SIZE 1024
//#define APIPOST "https://google.com/abc" 
#define APIPOST "http://142.250.67.46/" 

#define DATA "Hello_World"
#define TIME "1000"

void init_variables()
{
	uart0_complete = 0;
	uart1_complete = 0;
	uart2_complete = 0;
	uart0_tx_isr_count = 0;
	uart0_rcv_isr_count = 0;
	uart1_tx_isr_count = 0;
	uart1_rcv_isr_count = 0;
	uart2_tx_isr_count = 0;
	uart2_rcv_isr_count = 0;
	memset(u0_rcv_char,'\0', UARTX_BUFFER_SIZE);
	memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
	memset(u2_rcv_char,'\0', UARTX_BUFFER_SIZE);
	uart0_prev_rcv_isr_count_flag = 0;
	uart1_prev_rcv_isr_count_flag = 0;
	uart2_prev_rcv_isr_count_flag = 0;

	uart0_prev_tx_isr_count_flag = 0;
	uart1_prev_tx_isr_count_flag = 0;
	uart2_prev_tx_isr_count_flag = 0;
}
extern volatile unsigned int* pinmux_config_reg;

void free_uart_buffers()
{
	flush_uart( uart_instance[0] );
	flush_uart( uart_instance[1] );
	flush_uart( uart_instance[2] );

}


int str_length(char *str)
{
    int len=0;
    while(*str!='\0')
    {
        len++;
        str++;
    }
    return len;
}

int int_to_str(int data, char * str)
{
   int i=0;
   while( data > 0 )
   {
      str[i]='0'+ (data%10);
      data/=10;
      i++;
   }
   str[i]='\0';
   int n=str_length(str)-1;
   int len=n+1;
   i=0;
   while(i<n)
   {
      char ch=str[i];
      str[i]=str[n];
      str[n]=ch;
      i++;
      n--;
   }
   return len;
}

int write_cmd(uart_struct * uart_instance, char *str)
{
    while(*str!='\0')
    {
       write_uart_character(uart_instance,*str);
       str++;
    }
    write_uart_character(uart_instance,'\r');
}


int cmp_str(char *str1,char *str2)
{
   while(*str1!='\0' && *str2!='\0')
   {
      if(*str1==*str2)
      {
         str1++;
         str2++;
      }
      else
         return 0;
   }
   return 1;
}

int read_output(uart_struct * uart_instance, char *st)
{
   delay(1);
   u2_rcv_char[++uart2_rcv_isr_count] = '\0';
//	log_info("u2_rcv: %s", u2_rcv_char);
   if(strstr(u2_rcv_char,st)==NULL)
   {
      uart2_rcv_isr_count = 0;
	   memset(u2_rcv_char,'\0', UARTX_BUFFER_SIZE);
      return 0;
   }
   uart2_rcv_isr_count = 0;
	memset(u2_rcv_char,'\0', UARTX_BUFFER_SIZE);
   return 1;
}

int setup(uart_struct * uart_instance)
{
   write_cmd(uart_instance,"AT+CGEREP=0");
   if(read_output(uart_instance,"OK"))
   {
      log_info("\nDisabled unsolicited result code");
   }
   else
   {
      log_info("\nAT+CGEREP command failed");
      return 0;
   }

   write_cmd(uart_instance,"AT");
   if(read_output(uart_instance,"OK"))
   {
      log_info("\nCommunication Verified");
   }
   else
   {
      log_info("\nAT command failed");
      return 0;
   }

   write_cmd(uart_instance,"AT+CPIN?");
   if(read_output(uart_instance,"OK"))
   {
      log_info("\nCPIN Success");
   }
   else
   {
      log_info("\nAT+CPIN? command failed");
      return 0;
   }

   write_cmd(uart_instance,"AT+CSQ");
   if(read_output(uart_instance,"OK"))
   {
      log_info("\nSignal strength:OK");
   }
   else
   {
      log_info("\nAT+CSQ command failed");
      return 0;
   }
   
   write_cmd(uart_instance,"AT+CREG?");
   if(read_output(uart_instance,"OK"))
   {
      log_info("\nReg to Circuit Switching Domain");
   }
   else
   {
      log_info("\nAT+CREG? command failed");
      return 0;
   }

   write_cmd(uart_instance,"AT+CGREG?");
   if(read_output(uart_instance,"OK"))
   {
      log_info("\nReg to Packet  Switching Domain");
   }
   else
   {
      log_info("\nAT+CGREG? command failed");
      return 0;
   }
   
   write_cmd(uart_instance,"AT+CPSI?");
   if(read_output(uart_instance,"OK"))
   {
      log_info("\nUE System Info : OK");
   }
   else
   {
      log_info("\nAT+CPSI? command failed");
      return 0;
   }
   return 1;
}

int recall_HTTP_service(uart_struct * uart_instance)
{
   int i=5;
   while(i--)
   {
      delay(20);
      write_cmd(uart_instance,"AT+HTTPTERM");
      if(read_output(uart_instance,"OK"))
      {
         log_info("\nHTTP Service Terminated");
      }
      else
      {
         log_info("\nAT+HTTPTERM Failed");
         continue;
      }
      write_cmd(uart_instance,"AT+HTTPINIT");
      if(read_output(uart_instance,"OK"))
      {
         log_info("\nHTTP Service Started");
         return 1;
      }
      else
      {
         log_info("\nAT+HTTPINIT command failed again");
      }
   }
   return 0;
}

int SendDataPOST(uart_struct * uart_instance, char *api, char *data , char *time)
{
    write_cmd(uart_instance,"AT+HTTPINIT");
    if(read_output(uart_instance,"OK"))
    {
        log_info("\nHTTP Service Started");
    }
    else
    {
        log_info("\nAT+HTTPINIT command failed");
        if(!recall_HTTP_service(uart_instance))
            return 0;
    }
    char cmd[200]="AT+HTTPPARA=\"URL\",\"";
    int len1=str_length(cmd);
    int len2=str_length(api);
    int len3;

    int i,j,n,flag;
    i=len1;
    j=0;
    n=len1+len2;

    while(i<n)
    {
        cmd[i++]=api[j++];
    }
    cmd[i++]='"';
    cmd[i]='\0';
    //log_info("\nurl::%s::", cmd);
    write_cmd(uart_instance,cmd);
    if(read_output(uart_instance,"OK"))
    {
        log_info("\nURL set:%s",cmd);
    }
    else
    {
        log_info("\nAT+HTTPPARA command failed");
        return 0;
    }
    
    char cmd2[50]="AT+HTTPDATA=";
    len1=str_length(cmd2);
    int temp=str_length(data)+1;
    char str[32];
    len2=int_to_str(temp,str);
    len3=str_length(time);

    i=len1;
    j=0;
    n=len1+len2;
    
    while(i<n)
    {
        cmd2[i++]=str[j++];
    }
    cmd2[i++]=',';
    j=0;
    n+=len3;

    while(i<n)
    {
       cmd2[i++]=time[j++];
    }
    cmd2[i]='\0';

    write_cmd(uart_instance,cmd2);
    if(read_output(uart_instance,"DOWNLOAD"))
    {
        write_cmd(uart_instance,data);
        if(read_output(uart_instance,"OK"))
        {
           log_info("\nData Uploaded");
        }
        else
        {
           log_info("\nData Uploading failed");
           return 0;
        }
    }
    else
    {
        log_info("\nAT+HTTPDATA command failed");
    }
    
   

    flag=0;
    write_cmd(uart_instance,"AT+HTTPACTION=1");
		log_info("\n\tu2_rcv: %s", u2_rcv_char);


    if(read_output(uart_instance,"OK"))
    {
        while(flag!=1)
        {
            if(read_output(uart_instance,"+HTTPACTION"))
            {
                log_info("\nData Sent");
                flag=1;
            }
            else
                delay(10);
        }
        
    }
    else
    {
        log_info("\nAT+HTTPACTION command failed");
        return 0;
    }

    write_cmd(uart_instance,"AT+HTTPREAD=0,100");
    // if(read_output(uart_instance,"OK"))
    // {
    //     log_info("\nStop HTTP Service");
    // }
    // else
    // {
    //     log_info("\nAT+HTTPTERM command failed");
    //     return 0;
    // }
    
    return 1;
}
 //system clock/16*19200
 //x/hx 0x11300

int SendDataRF(uart_struct * uart_instance, int enddevice, char *rfdata){

   //Creating the RF Frame to send data to end device
    unsigned char data[500];
    //= {'\x7E','\x00' ,'\x12','\x10','\x01','\x00','\x13','\xA2','\x00','\x41','\xF1','\xD3','\xD7','\xFF','\xFE','\x00','\x00','\x63','\x6F','\x6F','\x64','\xBB'};

    //start delimiter    
    data[start] = '\x7E';

    //rf data
   //char rfdata[100] = "cood";
    for(int i=0;i<strlen(rfdata);i++){
        data[rfdatabase+i] = rfdata[i]; 
    }

    //length 2 bytes
    unsigned short int len = FIXEDOFFSET + strlen(rfdata);
    char *p = (char*)&len;
    data[lenbyte0] = *(p++); // length (2B)
    data[lenbyte1] = *p;

    //frametype and frame id
    data[frametype] = '\x10';
    data[frameid] = '\x01';

    //destination address 64-bit
    char addr[4][8] ={ {'\x00','\x13','\xA2','\x00','\x41','\xF1','\xD3','\xD7'},
                       {'\x00','\x13','\xA2','\x00','\x41','\xF1','\xD3','\xD7'},
                       {'\x00','\x13','\xA2','\x00','\x41','\xF1','\xD3','\xD7'},
                       {'\x00','\x13','\xA2','\x00','\x41','\xF1','\xD3','\xD7'} 
                     };
   
    for(int i=0;i<8;i++){
        data[addrbase+i] = addr[enddevice][i]; 
    }
    //destination address 16-bit
    data[addr16bit0] = '\xFF';
    data[addr16bit1] = '\xFE';
    data[broadcast] = '\x00';
    data[options] = '\x00';

    //checksum calculation
    unsigned int sum=0;
    for(int i=frametype;i<rfdatabase+strlen(rfdata);i++){
         sum+=data[i];
    }
    int checksum = 0xFF - (sum&0xFF);
    int checksumoffest = rfdatabase+strlen(rfdata);
    data[checksumoffest] = checksum;

    printf("%x\n", checksum);

    for(int i=start;i<rfdatabase+strlen(rfdata)+1;i++){
        printf("%d %x\n", i, data[i]);
    }

    //sending RF data

   write_uart_string(uart_instance, data);   

}

int calchecksum(char *data){
    //checksum calculation
    unsigned int sum=0;
    for(int i=frametype;i<strlen(data);i++){
         sum+=data[i];
    }
    if(sum&0xFF == 0xFF){
        return 1;
    }
    else return 0;
}


int main(void){

	unsigned int int_id = 1;
	register uint32_t retval;
	int i;
//	printf("\tUart 1 - Uart 2 Interrupt Test\n");

	init_variables();

	pinmux_enable_uart1_uart2();
	set_baud_rate(uart_instance[0], 115200);
	set_baud_rate(uart_instance[1], 9600);
	//set_baud_rate(uart_instance[2], 115200);

	plic_init();

	configure_interrupt(PLIC_INTERRUPT_26);
	isr_table[PLIC_INTERRUPT_26] = uart1_isr;
	// configure_interrupt(PLIC_INTERRUPT_27);
	// isr_table[PLIC_INTERRUPT_27] = uart2_isr;

	uart1_prev_tx_isr_count_flag = uart1_tx_isr_count;
	uart1_prev_rcv_isr_count_flag = uart1_rcv_isr_count;
	uart2_prev_tx_isr_count_flag = uart2_tx_isr_count;
	uart2_prev_rcv_isr_count_flag = uart2_rcv_isr_count;
	free_uart_buffers();
	printf("Before Tx00\n");
	printf("\n UART 1 Status Register: %x", uart_instance[1]->status);
	printf("\n UART 2 Status Register: %x", uart_instance[2]->status);
	enable_uart_interrupts(uart_instance[1],  ENABLE_RX_NOT_EMPTY );//| ENABLE_RX_FULL);
	// enable_uart_interrupts(uart_instance[2],  ENABLE_RX_NOT_EMPTY );//| ENABLE_RX_FULL);

	// Enable Global (PLIC) interrupts.
	asm volatile("li      t0, 8\t\n"
		     "csrrs   zero, mstatus, t0\t\n"
		    );

	asm volatile("li      t0, 0x800\t\n"
		     "csrrs   zero, mie, t0\t\n"
		    );

	asm volatile(
		     "csrr %[retval], mstatus\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	printf(" retval = %x\n", retval);
	asm volatile(
		     "csrr %[retval], mie\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	printf(" retval = %x\n", retval);

	asm volatile(
		     "csrr %[retval], mip\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );
	printf(" retval = %x\n", retval);
	// while(1){
	// 	int t;
	// 	read_uart_character(uart_instance[1], &t);
	// 	log_info("\n	printing t:%d", t);
	// }
	// if(setup(uart_instance[2]))
   // {
	//    log_info("sending post data");
   //     //SendDataGET(UART_INST,APIGET,"30");
   //     SendDataPOST(uart_instance[2],APIPOST,DATA,TIME);
	// 	delay(10);
	// 	printf("\n UART 2 from website: %s", u2_rcv_char);
   // }
   // else
   // {
   //    printf("\n------------------ERROR------------------------");
   //    //return -1;
   // }

	//i = 0;
	// write_uart_string(uart_instance[1], uart1_buf);
	// write_uart_string(uart_instance[2], uart2_buf);
	
   printf("starting Co-ordinator\n");
   int j=0;

	while(1)
	{
      SendDataRF(uart_instance[1], MQ2 , "Hi MQ2");

		//write_uart_string(uart_instance[1],"mnopqrstuvwx");
		for(int tt=0;tt<10;tt++){
			delay_loop(2000,1000);	
		}
		if(uart1_rcv_isr_count != 0)
			{
				u1_rcv_char[++uart1_rcv_isr_count] = '\0';
				printf("\n UART 1 recv. Chars: %s", u1_rcv_char);
            if(calchecksum(u1_rcv_char)){
               printf("Packet successfully received by enddevice!");
            }
            else{
               printf("Checksum error! Please resend packet");
            }
				uart1_rcv_isr_count = 0;
				memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
			}
		delay_loop(2000,1000);
	
	};


	return 0;
}








		//	printf("\n UART 1 Status Register: %x", uart_instance[1]->status);

		// if(uart1_prev_tx_isr_count_flag != uart1_tx_isr_count)
		// {
		// 	uart1_prev_tx_isr_count_flag = uart1_tx_isr_count;
		// }
		// if(uart1_prev_rcv_isr_count_flag != uart1_rcv_isr_count)
		// {
		// 	uart1_prev_rcv_isr_count_flag = uart1_rcv_isr_count;
		// }
		// 
	



		//if(uart2_prev_tx_isr_count_flag != uart2_tx_isr_count)
		// {
		// 	uart2_prev_tx_isr_count_flag = uart2_tx_isr_count;
		// }
		// if(uart2_prev_rcv_isr_count_flag != uart2_rcv_isr_count)
		// {
		// 	uart2_prev_rcv_isr_count_flag = uart2_rcv_isr_count;
		// }


			// 		if(uart2_rcv_isr_count != 0)
			// {
			// 	u2_rcv_char[++uart2_rcv_isr_count] = '\0';
			// 	// printf("\n UART 2 TxISR Count Flag: %d", uart2_tx_isr_count);
			// 	// printf("\n UART 2 Rcv ISR Count Flag: %d", uart2_rcv_isr_count);
			// 	// printf("\n UART 2 recv. Chars: %s", u2_rcv_char);
			// 	uart2_rcv_isr_count = 0;
			// 	memset(u2_rcv_char,'\0', UARTX_BUFFER_SIZE);
			// 	//break;
			// }




// void isr(uart_struct *instance)
// {
//    register uint32_t retval;
// 	if(instance == uart_instance[0])
//       init_variables_uart0();
//    else if(instance == uart_instance[1])
//       init_variables_uart1();
//    else
//       init_variables_uart2();

//    flush_uart(instance);
// 	plic_init();

// 	configure_interrupt(PLIC_INTERRUPT_26);
// 	isr_table[PLIC_INTERRUPT_26] = uart1_isr;

// 	// Enable Global (PLIC) interrupts.
// 	asm volatile("li      t0, 8\t\n"
// 		     "csrrs   zero, mstatus, t0\t\n"
// 		    );

// 	asm volatile("li      t0, 0x800\t\n"
// 		     "csrrs   zero, mie, t0\t\n"
// 		    );

// 	asm volatile(
// 		     "csrr %[retval], mstatus\n"
// 		     :
// 		     [retval]
// 		     "=r"
// 		     (retval)
// 		    );

// 	asm volatile(
// 		     "csrr %[retval], mie\n"
// 		     :
// 		     [retval]
// 		     "=r"
// 		     (retval)
// 		    );

// 	asm volatile(
// 		     "csrr %[retval], mip\n"
// 		     :

// 		     [retval]
// 		     "=r"
// 		     (retval)
// 		    );

// 	flush_uart(instance);
// 	enable_uart_interrupts(instance,  ENABLE_RX_NOT_EMPTY);// | ENABLE_RX_FULL);
// }



// delay_loop(2000,1000);
		// write_uart_string(uart_instance[1], "+++");
		// delay_loop(2000,1000);



		// write_uart_string(uart_instance[1], "ATDH0\r");
		// delay_loop(2000,1000);
		// if(uart1_rcv_isr_count != 0)
		// 	{
		// 		u1_rcv_char[++uart1_rcv_isr_count] = '\0';
		// 		// log_info("\n ATDH0 UART 1 TxISR Count Flag: %d", uart1_tx_isr_count);
		// 		// log_info("\n UART 1 Rcv ISR Count Flag: %d", uart1_rcv_isr_count);
		// 		printf("\n UART 1 recv. Chars: %s", u1_rcv_char);
		// 		uart1_rcv_isr_count = 0;
		// 		memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
		// 	}

		// write_uart_string(uart_instance[1], "ATDL0\r");
		// delay_loop(2000,1000);
		// if(uart1_rcv_isr_count != 0)
		// 	{
		// 		u1_rcv_char[++uart1_rcv_isr_count] = '\0';
		// 		// printf("\n ATDL0 UART 1 TxISR Count Flag: %d", uart1_tx_isr_count);
		// 		// printf("\n UART 1 Rcv ISR Count Flag: %d", uart1_rcv_isr_count);
		// 		printf("\n UART 1 recv. Chars: %s", u1_rcv_char);
		// 		uart1_rcv_isr_count = 0;
		// 		memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
		// 	}

		// write_uart_string(uart_instance[1], "ATCN\r");
		// delay_loop(2000,1000);
		// if(uart1_rcv_isr_count != 0)
		// 	{
		// 		u1_rcv_char[++uart1_rcv_isr_count] = '\0';
		// 		// printf("\n ATDCN UART 1 TxISR Count Flag: %d", uart1_tx_isr_count);
		// 		// printf("\n UART 1 Rcv ISR Count Flag: %d", uart1_rcv_isr_count);
		// 		printf("\n UART 1 recv. Chars: %s", u1_rcv_char);
		// 		uart1_rcv_isr_count = 0;
		// 		memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
		// 	}
		// delay_loop(2000,1000);

