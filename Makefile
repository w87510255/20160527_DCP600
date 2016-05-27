Q_OBJECT= main.o C_AudioDcp600.o UdpTransport.o C_Device.o
DCP600: ${Q_OBJECT}
	$(CC) $(CFLAGS) $^ -o $@ 
main.o:  C_AudioDcp600.h 
	g++ ${CFLAGS} -c main.cpp
C_AudioDcp600.o: UdpTransport.h  C_AudioDevice.h
	g++ ${CFLAGS} -c C_AudioDcp600.cpp
UdpTransport.o:  UdpTransport.h 
	g++ ${CFLAGS} -c UdpTransport.cpp
C_Device.o: head.h C_Device.h
	g++ ${CFLAGS} -c C_Device.cpp

Q_OBJECT= main.o C_AudioDcp600.o UdpTransport.o C_Device.o
CFLAGS=-g 
CC=g++

clean: 
	rm -f *.o DCP600











