# include	"../ingres.h"
# include	"../aux.h"
# include	"../symbol.h"
# include	"../tree.h"
# include	"../pipes.h"
# include	"qrymod.h"

/*
**  UTIL.C -- qrymod support routines
**
**	This module contains a set of support routines for use
**	by the query modification routines.
**
**	Defined Constants:
**		none
**
**	Defines:
**		trimqlend() -- trim QLEND node off of qualification.
**		appqual() -- append qualification to tree.
**		ferror() -- issue fatal error
**		lsetbit() -- set bit in large set.
**		mergevar() -- merge variables to link terms
**		makezero() -- make a zero node
**
**	Required By:
**		almost all
**
**	Trace Flags:
**		00 -> 19
**
**	History:
**		2/14/79 -- version 6.2/0 release.
*/
/*
**  TRIMQLEND -- trim QLEND node off of qualification
**
**	The QLEND node, and possible the AND node preceeding it,
**	are trimmed off.  The result of this routine should be
**	a very ordinary tree like you might see in some textbook.
**
**	A fast not on the algorithm: the pointer 't' points to the
**	current node (the one which we are checking for a QLEND).
**	's' points to 't's parent, and 'r' points to 's's parent;
**	'r' is NULL at the top of the tree.
**
**	This routine works correctly on trees with no QLEND in
**	the first place, returning the original tree.
**
**	If there is a QLEND, it must be on the far right branch
**	of the tree, that is, the tree must be INGRES-canonical.
**
**	Parameters:
**		qual -- the qualification to be trimmed.
**
**	Returns:
**		A pointer to the new qualification.
**		NULL if the qualification was null once the
**			QLEND is stripped.
**
**	Side Effects:
**		The tree pointed to by 'qual' may be modified.
**
**	Called By:
**		protect()
**		appqual()
**		maybe others.
**
**	Trace Flags:
**		none
**
**	Syserrs:
**		none
*/

QTREE *
trimqlend(qual)
QTREE	*qual;
{
	register QTREE	*t;
	register QTREE	*s;
	register QTREE	*r;

	t = qual;

	/* check for the simple null qualification case */
	if (t == NULL || t->sym.type == QLEND)
		return (NULL);
	
	/* scan tree for QLEND node */
	for (r = NULL, s = t; (t = t->right) != NULL; r = s, s = t)
	{
		if (t->sym.type == QLEND)
		{
			/* trim of QLEND and AND node */
			if (r == NULL)
			{
				/* only one AND -- return its operand */
				return (s->left);
			}

			r->right = s->left;
			break;
		}
	}

	/* return tree with final AND node and QLEND node pruned */
	return (qual);
}
/*
**  APPQUAL -- append qualification to tree
**
**	The qualification is conjoined to the qualificaion of the
**	tree which is passed.
**
**	Parameters:
**		qual -- a pointer to the qualification to be appended.
**		root -- a pointer to the tree to be appended to.
**
**	Returns:
**		none
**
**	Side Effects:
**		Both 'qual' ad 'root' may be modified.  Note that
**			'qual' is linked into 'root', and must be
**			retained.
**
**	Requires:
**		tree() -- to create a new AND node.
**
**	Called By:
**		view
**		protect
**		integrity
**
**	Trace Flags:
**		13
*/

appqual(qual, root)
QTREE	*qual;
QTREE	*root;
{
	register QTREE	*p;
	register QTREE	*r;

	r = root;
#	ifdef xQTR3
	if (r == NULL)
		syserr("appqual: NULL root");
#	endif

	/*
	**  Find node before QLEND node
	**	p points the node we are examining, r points to
	**	it's parent.
	*/

	while ((p = r->right) != NULL && p->sym.type != QLEND)
	{
#		ifdef xQTR3
		if (p->sym.type != AND)
			syserr("appqual: node %d", p->sym.type);
#		endif
		r = p;
	}
	
	/* link in qualification */
	r->right = qual;
}
/*
**  FERROR -- issue fatal error message and abort query
**
**	This call is almost exactly like 'error' (which is called),
**	but never returns: the return is done by 'reset'.  Also, the
**	R_up pipe is flushed.
**
**	Parameters:
**		errno -- the error number.
**		qmode -- the query mode to pass as $0, -1 if none.
**		vn -- the varno of the relation name to pass as
**			$1, -1 if none.
**		p1 to p5 -- the parameters $2 through $6
**
**	Returns:
**		non-local (via reset())
**
**	Side Effects:
**		The error message is generated.
**
**	Defines:
**		ferror
**		Qmdname -- the names of the various Qmodes.
**
**	Requires:
**		error
**		reset
**		Pipe
**		rdpipe
**		wrpipe
**		Nullsync -- send null query on error -- defined
**			in main.c
**		nullsync -- routine to do same -- defined in
**			issue.c.
**
**	Called By:
**		MANY
**
**	Trace Flags:
**		none
**
**	Diagnostics:
**		none of it's own.
**
**	Syserrs:
**		none
*/

