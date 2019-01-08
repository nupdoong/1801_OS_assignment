#include <stdio.h>
#include <string.h>
#include "alloc.h"

extern int type;

int main(int argc,char *argv[])
{

  if(argc!=2){
    printf("err\n");
    printf("run with input.txt\n");
    return 1;
  }

  FILE *input;
  if((input=fopen(argv[1],"r"))==NULL){
    printf("err open input file\n");
    return 1;
  }
  char buf[4096];
  char *tok;
  int i=0;

  fgets(buf,sizeof(buf),input);
  tok=strtok(buf," ");
  int line;
  if((line=atoi(tok))==0){
    printf("Format Err\n");
    return 1;
  }
  tok=strtok(NULL," ");
  switch(tok[0]){
    case 'F'://first fit
      type=FIRST;
      break;
    case 'B'://best fit
      type=BEST;
      break;
    case 'W'://worst fit
      type=WORST;
      break;
    default://err
      printf("wrong format\n");
      exit(1);
  }
 
  char **list=(char**)malloc(sizeof(char*)*line);
  int j=0,after_size,tmp;
  for(i=0;i<line;i++){
    memset(buf,'\0',sizeof(buf));
    fgets(buf,sizeof(buf),input);
    buf[strlen(buf)-1]='\0';
    switch(buf[0]){
      case 'm':
      case 's':
        list[j]=m_malloc(strlen(buf+2)+1);
        strcpy(list[j++],buf+2);
	break;
      case 'f':
        tmp=atoi(buf+2);
        m_free(list[tmp]); 
        break;
      case 'r':
        tok=strtok(buf+2," ");
        tmp=atoi(tok);
        tok=strtok(NULL," ");
        after_size=atoi(tok);
        m_realloc(list[tmp],after_size);
        break;
      case 'e':
        tmp=atoi(buf+2);
        list[j++]=m_malloc(tmp);
        break;
    }
  }
  print_list();
  return 0;
}
