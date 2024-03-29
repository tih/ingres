# include	"../../ingres.h"
# include	"../../symbol.h"
# include	"../../pipes.h"
# include	"IIglobals.h"

/*
**	IIcvar -- write C variable values to parser
**
**
**		IIcvar is used to write the contents
**		of a C-variable to the quel parser.
**
**		Floats are converted to doubles first.
**
*/

IIcvar(obj, type, len)
char	*obj;
int	type;
int	len;
{
	register int	length;
	register char	*addr;
	int		t;
	double		d;
	struct
	{
		float	flt;
	};

	t = type;	/* copy type of symbol */
	length = len;	/* and its length */
	addr = obj;	/* and a pointer to it */

	switch (t)
	{

	  case opFLOAT:
		/* convert from f4 to f8 */
		d = addr->flt;
		addr = &d;
		length = sizeof d;
		t = opDOUBLE;
		break;

	  case opSTRING:
		length = IIlength(addr) + 1;	/* length includes null byte at end */

	  case opINT:
	  case opLONG:
	  case opDOUBLE:
		break;

	  default:
		IIsyserr("IIcvar:bad type %d", t);
	}


#	ifdef xETR1
	if (IIdebug)
		printf("IIcvar:type %d, length %d\n", t, length);
#	endif

	if (IIwrpipe(P_NORM, &IIoutpipe, IIw_down, &t, 1) != 1)
		IIsyserr("IIcvar:can't write to parser(1) %d", IIw_down);

	if (IIwrpipe(P_NORM, &IIoutpipe, IIw_down, addr, length) != length)
		IIsyserr("IIcvar:can't write to parser(2) %d", IIw_down);
}
