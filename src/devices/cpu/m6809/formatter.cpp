#include "emu.h"
#include "debugger.h"
#include "m6809.h"

unsigned char m6809_base_device::read_mem8(unsigned short a)
{
	return(read_memory(a));
}

unsigned short m6809_base_device::read_mem16(unsigned short a)
{
	unsigned short	d = read_memory(a);
	d <<= 8;
	d |= read_memory(a + 1);
	return(d);
}

unsigned int m6809_base_device::read_mem32(unsigned short a)
{
	unsigned int	d = read_memory(a);
	d <<= 8;
	d |= read_memory(a + 1);
	d <<= 8;
	d |= read_memory(a + 2);
	d <<= 8;
	d |= read_memory(a + 3);
	return(d);
}


#define TOUCH(x)

/*****************************************************************************/


#define FLG_SHORT     (1 << 0)
#define FLG_LONG      (1 << 1)
#define FLG_DLONG     (1 << 2)
#define FLG_ZERO      (1 << 3)
#define FLG_PLUS      (1 << 4)
#define FLG_MINUS     (1 << 5)
#define FLG_SPACE     (1 << 6)
#define FLG_HASH      (1 << 7)
#define FLG_BYTE      (1 << 8)


/*****************************************************************************/

#define Str_ConvertASCIIChar(x) ( (char) (x) )



/*****************************************************************************/





/*****************************************************************************/


#define GetCh()    (*fmt++)
#define PutCh(ch)  {error = vsprintfputc(Str_ConvertASCIIChar(ch), (SprintfData *) userData); if (error < 0) goto exit; else numChars++;}
#define PutWCh(ch) {error = vsprintfputc(ch,(SprintfData *) userData); if (error < 0) goto exit; else numChars++;}


