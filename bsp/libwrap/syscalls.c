/*
Copyright (c) 2013, The Regents of the University of California (Regents).
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of the Regents nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


modified for shakti

*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <sys/signal.h>
#include <encoding.h>

#define SYS_write 64

#undef strcmp

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;

/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
static uintptr_t syscall(uintptr_t which, uint64_t arg0, uint64_t arg1, uint64_t arg2)
{
	volatile uint64_t magic_mem[8] __attribute__((aligned(64)));
	magic_mem[0] = which;
	magic_mem[1] = arg0;
	magic_mem[2] = arg1;
	magic_mem[3] = arg2;
	//  __sync_synchronize();

	tohost = (uintptr_t)magic_mem;
	while (fromhost == 0)
		;
	//  fromhost = 0;

	//  __sync_synchronize();
	return magic_mem[0];
}

#define NUM_COUNTERS 2
static uintptr_t counters[NUM_COUNTERS];
static char* counter_names[NUM_COUNTERS];
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void setStats(int enable)
{
	int i = 0;
#define READ_CTR(name) do { \
	while (i >= NUM_COUNTERS) ; \
	uintptr_t csr = read_csr(name); \
	if (!enable) { csr -= counters[i]; counter_names[i] = #name; } \
	counters[i++] = csr; \
} while (0)

	READ_CTR(mcycle);
	READ_CTR(minstret);

#undef READ_CTR
	}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void __attribute__((noreturn)) tohost_exit(uintptr_t code)
{
	
	tohost = (code << 1) | 1;
	while (1);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
uintptr_t __attribute__((weak)) handle_trap(uintptr_t cause, uintptr_t epc, uintptr_t regs[32])
{
	tohost_exit(1337);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void exit(int code)
{
	tohost_exit(code);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void abort()
{
	exit(128 + SIGABRT);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void printstr(const char* s)
{
	syscall(SYS_write, 1, (uintptr_t)s, strlen(s));
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
int __attribute__((weak)) main(int argc, char** argv)
{
	// single-threaded programs override this function.
	printstr("Implement main(), foo!\n");
	return -1;
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void _init(int cid, int nc)
{
	//init_tls();
	//thread_entry(cid, nc);
	// only single-threaded programs should ever get here.
	int ret = main(0, 0);

	char buf[NUM_COUNTERS * 32] __attribute__((aligned(64)));
	char* pbuf = buf;
	for (int i = 0; i < NUM_COUNTERS; i++)
		if (counters[i])
			//      pbuf += sprintf(pbuf, "%s = %d\n", counter_names[i], counters[i]);
			printf("%s = %d\n",counter_names[i],counters[i]);
	//  if (pbuf != buf)
	//    printstr(buf);

	exit(ret);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void printhex(uint64_t x)
{
	char str[17];
	int i;
	for (i = 0; i < 16; i++)
	{
		str[15-i] = (x & 0xF) + ((x & 0xF) < 10 ? '0' : 'a'-10);
		x >>= 4;
	}
	str[16] = 0;

	printstr(str);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
static inline void printnum(void (*putch)(int, void**), void **putdat,
		unsigned long long num, unsigned base, int width, int padc)
{
	unsigned digs[sizeof(num)*CHAR_BIT];
	int pos = 0;

	while (1)
	{
		digs[pos++] = num % base;
		if (num < base)
			break;
		num /= base;
	}

	while (width-- > pos)
		putch(padc, putdat);

	while (pos-- > 0)
		putch(digs[pos] + (digs[pos] >= 10 ? 'a' - 10 : '0'), putdat);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
static unsigned long long getuint(va_list *ap, int lflag)
{
	if (lflag >= 2)
		return va_arg(*ap, unsigned long long);
	else if (lflag)
		return va_arg(*ap, unsigned long);
	else
		return va_arg(*ap, unsigned int);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
static long long getint(va_list *ap, int lflag)
{
	if (lflag >= 2)
		return va_arg(*ap, long long);
	else if (lflag)
		return va_arg(*ap, long);
	else
		return va_arg(*ap, int);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
float pow_10(unsigned int y)
{
	unsigned int x;
	float result;

	for (unsigned int i=0; i <y; i++)
	{
		x += 10;
	}

	return ((float) x);
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void reverse(char *str, int len) 
{ 
  int i=0, j=len-1, temp; 
  while (i<j) 
  { 
    temp = str[i]; 
    str[i] = str[j]; 
    str[j] = temp; 
    i++; j--; 
  } 
} 
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
int intToStr(int x, char str[], int d) 
{ 
  int i = 0; 


  while (x) 
  { 
    str[i++] = (x%10) + '0'; 
    x = x/10; 
  } 

  while (i < d) 
    str[i++] = '0'; 

  reverse(str, i); 
  str[i] = '\0'; 
  return i; 
} 
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
void ftoa(float n, char *res, int afterpoint) 
{ 
  int i=0;
  char temp[30]={'\0'};

  // Extract integer part 
  int ipart = (int)n; 

  // Extract floating part 
  float fpart = (float) (n - (float)ipart); 
  int j=0;

  if(n < (0.0f))
  {
    res[j]='-';
    j=1;

  }

  if (ipart == 0)
  {
    res[j]='0';
    j=j+1;
  }
  else{

    if (ipart < 0/1)
    {

      ipart = (-1)*ipart;

    }

    i = intToStr(ipart, temp, 0); 

    strcpy(res+j,temp);
  }

  i = i+j;


  if (afterpoint != 0) 
  { 
    res[i] = '.'; // add dot 

    if (fpart < 0/1)
    {

      fpart = (-1)*fpart;

    }
    else if (fpart == 0/1)
    {
      fpart = fpart;
    }

    fpart = fpart * pow_10(afterpoint); 

    intToStr((int)fpart, res + i + 1, afterpoint); 
  } 
} 
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
static void vprintfmt(void (*putch)(int, void**), void **putdat, const char *fmt, va_list ap)
{
	register const char* p;
	const char* last_fmt;
	register int ch, err;
	unsigned long long num;
	int base, lflag, width, precision, altflag, i;
	float float_num = 0;
	char float_arr[30] = {'\0'};
	char padc;

	while (1) {
		while ((ch = *(unsigned char *) fmt) != '%') {
			if (ch == '\0')
				return;
			fmt++;
			putch(ch, putdat);
		}
		fmt++;

		// Process a %-escape sequence
		last_fmt = fmt;
		padc = ' ';
		width = -1;
		precision = -1;
		lflag = 0;
		altflag = 0;
reswitch:
		switch (ch = *(unsigned char *) fmt++) {

			// flag to pad on the right
			case '-':
				padc = '-';
				goto reswitch;

				// flag to pad with 0's instead of spaces
			case '0':
				padc = '0';
				goto reswitch;

				// width field
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				for (precision = 0; ; ++fmt) {
					precision = precision * 10 + ch - '0';
					ch = *fmt;
					if (ch < '0' || ch > '9')
						break;
				}
				goto process_precision;

			case '*':
				precision = va_arg(ap, int);
				goto process_precision;

			case '.':
				if (width < 0)
					width = 0;
				goto reswitch;

			case '#':
				altflag = 1;
				goto reswitch;

process_precision:
				if (width < 0)
					width = precision, precision = -1;
				goto reswitch;

				// long flag (doubled for long long)
			case 'l':
				lflag++;
				goto reswitch;

				// character
			case 'c':
				putch(va_arg(ap, int), putdat);
				break;

				// string
			case 's':
				if ((p = va_arg(ap, char *)) == NULL)
					p = "(null)";
				if (width > 0 && padc != '-')
					for (width -= strnlen(p, precision); width > 0; width--)
						putch(padc, putdat);
				for (; (ch = *p) != '\0' && (precision < 0 || --precision >= 0); width--) {
					putch(ch, putdat);
					p++;
				}
				for (; width > 0; width--)
					putch(' ', putdat);
				break;

				// (signed) decimal
			case 'd':
				num = getint(&ap, lflag);
				if ((long long) num < 0) {
					putch('-', putdat);
					num = -(long long) num;
				}
				base = 10;
				goto signed_number;

			case 'f':

				float_num =  va_arg(ap, double);

				ftoa(float_num, float_arr, 6); 

				for( i = 0; float_arr[i] != '\0'; i++)
				{
					putch(float_arr[i], putdat);
					if(i > 29) break;
				}
				break;

				// unsigned decimal
			case 'u':
				base = 10;
				goto unsigned_number;

				// (unsigned) octal
			case 'o':
				// should do something with padding so it's always 3 octits
				base = 8;
				goto unsigned_number;

			case 'x':
				base = 16;
unsigned_number:
				num = getuint(&ap, lflag);
signed_number:
				printnum(putch, putdat, num, base, width, padc);
				break;

				// escaped '%' character
			case '%':
				putch(ch, putdat);
				break;

				// unrecognized escape sequence - just print it literally
			default:
				putch('%', putdat);
				fmt = last_fmt;
				break;
		}
	}
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
int printf(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	vprintfmt((void*)putchar, 0, fmt, ap);

	va_end(ap);
	return 0; // incorrect return value, but who cares, anyway?
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
int sprintf(char* str, const char* fmt, ...)
{
	va_list ap;
	char* str0 = str;
	va_start(ap, fmt);

	void sprintf_putch(int ch, void** data)
	{
		char** pstr = (char**)data;
		**pstr = ch;
		(*pstr)++;
	}

	vprintfmt(sprintf_putch, (void**)&str, fmt, ap);
	*str = 0;

	va_end(ap);
	return str - str0;
}

/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
size_t strlen(const char *s)
{
	const char *p = s;
	while (*p)
		p++;
	return p - s;
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
size_t strnlen(const char *s, size_t n)
{
	const char *p = s;
	while (n-- && *p)
		p++;
	return p - s;
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
int strcmp(const char* s1, const char* s2)
{
	unsigned char c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;
	} while (c1 != 0 && c1 == c2);

	return c1 - c2;
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
char* strcpy(char* dest, const char* src)
{
	char* d = dest;
	while ((*d++ = *src++))
		;
	return dest;
}
/** @fn 
 * @brief 
 * @details 
 * @warning 
 * @param[in] 
 * @param[Out] 
 */
long atol(const char* str)
{
	long res = 0;
	int sign = 0;

	while (*str == ' ')
		str++;

	if (*str == '-' || *str == '+') {
		sign = *str == '-';
		str++;
	}

	while (*str) {
		res *= 10;
		res += *str++ - '0';
	}

	return sign ? -res : res;
}
