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
  
  // int startPoint =  nbytes < chunkSize ? nbytes : chunkSize;
  int startPoint = 0;
  // printf("%s", startPoint);
  // fflush(stdout);
  // write(1, lines, startPoint);
  
  // printf("hello1\n");
  while (nbytes > 0){
    // printf("hello2\n");
    if (chunkSize <= nbytes){
      // printf("hello3\n");
      write(1, lines + startPoint, chunkSize);
      // printf("hello4\n");
      startPoint += chunkSize;
      nbytes -= chunkSize;
    }
    else{
      // printf("hello5\n");
      write(1, lines + startPoint, nbytes);
      // printf("hello6\n");
      startPoint += nbytes;
      nbytes -= nbytes;
    }
  }

  return true;
}


    // if (write(1, lines, sizeof(x)) == -1){
    //   return false; //on error, write returns -1
    // }

    // if (startPoint == 0){
    //   memcpy(lines, &x, sizeof(x)); 
    // }
    // else {
    //   memcpy(lines + sizeof(nbytes), &x, sizeof(x));
    // }



    // do{
  //   if (chunkSize < nbytes){
  //     if (startPoint==0){
  //       printf("1\n");
  //       write(1, lines, chunkSize);
  //     }
  //     else {
  //       printf("2\n");
  //       write(1, lines + startPoint, chunkSize);
  //     }
  //     nbytes -= chunkSize; 
  //     startPoint += chunkSize;
  //   }
    // else {
    //   if (startPoint==0){
    //     write(1, lines, nbytes);
    //   }
    //   else {
    //     write(1, lines + startPoint, nbytes);
    //   }
    //   nbytes -= nbytes; 
    //   startPoint += nbytes;
    // }
  // }
  // while(0 <= nbytes);