#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operations.h"

int main() {
    // Image source et destination
    ColorImage* colorSourceImage = NULL;
    GrayImage* graySourceImage = NULL;
    ColorImage* colorOutputImage = NULL;
    GrayImage* grayOutputImage = NULL;

    // Nom des fichiers
    char sourceFileName[100];
    char outputFileName[100];

    int choice;

    // Menu principal
    do {
        printf("\nMenu Principal:\n");
        printf("1. Charger une image\n");
        printf("2. Enregistrer une image\n");
        printf("3. Appliquer un effet de flou\n");
        printf("4. Appliquer un effet de miroir\n");
        printf("5. Appliquer une rotation\n");
        printf("6. Quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Entrez le nom du fichier source : ");
                scanf("%s", sourceFileName);

                // Detecter automatiquement le type d'image (PGM ou PPM)
                if (detectImageType(sourceFileName) == PGM) {
                    graySourceImage = openPGM(sourceFileName);
                    if (graySourceImage == NULL) {
                        fprintf(stderr, "Erreur lors du chargement de l'image.\n");
                    } else {
                        printf("Image PGM chargée avec succès.\n");
                        grayOutputImage = graySourceImage; // Copie de l'image source pour travailler dessus
                        colorSourceImage = NULL; // Réinitialiser l'image couleur
                        colorOutputImage = NULL;
                    }
                } else if (detectImageType(sourceFileName) == PPM) {
                    colorSourceImage = openPPM(sourceFileName);
                    if (colorSourceImage == NULL) {
                        fprintf(stderr, "Erreur lors du chargement de l'image.\n");
                    } else {
                        printf("Image PPM chargée avec succès.\n");
                        colorOutputImage = colorSourceImage; // Copie de l'image source pour travailler dessus
                        graySourceImage = NULL; // Réinitialiser l'image en niveaux de gris
                        grayOutputImage = NULL;
                    }
                } else {
                    fprintf(stderr, "Format d'image non pris en charge.\n");
                }
                break;

            case 2:
                if (grayOutputImage == NULL && colorOutputImage == NULL) {
                    fprintf(stderr, "Aucune image à enregistrer.\n");
                } else {
                    printf("Entrez le nom du fichier de destination : ");
                    scanf("%s", outputFileName);
                    if (grayOutputImage != NULL) {
                        savePGM(outputFileName, grayOutputImage);
                        printf("Image PGM enregistrée avec succès.\n");
                    } else if (colorOutputImage != NULL) {
                        savePPM(outputFileName, colorOutputImage);
                        printf("Image PPM enregistrée avec succès.\n");
                    }
                }
                break;

            case 3:
                if (grayOutputImage != NULL || colorOutputImage != NULL) {
                    int blurIntensity;
                    printf("Entrez l'intensité du flou (1-10) : ");
                    scanf("%d", &blurIntensity);

                    if (blurIntensity < 1 || blurIntensity > 10) {
                        fprintf(stderr, "L'intensité du flou doit être comprise entre 1 et 10.\n");
                    } else {
                        if (grayOutputImage != NULL) {
                            grayOutputImage = applyBlur(grayOutputImage, blurIntensity);
                            printf("Effet de flou appliqué avec succès.\n");
                        } else if (colorOutputImage != NULL) {
                            colorOutputImage = applyBlurColor(colorOutputImage, blurIntensity);
                            printf("Effet de flou appliqué avec succès.\n");
                        }
                    }
                } else {
                    fprintf(stderr, "Aucune image chargée pour appliquer l'effet de flou.\n");
                }
                break;

            case 4:
                if (grayOutputImage != NULL || colorOutputImage != NULL) {
                    int mirrorDirection;
                    printf("Choisissez la direction de l'effet miroir :\n");
                    printf("1. Horizontal\n");
                    printf("2. Vertical\n");
                    printf("Entrez votre choix : ");
                    scanf("%d", &mirrorDirection);

                    if (mirrorDirection < 1 || mirrorDirection > 2) {
                        fprintf(stderr, "Direction non valide pour l'effet miroir.\n");
                    } else {
                        if (grayOutputImage != NULL) {
                            grayOutputImage = applyMirror(grayOutputImage, mirrorDirection);
                            printf("Effet miroir appliqué avec succès.\n");
                        } else if (colorOutputImage != NULL) {
                            colorOutputImage = applyMirrorColor(colorOutputImage, mirrorDirection);
                            printf("Effet miroir appliqué avec succès.\n");
                        }
                    }
                } else {
                    fprintf(stderr, "Aucune image chargée pour appliquer l'effet miroir.\n");
                }
                break;

            case 5:
                if (grayOutputImage != NULL || colorOutputImage != NULL) {
                    double angle;
                    printf("Entrez l'angle de rotation en degrés : ");
                    scanf("%lf", &angle);

                    if (grayOutputImage != NULL) {
                        grayOutputImage = rotateAndResizeImage(grayOutputImage, angle);
                        printf("Rotation avec redimensionnement appliquée avec succès.\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = rotateAndResizeImageColor(colorOutputImage, angle);
                        printf("Rotation avec redimensionnement appliquée avec succès.\n");
                    }
                } else {
                    fprintf(stderr, "Aucune image chargée pour appliquer la rotation avec redimensionnement.\n");
                }
                break;

            case 6:
                // Quitter le programme
                break;

            default:
                fprintf(stderr, "Choix non valide. Veuillez réessayer.\n");
                break;
        }

    } while (choice != 6);

    // Libérer la mémoire
    if (graySourceImage != NULL) {
        free(graySourceImage->data);
        free(graySourceImage);
    }

    if (colorSourceImage != NULL) {
        free(colorSourceImage->r);
        free(colorSourceImage->g);
        free(colorSourceImage->b);
        free(colorSourceImage);
    }

    if (grayOutputImage != NULL) {
        free(grayOutputImage->data);
        free(grayOutputImage);
    }

    if (colorOutputImage != NULL) {
        free(colorOutputImage->r);
        free(colorOutputImage->g);
        free(colorOutputImage->b);
        free(colorOutputImage);
    }

    return 0;
}
