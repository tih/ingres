/*
**This file contains the specification of QUEL recognized by the parser.
*/

/* COMMANDS */
%term		APPEND	COPY	CREATE	DELETE	DESTROY	HELP	INDEX	MODIFY
%term		PRINT	RANGE	REPLACE	RETRIEVE	SAVE		
%term		DEFINE	PERMIT	VIEW	INTEGRITY

/* 'NOISE' WORDS */
%term		ALL	BY	FROM	IN	INTO	UNIQUE	AT
%term		IS	OF	ON	ONTO	TO	UNTIL	WHERE

/* CONSTANTS */
%term		NAME	SCONST	I2CONST	I4CONST F4CONST	F8CONST

/* PUNCTUATION */
%term		COMMA	LPAREN	PERIOD	RPAREN	COLON	BGNCMNT	ENDCMNT

/* UNARY ARITHMETIC OPERATORS */
%term		UAOP

/* BINARY ARITHMETIC OPERATORS */
%term		BAOP	BAOPH

/* BOUNDS OPERATORS */
%term		BDOP

/* EQUALITY OPERATORS */
%term		EOP

/* LOGICAL OPERATORS */
%term		LBOP	LUOP

/* FUNCTIONAL OPERATORS */
%term		FOP	FBOP

/* AGGREGATE OPERATORS */
%term		AGOP


/* DEFINE ASCENDING PRECEDENCE FOR OPERATORS */
%left		LBOP
%left		LUOP
%left		UAOP
%left		BAOP
%left		BAOPH
%binary		unaryop

%{
/* SCANNER/PARSER GLOBALS & TABLES */
# include	"../ingres.h"
# include	"../aux.h"
# include	"../tree.h"
# include	"../symbol.h"
# include	"parser.h"
%}
%%
%{
register int			i;
register struct atstash		*aptr;
char				permbuf[3];
/* space for two names, their null bytes and the seperator */
char				modbuf[(2 * (MAXNAME + 1)) + 1];
static char			hqmbuf[2];
%}
program:	xprogram
	|
;
xprogram:	xprogram stmnt =
			endquelst(Opflag);
	|	stmnt =
			endquelst(Opflag);
;
stmnt:		append
	|	copy
	|	create
	|	delete 	
	|	destroy
	|	help
	|	index	
	|	integrity
	|	modify
	|	permit
	|	print
	|	range
	|	replace	
	|	retrieve 
	|	save
	|	view
;
range:		rngstmnt OF NAME IS NAME =
		{
			if ((i = openr(&Reldesc, -1, $5)) < 0)
				syserr("relname: error in openr '%d'", i);
			if (i > 0)
				/* invalid relation name */
				yyerror(RNGEXIST, $5, 0);
			rngent(R_EXTERNAL, $3, $5, Reldesc.relowner, Reldesc.relatts, Reldesc.relstat);
		}
;
rngstmnt:	RANGE =
		{
			Opflag = mdRANGE;
		}
;
append:		apstmnt apto relation tlclause qualclause =
		{
			/* make root node */
			Lastree = tree($4, $5, ROOT, 0, 0);
		}
;
apstmnt:	APPEND =
		{
			Opflag = mdAPP;
		}
;
apto:		INTO
	|	ONTO
	|	TO
	|	ON
	|	;
;
delete:		delstmnt delwd relation qualclause =
		{
			/* make root node for delete, with a TIDNODE at leftmost */
			Lastree = tree(tree(NULL, Tidnode, RESDOM, 4, NULL), $4, ROOT, 0, 0);
		}
;
delstmnt:	DELETE =
		{
			Opflag = mdDEL;
		}
;
delwd:		IN
	|	ON
	|	FROM
	|	;
;
replace:	repstmnt repkwd relation tlclause qualclause =
		{
			/* make root node for replace */
			Lastree = tree($4, $5, ROOT, 0, 0);
		}
;
repstmnt:	REPLACE =
		{
			Opflag = mdREPL;
		}
;
repkwd:		INTO
	|	IN
	|	ON
	|	;
;
retrieve:	retstmnt retclause tlclause qualclause =
		{
			/* make root node for retrieve */
			Lastree = tree($3, $4, ROOT, 0, 0);
		}