extern struct pipfrmt	Pipe;
char	*Qmdname[]
{
	"[ERROR]",	/* 0 = mdRETTERM */
	"RETRIEVE",	/* 1 = mdRETR */
	"APPEND",	/* 2 = mdAPP */
	"REPLACE",	/* 3 = mdREPL */
	"DELETE",	/* 4 = mdDEL */
	"",		/* 5 = mdCOPY */
	"",		/* 6 = mdCREATE */
	"",		/* 7 = mdDESTROY */
	"",		/* 8 = mdHELP */
	"",		/* 9 = mdINDEX */
	"",		/* 10 = mdMODIFY */
	"",		/* 11 = mdPRINT */
	"",		/* 12 = mdRANGE */
	"",		/* 13 = mdSAVE */
	"DEFINE",	/* 14 = mdDEFINE */
	"RET_UNIQUE",	/* 15 = mdRET_UNI */
	"",		/* 16 = mdVIEW */
	"",		/* 17 = mdUPDATE */
	"",		/* 18 = mdRESETREL */
	"",		/* 19 = mdERIC */
	"",		/* 20 = mdNETQRY */
	"",		/* 21 = mdMOVEREL */
	"",		/* 22 = mdPROT */
	"",		/* 23 = mdINTEG */
	"",		/* 24 = mdDCREATE */
};


ferror(errno, qmode, vn, p1, p2, p3, p4, p5, p6)
int	errno;
int	qmode;
int	vn;
char	*p1, *p2, *p3, *p4, *p5, *p6;
{
	register char	*x1;
	register char	*x2;
	char		xbuf[MAXNAME + 1];
	register int	i;
	extern char	*trim_relname();
	extern int	Nullsync;
	struct retcode	*rc;

	/* flush the upward read pipe */
	rdpipe(P_SYNC, &Pipe, R_up);

	/* set up qmode and varno parameters */
	x1 = x2 = "";
	i = qmode;
	if (i >= 0)
		x1 = Qmdname[i];
	i = vn;
	if (i >= 0)
		smove(trim_relname(Rangev[i].relid), x2 = xbuf);

	/* issue the error message */
	error(errno, x1, x2, p1, p2, p3, p4, p5, p6, 0);

	/* issue null query if running Equel */
	if (Nullsync)
		rc = nullsync();
	else
		rc = NULL;
	
	/* sync to above */
	wrpipe(P_PRIME, &Pipe, 0, 0, 0);
	if (rc != NULL)
		wrpipe(P_NORM, &Pipe, W_up, rc, sizeof *rc);
	wrpipe(P_END, &Pipe, W_up);

	/* return to main loop for next query */
	reset();
}
/*
**  XERROR -- interface from need() to ferror()
**
**	Parameters:
**		err -- the error number.
**
**	Returns:
**		never
**
**	Side Effects:
**		see ferror
*/

xerror(err)
int	err;
{
	ferror(err, -1, -1, 0);
}
/*
**  LSETBIT -- set a bit in a domain set
**
**	Parameters:
**		bitno -- the bit number to set (0 -> 127)
**		xset -- the set to set it in.
**
**	Returns:
**		none
**
**	Side Effects:
**		none
**
**	Called By:
**		makedset()
**
**	History:
**		8/21/78 (eric) -- written.
*/

