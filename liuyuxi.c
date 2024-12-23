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
  sb->buf=(char*)malloc(alloc*(sizeof(char)));
  
}
// 将字符串填充到 sb 中，长度为 len，容量为 alloc
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
 // strbuf_init(sb,alloc);
  sb->len =len;
  sb->alloc=alloc;

  // if(alloc<len+1)
  // {

  //   return;
  // }
//  free(sb->buf);
 // sb->buf=(char*)realloc(sb->buf,alloc);
  //memcpy(sb->buf,str,len+1);
 sb->buf=(char*)str;
  //sb->buf[len+1]='\0';
  

}
  
// 释放 sb 结构体的内存
void strbuf_release(struct strbuf *sb)
{
  free(sb->buf);
  sb->buf=NULL;
  sb->len=0;
  sb->alloc=0;

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
  
  //size_t t=0;
  if(first->len!=second->len)
  {
    return 1;
  }
   if(first->alloc!=second->alloc)
    {
        return 0;
    }

    else
    {
    return memcmp(first->buf,second->buf,first->alloc)==0?0:1;
    }
    //else
    //{
     // for(int i=0;i<first->alloc;i++)
     // {
      // if(first->buf[i]==second->buf[i])
      //  {
       //   t++;
       // }
      //}
      //if(t==first->alloc)
      //{
      //  return 1;
      //}
     
    //}
}
// 清空 sb
void strbuf_reset(struct strbuf *sb)
{
  //memset(sb,0X10,sizeof(*sb));
// if(sb->buf!=NULL)
 //{
 sb->len=0;
 sb->alloc=16;
 sb->buf=(char*)realloc(sb->buf,sb->alloc);
  
 //}
 //free(sb->buf);
}
// 确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用。
void strbuf_grow(struct strbuf *sb, size_t extra)
{
   sb->buf=(char*)realloc(sb->buf,2*(sb->len+extra));
   sb->alloc=2*(sb->len+extra);
   if (sb->buf == NULL) 
    {
      free(sb);
      return; 
    }
//  free(sb->buf);
}
//sb加入长度为len的数据data
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
  
if (len == 0) 
return; 

strbuf_grow(sb, sb->len + len + 1); 
memcpy(sb->buf + sb->len, data, len);
sb->alloc=20;
sb->len += len;
sb->buf[sb->len] = '\0';
}





// 向 sb 追加一个字符 c。
void strbuf_addch(struct strbuf *sb, int c)
{
  if(sb->alloc<=sb->len+1)
  {
  sb->alloc++;
  }
  sb->buf=(char*)realloc(sb->buf,sb->alloc);
  if(sb->buf==NULL)
  {
   return ;
  }
  
    
    sb->buf[sb->len]=(char)c;
    sb->len++;
    sb->buf[sb->len]='\0';
   
//free(sb->buf);
}

   


// 向 sb 追加一个字符串 s。
void strbuf_addstr(struct strbuf *sb, const char *s)
{
  
    int h=strlen(s);
    if(sb->alloc<=h+sb->len)
    {
      sb->alloc=sb->alloc+h;
    }
    sb->buf=(char*)realloc(sb->buf,sb->alloc+1);
    if(sb->buf==NULL)
  {
    return;
  }memcpy(sb->buf+sb->len,s,h);
    sb->len+=h;
    sb->buf[sb->len]='\0';
    //free(sb->buf);
   
}

// 向一个 sb 追加另一个 strbuf 的数据。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
  if(sb->alloc<=sb->len+sb2->len)
  {
    sb->alloc=sb->len+sb2->len+1;
    sb->buf=(char*)realloc(sb->buf,sb->alloc);
    if(sb->buf==NULL)
    {
      return;
    }
  }
  
    sb->buf=(char*)realloc(sb->buf,sb->alloc);
    if(sb->buf==NULL)
    {
     return;
    }
    //memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
    for(size_t i=0;i<sb2->len;i++)
    {
      sb->buf[sb->len+i]=sb2->buf[i];
    }
    sb->len+=sb2->len;
    sb->buf[sb->len]='\0';
 // free(sb->buf); 

}



// 设置 sb 的长度 len。
void strbuf_setlen(struct strbuf *sb, size_t len)
{
     sb->len = len;
    if(sb->len < sb->alloc)
    sb->buf[sb->len] = '\0';
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
    sb->buf[j++]=*a++;
    
  }
   sb->len = sb->len + len;
    sb->buf[sb->len] = '\0';
}

// 去除 sb 缓冲区右端的所有空格、制表符和'\t'字符。
void strbuf_ltrim(struct strbuf *sb)
{
  {
  int i=0;
  while(i>=0 && isspace(sb->buf[i]))
  {
    i++;
  }
  if(i>0)
  {
    memmove(sb->buf,sb->buf+i,sb->len-i);
    sb->len-=i;
  }
}
sb->buf[sb->len]='\0';
}