;
retstmnt:	RETRIEVE =
		{
			Opflag = mdRETR;
		}
;
retclause:	retkwd relation =
		{
			/* set up pipe block and save relname for create */
			Rsdmno = 0;
			setp("0");	/* relstat = nil */
			setp(Resrng->relnm);
		}
	|	=
		{
			/* no result relation, output to terminal */
			Rsdmno = 0;
			Resrng = 0;
		}
	|	UNIQUE =
		{
			Opflag = mdRET_UNI;
			Rsdmno = 0;
			Resrng = 0;
		}
;
retkwd:		INTO
	|	TO
	|	;
;
view:		viewclause tlclause qualclause =
		{
			Lastree = tree($2, $3, ROOT, 0, 0);
		}
;
viewclause:	viewstmnt relation =
		{
			Rsdmno = 0;
			setp("0040");	/* relstat = S_VIEW */
			setp(Resrng->relnm);
		}
;
viewstmnt:	DEFINE VIEW =
		{
			Opflag = mdVIEW;
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
		}
;
permit:		permstmnt permlist permrel permtarg permwho permplace permtd qualclause =
		{
			Lastree = tree($4, $8, ROOT, 0, 0);
		}
;
permstmnt:	DEFINE PERMIT =
		{
			Opflag = mdPROT;
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
		}
;
permlist:	permxlist
	|	permlist COMMA permxlist
;
permxlist:	ALL =
			permcom(-1);	/* means 'all' commands */
	|	RETRIEVE =
			permcom(mdRETR);
	|	DELETE =
			permcom(mdDEL);
	|	APPEND =
			permcom(mdAPP);
	|	REPLACE =
			permcom(mdREPL);
;
permrel:	permword relation =
		{
			/* put command vector into list now since this always happens */
			setp(iocv(Permcomd));
			Permcomd = 0;		/* reset command map */
			setp(Resrng->relnm);
			bmove(Resrng->relnowner, permbuf, 2);
			permbuf[2] = 0;
			setp(permbuf);
		}
;
permword:	ON
	|	OF
	|	TO
;
permtarg:	LPAREN permtlist RPAREN =
		{
			$$ = $2;
		}
	|	=
		{
			$$ = NULL;
		}
;
permtlist:	permtlelm
	|	permtlist COMMA permtlelm =
		{
			/*
			** attach bulk of permit tl to leftmost node of new elem
			*/
			$$ = tlprepend($1, $3);
		}
;
permtlelm:	NAME =
		{
			/* Resrng is set by the "relation" production */
			Trname = $1;
			aptr = attlookup(Resrng, Trname);
			$$ = tree(NULL, NULL, VAR, 6, Resrng->rentno, aptr);
			$$ = addresdom(NULL, $$);
		}
;
permwho:	TO NAME =
			setp($2);
	|	TO ALL =
			setp("all");
;
permplace:	AT NAME =
			setp($2);
	|	AT ALL =
			setp("all");
	|	=
			setp("all");		/* default is all */
;
permtd:		permtime permday
	|	permdeftime permday
	|	permtime permdefday
	|	permdeftime permdefday
;
permdeftime:	=
		{
			setp("0");
			setp("1440");
		}
;
permdefday:	=
		{
			setp("sun");
			setp("sat");
		}
;
permtime:	FROM I2CONST COLON I2CONST TO I2CONST COLON I2CONST =
		{
			setp(iocv(timeofday($2, $4)));
			setp(iocv(timeofday($6, $8)));
		}
;
permday:	ON NAME TO NAME =
		{
			setp($2);
			setp($4);
		}
;
integrity:	integstmnt integnoise relation integis qual =
		{
			Lastree = tree(NULL, norml($5), ROOT, 0, 0);
			Qlflag--;	/* turn off here */
		}
;
integstmnt:	DEFINE INTEGRITY =
		{
			Opflag = mdINTEG;
			Qlflag++;	/* OK to turn on here because integrity doesn't have a targ list */
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
		}
;
integnoise:	ON
	|	ONTO
	|	IN
	|	OF
	|	/* null */
;
integis:	IS
	|	/* null*/
