/***************************************************************************
 * Project           			:  shakti devt board
 * Name of the file	     		:  printf.c
 * Brief Description of file    :  Print based command and control by uart
 * Name of Author    	        :  Sathya Narayanan N & Balaji venkat
 * Email ID                     :  sathya281@gmail.com

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
#include <stdarg.h>
#include "utils.h"
#include "uart.h"

/** @fn size_t strnlen
 * @brief
 * @details
 * @warning
 * @param[in] const char
 * @param[Out] unsigned integer
 */
size_t strnlen(const char *s, size_t n)
{
	const char *p = s;

	while (n-- && *p)
		p++;

	return (p - s);
}

/** @fn  itoa
 * @brief
 * @details
 * @warning
 * @param[in] unsigned long long int, unsigned 
 * @param[Out] No output parameter
 */
static inline void itoa (unsigned long long int number, unsigned base)
{
	unsigned int size = 0;
	int i = 0;
	unsigned int intermediate = 0;
	unsigned int digits[sizeof(number)*8];

	while (1) {

		digits[i] = number % base;

		if (number < base)
			break;

		number /= base;
		i++;
	}
	i++;
	while (i-- > 0)
	{
		if (digits[i] >= 10)
			intermediate = 'a' - 10;
		else
			intermediate = '0';

		putchar(digits[i] + intermediate);
	}
}

/** @fn _printf_
 * @brief
 * @details
 * @warning
 * @param[in] const char
 * @param[Out] No output parameter
 */
void _printf_(const char *fmt, va_list ap)
{
	register const char* p;
	const char* last_fmt;
	register int ch;
	unsigned long long num;
	int base, lflag, i;
	float float_num = 0;
	char float_arr[30] = {'\0'};
	int backtothebeginning;

	for (;;) {
		for (;(ch = *(unsigned char *) fmt) != '%'; fmt++) {
			if (ch == '\0')
				return;
			putchar(ch);
		}
		fmt++;

		// Process a %-escape sequence
		last_fmt = fmt;
		lflag = 0; 

		backtothebeginning = 0;
		for (;;) {

			switch (ch = *(unsigned char *) fmt++) {

				// long flag (doubled for long long)
				case 'l':
					lflag++;
					backtothebeginning = 1;
					break;

					// character
				case 'c':
					putchar(va_arg(ap, int));
					break;

					// string
				case 's':
					if ((p = va_arg(ap, char *)) == NULL)
						p = "(null)";
					for (; (ch = *p) != '\0' ;) {
						putchar(ch);
						p++;
					}
					break;

					// (signed) decimal
				case 'd':
					base = 10;

					if (lflag >= 2)
						num = va_arg(ap, long long);
					else if (lflag ==1)
						num = va_arg(ap, long);
					else
						num = va_arg(ap, int);

					if ((long long) num < 0) {
						putchar('-');
						num = -(long long) num;
					}

					itoa( num, base);

					break;
 #if defined(ARTIX7_35T) || defined(AARDONYX) 
				case 'f':
					float_num =  va_arg(ap, double);

					ftoa(float_num, float_arr, 6);

					for( i = 0; float_arr[i] != '\0'; i++)
					{
						putchar(float_arr[i]);
						if(i > 29) break;
					}
					break;
#endif

					// unsigned decimal
				case 'u':
					base = 10;

					if (lflag >= 2)
						num = va_arg(ap, unsigned long long);
					else if (lflag)
						num = va_arg(ap, unsigned long);
					else
						num = va_arg(ap, unsigned int);

					itoa( num, base);

					break;

					// (unsigned) octal
				case 'o':
					// should do something with padding so it's always 3 octits
					base = 8;

					if (lflag >= 2)
						num = va_arg(ap, unsigned long long);
					else if (lflag)
						num = va_arg(ap, unsigned long);
					else
						num = va_arg(ap, unsigned int);

					itoa( num, base);

					break;

				case 'x':
					base = 16;

					if (lflag >= 2)
						num = va_arg(ap, unsigned long long);
					else if (lflag)
						num = va_arg(ap, unsigned long);
					else
						num = va_arg(ap, unsigned int);

					itoa( num, base);

					break;

					// escaped '%' character
				case '%':
					putchar(ch);
					break;

					// unrecognized escape sequence - just print it literally
				default:
					putchar('%');
					fmt = last_fmt;
					break;
			}

			if (backtothebeginning)
			{
				backtothebeginning = 0;
				continue;
			}
			else
				break;
		}
	}
}

/** @fn printf
 * @brief
 * @details
 * @warning
 * @param[in] const char*
 * @param[Out] int
 */
int printf(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	_printf_(fmt, ap);

	va_end(ap);
	return 0; // incorrect return value, but who cares, anyway?
}

