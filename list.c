#include "cclosures.h"

typedef void* Object;

typedef int BOOL;
#define FALSE 0
#define TRUE 1

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

void free_list (pair_t* lis) {
    if (lis) {
	free_list (lis->cdr);
	free (lis);
    }
}


DEFINTERFACE(map_fn_t, Object, Object);

DEFN(pair_t*, map,
     map_fn_t fn,
     pair_t* lis) {
    if (!lis) {
	return NULL;
    } else {
	return cons(CALL(fn, lis->car),
		    SELFCALL(map, fn, lis->cdr));
    }
}

DEFINTERFACE(for_each_proc_t, void, Object);

DEFN(void, for_each,
     for_each_proc_t proc,
     pair_t* lis) {
    if (! lis) {
	return;
    } else {
	CALL(proc, lis->car);
	SELFCALL(for_each, proc, lis->cdr);
    }
}

DEFN(long, parse_long,
     char* v) {
    return atol(v);
}

DEFN(void, print_long,
     long v) {
    printf("%ld\n", v);
}

/*
DEFN(long, inc_long,
     long v) {
    return (v + 1);
}
*/

DEFCLOSURE(long, add_long,
	   { long a; },
	   long b) {
    return (env->a + b);
}
DEFN(struct add_long_closure*, make_add_long,
     long a) {
    RETURN_CLOSURE(add_long, a);
}


/* a show function returns TRUE if the Object did fit into the
   provided char buffer. */
DEFINTERFACE(show_t, BOOL, char*res, size_t size_of_res, Object);

DEFN(BOOL, show_string, char*res, size_t size_of_res, Object v) {
    return (snprintf (res, size_of_res, "%s", (char*)v) < size_of_res);
}

DEFN(BOOL, show_long, char*res, size_t size_of_res, Object v) {
    return (snprintf (res, size_of_res, "%ld", (long)v) < size_of_res);
}

DEFCLOSURE(void, printlist_pitem,
	   {
	       char *between;
	       show_t show;
	       int bufsiz;
	       char **bufferh;
	   },
	   Object v) {
    /* environment copies can be mutated (visible locally only, thus
       double pointer for buffer) */
    int retry=0;
    while (1) {
	assert (retry < 10);
	retry++;
	if (CALL(env->show, *(env->bufferh), env->bufsiz, v))
	    break;
	int bufsiz1= env->bufsiz*4;
	assert(bufsiz1 > 0); // just, well..
	char *buffer1= realloc(*(env->bufferh), bufsiz1);
	/* aheh ^ copies contents for nothing */
	assert(buffer1);
	*(env->bufferh)= buffer1;
	env->bufsiz= bufsiz1;
    }
    printf("%s%s", env->between, *(env->bufferh));
    env->between= ", ";
}
DEFN(void, printlist, show_t show, pair_t* lis) {
    /* printlist needs to take show since we don't have dynamic
       dispatch (nor the tagging in place to do it ad hoc) */
    char *between= "";
    int bufsiz= 100;
    char *buffer= malloc(bufsiz);
    char **bufferh= &buffer;
    /* stack-allocated closure */
    ALLOCA_CLOSURE(pitem, printlist_pitem);
    /* SET_CLOSURE is separate from ALLOCA_CLOSURE and expecting a
       pointer, so that it could be used from a sub-procedure call: */
    SET_CLOSURE(&pitem,between);
    SET_CLOSURE(&pitem,show);
    SET_CLOSURE(&pitem,bufsiz);
    SET_CLOSURE(&pitem,bufferh);

    printf("[");
    CALL(for_each, V(&pitem), lis);
    printf("]\n");

    /* don't need to deallocate pitem, but: */
    free (buffer);
}
