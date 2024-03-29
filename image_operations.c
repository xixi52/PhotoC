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
        fprintf(stderr, "Erreur lors de la creation du fichier PGM.\n");
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
        fprintf(stderr, "Erreur lors de la creation du fichier PPM.\n");
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

    // Appliquer un flou moyenne pondere en fonction de l'intensite
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

    // Appliquer un flou moyenne pondere en fonction de l'intensite pour chaque canal de couleur
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

// Fonction pour appliquer un effet miroir sur une image en niveaux de gris
GrayImage* applyMirror(GrayImage* image, int direction) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    if (direction == 1) { // Miroir horizontal
        for (int y = 0; y < image->height; ++y) {
            for (int x = 0; x < image->width; ++x) {
                result->data[y * image->width + x] = image->data[y * image->width + (image->width - 1 - x)];
            }
        }
    } else if (direction == 2) { // Miroir vertical
        for (int y = 0; y < image->height; ++y) {
            for (int x = 0; x < image->width; ++x) {
                result->data[y * image->width + x] = image->data[(image->height - 1 - y) * image->width + x];
            }
        }
    } else {
        fprintf(stderr, "Direction non valide pour l'effet miroir.\n");
        free(result->data);
        free(result);
        return NULL;
    }

    return result;
}

// Fonction pour appliquer un effet miroir sur une image en couleur
ColorImage* applyMirrorColor(ColorImage* image, int direction) {
    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    if (direction == 1) { // Miroir horizontal
        for (int y = 0; y < image->height; ++y) {
            for (int x = 0; x < image->width; ++x) {
                result->r[y * image->width + x] = image->r[y * image->width + (image->width - 1 - x)];
                result->g[y * image->width + x] = image->g[y * image->width + (image->width - 1 - x)];
                result->b[y * image->width + x] = image->b[y * image->width + (image->width - 1 - x)];
            }
        }
    } else if (direction == 2) { // Miroir vertical
        for (int y = 0; y < image->height; ++y) {
            for (int x = 0; x < image->width; ++x) {
                result->r[y * image->width + x] = image->r[(image->height - 1 - y) * image->width + x];
                result->g[y * image->width + x] = image->g[(image->height - 1 - y) * image->width + x];
                result->b[y * image->width + x] = image->b[(image->height - 1 - y) * image->width + x];
            }
        }
    } else {
        fprintf(stderr, "Direction non valide pour l'effet miroir.\n");
        free(result->r);
        free(result->g);
        free(result->b);
        free(result);
        return NULL;
    }

    return result;
}

// Fonction pour appliquer un effet de rotation sur une image en niveaux de gris
GrayImage* rotateAndResizeImage(GrayImage* image, double angle) {
    double radians = angle * M_PI / 180.0;
    int newWidth = (int)ceil(fabs(cos(radians)) * image->width + fabs(sin(radians)) * image->height);
    int newHeight = (int)ceil(fabs(sin(radians)) * image->width + fabs(cos(radians)) * image->height);

    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = newWidth;
    result->height = newHeight;
    result->data = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));

    int centerXOld = image->width / 2;
    int centerYOld = image->height / 2;

    int centerXNew = newWidth / 2;
    int centerYNew = newHeight / 2;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            double srcX = cos(radians) * (x - centerXNew) + sin(radians) * (y - centerYNew) + centerXOld;
            double srcY = -sin(radians) * (x - centerXNew) + cos(radians) * (y - centerYNew) + centerYOld;

            if (srcX >= 0 && srcX < image->width - 1 && srcY >= 0 && srcY < image->height - 1) {
                int x0 = (int)floor(srcX);
                int y0 = (int)floor(srcY);
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                double alpha = srcX - x0;
                double beta = srcY - y0;

                // Verifie si les indices ne depassent pas les dimensions de l'image source
                if (x0 >= 0 && x0 < image->width - 1 && y0 >= 0 && y0 < image->height - 1 &&
                    x1 >= 0 && x1 < image->width && y1 >= 0 && y1 < image->height) {
                    double interpolatedValue = (1 - alpha) * (1 - beta) * image->data[y0 * image->width + x0] +
                                               alpha * (1 - beta) * image->data[y0 * image->width + x1] +
                                               (1 - alpha) * beta * image->data[y1 * image->width + x0] +
                                               alpha * beta * image->data[y1 * image->width + x1];

                    result->data[y * newWidth + x] = (unsigned char)interpolatedValue;
                } else {
                    result->data[y * newWidth + x] = 0; // Fond noir (0)
                }
            } else {
                result->data[y * newWidth + x] = 0; // Fond noir (0)
            }
        }
    }

    return result;
}

