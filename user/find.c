#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
//实现 find "某个目录" 查找特定名称的文件

void find(char *path, char *filename){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    fprintf(2, "第一个参数必须是一个目录\n");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..")){
        find(buf, filename);
      }else if (st.type == T_FILE && strcmp(de.name, filename) == 0){
        printf("%s\n", buf);
      }
    }
    break;
  }
  close(fd);    
}

int main(int argc, char *argv[]){
    if (argc < 3){
        fprintf(2, "args numbers error\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}