lsetbit(bitno, xset)
int	bitno;
int	xset[8];
{
	register int	b;
	register int	n;
	register int	*x;

	x = xset;

	b = bitno;
	n = b >> LOG2WORDSIZE;
	b =& WORDSIZE - 1;

	x[n] =| 1 << b;
}
/*
**  MERGEVAR -- merge variable numbers to link terms
**
**	One specified variable gets mapped into another, effectively
**	merging those two variables.  This is used for protection
**	and integrity, since the constraint read from the tree
**	must coincide with one of the variables in the query tree.
**
**	Parameters:
**		va -- the variable which will dissappear.
**		vb -- the variable which 'va' gets mapped into.
**		root -- the root of the tree to map.
**
**	Returns:
**		none
**
**	Side Effects:
**		The tree pointed at by 'root' gets VAR and RESDOM
**			nodes mapped.
**		Range table entry for 'va' is deallocated.
**		The 'Remap' vector gets reset and left in an
**			undefined state.
**
**	Requires:
**		clrrange -- to clear the 'Remap' vector (called with
**			a FALSE parameter, so 'Rangev' is not cleared).
**		mapvars -- to do the actual mapping.
**		Remap -- to define the mapping for mapvars().
**		Rangev -- to check consistency and to clear the entry
**			for variable 'va'.
**
**	Called By:
**		protect.c
**		integrity.c
**
**	Trace Flags:
**		16
**
**	Diagnostics:
**		none
**
**	Syserrs:
**		mergevar: range %d %d
**			Either 'va' or 'vb' was out of range.
**		mergevar: incon %s %s
**			The variables for 'va' and 'vb' must range
**			over the same relation.  The strings are the
**			relation names if this does not hold.
**		mergevar: undef %d %d
**			The variables 'va' and 'vb' must both
**			be defined.
*/

mergevar(va, vb, root)
int	va;
int	vb;
QTREE	*root;
{
	register int	a;
	register int	b;

	a = va;
	b = vb;

#	ifdef xQTR1
	if (tTf(16, 0))
	{
		printf("\nmergevar(%d->%d)", a, b);
		treepr(root, NULL);
	}
#	endif

	/*
	**  Insure that 'va' and 'vb' are consistant, that is,
	**  that they both are in range, are defined, and range over
	**  the same relation.
	*/

	if (a < 0 || b < 0 || a >= MAXVAR + 1 || b >= MAXVAR + 1)
		syserr("mergevar: range %d %d", a, b);
	if (!Rangev[a].rused || !Rangev[b].rused)
		syserr("mergevar: undef %d %d", a, b);
	if (!bequal(Rangev[a].relid, Rangev[b].relid, MAXNAME) ||
	    !bequal(Rangev[a].rowner, Rangev[b].rowner, 2))
		syserr("mergevar: incon %.14s %.14s", Rangev[a].relid,
		    Rangev[b].relid);
	
	/*
	**  To do the actual mapping, we will set up 'Remap' and
	**  call 'mapvars()'.  This is because I am too lazy to
	**  do it myself.
	*/

	clrrange(FALSE);
	Remap[a] = b;
	mapvars(root);

	/* delete a from the range table */
	Rangev[a].rused = FALSE;
}
/*
**  MAKEZERO -- make a node with value 'zero'
**
**	A node is created with value representing the zero value
**	for the specified type, that is, 0 for integers, 0.0 for
**	floats, and the blank string for chars.
**
**	Parameters:
**		typ -- the node type.
**
**	Returns:
**		a pointer to the zero node.
**
**	Side Effects:
**		space is grabbed from Qbuf
**
**	Requires:
**		Qbuf -- for space.
**		need() -- to get that space.
**
**	History:
**		11/21/78 (eric) -- written.
*/

QTREE *
makezero(typ)
int	typ;
{
	register int		l;
	register struct symbol	*s;
	int			symbuf[(sizeof *s + 4) / sizeof l];

	s = &symbuf;
	s->type = typ;

	switch (typ)
	{
	  case INT:
		s->len = l = 2;
		s->value[0] = 0;
		break;

	  case FLOAT:
		s->len = l = 4;
		s->value->f4type = 0.0;
		break;

	  case CHAR:
		s->len = l = 2;
		s->value[0] = '  ';	/* (two spaces) */
		break;

	  default:
		syserr("makezero: typ %d", typ);
	}

	/* duplicate the node into Qbuf */
	l =+ sizeof *s;
	s = need(Qbuf, l);
	bmove(&symbuf, s, l);
	return (s);
}
