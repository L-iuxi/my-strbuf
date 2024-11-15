#ifndef _STRBUF_H
#define _STRBUF_H

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
// 声明结构体
struct strbuf 
{
  int len;    
  int alloc;   
  char *buf;   
};
// 初始化 sb 结构体，容量为 alloc
void strbuf_init(struct strbuf *sb, size_t alloc)
{
  sb->len=0;
  sb->alloc=alloc;   
  
}
// 将字符串填充到 sb 中，长度为 len，容量为 alloc
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
  sb->len=len;
  sb->alloc=alloc;
  sb->buf=(char*)malloc(alloc);
  memcpy(sb->buf,str,len);
}
// 释放 sb 结构体的内存
void strbuf_release(struct strbuf *sb)
{
  free(sb->buf);
return;
}
// 交换两个 strbuf
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
  struct strbuf t;
  t = *a;
  *a = *b;
  *b = t;
}
// 将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
  *sz=sb->alloc;
  char *buf=sb->buf;
  sb->buf=NULL;
  sb->len=0;
  sb->alloc=0;
  return buf;
}
// 比较两个 strbuf 的内存是否相同
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    if(first->alloc!=second->alloc)
    {
        return 0;
    }
    return memcmp(first->buf,second->buf,first->len)==0?1:0;
}
// 清空 sb
void strbuf_reset(struct strbuf *sb)
{
  sb->len=0;
  if(sb->buf!=NULL)
  {
  sb->buf[0]='\0';
  }
  sb->alloc=0;
}
// 确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用。
void strbuf_grow(struct strbuf *sb, size_t extra)
{
   sb->buf=(char*)realloc(sb->buf,sizeof(char)*(sb->len+extra));
  if(sb->buf==NULL)
  {
    return ;
  }
}

// 向 sb 追加长度为 len 的数据 data。
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    memcpy(sb->buf+sb->len,data,len);
    sb->len+=len;
}

// 向 sb 追加一个字符 c。
void strbuf_addch(struct strbuf *sb, int c)
{
    sb->len++;
    sb->buf[sb->len]=(char)c;
   
}

// 向 sb 追加一个字符串 s。
void strbuf_addstr(struct strbuf *sb, const char *s)
{
  
    int h=strlen(s);
    sb->buf=(char*)realloc(sb->buf,sb->len+h);
    if(sb->buf==NULL)
  {
    return;
  }
    memcpy(sb->buf+sb->len,s,h+1);
    sb->len+=h;
   
}

// 向一个 sb 追加另一个 strbuf 的数据。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
    sb->len+=sb2->len;
}

// 设置 sb 的长度 len。
void strbuf_setlen(struct strbuf *sb, size_t len)
{
  if(len<sb->len)
  {
    sb->len=len;
  }
  else
  {
    sb->buf=(char*)realloc(sb->buf,len);
    if(sb->buf==NULL)
  {
    return;
  }
    sb->len=len;
    sb->alloc=len;
  }

}
// 计算 sb 目前仍可以向后追加的字符串长度。
size_t strbuf_avail(const struct strbuf *sb)
{
  return sb->alloc-sb->len-1;
}

// 向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
  strbuf_grow(sb,len);
  memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);
  size_t j=pos;
  char *a=(char*)data;
  for(size_t i=0;i<len;i++)
  {
    sb->buf[j]=*a;
    j++;
    a++;
  }
}

// 去除 sb 缓冲区左端的所有空格、制表符和'\t'字符。
void strbuf_rtrim(struct strbuf *sb)
{
  {
  int i=0;
  while(i<sb->len && isspace(sb->buf[i]))
  {
    i++;
  }
  if(i>0)
  {
    memmove(sb->buf,sb->buf+i,sb->len-i);
    sb->len-=i;
  }
}
}
// 去除 sb 缓冲区右端的所有空格、制表符和'\t'字符。
void strbuf_ltrim(struct strbuf *sb)
{
  {
  int i=sb->len-1;
  while(i>=0 && isspace(sb->buf[i]))
  {
    i--;
  }
  if(i<sb->len)
  {
    memmove(sb->buf,sb->buf+i,sb->len-i);
    sb->len=sb->len-i-1;
  }
}
}

// 删除 sb 缓冲区从 pos 坐标开始长度为 len 的内容。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
  size_t end=pos+len;
  if(end>sb->len)
  {
    end=sb->len;
  }
  memmove(sb->buf+pos,sb->buf+end,sb->len-pos);
  sb->len-=end-pos;
  sb->buf[sb->len]='\0';
}

// 将文件描述符为 fd 的所有文件内容追加到 sb 中。sb 增长 hint ? hint : 8192 大小
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
  sb->buf=(char*)realloc(sb->buf,(hint ? hint : 8192));
  size_t total;
  size_t every;
  while(every=(read(fd,sb->buf,hint))>0)
  {
    total+=every;
    strbuf_add(sb, sb->buf, every);
  }
  
  return total;

}
// 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb。
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
  char c;
  size_t len=0;
  
  while(c=fgetc(fp)!=EOF)
  {
    if(c=='\n')
    {
      break;
    }
    sb->buf[len]=c;
    len++;
  }
  sb->buf[len]='\0';
  return (c==EOF && len==0)?-1:len;
}

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
  return 0;
}
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
{
  return 0;
}
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
{
  return 0;
}
#endif


int main() {
  
  return 0;
}