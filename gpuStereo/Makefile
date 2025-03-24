# Compiler
CC =gcc 

# Compiler flags
CCFLAGS =  -I/usr/include/opencv4 
CUFLAGS = -I/usr/include/opencv4 -w 

#Load Libraries
LDLIBS = -I/usr/include/opencv4 -ljpeg -lm -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_highgui -lopencv_imgcodecs -lopencv_calib3d -llapacke -llapack -lblas -lcuda

# Source files
CCSRCS = stereoCuda.cc   
CUSRCS = stereoKernel.cu stereoDepth.cu

# Object files (replace .cc with .o)
CCOBJS = $(CCSRCS:.cc=.o)
CUOBJS = $(CUSRCS:.cu=.o)

# Output executable
TARGET = stereoCuda 

# Rule to build the final executable
$(TARGET): $(CCOBJS) $(CUOBJS)
	nvcc $(CUFLAGS) -o $(TARGET) $(CCOBJS) $(CUOBJS) $(LDLIBS)

# Rule to build cc object files
%.o: %.cc 
	gcc $(CCFLAGS) -c $< -o $@

# Rule to build cu object files
%.o: %.cu 
	nvcc $(CUFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(CCOBJS) $(CUOBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