// Fonction pour appliquer un effet de rotation sur une image en couleur
ColorImage* rotateAndResizeImageColor(ColorImage* image, double angle) {
    double radians = angle * M_PI / 180.0;
    int newWidth = (int)ceil(fabs(cos(radians)) * image->width + fabs(sin(radians)) * image->height);
    int newHeight = (int)ceil(fabs(sin(radians)) * image->width + fabs(cos(radians)) * image->height);

    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = newWidth;
    result->height = newHeight;
    result->r = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));

    int centerXOld = image->width / 2;
    int centerYOld = image->height / 2;

    int centerXNew = newWidth / 2;
    int centerYNew = newHeight / 2;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            double srcX = cos(radians) * (x - centerXNew) + sin(radians) * (y - centerYNew) + centerXOld;
            double srcY = -sin(radians) * (x - centerXNew) + cos(radians) * (y - centerYNew) + centerYOld;

            if (srcX >= 0 && srcX < image->width - 1 && srcY >= 0 && srcY < image->height - 1) {
                int x0 = (int)floor(srcX);
                int y0 = (int)floor(srcY);
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                double alpha = srcX - x0;
                double beta = srcY - y0;

                // Verifie si les indices ne depassent pas les dimensions de l'image source
                if (x0 >= 0 && x0 < image->width - 1 && y0 >= 0 && y0 < image->height - 1 &&
                    x1 >= 0 && x1 < image->width && y1 >= 0 && y1 < image->height) {
                    double interpolatedR = (1 - alpha) * (1 - beta) * image->r[y0 * image->width + x0] +
                                           alpha * (1 - beta) * image->r[y0 * image->width + x1] +
                                           (1 - alpha) * beta * image->r[y1 * image->width + x0] +
                                           alpha * beta * image->r[y1 * image->width + x1];

                    double interpolatedG = (1 - alpha) * (1 - beta) * image->g[y0 * image->width + x0] +
                                           alpha * (1 - beta) * image->g[y0 * image->width + x1] +
                                           (1 - alpha) * beta * image->g[y1 * image->width + x0] +
                                           alpha * beta * image->g[y1 * image->width + x1];

                    double interpolatedB = (1 - alpha) * (1 - beta) * image->b[y0 * image->width + x0] +
                                           alpha * (1 - beta) * image->b[y0 * image->width + x1] +
                                           (1 - alpha) * beta * image->b[y1 * image->width + x0] +
                                           alpha * beta * image->b[y1 * image->width + x1];

                    result->r[y * newWidth + x] = (unsigned char)interpolatedR;
                    result->g[y * newWidth + x] = (unsigned char)interpolatedG;
                    result->b[y * newWidth + x] = (unsigned char)interpolatedB;
                } else {
                    result->r[y * newWidth + x] = 0; // Fond noir (0)
                    result->g[y * newWidth + x] = 0;
                    result->b[y * newWidth + x] = 0;
                }
            } else {
                result->r[y * newWidth + x] = 0; // Fond noir (0)
                result->g[y * newWidth + x] = 0;
                result->b[y * newWidth + x] = 0;
            }
        }
    }

    return result;
}

// Fonction pour appliquer un effet de translation sur une image en niveaux de gris
GrayImage* translateImage(GrayImage* image, const char direction, int pixels) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    memcpy(result->data, image->data, image->width * image->height);

    int size = image->width * image->height;
    int step = pixels % image->height;

    switch (direction) {
        case 'H': // Haut
            for (int i = 0; i < size; ++i) {
                result->data[i] = image->data[(i + step * image->width) % size];
            }
            break;
        case 'B': // Bas
            for (int i = 0; i < size; ++i) {
                result->data[i] = image->data[(i - step * image->width + size) % size];
            }
            break;
        case 'G': // Gauche
            for (int y = 0; y < image->height; ++y) {
                for (int x = 0; x < image->width; ++x) {
                    result->data[y * image->width + x] = image->data[y * image->width + (x + step) % image->width];
                }
            }
            break;
        case 'D': // Droite
            for (int y = 0; y < image->height; ++y) {
                for (int x = 0; x < image->width; ++x) {
                    result->data[y * image->width + x] = image->data[y * image->width + (x - step + image->width) % image->width];
                }
            }
            break;
        default:
            fprintf(stderr, "Direction non valide.\n");
            free(result->data);
            free(result);
            return NULL;
    }

    return result;
}

