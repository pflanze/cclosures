
#define STATIC static

#define DECLN(Rtype, Name, ...)						\
    struct Name##_env {};						\
    struct Name##_closure {						\
	Rtype (*const proc) (const struct Name##_env* env,		\
			     __VA_ARGS__);				\
	const struct Name##_env env;					\
    };									\
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
    STATIC Rtype Name##_proc (struct Name##_env* env, __VA_ARGS__) 

// #define IMPLCLOSURE  needed?


#define CALL(Name,...) Name->proc(&(Name->env), __VA_ARGS__)
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

#define RETURN_CLOSURE(clname, membernames,...)			      \
    LET_CLOSURE(return_closure_tmp, clname, membernames, __VA_ARGS__);	\
    return return_closure_tmp;


struct Empty_env {
};

struct Some_env {
    /* anything */
};


#define NEW(type) malloc(sizeof(type)) // ç

#define CAST(type,val) (type)(val)
