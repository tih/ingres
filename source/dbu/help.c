# include	"../ingres.h"
# include	"../aux.h"
# include	"../catalog.h"
# include	"../access.h"

/*
**  HELP - Provide Information to User
**
**	Arguments:
**		pv[i] - code
**			0 - print relation information
**			1 - print manual section
**			2 - print relation list
**			3 - print relation info for all accessible relations
**		pv[i+1] - name of entity for modes 0 or 1
**
**	Uses trace flags 10, 11, 12, and 13
*/

help(parmc, parmv)
int	parmc;
char	**parmv;
{
	struct descriptor		des;
	int				mode;
	register char			**pv;
	register int			ret;

	ret = 0;
	pv = parmv;
	getuser(-1);	/* init getuser for modes 1 & 2 */
	while (*pv != -1)
	{

		if (atoi(*pv++, &mode))
		{
		badmode:
			syserr("HELP: mode %s", *--pv);
		}

#		ifdef xZTR1
		if (tTf(10, -1) || tTf(0, 10))
		{
			printf("help %d", mode);
			if (mode != 2)
				printf(" %s", *pv);
			putchar('\n');
		}
#		endif
		switch (mode)
		{

		  case 0:	/* help relation */
			if (!openr(&des, -1, *pv))
			{
				rel_fmt(&des);
				*pv = 0;
			}
			pv++;
			break;

		  case 1:	/* help manual section */
			if (man(*pv))
				*pv = 0;
			pv++;
			break;

		  case 2:
		  case 3:
			relpr(mode);
			break;

		  default:
			goto badmode;
		}
	}
	getuser(0);	/* close getuser in case mode 1 or 2 */

	/* now rescan for error messages */
	pv = parmv;
	while (*pv != -1)
	{
		atoi(*pv++, &mode);

		switch (mode)
		{

		  case 0:
		  case 1:
			if (*pv)
				ret = error(5401 + mode, *pv, 0);
			break;

		  case 2:
		  case 3:
			continue;
		}
		pv++;
	}
	return (ret);
}


/*
**  Nroff Manual Section
**
**	The manual section given by 'name' is nroff'ed.  Returns one
**	on success, zero if the manual section is not found.
**
**	Uses trace flag 11
*/

man(name)
char	*name;
{
	char		manual[100];
	register int	i;
	int		stat;
	char		name_nr[18];
	register char	*naa;

	if (length(name) > 14)
		return (0);

	/* a null manual name gives table of contents */
	if (name[0] == 0)
		smove("../toc.nr", name_nr);
	else
		concat(name, ".nr", name_nr);

	concat(ztack(Pathname, "/doc/quel/"), name_nr, manual);
	if ((i = open(manual, 0)) < 0)
	{
		/* try a unix command instead */
		concat(ztack(Pathname, "/doc/unix/"), name_nr, manual);
		if ((i = open(manual, 0)) < 0)
			return (0);
	}
	if (close(i))
		syserr("cannot close %s", manual);
	ruboff(0);	/* wait for child's death if rubout occures */
	i = fork();
	if (i == 0)
	{
		signal(2, 0);	/* die on rubout */
		setuid(getuid());
#		ifndef xB_UNIX
		setgid(getgid());
#		endif
		naa = ztack(Pathname, "/doc/iaa");
		execl("/bin/nroff", "nroff", naa, manual, 0);
		execl("/usr/bin/nroff", "nroff", naa, manual, 0);
		syserr("help: exec: nroff");
	}
	/* wait for nroff if fork succeeded */
	if (i > 0)
		fullwait(i, "help: nroff");
	rubon();
	return (1);
}


/*
**  PRINT DATABASE INFORMATION
**
**	Prints a list of all the relations in the database, together
**	with their owner.
**
**	Uses trace flag 12
*/