// Fonction pour appliquer un effet de translation sur une image en couleur
ColorImage* translateImageColor(ColorImage* image, const char direction, int pixels) {
    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    memcpy(result->r, image->r, image->width * image->height);
    memcpy(result->g, image->g, image->width * image->height);
    memcpy(result->b, image->b, image->width * image->height);

    int size = image->width * image->height;
    int step = pixels % image->height;

    switch (direction) {
        case 'H': // Haut
            for (int i = 0; i < size; ++i) {
                result->r[i] = image->r[(i + step * image->width) % size];
                result->g[i] = image->g[(i + step * image->width) % size];
                result->b[i] = image->b[(i + step * image->width) % size];
            }
            break;
        case 'B': // Bas
            for (int i = 0; i < size; ++i) {
                result->r[i] = image->r[(i - step * image->width + size) % size];
                result->g[i] = image->g[(i - step * image->width + size) % size];
                result->b[i] = image->b[(i - step * image->width + size) % size];
            }
            break;
        case 'G': // Gauche
            for (int y = 0; y < image->height; ++y) {
                for (int x = 0; x < image->width; ++x) {
                    result->r[y * image->width + x] = image->r[y * image->width + (x + step) % image->width];
                    result->g[y * image->width + x] = image->g[y * image->width + (x + step) % image->width];
                    result->b[y * image->width + x] = image->b[y * image->width + (x + step) % image->width];
                }
            }
            break;
        case 'D': // Droite
            for (int y = 0; y < image->height; ++y) {
                for (int x = 0; x < image->width; ++x) {
                    result->r[y * image->width + x] = image->r[y * image->width + (x - step + image->width) % image->width];
                    result->g[y * image->width + x] = image->g[y * image->width + (x - step + image->width) % image->width];
                    result->b[y * image->width + x] = image->b[y * image->width + (x - step + image->width) % image->width];
                }
            }
            break;
        default:
            fprintf(stderr, "Direction non valide.\n");
            free(result->r);
            free(result->g);
            free(result->b);
            free(result);
            return NULL;
    }

    return result;
}

// Fonction pour appliquer un effet de pixelisation sur une image en niveaux de gris
GrayImage* pixelizeImage(GrayImage* image, int intensity) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    int blockSize = intensity;

    for (int y = 0; y < image->height; y += blockSize) {
        for (int x = 0; x < image->width; x += blockSize) {
            int sum = 0;
            int count = 0;

            for (int i = 0; i < blockSize && y + i < image->height; ++i) {
                for (int j = 0; j < blockSize && x + j < image->width; ++j) {
                    sum += image->data[(y + i) * image->width + x + j];
                    ++count;
                }
            }

            int average = (count > 0) ? sum / count : 0;

            for (int i = 0; i < blockSize && y + i < image->height; ++i) {
                for (int j = 0; j < blockSize && x + j < image->width; ++j) {
                    result->data[(y + i) * image->width + x + j] = average;
                }
            }
        }
    }

    return result;
}

// Fonction pour appliquer un effet de pixelisation sur une image en couleur
ColorImage* pixelizeImageColor(ColorImage* image, int intensity) {
    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    int blockSize = intensity;

    for (int y = 0; y < image->height; y += blockSize) {
        for (int x = 0; x < image->width; x += blockSize) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            for (int i = 0; i < blockSize && y + i < image->height; ++i) {
                for (int j = 0; j < blockSize && x + j < image->width; ++j) {
                    sumR += image->r[(y + i) * image->width + x + j];
                    sumG += image->g[(y + i) * image->width + x + j];
                    sumB += image->b[(y + i) * image->width + x + j];
                    ++count;
                }
            }

            int averageR = (count > 0) ? sumR / count : 0;
            int averageG = (count > 0) ? sumG / count : 0;
            int averageB = (count > 0) ? sumB / count : 0;

            for (int i = 0; i < blockSize && y + i < image->height; ++i) {
                for (int j = 0; j < blockSize && x + j < image->width; ++j) {
                    result->r[(y + i) * image->width + x + j] = averageR;
                    result->g[(y + i) * image->width + x + j] = averageG;
                    result->b[(y + i) * image->width + x + j] = averageB;
                }
            }
        }
    }

    return result;
}

// Fonction pour appliquer un effet de negatif sur une image en niveaux de gris
GrayImage* applyNegative(GrayImage* image) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    for (int i = 0; i < image->width * image->height; ++i) {
        result->data[i] = 255 - image->data[i];
    }

    return result;
}

