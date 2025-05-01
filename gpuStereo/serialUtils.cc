#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

 int serialPortOpen(){

    int serial_port = open("/dev/ttyACM0", O_RDWR);
    // Replace with your serial port 
    
    if (serial_port == -1) {
        perror("Error opening serial port");
        return -1;
    }

    // Set up the serial port
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(serial_port, &tty) != 0) {
        perror("Error getting current serial port settings");
        return -1;
    }

    // Configure serial port
    cfsetispeed(&tty, B9600); // Set input baud rate to 9600
    cfsetospeed(&tty, B9600); // Set output baud rate to 9600

    // Configure 8N1 (8 data bits, no parity, 1 stop bit)
    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;  // Clear data size
    tty.c_cflag |= CS8;     // 8 data bits
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver and local mode

    // Set input mode (non-canonical)
    tty.c_lflag &= ~ICANON; // Disable canonical mode
    tty.c_lflag &= ~ECHO;   // Disable echo
    tty.c_lflag &= ~ECHOE;  // Disable erasure
    tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
    tty.c_iflag &= ~(ICRNL | INLCR); // Disable conversion of newline characters

    // Set output mode
    tty.c_oflag &= ~OPOST; // Disable output processing

    // Set timeouts
    tty.c_cc[VMIN] = 1;   // Minimum number of characters to read
    tty.c_cc[VTIME] = 5;  // Timeout in deciseconds (0.5 second)

    // Apply the settings
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        perror("Error setting serial port attributes");
        return -1;
    }

  return serial_port;
  }

 // Write data to the serial port
 int serialPortWrite(const char *data, int serial_port){ 
    int n = write(serial_port, data, strlen(data));
    if (n < 0) {
        perror("Error writing to serial port");
        return -1;
    }
    return n;
 }

 // Read data from the serial port
 int serialPortRead(char* buf, int serial_port){
    int n = read(serial_port, buf, sizeof(buf) - 1); // Read data into buffer
    if (n < 0) {
        perror("Error reading from serial port");
        return -1;
    }

    return n;

 }

 int serialPortClose( int serial_port){
	int closeStatus = close(serial_port);
	 return closeStatus;
 }
