#include <stdio.h>
#include <stdbool.h>
#include <limits.h> //for CHAR_BIT 
#include <stdlib.h>
#include <string.h>
#include "./output.h"

bool
writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar (x) < 0)
	return false;
      x >>= CHAR_BIT; //num of bits in char
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}

bool writebyteschunks (unsigned long long x, int nbytes, int chunkSize){
  char* lines = (char *)malloc(nbytes); //make a memory of chunkSize
  memcpy(lines, &x, nbytes); //copy long long bits into a dynamically allocated memory 
  
  int startPoint = 0;
  
  while (nbytes > 0){
    if (chunkSize <= nbytes){
      write(1, lines + startPoint, chunkSize);
      startPoint += chunkSize;
      nbytes -= chunkSize;
    }
    else{
      write(1, lines + startPoint, nbytes);
      startPoint += nbytes;
      nbytes -= nbytes;
    }
  }

  return true;
}