;
relation:	NAME =
		{
#			ifdef	xPTR2
			tTfp(32, 0, "res rel name/var: '%s'\n", $1);
#			endif
			switch (Opflag)
			{
			  case mdRETR:
			  case mdVIEW:
				/* result better not be a rel name */
				if ((i = openr(&Reldesc, -1, $1)) < 0)
					syserr("relation: err openr '%d'", i);
				if (i == 0)
				{
					/* reln exists */
					if (bequal(Reldesc.relowner, Usercode, 2))
					{
						/* same owner, can't duplicate name */
						yyerror(RESEXIST, $1, 0);
					}
					else
					{
						/* owned by dba -- purge range table */
						rngdel($1);
					}
				}
				Resrng = rngent(R_INTERNAL, "", $1, Usercode, 0, 0);
				break;

			  case mdAPP:
				/* result is a rel name */
				Resrng = rnglook($1, LOOKREL);
				if (Resrng == 0)
				{
					if ((i = openr(&Reldesc, -1, $1)) < 0)
						syserr("relation: err openr '%d'", i);
					if (i)
						/* invalid relation name */
						yyerror(RESAPPEX, $1, 0);
					Resrng = rngent(R_INTERNAL, "", $1, Reldesc.relowner, Reldesc.relatts, Reldesc.relstat);
				}
				checkupd(Resrng);
				break;

			  case mdPROT:
			  case mdINTEG:
#			  ifdef	DISTRIB
			  case mdDISTRIB:
#			  endif
				/* the result is a tuple variable */
				Resrng = rnglook($1, LOOKVAR);
				if (Resrng == 0)
					/* variable not declared */
					yyerror(NOVBLE, $1, 0);
				break;

			  case mdREPL:
			  case mdDEL:
				/* the result is a tuple variable */
				Resrng = rnglook($1, LOOKVAR);
				if (Resrng == 0)
					/* variable not declared */
					yyerror(NOVBLE, $1, 0);
				checkupd(Resrng);
				Tidnode = tree(NULL, NULL, VAR, 6, Resrng->rentno, &Faketid);
				break;
			}
		}
;
tlclause:	LPAREN tlist RPAREN =
		{
			if (Patflag)
				/* no patt match in targ list */
				yyerror(NOPATMAT, 0);
			$$ = $2;

			/*
			** replace must have tid node as left branch
			**	(so does delete but it doesn't have a targ list)
			*/
			if (Opflag == mdREPL)
			{
				$$ = tlprepend(tree(NULL, Tidnode, RESDOM, 4, 0), $$);
			}
		}
;
tlist:		tlelm
	|	tlist COMMA tlelm =
		{
			/*
			** attach bulk of targ list to leftmost node
			** of new element
			*/
			$$ = tlprepend($1, $3);
		}
;
tlelm:		NAME is afcn =
		{
			Trname = $1;
			/* make a new resdom entry for targ list */
			$$ = addresdom(NULL, $3);
		}
	|	attrib =
		{
			/* makes a new resdom entry for targ list */
			$$ = addresdom(NULL, $1);
		}
	|	var PERIOD ALL =
		{
			if (Opflag == mdREPL)
				/* ALL not defined for REPLACE */
				yyerror(REPALL, $1->varname, 0);
			/* makes set of new resdom entries for targ list */
			$$ = xdot($1);
		}
;
is:		IS
	|	BY
;
qualclause:	where qual =
		{
#			ifdef xPTM
			if (tTf(76, 2))
				timtrace(25, 0);
#			endif
			$$ = norml($2);
			Qlflag--;
#			ifdef xPTM
			if (tTf(76, 2))
				timtrace(26, 0);
#			endif
		}
	|	=
		{
			/* null qualification */
			$$ = norml(NULL);
		}
;
where:		WHERE =
			Qlflag++;
;
qual:		LPAREN qual RPAREN =
			$$ = $2;
	|	LUOP qual =
			$$ = tree(NULL, $2, UOP, 2, $1);
	|	qual LBOP qual =
			$$ = tree($1, $3, $2, 0, 0);
	|	clause
;
clause:		afcn relop afcn =
			$$ = tree($1, $3, BOP, 2, $2);
;
relop:		EOP
	|	IS
	|	BDOP
