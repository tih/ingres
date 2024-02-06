/*
**  SET FLAG
**
**	This routine sets flags from argv.  You give arguments
**	of the argv, the flag to be detected, and the default
**	for that flag (if it is not supplied).  The return value
**	is the flag value.   For example:
**		setflag(argv, 'x', 2);
**	returns zero if the "-x" flag is stated, and one if the
**	"+x" flag is stated.  It returns 2 if the flag is not
**	stated at all.
**
**	History:
**		8/15/79 (eric) (6.2/7) -- changed to take -1
**			or 0 as argv[argc].
*/

setflag(argv, flagch, def)
char	**argv;
char	flagch;
int	def;
{
	register char	**p;
	register char	*q;
	register int	rtval;

	rtval = -1;
	for (p = &argv[1]; *p != (char *) 0 && *p != (char *) -1; p++)
	{
		q = *p;
		if (q[1] != flagch)
			continue;
		if (*q != '-' && *q != '+')
			continue;
		rtval = (q[0] == '+');
	}
	if (rtval < 0)
		rtval = def;
	return (rtval);
}
