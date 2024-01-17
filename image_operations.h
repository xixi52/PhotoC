#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

// Enumeration pour les types d'images
typedef enum {
    PGM, // Niveaux de gris
    PPM  // Couleur
} ImageType;

// Structure pour stocker une image en niveau de gris
typedef struct {
    int width;
    int height;
    unsigned char* data;
} GrayImage;

// Structure pour stocker une image en couleur (PPM)
typedef struct {
    int width;
    int height;
    unsigned char* r;
    unsigned char* g;
    unsigned char* b;
} ColorImage;

GrayImage* openPGM(const char* filename);
void savePGM(const char* filename, GrayImage* image);
GrayImage* applyBlur(GrayImage* image, int intensity);
GrayImage* applyMirror(GrayImage* image, int direction);
GrayImage* rotateAndResizeImage(GrayImage* image, double angle);
GrayImage* translateImage(GrayImage* image, const char direction, int pixels);
GrayImage* pixelizeImage(GrayImage* image, int intensity);
GrayImage* applyNegative(GrayImage* image);
GrayImage* adjustContrast(GrayImage* image, double intensity);
GrayImage* adjustBrightness(GrayImage* image, int delta);
GrayImage* thresholdGray(GrayImage* image, unsigned char threshold);
GrayImage* scaleGray(GrayImage* image, float scale);
void generateHistogramGray(GrayImage* image, const char* outputFileName);

ColorImage* openPPM(const char* filename);
void savePPM(const char* filename, ColorImage* image);
ColorImage* applyBlurColor(ColorImage* image, int intensity);
ColorImage* applyMirrorColor(ColorImage* image, int direction);
ColorImage* rotateAndResizeImageColor(ColorImage* image, double angle);
ColorImage* translateImageColor(ColorImage* image, const char direction, int pixels);
ColorImage* pixelizeImageColor(ColorImage* image, int intensity);
ColorImage* applyNegativeColor(ColorImage* image);
ColorImage* adjustContrastColor(ColorImage* image, double intensity);
ColorImage* adjustBrightnessColor(ColorImage* image, int delta);
ColorImage* thresholdColor(ColorImage* image, unsigned char threshold);
ColorImage* scaleColor(ColorImage* image, float scale);
void generateHistogramRed(ColorImage* image, const char* outputFileName);
void generateHistogramGreen(ColorImage* image, const char* outputFileName);
void generateHistogramBlue(ColorImage* image, const char* outputFileName);

ImageType detectImageType(const char* filename);

#endif // IMAGE_OPERATIONS_H
