#include "image_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

ImageType detectImageType(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return -1;
    }

    char magic[3];
    fscanf(file, "%2s", magic);

    fclose(file);

    if (strcmp(magic, "P5") == 0) {
        return PGM;
    } else if (strcmp(magic, "P6") == 0) {
        return PPM;
    } else {
        return -1; // Format non pris en charge
    }
}

// Fonction pour ouvrir une image PGM
GrayImage* openPGM(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier PGM.\n");
        return NULL;
    }

    char magic[3];
    fscanf(file, "%2s\n", magic);

    if (strcmp(magic, "P5") != 0) {
        fprintf(stderr, "Format PGM incorrect.\n");
        fclose(file);
        return NULL;
    }

    int width, height, maxval;
    fscanf(file, "%d %d\n%d\n", &width, &height, &maxval);

    GrayImage* image = (GrayImage*)malloc(sizeof(GrayImage));
    image->width = width;
    image->height = height;
    image->data = (unsigned char*)malloc(width * height * sizeof(unsigned char));

    fread(image->data, sizeof(unsigned char), width * height, file);

    fclose(file);
    return image;
}

// Fonction pour ouvrir une image PPM
ColorImage* openPPM(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier PPM.\n");
        return NULL;
    }

    char magic[3];
    fscanf(file, "%2s\n", magic);

    if (strcmp(magic, "P6") != 0) {
        fprintf(stderr, "Format PPM incorrect.\n");
        fclose(file);
        return NULL;
    }

    int width, height, maxval;
    fscanf(file, "%d %d\n%d\n", &width, &height, &maxval);

    ColorImage* image = (ColorImage*)malloc(sizeof(ColorImage));
    image->width = width;
    image->height = height;

    image->r = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    image->g = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    image->b = (unsigned char*)malloc(width * height * sizeof(unsigned char));

    for (size_t i = 0; i < width * height; i++)
    {
        fread(&image->r[i], sizeof(unsigned char), 1, file);
        fread(&image->g[i], sizeof(unsigned char), 1, file);
        fread(&image->b[i], sizeof(unsigned char), 1, file);
    }

    fclose(file);
    return image;
}

// Fonction pour sauvegarder une image PGM
void savePGM(const char* filename, GrayImage* image) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Erreur lors de la création du fichier PGM.\n");
        return;
    }

    fprintf(file, "P5\n%d %d\n255\n", image->width, image->height);
    fwrite(image->data, sizeof(unsigned char), image->width * image->height, file);

    fclose(file);
}

// Fonction pour sauvegarder une image PPM
void savePPM(const char* filename, ColorImage* image) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Erreur lors de la création du fichier PPM.\n");
        return;
    }

    fprintf(file, "P6\n%d %d\n255\n", image->width, image->height);

    for (size_t i = 0; i < image->width * image->height; i++)
    {
        fwrite(&image->r[i], sizeof(unsigned char), 1, file);
        fwrite(&image->g[i], sizeof(unsigned char), 1, file);
        fwrite(&image->b[i], sizeof(unsigned char), 1, file);
    }

    fclose(file);
}

// Fonction pour appliquer un flou sur une image en niveaux de gris
GrayImage* applyBlur(GrayImage* image, int intensity) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer un flou moyenne pondéré en fonction de l'intensité
    for (int y = 1; y < image->height - 1; ++y) {
        for (int x = 1; x < image->width - 1; ++x) {
            int sum = 0;
            int divisor = (2 * intensity + 1) * (2 * intensity + 1);

            for (int i = -intensity; i <= intensity; ++i) {
                for (int j = -intensity; j <= intensity; ++j) {
                    if((y + i) * image->width + x + j < 0) continue;
                    sum += image->data[(y + i) * image->width + x + j];
                }
            }
            result->data[y * image->width + x] = sum / divisor;
        }
    }

    return result;
}

// Fonction pour appliquer un flou sur une image en couleur
ColorImage* applyBlurColor(ColorImage* image, int intensity) {

    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer un flou moyenne pondéré en fonction de l'intensité pour chaque canal de couleur
    for (int y = 1; y < image->height - 1; ++y) {
        for (int x = 1; x < image->width - 1; ++x) {
            int sumR = 0, sumG = 0, sumB = 0;
            int divisor = (2 * intensity + 1) * (2 * intensity + 1);

            for (int i = -intensity; i <= intensity; ++i) {
                for (int j = -intensity; j <= intensity; ++j) {
                    if ((y + i) * image->width + x + j < 0) continue;
                    sumR += image->r[(y + i) * image->width + x + j];
                    sumG += image->g[(y + i) * image->width + x + j];
                    sumB += image->b[(y + i) * image->width + x + j];
                }
            }
            result->r[y * image->width + x] = sumR / divisor;
            result->g[y * image->width + x] = sumG / divisor;
            result->b[y * image->width + x] = sumB / divisor;
        }
    }

    return result;
}