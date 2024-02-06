tTf(m, n)
int m;
int n;
{
	extern char	tTany;
	extern int	tT[];

	if (n < 0)
		return (tT[m]);
	else
		return ((tT[m] >> n) & 01);
}
