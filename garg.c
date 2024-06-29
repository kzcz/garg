#include <garg.h>
#define __err(fmt,...) fprintf(stderr,"%s: " fmt "\n",ctx->argv[0],__VA_ARGS__);exit(1)
#define _gerr(fmt,...) fprintf(stderr,"%s" fmt,_ex,__VA_ARGS__);exit(1)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
unsigned long long max[]={0,0xFF,0xFFFF,0xFFFFFFFF};
char max_len[]={0,3,5,10};
typedef struct {
	char **argv;
	char *arg;
	} PACtx;
char *_rpr[]={"p_nopar","p_reqpar","p_optpar"};
char *_ex="Error while checking args: ";
void __calc_lens(Arg *_a){
	if(!_a){return;}
	int i=0;
	while(_a[i].arg!=0){
		_a[i].__len=(short)strlen(_a[i].arg);
		i++;
		}
	return;
	}
void __check_args(Arg *_a){
	if(!_a){return;}
	int i=0;
	while(_a[i].arg!=0){
		char tp=_a[i].tp;
		int p=tp>>6;
		int m=(tp>>2)&15;
		//int x=tp&3;
		if(p==3){_gerr("Type of \"%s\" is invalid.",_a[i].arg);}
		if(!p){
			if(tp){_gerr("\"%s\" is of type %s, but it has options set..",_a[i].arg,_rpr[0]);}
		}
		else{
			if(!m){_gerr("Match type of \"%s\" is M_NULL, but it's of type %s",_a[i].arg,_rpr[p]);}
		}
		i++;
		}
	return;
	}
void parse_arg(PACtx *ctx,int tp,char* par,void *ret){
	int m=(tp>>2)&15;
	switch(m){
		case m_str: {
			*(long *)ret=(long)par;
			break;
		}
		case m_path: {
			struct stat j;
			if(access(par,F_OK)){
				__err("File \"%s\" as argument to \"%s\" doesn't exist.",par,ctx->arg);
			}
			stat(par,&j);
			int q=tp&3;
			if((q==xs_path_fzdir)&&(!S_ISDIR(j.st_mode))){
				__err("\"%s\" expected a directory but got a file instead.",ctx->arg);
			}
			if((q==xs_path_fzfile)&&(!S_ISREG(j.st_mode))){
				__err("\"%s\" expected a file but got a file instead.",ctx->arg);
			}
			*(long *)ret=(long)par;
			break;
		}
		case m_int8:
		case m_int16:
		case m_int32:{
			int sl=strlen(par);
			int ml=max_len[m];
			if(sl>ml){__err("Argument for \"%s\" is too long.",ctx->arg);}
			unsigned long long z=0;
			int w;
			for(int i=0;par[i];i++){
				w=par[i];
				if((w<'0')||(w>'9')){__err("\"%s\" expected an integer, but got \"%s\" instead.",ctx->arg,par);}
				z=(z*10)+w-'0';
				}
			if(z>max[m]){
				__err("Argument for \"%s\" is too large.",ctx->arg);
			}
			int res=z&0xFFFFFFFF;
			if((tp&xs_int_notzero)&&(res==0)){
				__err("Argument for \"%s\" can't be zero.",ctx->arg);
				}
			*(int *)ret=res;
			break;
		}
	}
}
int initctx(ArgCtx* ctx,Arg *args,int argc,char *argv[]){
	ctx->idx=1;
	ctx->argc=(short)argc;
	ctx->argv=argv;
	if(!args){return 1;}
	__check_args(args);
	__calc_lens(args);
	ctx->args=args;
	return 0;
	}
int nextarg(ArgCtx* ctx,void *ret){
	Arg *args=ctx->args;
	Arg cur;
	if(ctx->idx>=ctx->argc){return -1;}
	char *arg=ctx->argv[ctx->idx];
	if(*arg!='-'){__err("Hanging expression \"%s\".",arg);}	
	short len=strlen(++arg);
	int i=0;
	while(args[i].arg){
		cur=args[i];
		if(cur.__len!=len){i++;continue;}
		if(!strcmp(arg,cur.arg)){
			ctx->idx++;int ha=((ctx->idx>=ctx->argc)||(*ctx->argv[ctx->idx]=='-'));
			switch(cur.tp>>6){
				case p_nopar: goto rt;
				case p_reqpar: {
					if(ha){__err("Missing parameter for \"%s\".",--arg);}
					goto parse;
					}
				case p_optpar: {
					if(ha){*(int *)ret=0;return i;}
					goto parse;
					}
				parse: {
						PACtx pax={ctx->argv,cur.arg};
						parse_arg(&pax,cur.tp,ctx->argv[ctx->idx],ret);
						ctx->idx++;
				}
				rt: {
					if(cur.bind==255) return i;
					return cur.bind;
				}
			}
		}
		i++;
	}
	__err("Unknown argument \"%s\".",arg);
}
