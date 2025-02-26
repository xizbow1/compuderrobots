#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "imageUtils.h"



PPMImage* readPPM(const char* filename, int type) {

    FILE* file = fopen(filename, "rb");
    int numBytes;

    if (!file) {
        perror("Error opening file");
        exit(0);
    }

    if(type==0) numBytes=1;//grayscale
    else numBytes=3;//color

    char format[3];
    fscanf(file, "%2s", format);

    if (format[0] != 'P' && (format[1] != '6' || format[1]!='5')) {
        fprintf(stderr, "Invalid PPM file format\n");
        fclose(file);
        exit(0);
    }

    PPMImage* img = (PPMImage*)malloc(sizeof(PPMImage));

    if (!img) {
        perror("Error allocating memory");
        fclose(file);
        exit(0);
    }

    fscanf(file, "%d %d", &img->width, &img->height);

    fscanf(file, "%d", &img->maxColor);

    fgetc(file); // Consume newline character

    img->data = (unsigned char*)malloc(numBytes * img->width * img->height);

    if (!img->data) {
        perror("Error allocating memory");
        free(img);
        fclose(file);
        exit(0);
    }

    fread(img->data, numBytes, img->width * img->height, file);
    
    fclose(file);

    return img;
}

void writePPM(const char *filename, int width, int height, int maxColor, int type, unsigned char *imageData) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        exit(0);
    }

    if(type ==0){

      // Write Black and White  PPM header
      fprintf(file, "P5\n%d %d\n%d\n", width, height, maxColor);

      // Write pixel data
      fwrite(imageData, 1, width * height, file);
    }else{

      // Write Color PPM header
      fprintf(file, "P6\n%d %d\n%d\n", width, height, maxColor);

      // Write pixel data
      fwrite(imageData, 3, width * height, file);
}

    fclose(file);
}


void freePPM(PPMImage* img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

void convertPPMToBW(const char *filename,const char *outfilename) {

    PPMImage* image = readPPM(filename,1);

    unsigned char* outImage = (unsigned char*) malloc(image->width * image->height * sizeof(unsigned char));

    if (!image) {
        fprintf(stderr, "Error reading PPM file\n");
        exit(0);
    }

    // Now you can access image properties and data
    printf("Width: %d\n", image->width);
    printf("Height: %d\n", image->height);
    printf("MaxColor: %d\n", image->maxColor);

    for(int i = 0; i < (image->width * image->height); i++) {
	int byteIndex = 3 * i;
	int red = (int) image->data[byteIndex];
	int green = (int) image->data[byteIndex+1];
	int blue = (int) image->data[byteIndex+2];
	float intensity = 0.3 * red + 0.6 * green + 0.1 * blue;
	outImage[i] = (unsigned char) intensity;
    }
    
    writePPM(outfilename, image->width, image->height, image->maxColor,0, outImage);

    // Don't forget to free the memory when done
    freePPM(image);

}

void convertJPGToPPM(const char *jpg_filename, const char *ppm_filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *infile;
    FILE *outfile;
    JSAMPARRAY buffer;
    int row_stride;

    // Open the input JPEG file
    if ((infile = fopen(jpg_filename, "rb")) == NULL) {
        fprintf(stderr, "Error: cannot open input file %s\n", jpg_filename);
        exit(1);
    }

    // Set up error handling
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // Specify the source of the compressed data (the JPEG file)
    jpeg_stdio_src(&cinfo, infile);

    // Read the JPEG header to get image info
    jpeg_read_header(&cinfo, TRUE);

    // Start the decompression process
    jpeg_start_decompress(&cinfo);

    // Set up the output file
    if ((outfile = fopen(ppm_filename, "wb")) == NULL) {
        fprintf(stderr, "Error: cannot open output file %s\n", ppm_filename);
        exit(1);
    }

    // Write the PPM header
    fprintf(outfile, "P6\n%d %d\n255\n", cinfo.output_width, cinfo.output_height);

    // Allocate a buffer to hold one row of pixel data
    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    // Read the image data and write to the PPM file
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        fwrite(buffer[0], 1, row_stride, outfile);
    }

    // Finish the decompression process
    jpeg_finish_decompress(&cinfo);

    // Clean up
    fclose(infile);
    fclose(outfile);
    jpeg_destroy_decompress(&cinfo);
}