int m6809_base_device::DoFormat(void *userData, unsigned short argptrpointer, bool wides)
{
	char			ch;
	int				error;
	int				numChars;
	int				flags;
	int				width;
	int				precision;
	bool            done;
	const char     *prefix;
	const char     *value;
	char            postfix[8];
	unsigned short	fmtbase;
	char            buf[512];
	int				len;
	int				leading_zeroes;
	int				i;
	char			fmtbuff[512];
	char		*fmt;
	unsigned short	argptr = argptrpointer;

	// Get fmt ptr
	fmtbase = read_mem16(argptr);
	((SprintfData *) userData)->sd_ArgumentsParsed++;
	argptr += sizeof(fmtbase);

	int fmtlen = 0;
	while (fmtlen < sizeof(fmtbuff))
	{
		fmtbuff[fmtlen] = read_mem8(fmtbase++);
		if (!fmtbuff[fmtlen])
			break;
		fmtlen++;
	}
	fmt = (char *) fmtbuff;

	i = 0;

	error = 0;
	numChars = 0;
	while (TRUE)
	{
		ch = GetCh();
		if (ch == 0)
			break;

		if (ch != '%')
		{
			PutWCh(ch);
			continue;
		}

		/* %[flags][width][.precision][size]type */

		/* isolate the flags */
		flags = 0;
		done = FALSE;
		do
		{
			ch = GetCh();
			switch (ch)
			{
				case '-':	flags |= FLG_MINUS; break;
				case '+':	flags |= FLG_PLUS;  break;
				case ' ':	flags |= FLG_SPACE; break;
				case '#':	flags |= FLG_HASH;  break;
				case '0':	flags |= FLG_ZERO;  break;
				default:	done = TRUE;        break;
			}
		} while (!done);

		/* get the field width */
		if (ch == '*')
		{
			unsigned short 	widthptr;
			widthptr = read_mem16(argptr);
			width = read_mem32(widthptr);
			((SprintfData *) userData)->sd_ArgumentsParsed++;
			argptr += sizeof(widthptr);
			//width = va_arg(va, int);
			if (width < 0)
			{
				flags |= FLG_MINUS;
				width = -width;
			}

			ch = GetCh();
		}
		else if ((ch >= '0') && (ch <= '9'))
		{
			width = 0;
			do
			{
				width = width * 10 + (ch - '0');
				ch = GetCh();
			} while ((ch >= '0') && (ch <= '9'));
		}
		else
		{
			/* unlimited */
			width = -1;
		}

		/* get the field precision */
		if (ch == '.')
		{
			ch = GetCh();
			if (ch == '*')
			{
				unsigned short precptr;
				precptr = read_mem16(argptr);
				precision = read_mem32(precptr);
				argptr += sizeof(precptr);
				((SprintfData *) userData)->sd_ArgumentsParsed++;
				//precision = va_arg(va, int);
				if (precision < 0)
					precision = 0;

				ch = GetCh();
			}
			else
			{
				precision = 0;
				while ((ch >= '0') && (ch <= '9'))
				{
					precision = precision * 10 + (ch - '0');
					ch = GetCh();
				}
			}
		}
		else
		{
			/* unlimited */
			precision = -1;
		}

		/* isolate the size */
		while (TRUE)
		{
			if (ch == 'h')
			{
				ch = GetCh();
				if (ch == 'h')
				{
					ch = GetCh();
					flags |= FLG_BYTE;
				}
				else
				{
					flags |= FLG_SHORT;
				}
			}
			else if (ch == 'l')
			{
				ch = GetCh();
				if (ch == 'l')
				{
					flags |= FLG_DLONG;
					ch = GetCh();
				}
				else
				{
					flags |= FLG_LONG;
				}
			}
			else if (ch == 'L')
			{
				flags |= FLG_DLONG;
				ch = GetCh();
			}
			else
			{
				break;
			}
		}

		value = "";
		len = 0;
		prefix = "";
		postfix[0] = 0;
		leading_zeroes = 0;

		switch (ch)
		{
			case'i':
			case'd':
			{
				short num;

				if (flags & FLG_BYTE)
				{
					unsigned short numptr;
					numptr = read_mem16(argptr);
					num = (short) read_mem8(numptr);
					if (num & 0x80)
						num |= 0xFF00;
					argptr += sizeof(numptr);
					((SprintfData *) userData)->sd_ArgumentsParsed++;
				}
				else
				{
					unsigned short numptr;
					numptr = read_mem16(argptr);
					num = (short) read_mem16(numptr);
					((SprintfData *) userData)->sd_ArgumentsParsed++;
					argptr += sizeof(numptr);
					//num = va_arg(va, int);
				}

				if (num < 0)
				{
					num = -num;
					prefix = "-";
				}

				if (precision < 0)
					precision = 1;

				len = 0;
				do
				{
					len++;
					buf[sizeof(buf) - len] = num % 10 + '0';
					num /= 10;
				} while (num);
				value = &buf[sizeof(buf) - len];

				if (precision > len)
				{
					leading_zeroes = precision - len;
					len = precision;
				}
			}
			break;

			case'u':
			case'o':
			{
				unsigned int num;
				unsigned int base;

				if (ch == 'o')
					base = 8;
				else
					base = 10;


				if (flags & FLG_BYTE)
				{
					unsigned short numptr;
					numptr = read_mem16(argptr);
					num = (unsigned int) read_mem8(numptr);
					argptr += sizeof(numptr);
					((SprintfData *) userData)->sd_ArgumentsParsed++;
				}
				else
				{
					unsigned short numptr;
					numptr = read_mem16(argptr);
					argptr += sizeof(numptr);
					num = (unsigned int) read_mem16(numptr);
					((SprintfData *) userData)->sd_ArgumentsParsed++;
					//num = va_arg(va, unsigned int);
				}

				if (precision < 0)
					precision = 1;

				if ((ch == 'o') && num)
					prefix = "0";

				len = 0;
				do
				{
					len++;
					buf[sizeof(buf) - len] = (char) (num % base + '0') & 0xFF;
					num /= base;
				} while (num);
				value = &buf[sizeof(buf) - len];

				if (precision > len)
				{
					leading_zeroes = precision - len;
					len = precision;
				}
			}
			break;

			case'p':
			case'P':
			case'x':
			case'X':
			{
				unsigned int     num;
				const char *map;

				if (flags & FLG_BYTE)
				{
					unsigned short numptr;
					numptr = read_mem16(argptr);
					num = (unsigned int) read_mem8(numptr);
					argptr += sizeof(numptr);
					((SprintfData *) userData)->sd_ArgumentsParsed++;
				}
				else
				{
					unsigned short numptr;
					numptr = read_mem16(argptr);
					num = (unsigned int) read_mem16(numptr);
					argptr += sizeof(numptr);
					((SprintfData *) userData)->sd_ArgumentsParsed++;
					//num = va_arg(va, unsigned int);
				}

				if ((ch == 'X') || (ch == 'P'))
					map = "0123456789ABCDEF";
				else
					map = "0123456789abcdef";

				if (flags & FLG_HASH)
				{
					if (ch == 'X')
						prefix = "0X";
					else if (ch == 'x')
						prefix = "0x";
					else
						prefix = "@";
				}

				if (precision < 0)
				{
					if ((ch == 'p') || (ch == 'P'))
						precision = 8;
					else
						precision = 1;
				}

				len = 0;
				do
				{
					len++;
					buf[sizeof(buf) - len] = map[num & 15];
					num /= 16;
				} while (num);
				value = &buf[sizeof(buf) - len];

				if (precision > len)
				{
					leading_zeroes = precision - len;
					len = precision;
				}
			}
			break;

			case 'n':
			{
				//if (flags & FLG_DLONG)
				//	*((uint64 *)va_arg(va, uint64 *)) = numChars;
				//else if (flags & FLG_SHORT)
				//	*((unsigned short *)va_arg(va, unsigned short *)) = numChars;
				//else
				//	*((unsigned int *)va_arg(va, unsigned int *)) = numChars;
				break;
			}

			case 's':
			{
				unsigned short base;
				base = read_mem16(argptr);
				argptr += sizeof(base);
				((SprintfData *) userData)->sd_ArgumentsParsed++;
				//				value = va_arg(va, char *);
				if (base == 0)
				{
					value = "<NULL>";
					len = 6;
				}
				else
				{
					while ((precision < 0 || len < precision)) //&& (read_mem8( (base + len) ) ) )
					{
						buf[len] = read_mem8(base + len);
						if (!buf[len])
							break;
						len++;
					}
					value = buf;
				}

				break;
			}

			case 0: PutCh('%');
				goto exit;

			case 'c':
			{
				//buf[0] = va_arg(va, int);

				value = buf;
				len = 1;
				break;
			}

			default:
			{
				/* Handle unknown formatting commands
				* by just printing the command character using the
				* specified width and precision. This is how %% is
				* handled.
				*/
				buf[0] = ch;
				value = buf;
				len = 1;
				break;
			}
		}

		if (prefix[0] == 0)
		{
			if (flags & FLG_PLUS)
				prefix = "+";
			else if (flags & FLG_SPACE)
				prefix = " ";
		}
		else if (flags & FLG_ZERO)
		{
			/* prefix goes before zero padding, but after space padding */

			i = 0;
			while (prefix[i])
				PutCh(prefix[i++]);

			width -= i;
		}

		if ((width > 0) && ((flags & FLG_MINUS) == 0))
		{
			int  j;
			char fillChar;

			j = width - strlen(prefix) - len - strlen(postfix);
			fillChar = (flags & FLG_ZERO) ? '0' : ' ';

			if (j > 0)
			{
				width -= j;
				while (j-- > 0)
					PutCh(fillChar);
			}
		}

		if ((flags & FLG_ZERO) == 0)
		{
			i = 0;
			while (prefix[i])
				PutCh(prefix[i++]);

			width -= i;
		}

		len -= leading_zeroes;
		while (leading_zeroes--)
			PutCh('0');

		i = 0;
		while (i < len)
			PutCh(value[i++]);

		width -= len;

		i = 0;
		while (postfix[i])
			PutCh(postfix[i++]);

		width -= i;

		if (flags & FLG_MINUS)
		{
			/* handle padding for left-justified field */

			if (width > 0)
			{
				while (width-- > 0)
					PutCh(' ');
			}
		}
	}

exit:

	if (error < 0)
		return error;

	return numChars;
}


