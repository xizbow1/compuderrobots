#ifndef SERIAL_H
#define SERIAL_H


using namespace std;

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

 int serialPortOpen();
 int serialPortWrite(const char *data, int serial_port); 
 int serialPortRead(char* buf, int serial_port);
 int serialPortClose( int serial_port);

#endif

