
/* See http://byerley.cs.waikato.ac.nz/~tonym/201/ass9/node2.html for
   documentation
*/

char *gen();
void transmit_frame(char *data);
int  receive_frame(char *data);
int  res(int ok);
void connect_transmitter();
void connect_receiver();
