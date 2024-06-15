typedef struct{
	char *arg;
	char tp; // 2 bits for parameter type, 4 for match type, 2 for extended settings
	char bind; // if not 255, return this instead of arg index
	short __len; // length, pre-computed at initctx
	} Arg;
typedef struct {
	Arg* args;
	short argc;
	short idx;
	char **argv;
	int __pad;
	} ArgCtx;
enum {
	p_nopar,
	p_reqpar,
	p_optpar
}; // parameter type
enum {
	M_NULL,
	m_int8,
	m_int16,
	m_int32,
	m_path,
	m_str,
	M_RES0
}; //match type
enum {
	xs_int_notzero=1
};
enum {
	xs_path_fzdir=2,
	xs_path_fzfile=3
	};
#define pack_tp(par,mat,xs) ((par&3)<<6)|((mat&15)<<2)|(xs&3)
int initctx(ArgCtx* ctx,Arg *args,int argc,char *argv[]);
int nextarg(ArgCtx* ctx,void *ret);