relpr(mode)
int	mode;
{
	extern struct descriptor	Reldes;
	register struct descriptor	*d;
	register int			i;
	register char			*cp;
	struct tup_id			limtid, tid;
	char				buf[MAXLINE + 1];
	char				lastuser[2];
	struct relation			rel;

	opencatalog("relation", 0);
	d = &Reldes;
	if (i = find(d, NOKEY, &tid, &limtid))
		syserr("help: relpr: find %d", i);

	lastuser[0] = '\0';

	if (mode == 2)
		printf("\n relation name     relation owner\n\n");

	while ((i = get(d, &tid, &limtid, &rel, 1)) == 0)
	{
		if (mode == 2)
		{
			if (!bequal(lastuser, rel.relowner, 2))
			{
				if (getuser(rel.relowner, buf))
				{
					/* cant find user code */
					cp = bmove("  ", buf, 2);
					cp = bmove(rel.relowner, cp, 2);
					*cp = '\0';
				}
				else
				{
					for (cp = buf; *cp != ':'; cp++)
						;
					*cp = '\0';
				}
				bmove(rel.relowner, lastuser, 2);
			}
			printf(" %.12s      %s\n", rel.relid, buf);
		}
		else
		{
			if ((rel.relstat & S_CATALOG) || bequal("_SYS", rel.relid, 4))
				continue;
			if (bequal(Usercode, rel.relowner, 2) || bequal(Admin.adhdr.adowner, rel.relowner, 2))
				rel_fmt(&rel);
		}
	}

	if (i < 0)
		syserr("help: relpr: get %d", i);
	if (mode == 2)
		printf("\n");
	return (0);
}



/*
**  Print Relation Information
**
**	Prints detailed information regarding the relation.
**
**	Uses trace flag 13
*/

rel_fmt(rel)
struct relation	*rel;
{
	struct tup_id			limtid, tid;
	char				buf[MAXLINE + 1];
	struct attribute		att;
	struct index			indkey, ind;
	register int			i;
	int				j;
	extern struct descriptor	Attdes, Inddes;
	register struct relation	*r;
	char				*trim_relname();

	r = rel;
	printf("\nRelation:\t\t%s\n", trim_relname(r->relid));
	i = getuser(r->relowner, buf);
	if (i)
	{
		smove("(xx)", buf);
		bmove(r->relowner, &buf[1], 2);
	}
	else
	{
		for (i = 0; buf[i] != ':'; i++)
			continue;
		buf[i] = 0;
	}
	printf("Owner:\t\t\t%s\n", buf);
	printf("Tuple width:\t\t%d\n", r->relwid);
	if (r->relsave != 0)
	{
		printf("Saved until:\t\t%s", ctime(&r->relsave));
	}
	if ((r->relstat & S_VIEW) == 0)
	{
		printf("Number of tuples:\t%s\n", locv(r->reltups));
		printf("Storage structure:\t");
		i = r->relspec;
		if (i < 0)
		{
			printf("compressed ");
			i = -i;
		}
		switch (i)
		{
	
		  case M_HEAP:
			printf("paged heap\n");
			break;
	
		  case M_ISAM:
			printf("ISAM file\n");
			break;
	
		  case M_HASH:
			printf("random hash\n");
			break;
	
		  default:
			printf("unknown structure %d\n", i);
			break;
	
		}
	}

	printf("Relation type:\t\t");
	if (r->relstat & S_CATALOG)
		printf("system catalog\n");
	else if (r->relstat & S_VIEW)
		printf("view\n");
	else
		if (r->relindxd < 0)
		{
			printf("secondary index on ");
			opencatalog("indexes", 0);
			setkey(&Inddes, &indkey, r->relowner, IOWNERP);
			setkey(&Inddes, &indkey, r->relid, IRELIDI);
			if (!getequal(&Inddes, &indkey, &ind, &tid))
				printf("%s\n", trim_relname(ind.irelidp));
			else
				printf("unknown relation\n");
		}
		else
		{
			if (r->relstat & S_DISTRIBUTED)
				printf("distributed ");
			printf("user relation\n");
		}
	if (r->relindxd > 0)
	{
		printf("Secondary Indices:\t");
		opencatalog("indexes", 0);
		setkey(&Inddes, &indkey, r->relid, IRELIDP);
		setkey(&Inddes, &indkey, r->relowner, IOWNERP);
		if (i = find(&Inddes, EXACTKEY, &tid, &limtid, &indkey))
			syserr("help: find %d indexes", i);
		j = FALSE;
		while ((i = get(&Inddes, &tid, &limtid, &ind, 1)) == 0)
		{
			if (!bequal(&indkey, &ind, MAXNAME + 2))
				continue;
			if (j)
				printf(", ");
			j =TRUE;
			printf("%s", trim_relname(ind.irelidi));
		}
		if (i < 0)
			syserr("help:get indexes %d", i);
		if (!j)
			printf("unknown");
	}
	printf("\n");

	opencatalog("attribute", 0);
	printf("\n attribute name    type  length  keyno.\n\n");
	seq_init(&Attdes, r);
	while (seq_attribute(&Attdes, r, &att))
	{
		printf(" %.12s	    %c%8d",
			att.attname, typeunconv(att.attfrmt), att.attfrml & 0377);
		if (att.attxtra)
			printf("%7d", att.attxtra);
		printf("\n");
	}

	printf("\n");
	return (0);
}
