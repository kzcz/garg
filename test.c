#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include "/sdcard/GArg/garg.h"
int main(int argc, char *argv[])
{
	struct passwd *pwd;
	struct group *grp;
	struct stat res;
	ArgCtx ctx;
	int r;
	Arg v[]={
		{"t0",pack_tp(p_reqpar,m_int32,xs_int_notzero),255,0},
		{"t1",pack_tp(p_reqpar,m_int32,xs_int_notzero),255,0},
		{"name",pack_tp(p_optpar,m_str,0),255,0},
		{"dir",pack_tp(p_reqpar,m_path,xs_path_fzdir),255,0},
		{"n",pack_tp(p_optpar,m_str,0),2,0},
		{0,0,0,0}
		};
	initctx(&ctx,v,argc,argv);
	long arg;
	int t0=0,t1=0;
	char * name=NULL;
	char * dir=NULL;
	while((r=nextarg(&ctx,&arg))!=-1){
		switch(r){
			case 0:
				t0=arg;
				break;
			case 1:
				t1=arg;
				break;
			case 2:
				name=(char *)arg;
				break;
			case 3:
				dir=(char*)arg;
				break;
			default:
				printf("Got %d\n",r);break;
			}
		}
	if(t0==0){
		puts("Missing t0!");return 1;
		}
	if(t1==0){
		puts("Missing t1!");return 1;
		}
	if(dir==NULL){
		puts("Missing directory!");return 1;
		}
	if(name==NULL){
		puts("No name given, defaulting to test.txt");
		name="test.txt";
		}
	printf("t0 + t1: %d\n",t0+t1);
	char * fname=malloc(strlen(name)+strlen(dir)+2);
	sprintf(fname,"%s/%s",dir,name);
	if(access(fname,F_OK)){
		printf("%s doesn't exist.\n",fname);return 1;
		}
	stat(fname,&res);
	pwd=getpwuid(res.st_uid);
	if(!pwd){perror("getpwuid");return 1;}
	char *owner=pwd->pw_name;
	grp=getgrgid(res.st_gid);
	if(!grp){perror("getgrguid");return 1;}
	char *group=grp->gr_name;
	printf("File: %s\nOwner: %s (UID: %d)\nGroup: %s (GID: %d)\n",fname,owner,res.st_uid,group,res.st_gid);
	return 0;
}