/*****************************************************************************/

int m6809_base_device::Str_FormatASCII(void *userData, unsigned short ap)
{
	return DoFormat(userData, ap, FALSE);
}




/*****************************************************************************/



int m6809_base_device::vsprintfputc(char c, SprintfData *sd)
{
	if (sd->sd_LengthUsed < sd->sd_MaxLength)
	{
		*sd->sd_Dest++ = (char) c;
		sd->sd_LengthUsed++;
	}
	return c;
}


/*****************************************************************************/


// Returns the # of bytes to bump the PC due to arguments being read.
unsigned short m6809_base_device::gvsprintf(char *buff, int maxlen, unsigned short ap)
{
	SprintfData sd;
	int         result;

	// One char for the nul
	maxlen--;

	sd.sd_Dest = buff;
	sd.sd_MaxLength = maxlen;
	sd.sd_LengthUsed = 0;
	sd.sd_ArgumentsParsed = 0;

	Str_FormatASCII(&sd, ap);
	*sd.sd_Dest = 0;


	result = sd.sd_ArgumentsParsed * sizeof(unsigned short);
	return result;
}



unsigned short m6809_base_device::gsprintf(char *buf, int maxlen, unsigned short argptr)
{
	unsigned short result;

	buf[0] = 0;
	result = gvsprintf(buf, maxlen, argptr);

	return(result);
}

