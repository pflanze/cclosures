#include <stdlib.h>

#include "cclosures.h"

#include <stdio.h>

typedef void* Object;

/* lists */

struct pair {
    Object car;
    Object cdr;
};
// flat kinds of pairs?...  then multitude of map implementations
// needed yeah, or dyn dispatch

typedef struct pair pair_t;

pair_t* cons (Object car, Object cdr) {
    LET_NEW(new, pair_t);
    new->car= car;
    new->cdr= cdr;
    return new;
}


/* interface, with undefined env */
struct mapfn_closure {
    Object(*proc)(struct Some_env* env, Object v);
    struct Some_env env;
};

DEFN(pair_t*, map,
     struct mapfn_closure* mapfn,
     pair_t* lis) {
    if (!lis) {
	return NULL;
    } else {
	return cons(CALL(mapfn, lis->car),
		    SELFCALL(map, mapfn, lis->cdr));
    }
}

DEFN(void, for_each,
     struct for_each_closure* proc,
     pair_t* lis) {
    if (! lis) {
	return;
    } else {
	CALL(proc, proc, lis->car);
	SELFCALL(for_each, proc, lis->cdr);
    }
}

DEFN(long, cj_atol,
     char* v) {
    return atol(v);
}

DEFN(void, print_long,
     long v) {
    printf("%ld\n", v);
}

DEFN(long, inc_long,
     long v) {
    return (v + 1);
}

DEFCLOSURE(long, add_long,
	   { long a; },
	   long b) {
    return (env->a + b);
}

DEFN(struct add_long_closure*, make_add_long,
     long a) {
    RETURN_CLOSURE(add_long, a);
}


DEFCLOSURE(void, printlis,
	   {char* prefix;},
	   pair_t* lis,
	   int i,
	   pair_t* origlis) {
    if (! lis) {
	SELFCALL(printlis, origlis, i, origlis);
    } else {
	printf("%s element %i is value: '%s'\n",
	       env->prefix, i, UNSAFECAST(char*,lis->car));
	SELFCALL(printlis, lis->cdr, i+1, origlis);
    }
}

DEFN(struct printlis_closure*, make_printlis,
     char* prefix) {
    RETURN_CLOSURE(printlis, prefix);
}


// mutual recursion example
typedef int BOOL;
#define FALSE 0
#define TRUE 1

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


int main (int argc, char** argv) {
    if (argc==2) {
	long v= atol(argv[1]);
	printf("is number %ld even?: %s\n",
	       v,
	       (CALL(evenp,v) ? "yes" : "no"));
    } else {
	pair_t* lis=NULL;
	int i;
	for (i=argc-1; i>=0; i--) {
	    lis= cons(argv[i], lis);
	}
	struct printlis_closure* myprintlis= CALL(make_printlis,"argv");
	CALL(myprintlis, lis, 0, lis);
    }
    return 0;
}
