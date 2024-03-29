# include	"../ingres.h"
# include	"../aux.h"
# include	"../fileio.h"
# include	"monitor.h"

/*
**  DO MACRO EVALUATION OF QUERY BUFFER
**
**	The logical query buffer is read and passed through the macro
**	processor.  The main purpose of this is to evaluate {define}'s.
**	If the 'pr' flag is set, the result is printed on the terminal,
**	and so becomes a post-evaluation version of print.
**
**	Uses trace flag 12
*/

eval(pr)
int	pr;
{
	register FILE	*tfile;
	char		tbuf[IOBUFSIZ];
	register char	c;
	extern int	getc();
	char		tfilename[40];

	Autoclear = 0;
	clrline(1);

	/* open temp file and reopen query buffer for reading */
	if (!pr)
	{
		concat("/tmp/INGTQ", Fileset, tfilename);
		if ((tfile = fopen(tfilename, "w", tbuf)) == NULL)
			syserr("eval: open(%s)", tfilename);
	}
	if (freopen(Qbname, "r", Qryiop) == NULL)
		syserr("eval: freopen 1");

	/* COPY FILE */
	macinit(&getc, Qryiop, 1);
	while ((c = macgetch()) > 0)
	{
		if (pr)
			putchar(c);
		else
			if (putc(c, tfile) == EOF)
				syserr("eval: putc");
	}

	if (!pr)
	{
		/* link temp file back to query buffer */
		fclose(tfile);
		unlink(Qbname);
		if (link(tfilename, Qbname))
			syserr("eval: link");
		unlink(tfilename);
	}

	/* reopen query buffer (now evaluated) */
	if (freopen(Qbname, "a", Qryiop) == NULL)
		syserr("eval: freopen 2");

	cgprompt();
}
