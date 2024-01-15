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