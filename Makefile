# Compiler
CC = g++ 

# Compiler flags
CFLAGS = -Wall 

# Source files
SRCS = readData.cc readParams.cc utils.cc matrixUtils.cc calib.cc

# Object files (replace .cc with .o)
OBJS = $(SRCS:.cc=.o)

# Output executable
TARGET = calib

# Rule to build the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