;
afcn:		aggrfcn
	|	attribfcn
	|	afcn BAOPH afcn =
			$$ = tree($1, $3, BOP, 2, $2);
	|	afcn BAOP afcn =
			$$ = tree($1, $3, BOP, 2, $2);
	|	afcn UAOP afcn =
			$$ = tree($1, $3, BOP, 2, $2);
	|	LPAREN afcn RPAREN =
			$$ = $2;
	|	uop afcn	%prec unaryop	=
			$$ = tree(NULL, $2, UOP, 2, $1);
	|	FOP LPAREN afcn RPAREN =
			$$ = tree($3, NULL, UOP, 2, $1);
	|	FBOP LPAREN afcn COMMA afcn RPAREN =
			$$ = tree($3, $5, BOP, 2, $1);
;
aggrfcn:	AGOP LPAREN afcn BY domseq qualclause RPAREN =
		{
#			ifdef	xPTR2
			tTfp(35, 0, "agg func\n");
#			endif
			windup($5);
			$$ = tree(tree($5, tree(NULL, $3, AOP, 6, $1), BYHEAD, 0, 0), $6, AGHEAD, 0, 0);
		}
	|	AGOP LPAREN afcn qualclause RPAREN =
		{
			$$ = tree(tree(NULL, $3, AOP, 6, $1), $4,  AGHEAD, 0, 0);
		}
;
domseq:		targdom
	|	domseq COMMA targdom =
		{
			$$ = tlprepend($1, $3);
		}
;
targdom:	afcn =
		{
			$$ = tree(NULL, $1, RESDOM, 4, Rsdmno);
		}
;
attrib:		var PERIOD NAME =
		{
#			ifdef	xPTR2
			tTfp(37, 0, "attrib %s.%s found\n", $1->varname, $3);
#			endif

			/* remember attribute name */
			Trname = $3;

			/* look up attribute */
			aptr = attlookup($1, Trname);
			$$ = tree(NULL, NULL, VAR, 6, $1->rentno, aptr);
		}
;
var:		NAME =
		{
			$$ = rnglook($1, LOOKVAR);
			if ($$ == 0)
				/* variable not declared */
				yyerror(NOVBLE, $1, 0);
		}
;
attribfcn:	I2CONST =
		{
			$$ = tree(NULL, NULL, INT, 2, $1);
		}
	|	I4CONST =
		{
			$$ = tree(NULL, NULL, INT, 4, $1);
		}
	|	F4CONST =
		{
			$$ = tree(NULL, NULL, FLOAT, 4, $1);
		}
	|	F8CONST =
		{
			$$ = tree(NULL, NULL, FLOAT, 8, $1);
		}
	|	SCONST =
		{
			if (patmat($1) && !Qlflag)
				Patflag = 1;
			$$ = tree(NULL, NULL, CHAR, length($1), $1);
		}
	|	NAME =
		{
			$$ = tree(NULL, NULL, COP, 2, $1);
		}
	|	attrib
;
uop:		UAOP	%prec unaryop	=
		{
			if ($1 == opADD)
				$$ = opPLUS;
			else
				if ($1 == opSUB)
					$$ = opMINUS;
		}
;
copy:		copstmnt alias LPAREN coparam RPAREN keywd SCONST =
		{
#			ifdef	xPTR2
			tTfp(41, 0, "copy %s,%s\n", $2, $7);
#			endif
			setp($7);
		}
;
copstmnt:	COPY =
		{
			Opflag = mdCOPY;
		}
;
coparam:	cospecs
	|	;
;
cospecs:	alias is coent
	|	cospecs COMMA alias is coent
;
coent:		alias
	|	SCONST =
		{
			setp($1);
		}
;
alias:		NAME =
		{
			setp($1);
			if (Opflag == mdDESTROY || Opflag == mdCREATE
#				ifdef	DISTRIB
				|| Opflag == mdDCREATE
#				endif
							)
				rngdel($1);
		}
;
specs:		alias is alias
	|	specs COMMA alias is alias
;
keywd:		INTO =
		{
			setp("\0");
			setp("i");
		}
	|	FROM =
		{
			setp("\0");
			setp("f");
		}
