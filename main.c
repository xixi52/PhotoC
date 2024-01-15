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
        printf("3. Quitter\n");
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
                // Quitter le programme
                break;

            default:
                fprintf(stderr, "Choix non valide. Veuillez réessayer.\n");
                break;
        }

    } while (choice != 7);

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