// Fonction pour appliquer un effet de negatif sur une image en couleur
ColorImage* applyNegativeColor(ColorImage* image) {
    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    for (int i = 0; i < image->width * image->height; ++i) {
        result->r[i] = 255 - image->r[i];
        result->g[i] = 255 - image->g[i];
        result->b[i] = 255 - image->b[i];
    }

    return result;
}

// Fonction pour ajuster le contraste d'une image en niveaux de gris
GrayImage* adjustContrast(GrayImage* image, double intensity) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer l'ajustement de contraste pixel par pixel
    for (int i = 0; i < image->width * image->height; ++i) {
        double adjustedValue = (image->data[i] - 128) * intensity + 128;
        result->data[i] = (unsigned char)fmin(fmax(adjustedValue, 0), 255);
    }

    return result;
}

// Fonction pour ajuster le contraste d'une image en couleur
ColorImage* adjustContrastColor(ColorImage* image, double intensity) {
    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer l'ajustement de contraste canal par canal
    for (int i = 0; i < image->width * image->height; ++i) {
        double adjustedR = (image->r[i] - 128) * intensity + 128;
        double adjustedG = (image->g[i] - 128) * intensity + 128;
        double adjustedB = (image->b[i] - 128) * intensity + 128;

        result->r[i] = (unsigned char)fmin(fmax(adjustedR, 0), 255);
        result->g[i] = (unsigned char)fmin(fmax(adjustedG, 0), 255);
        result->b[i] = (unsigned char)fmin(fmax(adjustedB, 0), 255);
    }

    return result;
}

// Fonction pour ajuster la luminosite d'une image en niveaux de gris
GrayImage* adjustBrightness(GrayImage* image, int delta) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer l'ajustement de luminosite pixel par pixel
    for (int i = 0; i < image->width * image->height; ++i) {
        int adjustedValue = image->data[i] + delta;
        result->data[i] = (unsigned char)fmin(fmax(adjustedValue, 0), 255);
    }

    return result;
}

// Fonction pour ajuster la luminosite d'une image en couleur
ColorImage* adjustBrightnessColor(ColorImage* image, int delta) {
    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer l'ajustement de luminosite canal par canal
    for (int i = 0; i < image->width * image->height; ++i) {
        int adjustedR = image->r[i] + delta;
        int adjustedG = image->g[i] + delta;
        int adjustedB = image->b[i] + delta;

        result->r[i] = (unsigned char)fmin(fmax(adjustedR, 0), 255);
        result->g[i] = (unsigned char)fmin(fmax(adjustedG, 0), 255);
        result->b[i] = (unsigned char)fmin(fmax(adjustedB, 0), 255);
    }

    return result;
}

// Fonction pour seuiller une image en niveaux de gris
GrayImage* thresholdGray(GrayImage* image, unsigned char threshold) {
    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = image->width;
    result->height = image->height;
    result->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer le seuillage pixel par pixel
    for (int i = 0; i < image->width * image->height; ++i) {
        result->data[i] = (image->data[i] < threshold) ? 0 : 255;
    }

    return result;
}

// Fonction pour seuiller une image en couleur
ColorImage* thresholdColor(ColorImage* image, unsigned char threshold) {
    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = image->width;
    result->height = image->height;
    result->r = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));

    // Appliquer le seuillage canal par canal
    for (int i = 0; i < image->width * image->height; ++i) {
        result->r[i] = (image->r[i] < threshold) ? 0 : 255;
        result->g[i] = (image->g[i] < threshold) ? 0 : 255;
        result->b[i] = (image->b[i] < threshold) ? 0 : 255;
    }

    return result;
}

// Fonction pour mettre a l'echelle une image en niveaux de gris
GrayImage* scaleGray(GrayImage* image, float scale) {
    int newWidth = (int)(image->width * scale);
    int newHeight = (int)(image->height * scale);

    GrayImage* result = (GrayImage*)malloc(sizeof(GrayImage));
    result->width = newWidth;
    result->height = newHeight;
    result->data = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // Coordonnees dans l'image d'origine
            float srcX = x / scale;
            float srcY = y / scale;

            // Coordonnees des pixels voisins
            int x1 = (int)srcX;
            int y1 = (int)srcY;
            int x2 = fmin(x1 + 1, image->width - 1);
            int y2 = fmin(y1 + 1, image->height - 1);

            // Coefficients pour l'interpolation bilineaire
            float dx = srcX - x1;
            float dy = srcY - y1;

            // Interpolation bilineaire
            float interpolatedValue = (1 - dx) * (1 - dy) * image->data[y1 * image->width + x1] +
                                      dx * (1 - dy) * image->data[y1 * image->width + x2] +
                                      (1 - dx) * dy * image->data[y2 * image->width + x1] +
                                      dx * dy * image->data[y2 * image->width + x2];

            result->data[y * newWidth + x] = (unsigned char)interpolatedValue;
        }
    }

    return result;
}

