LIBDIR=.

CFLAGS=-g -Wall

all: sender receiver

$(LIBDIR)/libcomms.a: lib.cc Makefile
	g++ -c $(CFLAGS) lib.cc
	-rm -f $(LIBDIR)/libcomms.a
	ar -q $(LIBDIR)/libcomms.a lib.o
	ranlib $(LIBDIR)/libcomms.a
	chmod 644 $(LIBDIR)/libcomms.a       


sender:  sender.cc $(LIBDIR)/libcomms.a Makefile
	g++ -o sender $(CFLAGS) sender.cc $(LIBDIR)/libcomms.a


receiver:  receiver.cc $(LIBDIR)/libcomms.a Makefile
	g++ -o receiver $(CFLAGS) receiver.cc $(LIBDIR)/libcomms.a

clean:	
	-rm -f lib.o libcomms.a sender receiver
