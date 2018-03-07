#include "util.h"

//error handlers
void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}
                                                        
void err_quit(const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  printf(fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}