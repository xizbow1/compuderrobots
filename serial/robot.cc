#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include "serialUtils.h"

//motor commands are strings with the following structure. The
//STR186 steer to 186 degrees 0 degrees = hard left 180 degress hard right 90 degrees straight
//RFW128 right forward 128. 255 full speed 0 zero speed
//LBK255 left backwards full speed
//etc

int main() {
     const int cmdLength = 7;
     char cmd[cmdLength];
     int portID;
     int bytesWritten;
//     int bytesRead;

    portID = serialPortOpen();
    if(portID<0){
	    printf("Error opening serial port \n");
	    exit(0);
    } 

  for(int loop=0;loop<5;loop++){ 

    // Reading a string with spaces
        printf("Enter a motor command: ");
        fgets(cmd, sizeof(cmd), stdin);
    
     // Output the value
        printf("You entered: %s", cmd);// Write data to the serial port
    
	bytesWritten = serialPortWrite(cmd,portID);
        if (bytesWritten>0)
        printf("Sent %d bytes: %s\n", bytesWritten, cmd);
  }

    /*
      // Set the buffer to zero and read data from the serial port
    memset(buf, 0, sizeof(buf));
    bytesRead = serialPortRead(buf,portID);
    if(bytesRead>0)
	    printf("Received %d bytes: %s\n", bytesRead, buf);
   */

    // Close the serial port
    if(serialPortClose(portID)<0){
	    printf("Could not close serial port \n");
	    exit(0);
    } else {
	    printf("Serial port closed \n");
    }

    return 0;

}

