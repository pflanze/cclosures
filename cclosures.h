
#define STATIC static

#define DECLN(Rtype, Name, ...)						\
    struct Name##_env {};						\
    struct Name##_closure {						\
	Rtype (*const proc) (const struct Name##_env* env,		\
			     __VA_ARGS__);				\
	const struct Name##_env env;					\
    };									\
    typedef struct Name##_closure* Name##_closure_t;			\
    STATIC Rtype Name##_proc (const struct Name##_env* env, __VA_ARGS__); \
    STATIC const struct Name##_closure Name##_flat = { Name##_proc, {} }; \
    STATIC const struct Name##_closure* const Name = &(Name##_flat);	\
    STATIC Rtype Name##_proc (const struct Name##_env* env, __VA_ARGS__) 

#define DEFN DECLN

#define IMPLN(Rtype, Name, ...)						\
    STATIC Rtype Name##_proc (const struct Name##_env* env, __VA_ARGS__)

#define DEFCLOSURE(Rtype, Name, Env, ...)				\
    struct Name##_env Env;						\
    struct Name##_closure {						\
	Rtype (*proc) (struct Name##_env* env,				\
		       __VA_ARGS__);					\
	struct Name##_env env;						\
    };									\
    typedef struct Name##_closure* Name##_closure_t;			\
    STATIC Rtype Name##_proc (struct Name##_env* env, __VA_ARGS__) 

// #define IMPLCLOSURE  needed?


#define CALL(Name,...) (Name)->proc(&((Name)->env), __VA_ARGS__)
#define SELFCALL(Name,...) Name##_proc(env, __VA_ARGS__)


#define LET_NEW(varname, type) type* varname = NEW(type)

#include <string.h>

#define LET_CLOSURE(varname, clname, membernames,...)			\
    LET_NEW(varname, struct clname##_closure);				\
    {									\
	struct clname##_closure let_closure_tmp = {			\
	    clname##_proc,						\
	    {								\
		membernames,						\
		__VA_ARGS__						\
	    }								\
	};								\
	memcpy(varname,&let_closure_tmp, sizeof(struct clname##_closure)); \
    }

/*
  stack-allocate a closure (initialization of the env is separate on
  purpose: allocate before calling functions that return closures!)
*/
#define ALLOCA_CLOSURE(varname, clname)		\
    struct clname##_closure varname = { \
	clname##_proc			\
    };

/*
  initialize a pre-allocated closure. Give *ptr for pointers.  Call
  for every field.
  Instead of:
      res->env.a = a;
  this leads to:
      SET_CLOSURE(res, a);
*/
#define SET_CLOSURE(cl, field)		\
    (cl)->env.field = field;


#define RETURN_CLOSURE(clname, membernames,...)			      \
    LET_CLOSURE(return_closure_tmp, clname, membernames, __VA_ARGS__);	\
    return return_closure_tmp;


struct Empty_env {
};

struct Some_env {
    /* anything */
};

#define CAST(type,val) (type)(val)

#define VOID(val) CAST(void*,val)


#define DEFINTERFACE(Name_t,Procreturntype,...)			\
    typedef struct Name_t##struct {				\
	Procreturntype(*proc)(struct Some_env*, __VA_ARGS__);	\
	struct Some_env env;					\
    } *Name_t;


#define NEW(type) malloc(sizeof(type)) // ç

