/*
**	IIbmove -- run time byte mover
**
**		IIbmove moves "l" bytes pointed to by "s",
**		to the area pointed to by "d".
**
**		Returns a pointer to the 
**		byte after the area filled.
**
**		Doesn't append a null byte.
**
*/

IIbmove(s, d, l)
char	*s, *d;
int	l;
{
	register char	*ss, *dd;
	register int	len;

	ss = s;
	dd = d;
	len = l + 1;

	while (--len)
		*dd++ = *ss++;

	return (dd);
}
