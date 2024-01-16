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
    char direction;
    int pixels;
    double contrastIntensity;
    int brightnessDelta;
    unsigned char threshold;

    // Menu principal
    do {
        printf("\nMenu Principal:\n");
        printf("1. Charger une image\n");
        printf("2. Enregistrer une image\n");
        printf("3. Appliquer un effet de flou\n");
        printf("4. Appliquer un effet de miroir\n");
        printf("5. Appliquer une rotation\n");
        printf("6. Appliquer une translation\n");
        printf("7. Appliquer un effet de pixelisation\n");
        printf("8. Appliquer un effet de negatif\n");
        printf("9. Modifier le contraste\n");
        printf("10. Modifier la luminosité\n");
        printf("11. Modifier le seuillage\n");
        printf("12. Quitter\n");
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
                // Translater les pixels
                if (grayOutputImage != NULL) {
                    printf("Entrez la direction de la translation (H/B/G/D) : ");
                    scanf(" %c", &direction);
                    printf("Entrez le nombre de pixels à déplacer : ");
                    scanf("%d", &pixels);
                    grayOutputImage = translateImage(grayOutputImage, direction, pixels);
                    printf("Pixels translater avec succès.\n");
                } else if (colorOutputImage != NULL) {
                    printf("Entrez la direction de la translation (H/B/G/D) : ");
                    scanf(" %c", &direction);
                    printf("Entrez le nombre de pixels à déplacer : ");
                    scanf("%d", &pixels);
                    colorOutputImage = translateImageColor(colorOutputImage, direction, pixels);
                    printf("Pixels translater avec succès.\n");
                } else {
                    fprintf(stderr, "Aucune image chargée pour translater les pixels.\n");
                }
                break;

            case 7:
                if (colorOutputImage != NULL) {
                    int pixelizeIntensity;
                    printf("Entrez l'intensité de pixelisation (1-10) : ");
                    scanf("%d", &pixelizeIntensity);

                    if (pixelizeIntensity >= 1 && pixelizeIntensity <= 10) {
                        colorOutputImage = pixelizeImageColor(colorOutputImage, pixelizeIntensity);
                        printf("Pixelisation couleur appliquée avec succès.\n");
                    } else {
                        fprintf(stderr, "L'intensité de pixelisation doit être comprise entre 1 et 10.\n");
                    }
                } else if (grayOutputImage != NULL) {
                    int pixelizeIntensity;
                    printf("Entrez l'intensité de pixelisation (1-10) : ");
                    scanf("%d", &pixelizeIntensity);

                    if (pixelizeIntensity >= 1 && pixelizeIntensity <= 10) {
                        grayOutputImage = pixelizeImage(grayOutputImage, pixelizeIntensity);
                        printf("Pixelisation noir et blanc appliquée avec succès.\n");
                    } else {
                        fprintf(stderr, "L'intensité de pixelisation doit être comprise entre 1 et 10.\n");
                    }
                } else {
                    fprintf(stderr, "Aucune image chargée pour appliquer la pixelisation.\n");
                }
                break;

            case 8:
                if (grayOutputImage != NULL) {
                    grayOutputImage = applyNegative(grayOutputImage);
                    printf("Effet négatif appliqué avec succès (noir et blanc).\n");
                } else if (colorOutputImage != NULL) {
                    colorOutputImage = applyNegativeColor(colorOutputImage);
                    printf("Effet négatif appliqué avec succès (couleur).\n");
                } else {
                    fprintf(stderr, "Aucune image chargée pour appliquer l'effet négatif.\n");
                }
                break;

            case 9:
                printf("Entrez l'intensité de contraste (0.1-2.0) : ");
                scanf("%lf", &contrastIntensity);

                if (contrastIntensity >= 0.1 && contrastIntensity <= 2.0) {
                    if (grayOutputImage != NULL) {
                        grayOutputImage = adjustContrast(grayOutputImage, contrastIntensity);
                        printf("Contraste ajusté avec succès (PGM).\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = adjustContrastColor(colorOutputImage, contrastIntensity);
                        printf("Contraste ajusté avec succès (PPM).\n");
                    } else {
                        fprintf(stderr, "Aucune image chargée pour ajuster le contraste.\n");
                    }
                } else {
                    fprintf(stderr, "L'intensité de contraste doit être comprise entre 0.1 et 2.0.\n");
                }
                break;

            case 10:
                printf("Entrez la valeur d'ajustement de luminosité (-255 à 255) : ");
                scanf("%d", &brightnessDelta);

                if (brightnessDelta >= -255 && brightnessDelta <= 255) {
                    if (grayOutputImage != NULL) {
                        grayOutputImage = adjustBrightness(grayOutputImage, brightnessDelta);
                        printf("Luminosité ajustée avec succès (PGM).\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = adjustBrightnessColor(colorOutputImage, brightnessDelta);
                        printf("Luminosité ajustée avec succès (PPM).\n");
                    } else {
                        fprintf(stderr, "Aucune image chargée pour ajuster la luminosité.\n");
                    }
                } else {
                    fprintf(stderr, "La valeur d'ajustement de luminosité doit être entre -255 et 255.\n");
                }
                break;

            case 11:
                printf("Entrez la valeur de seuil (0-255) : ");
                scanf("%hhu", &threshold);

                if (threshold <= 255) {
                    if (grayOutputImage != NULL) {
                        grayOutputImage = thresholdGray(grayOutputImage, threshold);
                        printf("Seuillage appliqué avec succès (PGM).\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = thresholdColor(colorOutputImage, threshold);
                        printf("Seuillage appliqué avec succès (PPM).\n");
                    } else {
                        fprintf(stderr, "Aucune image chargée pour appliquer le seuillage.\n");
                    }
                } else {
                    fprintf(stderr, "La valeur de seuil doit être entre 0 et 255.\n");
                }
                break;

            case 12:
                // Quitter le programme
                break;

            default:
                fprintf(stderr, "Choix non valide. Veuillez réessayer.\n");
                break;
        }

    } while (choice != 12);

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
