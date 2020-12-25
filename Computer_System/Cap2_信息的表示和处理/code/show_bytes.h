#ifndef SHOW_BYTES
#define SHOW_BYTES 
#include <stdio.h>
typedef unsigned char * byte_pointer; // 将数据类型byte_pointer定义为一个指向unsigned char的指针

void show_bytes(byte_pointer start, size_t len){  // 输入一个字节序列的地址，打印出每个16进制表示的字节
  size_t t;
  for (int i = 0; i < len; ++i)
  {
    printf(" %.2x", start[i]);
  }
  printf("\n");
}

void show_int(int x){
  show_bytes((byte_pointer) &x, sizeof(int));
}
void show_float(float x){
  show_bytes((byte_pointer) &x, sizeof(float));
}
void show_pointer(void *x){
  show_bytes((byte_pointer) &x, sizeof(void *));
}
#endif