// Fonction pour mettre a l'echelle une image en couleur
ColorImage* scaleColor(ColorImage* image, float scale) {
    int newWidth = (int)(image->width * scale);
    int newHeight = (int)(image->height * scale);

    ColorImage* result = (ColorImage*)malloc(sizeof(ColorImage));
    result->width = newWidth;
    result->height = newHeight;
    result->r = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));
    result->g = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));
    result->b = (unsigned char*)malloc(newWidth * newHeight * sizeof(unsigned char));

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // Coordonnees dans l'image d'origine
            float srcX = x / scale;
            float srcY = y / scale;

            // Coordonnees des pixels voisins
            int x1 = (int)srcX;
            int y1 = (int)srcY;
            int x2 = fmin(x1 + 1, image->width - 1);
            int y2 = fmin(y1 + 1, image->height - 1);

            // Coefficients pour l'interpolation bilineaire
            float dx = srcX - x1;
            float dy = srcY - y1;

            // Interpolation bilineaire pour chaque canal de couleur
            float interpolatedR = (1 - dx) * (1 - dy) * image->r[y1 * image->width + x1] +
                                  dx * (1 - dy) * image->r[y1 * image->width + x2] +
                                  (1 - dx) * dy * image->r[y2 * image->width + x1] +
                                  dx * dy * image->r[y2 * image->width + x2];

            float interpolatedG = (1 - dx) * (1 - dy) * image->g[y1 * image->width + x1] +
                                  dx * (1 - dy) * image->g[y1 * image->width + x2] +
                                  (1 - dx) * dy * image->g[y2 * image->width + x1] +
                                  dx * dy * image->g[y2 * image->width + x2];

            float interpolatedB = (1 - dx) * (1 - dy) * image->b[y1 * image->width + x1] +
                                  dx * (1 - dy) * image->b[y1 * image->width + x2] +
                                  (1 - dx) * dy * image->b[y2 * image->width + x1] +
                                  dx * dy * image->b[y2 * image->width + x2];

            result->r[y * newWidth + x] = (unsigned char)interpolatedR;
            result->g[y * newWidth + x] = (unsigned char)interpolatedG;
            result->b[y * newWidth + x] = (unsigned char)interpolatedB;
        }
    }

    return result;
}

