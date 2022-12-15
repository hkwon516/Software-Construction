/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.
   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.
   Copyright 2015, 2017, 2020 Paul Eggert
   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <cpuid.h>
#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./options.h"
#include "./output.h"
#include "./rand64-hw.h"
#include "./rand64-sw.h"

/* Main program, which outputs N bytes of random data.  */
int 
main (int argc, char **argv)
{
   /* Check arguments.  */
  struct opts opts; 
  opts.valid = false; 
  parse_arg(argc,argv, &opts);
  if (!opts.valid){
    fprintf (stderr, "%s: incorrect usage \n", argv[0]);
    return 1; 
  }

  /* If there's no work to do, don't worry about which library to use.  */
  if (opts.nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  if (opts.input == RDRAND && rdrand_supported()){
    initialize = hardware_rand64_init;
    initialize();
    rand64 = hardware_rand64;
    finalize = hardware_rand64_fini;
  }
  else if (opts.input == RDRAND && !rdrand_supported())
  {
    fprintf(stderr,"Error\n");
    exit(1);
  }  
  else if (opts.input == SLASH_F){
    software_rand64_file_init(opts.r_src);
    rand64 = software_rand64;
    finalize = software_rand64_fini;
  }
  else if (opts.input == MRAND48_R){
    initialize = software_rand64_init;
    initialize();
    rand64 = hardware_rand64;
    finalize = hardware_rand64_fini;
  }
  else{ //default
    initialize = hardware_rand64_init;
    initialize();
    rand64 = hardware_rand64;
    finalize = hardware_rand64_fini;
  }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  if (opts.output == N) {  //N bytes
    // printf("1\n");
    unsigned long long x = rand64 ();
    // int outbytes = opts.nbytes < wordsize ? opts.nbytes : wordsize;
    int outbytes = opts.nbytes;
    int chunksize = opts.block_size;
    writebyteschunks(x, outbytes, chunksize);

  }
  else { //stdio 
    do
    {
      unsigned long long x = rand64 ();
      int outbytes = opts.nbytes < wordsize ? opts.nbytes : wordsize;
      if (!writebytes (x, outbytes))
      {
        output_errno = errno;
        break;
      }
      opts.nbytes -= outbytes;
    }
    while (0 < opts.nbytes);
  }

  

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}
