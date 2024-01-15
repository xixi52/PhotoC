#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

// Enumération pour les types d'images
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

ColorImage* openPPM(const char* filename);
void savePPM(const char* filename, ColorImage* image);
ColorImage* applyBlurColor(ColorImage* image, int intensity);

ImageType detectImageType(const char* filename);

#endif // IMAGE_OPERATIONS_H