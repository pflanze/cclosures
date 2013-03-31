#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "cclosures.h"
#include "list.c"


// mutual recursion example

DEFN(BOOL, zerop,
     long n) {
    return (n == 0);
}

DECLN(BOOL, evenp,
     long n);

DEFN(BOOL, oddp,
     long n) {
    if (CALL(zerop, n)) {
	return FALSE;
    } else {
	return CALL(evenp, n-1);
    }
}

IMPLN(BOOL, evenp,
      long n) {
    if (CALL(zerop, n)) {
	return TRUE;
    } else {
	return CALL(oddp, n-1);
    }
}

/*
DEFCLOSURE(void, printlis,
	   {char* prefix;},
	   pair_t* lis,
	   int i,
	   pair_t* origlis) {
    if (! lis) {
	SELFCALL(printlis, origlis, i, origlis);
    } else {
	printf("%s element %i is value: '%s'\n",
	       env->prefix, i, CAST(char*,lis->car));
	SELFCALL(printlis, lis->cdr, i+1, origlis);
    }
}
DEFN(struct printlis_closure*, make_printlis,
     char* prefix) {
    RETURN_CLOSURE(printlis, prefix);
}
*/


int main (int argc, char** argv) {
    if (argc==2) {
	/* demonstrate mutual recursion */
	long v= atol(argv[1]);
	printf("is number %ld even?: %s\n",
	       v,
	       (CALL(evenp,v) ? "yes" : "no"));
    } else {
	/* demonstrate lists, map, for_each, and 'manual currying' */
	pair_t* lis=NULL;
	int i;
	for (i=argc-1; i>=1; i--) {
	    lis= cons(argv[i], lis);
	}
	printf ("Original list (strings): "); CALL(printlist, V(show_string), lis);

	{
	    pair_t* lis1= CALL(map, V(parse_long), lis);
	    printf ("Parsed list (longs): "); CALL(printlist, V(show_long), lis1);
		
	    {
		struct add_long_closure*inc = CALL(make_add_long, 1);
		pair_t* lis2= CALL(map, V(inc), lis1);
		printf ("Incremented list: "); CALL(printlist, V(show_long), lis2);
		free(inc);
		free_list(lis2);
	    }

	    free_list(lis1);
	}
	
	/* demonstrate infinite recursion, XXX stupid? */
	//struct printlis_closure* myprintlis= CALL(make_printlis,"argv");
	//CALL(myprintlis, lis, 0, lis);
	//free(myprintlis);

	free_list(lis);
    }
    return 0;
}
