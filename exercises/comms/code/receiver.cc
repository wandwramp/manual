#include <stdio.h>
#include "libcomms.h"

int check(char *in) {
  char   parity;
  int    bit;
  int    ok;
  
  
  parity = '0';
  ok = 1;
  for ( bit = 0; in[bit] != '\0'; /*null*/) {
   if ( in[bit] != '0' && in[bit] != '1' ) {
     printf("Bad character (%c <%02X>) in input (at position %d)\n", 
	    in[bit], in[bit], bit);
     ++bit;
     continue;
   }
   
   
   
   if ( ((bit+1) % 9) != 0) {
#ifdef DEBUG
     printf("%c", in[bit]);
#endif
     if ( in[bit] == '1' ) {
         parity = ( parity == '0' ? '1' : '0');  /*toggle parity*/
     }
   } else {                                      /*Add Parity*/
#ifdef DEBUG
     printf(" %c (%c)\n", in[bit], parity);
#endif
     if ( in[bit] != parity ) {
       ok = 0;
     }
     parity = '0';
   } /*else*/
   
   ++bit;
   
  } /*for*/
  
  return ok;
}



int main() {
  char coded[512];
  int  ok;
  int  test;
  
  connect_receiver();
  
  test = 0;
  while ( -1 != receive_frame(coded) ) {
    while ( test > 179 && test < 192 ) {
      if ( res(ok = 0) ) {
	printf("%d It is true that there was %s error\n", test, 
	       (ok?"no":"an"));
      } else {
	printf("%d It is not true that there was %s error\n", test, 
	       (ok?"no":"an"));
      }
      ++test;
    }
    
#ifdef DEBUG
    printf("TEST %d\n~~~~\n", test);
#endif
    
#ifdef DEBUG
    printf("\nTesting\n");
#endif
    if ( res(ok = check(coded)) ) {
      printf("%d It is true that there was %s error\n", test, 
	     (ok?"no":"an"));
    } else {
      printf("%d It is not true that there was %s error\n", test, 
	     (ok?"no":"an"));
    }
    
    ++test;
  }
}