// Fonction pour generer un histogramme en noir et blanc et sauvegarder en fichier PGM
void generateHistogramGray(GrayImage* image, const char* outputFileName) {
    FILE* file = fopen(outputFileName, "w");
    if (!file) {
        fprintf(stderr, "Erreur lors de la creation du fichier PGM pour l'histogramme.\n");
        return;
    }

    fprintf(file, "P2\n");
    fprintf(file, "256 256\n"); // Taille de l'histogramme
    fprintf(file, "255\n"); // Valeur maximale d'intensite

    // Calculer l'histogramme
    int pixels[256] = {0};
    for (int i = 0; i < image->width * image->height; ++i) {
        pixels[image->data[i]]++;
    }

    // Trouver la valeur maximale de l'histogramme
    int maxFrequency = 0;
    for (int i = 0; i < 256; ++i) {
        if (pixels[i] > maxFrequency) {
            maxFrequency = pixels[i];
        }
    }

    // Normaliser et ecrire l'histogramme dans le fichier PGM
    for (int j = 0; j < 256; ++j) {
        for (int i = 0; i <= 255; ++i) {
            int normalizedHeight = (pixels[i] * 255) / maxFrequency;
            if (j >= 255 - normalizedHeight) {
                fprintf(file, "0 ");  // attribuer 0 pour les pixels representant l'histogramme
            } else {
                fprintf(file, "255 ");  // attribuer 255 pour les pixels qui ne representent pas l'histogramme
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Histogramme en noir et blanc genere avec succes.\n");
}

// Fonction pour generer un histogramme rouge en ppm
void generateHistogramRed(ColorImage* image, const char* outputFileName) {
    char outputFileNameRed[256];  // ou une taille appropriee
    snprintf(outputFileNameRed, sizeof(outputFileNameRed), "%s_red.ppm", outputFileName);

    FILE* file = fopen(outputFileNameRed, "w");
    if (!file) {
        fprintf(stderr, "Erreur lors de la creation du fichier PPM pour l'histogramme rouge.\n");
        return;
    }

    fprintf(file, "P3\n");
    fprintf(file, "256 256\n"); // Taille de l'histogramme
    fprintf(file, "255\n"); // Valeur maximale d'intensite

    // Calculer l'histogramme rouge
    int pixels[256] = {0};
    for (int i = 0; i < image->width * image->height; ++i) {
        pixels[image->r[i]]++;
    }

    // Trouver la valeur maximale de l'histogramme
    int maxFrequency = 0;
    for (int i = 0; i < 256; ++i) {
        if (pixels[i] > maxFrequency) {
            maxFrequency = pixels[i];
        }
    }

    // Normaliser et ecrire l'histogramme dans le fichier PPM
    for (int j = 0; j < 256; ++j) {
        for (int i = 0; i <= 255; ++i) {
            int normalizedHeight = (pixels[i] * 255) / maxFrequency;
            if (j >= 255 - normalizedHeight) {
                fprintf(file, "255 0 0 ");  // Rouge
            } else {
                fprintf(file, "255 255 255 ");  // Fond blanc
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Histogramme rouge genere avec succes.\n");
}

// Fonction pour generer un histogramme bleu en ppm
void generateHistogramBlue(ColorImage* image, const char* outputFileName) {
    char outputFileNameBlue[256];
    snprintf(outputFileNameBlue, sizeof(outputFileNameBlue), "%s_blue.ppm", outputFileName);

    FILE* file = fopen(outputFileNameBlue, "w");
    if (!file) {
        fprintf(stderr, "Erreur lors de la creation du fichier PPM pour l'histogramme bleu.\n");
        return;
    }

    fprintf(file, "P3\n");
    fprintf(file, "256 256\n"); // Taille de l'histogramme
    fprintf(file, "255\n"); // Valeur maximale d'intensite

    // Calculer l'histogramme bleu
    int pixels[256] = {0};
    for (int i = 0; i < image->width * image->height; ++i) {
        pixels[image->b[i]]++;
    }

    // Trouver la valeur maximale de l'histogramme
    int maxFrequency = 0;
    for (int i = 0; i < 256; ++i) {
        if (pixels[i] > maxFrequency) {
            maxFrequency = pixels[i];
        }
    }

    // Normaliser et ecrire l'histogramme dans le fichier PPM
    for (int j = 0; j < 256; ++j) {
        for (int i = 0; i <= 255; ++i) {
            int normalizedHeight = (pixels[i] * 255) / maxFrequency;
            if (j >= 255 - normalizedHeight) {
                fprintf(file, "0 0 255 ");  // Bleu
            } else {
                fprintf(file, "255 255 255 ");  // Fond blanc
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Histogramme bleu genere avec succes.\n");
}

// Fonction pour generer un histogramme vert en ppm
void generateHistogramGreen(ColorImage* image, const char* outputFileName) {
    char outputFileNameGreen[256];
    snprintf(outputFileNameGreen, sizeof(outputFileNameGreen), "%s_green.ppm", outputFileName);

    FILE* file = fopen(outputFileNameGreen, "w");
    if (!file) {
        fprintf(stderr, "Erreur lors de la creation du fichier PPM pour l'histogramme vert.\n");
        return;
    }

    fprintf(file, "P3\n");
    fprintf(file, "256 256\n"); // Taille de l'histogramme
    fprintf(file, "255\n"); // Valeur maximale d'intensite

    // Calculer l'histogramme vert
    int pixels[256] = {0};
    for (int i = 0; i < image->width * image->height; ++i) {
        pixels[image->g[i]]++;
    }

    // Trouver la valeur maximale de l'histogramme
    int maxFrequency = 0;
    for (int i = 0; i < 256; ++i) {
        if (pixels[i] > maxFrequency) {
            maxFrequency = pixels[i];
        }
    }

    // Normaliser et ecrire l'histogramme dans le fichier PPM
    for (int j = 0; j < 256; ++j) {
        for (int i = 0; i <= 255; ++i) {
            int normalizedHeight = (pixels[i] * 255) / maxFrequency;
            if (j >= 255 - normalizedHeight) {
                fprintf(file, "0 255 0 ");  // Vert
            } else {
                fprintf(file, "255 255 255 ");  // Fond blanc
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Histogramme vert genere avec succes.\n");
}




