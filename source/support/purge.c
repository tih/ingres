# include	"../ingres.h"
# include	"../aux.h"
# include	"../lock.h"
# include	"../fileio.h"

/*
**  PURGE DATABASE
**
**	This stand-alone routine cleans up a database.  This includes:
**
**	- Destroy temporary relations, i.e., relations with names
**		beginning with "_SYS".
**	- Destroy expired relations
**	- Clean out junk files, such as core, etc.
**	- As a suggested future expansion, reformat relations which
**		have many overflow pages, or heaps with lots of old
**		deleted tuples, etc.
**
**	It may be called by the ingres superuser or by the dba of
**	a database.  There are two modes.  The first is where databases
**	to be purged are explicitly named.  If none are named, then
**	all databases owned by the particular user (or all databases
**	if the INGRES superuser) are purged.
**
**	Flags:
**	-p	enable the purge feature, i.e., clean out expired
**		relations as well as temporary relations.
**	-s	attempt to run in superuser mode.  The user must be
**		login "ingres" for this to succeed.
**	-a	ask the user before each database.
**	-f	clean out rather than report junk files.
**
**	History:
*/


char		All;
char		Superuser;
char		Ask;
char		Purge;
char		Clean;
extern int	Wait_action;
extern int	Status;
extern char	*Usercode;
long		Today;

struct directory
{
	int	inumber;
	char	fname[14];
	char	null;
};

main(argc, argv)
int	argc;
char	*argv[];
{
	register char	*db;
	register int	i;
	char		fake[256];

#	ifdef xTTR1
	tTrace(&argc, argv, 'T');
#	endif

	/* set up arguments and operating modes */
	initialize(argc, argv);
	time(&Today);
#	ifdef	xTTR2
	tTfp(10, 2, "Usercode: %.2s\n", Usercode);
#	endif

	while (db = getnxtdb())
	{
		purgedb(db);
	}
	printf("\npurge completed\n");
}



rubproc()
{
	unldb();
	exit(-1);
}



/*
**  PURGE DATABASE
**
**	The database is purged of temporaries, expired relations, and
**	junk.
*/

extern struct descriptor	Reldes;


purgedb(db1)
char	*db1;
{
	register char		*db;
	struct relation		rel, key;
	struct tup_id		rtid, rlimtid;
	register int		i;
	register char		c;
	long			l;
	struct directory	direc;
	FILE			*fd;
	char			fdbuf[IOBUFSIZ];
	int			darg[3];
	char			*pv[2];
	char			pbuff[MAXNAME + 3];

	db = db1;
#	ifdef	xTTR2
	tTfp(11, 0, "entered purgedb(%s)\n", db);
#	endif
	printf("Database %s", db);
	if (!ask("? "))
		return;
	if (!Ask)
		printf(":\n");
	acc_init();

	/* set exclusive lock on data base */
#	ifdef	xTTR2
	tTfp(11, 1, "calling db_lock(%d)\n", M_EXCL);
#	endif
	db_lock(M_EXCL);

	/* open the relation relation for read-write */
	opencatalog("relation", 2);

	if (find(&Reldes, NOKEY, &rtid, &rlimtid))
	{
		printf("\tcannot find in %s\n", db);
		closecatalog(TRUE);	/* really close cache */
		unldb();		/* unlock the database */
		acc_close();
		return;
	}

	while (get(&Reldes, &rtid, &rlimtid, &rel, 1) == 0)
	{
		i = 0;

		/* check for temp rel */
		if (bequal(rel.relid, "_SYS", 4))
			i++;

		/* check for expired */
		if (rel.relsave < Today && rel.relsave != 0 && i == 0)
		{
			printf("\t%.14s: expired\n", rel.relid);
			if (Purge)
				if (ask("\t\tPURGE? "))
					i++;
		}

		/* if this relation should be purged -- call destroy */
		if (i != 0)
		{
			printf("\tpurging %.12s\n", rel.relid);

			/* set up parameter vector for destroy */
			bmove(rel.relid, pbuff, MAXNAME + 2);
			pbuff[MAXNAME + 2] = '\0';
			pv[0] = pbuff;
			pv[1] = -1;
			if (destroy(1, pv) != 0)
				syserr("cannot destroy %s\n", pv[0]);
			closecatalog(FALSE);	/* to flush */
		}
	}

	/* open the directory to check for extra files */
	if ((fd = fopen(".", "read", fdbuf)) == NULL)
	{
		printf("\tcannot open .\n");
		closecatalog(TRUE);		/* really */
		unldb();		/* unlock the database */
		acc_close();
		return;
	}
	direc.null = 0;
	l = 32;		/* fseek needs a long address */
	fseek(fd, l, 0);

	/* scan the directory */
	while (fread(fd, &direc, 16) == 16)
	{
		/* throw out null entries */
		if (direc.inumber == 0)
			continue;

		/* throw out legitimate files */
		if (sequal(direc.fname, "admin"))
			continue;

		/* always purge _SYS files */
		if (!bequal(direc.fname, "_SYS", 4))
		{
			if (direc.fname[13] != 0)
			{
				/* it might be a relation */
				clearkeys(&Reldes);
				setkey(&Reldes, &key, direc.fname, RELID);
				setkey(&Reldes, &key, &direc.fname[MAXNAME], RELOWNER);
				if (getequal(&Reldes, &key, &rel, &rtid) <= 0)
				{
					/* it is a relation (or should be saved) */
					continue;
				}
			}

			/* it is a funny file!!! */
			if (!Clean)
			{
				printf("\t%s: file\n", direc.fname);
				continue;
			}
		}

		/* purge the file */
		printf("\tunlinking %s\n", direc.fname);
		if (unlink(direc.fname))
			printf("\tcannot unlink\n");
	}
	closecatalog(TRUE);	/* close catalogs */
	unldb();		/* unlock the database */
	acc_close();
	fclose(fd);
}
