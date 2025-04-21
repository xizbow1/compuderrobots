
#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

typedef struct {
    int width;
    int height;
    int maxColor;
    unsigned char* data;
} PPMImage;

PPMImage* readPPM(const char* filename, int type);

void writePPM(const char *filename, int width, int height, int maxColor, int type, unsigned char *imageData);

void freePPM(PPMImage* img); 

void convertPPMToBW(const char *filename,const char *outfilename); 

void convertJPGToPPM(const char *jpg_filename, const char *ppm_filename);

#endif