;
create:		crestmnt alias LPAREN specs RPAREN
;
crestmnt:	CREATE =
		{
			Opflag = mdCREATE;

			/* set up parameters for regular create */
			setp("0");		/* relstat = nil */
		}
;
destroy:	destmnt keys
	|	destqm destlist
;
destmnt:	DESTROY =
		{
			Opflag = mdDESTROY;
		}
;
destqm:		destmnt INTEGRITY NAME =
		{
			Opflag = mdREMQM;
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
			setp("6");
			setp($3);
		}
	|	destmnt PERMIT NAME =
		{
			Opflag = mdREMQM;
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
			setp("5");
			setp($3);
		}
;
destlist:	I2CONST =
			setp(iocv($1->i2type));
	|	destlist COMMA I2CONST =
			setp(iocv($3->i2type));
	|	ALL
;
help:		helstmnt hlist
	|	helstmnt =
			setp("2");	/* all relns */
	|	helqmstmnt hqmlist
;
helstmnt:	HELP =
		{
			Opflag = mdHELP;
		}
;
helqmstmnt:	HELP VIEW =
		{
			Opflag = mdDISPLAY;
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
			smove("4", hqmbuf);
		}
	|	HELP PERMIT =
		{
			Opflag = mdDISPLAY;
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
			smove("5", hqmbuf);
		}
	|	HELP INTEGRITY =
		{
			Opflag = mdDISPLAY;
			if (!Qrymod)
				/* no qrymod in database */
				yyerror(NOQRYMOD, 0);
			smove("6", hqmbuf);
		}
;
hlist:		hparam
	|	hlist COMMA hparam
	|	ALL =
			setp("3");
;
hparam:		NAME =
		{
			/* relation */
			setp("0");
			setp($1);
		}
	|	SCONST =
		{
			/* manual page */
			setp("1");
			setp($1);
		}
;
hqmlist:	NAME =
		{
			setp(hqmbuf);
			setp($1);
		}
	|	hqmlist COMMA NAME =
		{
			setp(hqmbuf);
			setp($3);
		}
;
index:		instmnt LPAREN keys RPAREN =
		{
			if (Rsdmno > MAXKEYS)
				/* too many attributes in key */
				yyerror(INDEXTRA, 0);
		}
;
instmnt:	INDEX ON NAME IS NAME =
		{
			/* init INDEX command */
			Opflag = mdINDEX;
			Rsdmno = 0;
			setp($3);
			setp($5);
			Indexname = $5;
		}
;
modify:		modstmnt alias TO modstorage modkeys modqual
;
modstmnt:	MODIFY =
		{
			Opflag = mdMODIFY;
			Rsdmno = 0;
		}
;
modstorage:	NAME =
			setp($1);
modkeys:	modstkey modrptkey
	|	;
;
modstkey:	ON =
			setp("name");
;
modrptkey:	modbasekey
	|	modrptkey COMMA modbasekey
;
modbasekey:	NAME =
			setp($1);
	|	NAME COLON NAME =
		{
			concat($1, ztack(":", $3), modbuf);
			setp(modbuf);
		}
;
modqual:	modcond modfill
	|	;
;
modcond:	WHERE =
			setp("\0");
;
modfill:	modfillnum
	|	modfill COMMA modfillnum
;
modfillnum:	NAME IS I2CONST =
		{
			setp($1);
			setp(iocv($3->i2type));
		}
;
keys:		alias =
			Rsdmno++;
	|	keys COMMA alias =
			Rsdmno++;
;
print:		prinstmnt keys
;
prinstmnt:	PRINT =
		{
			Opflag = mdPRINT;
		}
;
save:		savstmnt alias UNTIL date
;
savstmnt:	SAVE =
		{
			Opflag = mdSAVE;
		}
;
date:		month day_year day_year
;
month:		alias
	|	day_year
;
day_year:	I2CONST =
		{
			i = iocv($1->i2type);
#			ifdef	xPTR3
			tTfp(57, 1, "day_year: %s\n", i);
#			endif
			setp(i);
		}
;
%%
# include	"../scanner.h"
# include	"../unix.h"
# include	"tables.y"
# include	"yyerror.y"
