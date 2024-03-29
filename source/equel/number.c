#
# include	"../fileio.h"
# include	"constants.h"
# include	"globals.h"

/*
**  NUMBER -- process a numeric token
**	Number gets a number as, either floating or integer,
**	with the QUEL format, from inside a quel statement,
**	and adds it to the symbol space.
**
**	Parameters:
**		chr -- the first character of the number
**
**	Returns:
**		The lexical code for the appropriate
**		type of number.
**
**	Side Effects:
**		Adds a token to the symbols space.
**		yylval is set to the node added.
**
**	Called By:
**		yylex()
**		operator()
**
**	Files:
**		globals.h
**		../fileio.h
**		constants.h
**
**	History:
**		4/20/78 -- (marc) plagiarized from rick berman's
**			quel scanner number routine, and modified to 
**			handle errors differently.
*/	


number(chr)
char	chr;
{
	extern char	Cmap [];
	extern int	yylval;
	double		ftemp;
	long		ltemp;
	int		itemp;
	char		buf [256];
	register char	*ptr;
	register int	ret_type;

	ptr = buf;
	if ((*ptr = chr) != '.')
	{
		do
		{
			/* get integer portion */
			if ((ptr - buf) >= 256)
			{
				/* buffer overflow 
				 * return integer 0,
				 * and signal error.
				 */

bufovflo :
				*ptr = '\0';
				yysemerr("numeric too long", buf);
				yylval = addsym("0");
				return (Tokens.sp_i2const);
			}

			*++ptr = getch();
		}  while (Cmap[*ptr] == NUMBR);
	}

	/* do rest of type determination */
	switch (*ptr)
	{
	  case '.':
		/* floating point */
		do
		{
			/* fill into ptr with up to next non-digit */
			if ((ptr - buf) >= 256)
				/* buf oflo */
				goto bufovflo;	
			*++ptr = getch();
		}  while (Cmap[*ptr] == NUMBR);
		if (*ptr != 'e' && *ptr != 'E')
		{
			backup(*ptr);
			*ptr = '\0';
			goto convr;
		}

	  case 'e':
	  case 'E':
		if ((ptr - buf) >= 256)
			/* buf oflo */
			goto bufovflo;
		*++ptr = getch();
		if (Cmap[*ptr] == NUMBR || *ptr == '-' || *ptr == '+')
		{
			do
			{
				/* get exponent */
				if ((ptr - buf) >= 256)
					/* buf oflo */
					goto bufovflo;
				*++ptr = getch();
			} while (Cmap[*ptr] == NUMBR);
		}
		backup(*ptr);
		*ptr = '\0';

convr:
		if (atof(buf, &ftemp))
		{
			/* floating conversion error */
			yysemerr("numeric ofverflow", buf);
			yylval = addsym("0");
			return (Tokens.sp_f8const);
		}
		yylval = addsym(salloc(buf));
		ret_type = Tokens.sp_f8const;
		break;

	  default:
		/* integer */
		backup(*ptr);
		*ptr = '\0';

		/* long conversion error */
		if (atol(buf, &ltemp) || ltemp > 32767 
		   || ltemp < -32768)
			goto convr;
		itemp = ltemp;
		yylval = addsym(salloc(buf));
		ret_type = Tokens.sp_i2const;
		break;
	}
	return (ret_type);
}
