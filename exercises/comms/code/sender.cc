#define DEBUG
#include <stdio.h>
#include "libcomms.h"

void encode(const char *in, char out[]) {
  char   parity;
  int    in_bit,  out_bit;


  parity = '0';
  out_bit = 0;
  for ( in_bit = 0; in[in_bit] != '\0'; /*null*/) {
    if ( in[in_bit] != '0' && in[in_bit] != '1' ) {
      printf("Bad character (%c) in input\n", in[in_bit]);
      continue;
    }
    
    out[out_bit] = in[in_bit];
    
    if ( in[in_bit] == '1' ) {
      parity = ( parity == '0' ? '1' : '0');  /*toggle parity*/
    }
    
#ifdef DEBUG
    printf("%c", in[in_bit]);
#endif
    
    ++in_bit;
    ++out_bit;
    
    
    if ( (in_bit % 8) == 0 ) {                /*Add Parity*/
#ifdef DEBUG
      printf(" %c\n", parity);
#endif
    
      out[out_bit++] = parity;
      parity = '0';

    }

  } /*for*/
  out[out_bit++] = '\0';
}




int main() {
  char *raw;
  char coded[512];
  int  test;
  
  test = 0;
#ifdef DEBUG
  printf("Master\n");
#endif
  
  connect_transmitter();
  
  while ( NULL != (raw = gen()) ) {
    ++test;
#ifdef DEBUG
    printf("TEST %d\n~~~~\n", test);
#endif
    
#ifdef DEBUG
    printf("Coding\n");
#endif
    encode(raw, coded);
    
    transmit_frame(coded);
    
  }
}