// 去除 sb 缓冲区左端的所有空格、制表符和'\t'字符。
void strbuf_rtrim(struct strbuf *sb)
{
  size_t i = sb->len - 1;
    while(isspace((unsigned char)sb->buf[i-1]) && i > 0)
    i--;
    if(i < sb->len)
    {
        sb->buf[i] = '\0';
        sb->len = i;
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
{ size_t add = hint?hint:8192;
    char *aim = (char*)malloc(add);
    if(aim == NULL)
    return -1;
    size_t total = 0;
    while(1)
    {
        ssize_t read1 = read(fd,aim,add);
        if(read1 < 0)
        {
            free(aim);
            return -1;
        }
        else if(read1 == 0)
        break;
        else
        {
            if(sb->len + read1 > sb->alloc)
            {
                //size_t alloc1 = sb->len + add;
                size_t alloc1 = (sb->len + add) * 2;
                sb->buf = (char*)realloc(sb->buf,alloc1); 
                if(sb->buf == NULL)
                {
                    free(aim);
                    return -1;
                }
                sb->alloc = alloc1;
            }
        }
        memcpy(sb->buf + sb->len,aim,read1);
        sb->len = sb->len + read1;
        total = total + read1;
    }
    sb->buf[sb->len] = '\0';
    free(aim);
    return total;
}
// 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb。
int strbuf_getline(struct strbuf *sb, FILE *fp)
 
{
strbuf_grow(sb,1024);
char* sd =(char*)malloc(1024);
  if(fgets(sd,1024,fp) == NULL)
    return 0;
    size_t sz = strlen(sd);
    if(sz > 0 && sd[sz-1] == '\n')
    {
      sd[sz-1] = '\0';
    }
    if(sb->buf[sb->len] == '\0' && sb->len > 1)
    {
      sb->len-=1;
    }
    for(int i = 0; i < sz; i++)
    {
        sb->buf[sb->len] = sd[i];
        sb->len++;
    }
    
    sb->buf[sb->len] = '\0';
    free(sd);
    return 0;
 }

//实现字符串切割（C 系字符串函数的一个痛点）。
// /**
// * @brief 将指定长度的字符串按切割字符切割成多个 strbuf
// *
// * @param str 要切割的字符串
// * @param len 字符串的长度
// * @param terminator 切割字符
// * @param max 最大切割数量（可选）
// * @return struct strbuf** 指向 struct strbuf 的指针数组，数组的最后一个元素为 NULL
// *
// * @note 函数将字符串 str 根据切割字符 terminator 切割成多个 strbuf，并返回结果。可选参数 max 用于限定最大切割数量。
// */
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
  if(len == 0 || str == NULL)
    {
      return NULL;
    }
  //if(max<=0)
  //{
   // max=len+1;
  //}
  struct  strbuf **result=(struct strbuf**)malloc(sizeof(struct strbuf*)*(max+1));
  if(result==NULL)
  {
    return NULL;
  }
  int t=0;
  size_t start=0;
  //size_t new_len;
  
    for(int i=0;i<len;i++)
    {
      if(str[i]==(char)terminator || i==len-1)
      {
        if(str[start] == terminator)
            {
                start++;
                continue;
            }
        //new_len=i-start;
        struct strbuf *new_str=(struct strbuf*)malloc(sizeof(struct strbuf));
        if(new_str==NULL)
        { 
         return NULL;
        }
        if(i==len-1)
        {
          new_str->len=len-start;
          new_str->alloc=new_str->len+1;
        }
        else
        {
        new_str->len=i-start;
        new_str->alloc=new_str->len+1;
        }
        new_str->buf=(char*)malloc(new_str->len+1);
        //new_str->len=i-start;
       // new_str->alloc=i-start+1;
        if(new_str->buf==NULL)
        {
          
          return NULL;
        }
        memcpy(new_str->buf,str+start,new_str->len);
        new_str->buf[new_str->len]='\0';
        result[t]=new_str;
        t++;
        start=i+1;
        if(t == max && max > 0)
            {
                start = i + 1;
                break;

            }
           // t++;
      }
    }
    result[t]=NULL;
    //result[max+1]=NULL;
      return result;
        
        
    

}

  
//实现判断一个 strbuf 是否以指定字符串开头的功能（C 系字符串函数的另一个痛点）。
// /**
// * @brief 判断目标字符串是否以指定前缀开头
// *
// * @param target_str 目标字符串
// * @param str 前缀字符串
// * @param strnlen target_str 的长度
// * @return bool 前缀相同返回 true，否则返回 false
// */
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)

{

if(strnlen == 0)
{
  return true;
}
if (target_str == NULL || str == NULL ) {
return false;
}
  int h = 0;
    while (str[h] != '\0') {
        h++;
    }

    if (h > strnlen) {
        return false;
    }

    for (int i = 0; i < h; i++) {
        if (target_str[i] != str[i]) {
            return false;
        }
    }

 
    return true;
}


//获取字符串从坐标 [begin, end) 的所有内容（可以分成引用和拷贝两个模式） 。
// /**
// * @brief 获取目标字符串的指定子串
// *
// * @param target_buf 目标字符串
// * @param begin 开始下标（包含）
// * @param end 结束下标（不包含）
// * @param len target_buf 的长度
// * @return char* 指向获取的子串的指针，如果参数不合法则返回 NULL
// *
// * @note 下标从0开始，[begin, end)表示左闭右开区间
// */
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
{
  int t=0;
  if(target_buf==NULL||begin<0||end>len||begin>end)
  {
    return NULL;
  }
  size_t h=end-begin;
 char *sb=(char*)malloc(h+1);
 if(begin == end)
    {
        sb[0] = '\0';
    }
  if(sb==NULL)
  {
    return NULL;
  }
  for(int i=begin;i<end;i++)
  {
    sb[t]=target_buf[i];
    t++;
  }
  sb[t]='\0';
  return sb;
}
#endif



