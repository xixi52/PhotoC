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
        printf("10. Modifier la luminosite\n");
        printf("11. Modifier le seuillage\n");
        printf("12. Modifier l'echelle\n");
        printf("13. Quitter\n");
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
                        printf("Image PGM chargee avec succes.\n");
                        grayOutputImage = graySourceImage; // Copie de l'image source pour travailler dessus
                        colorSourceImage = NULL; // Reinitialiser l'image couleur
                        colorOutputImage = NULL;
                    }
                } else if (detectImageType(sourceFileName) == PPM) {
                    colorSourceImage = openPPM(sourceFileName);
                    if (colorSourceImage == NULL) {
                        fprintf(stderr, "Erreur lors du chargement de l'image.\n");
                    } else {
                        printf("Image PPM chargee avec succes.\n");
                        colorOutputImage = colorSourceImage; // Copie de l'image source pour travailler dessus
                        graySourceImage = NULL; // Reinitialiser l'image en niveaux de gris
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
                        printf("Image PGM enregistree avec succes.\n");
                    } else if (colorOutputImage != NULL) {
                        savePPM(outputFileName, colorOutputImage);
                        printf("Image PPM enregistree avec succes.\n");
                    }
                }
                break;

            case 3:
                if (grayOutputImage != NULL || colorOutputImage != NULL) {
                    int blurIntensity;
                    printf("Entrez l'intensite du flou (1-10) : ");
                    scanf("%d", &blurIntensity);

                    if (blurIntensity < 1 || blurIntensity > 10) {
                        fprintf(stderr, "L'intensite du flou doit etre comprise entre 1 et 10.\n");
                    } else {
                        if (grayOutputImage != NULL) {
                            grayOutputImage = applyBlur(grayOutputImage, blurIntensity);
                            printf("Effet de flou applique avec succes.\n");
                        } else if (colorOutputImage != NULL) {
                            colorOutputImage = applyBlurColor(colorOutputImage, blurIntensity);
                            printf("Effet de flou applique avec succes.\n");
                        }
                    }
                } else {
                    fprintf(stderr, "Aucune image chargee pour appliquer l'effet de flou.\n");
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
                            printf("Effet miroir applique avec succes.\n");
                        } else if (colorOutputImage != NULL) {
                            colorOutputImage = applyMirrorColor(colorOutputImage, mirrorDirection);
                            printf("Effet miroir applique avec succes.\n");
                        }
                    }
                } else {
                    fprintf(stderr, "Aucune image chargee pour appliquer l'effet miroir.\n");
                }
                break;

            case 5:
                if (grayOutputImage != NULL || colorOutputImage != NULL) {
                    double angle;
                    printf("Entrez l'angle de rotation en degres : ");
                    scanf("%lf", &angle);

                    if (grayOutputImage != NULL) {
                        grayOutputImage = rotateAndResizeImage(grayOutputImage, angle);
                        printf("Rotation avec redimensionnement appliquee avec succes.\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = rotateAndResizeImageColor(colorOutputImage, angle);
                        printf("Rotation avec redimensionnement appliquee avec succes.\n");
                    }
                } else {
                    fprintf(stderr, "Aucune image chargee pour appliquer la rotation avec redimensionnement.\n");
                }
                break;

            case 6:
                // Translater les pixels
                if (grayOutputImage != NULL) {
                    printf("Entrez la direction de la translation (H/B/G/D) : ");
                    scanf(" %c", &direction);
                    printf("Entrez le nombre de pixels à deplacer : ");
                    scanf("%d", &pixels);
                    grayOutputImage = translateImage(grayOutputImage, direction, pixels);
                    printf("Pixels translater avec succes.\n");
                } else if (colorOutputImage != NULL) {
                    printf("Entrez la direction de la translation (H/B/G/D) : ");
                    scanf(" %c", &direction);
                    printf("Entrez le nombre de pixels à deplacer : ");
                    scanf("%d", &pixels);
                    colorOutputImage = translateImageColor(colorOutputImage, direction, pixels);
                    printf("Pixels translater avec succes.\n");
                } else {
                    fprintf(stderr, "Aucune image chargee pour translater les pixels.\n");
                }
                break;

            case 7:
                if (colorOutputImage != NULL) {
                    int pixelizeIntensity;
                    printf("Entrez l'intensite de pixelisation (1-10) : ");
                    scanf("%d", &pixelizeIntensity);

                    if (pixelizeIntensity >= 1 && pixelizeIntensity <= 10) {
                        colorOutputImage = pixelizeImageColor(colorOutputImage, pixelizeIntensity);
                        printf("Pixelisation couleur appliquee avec succes.\n");
                    } else {
                        fprintf(stderr, "L'intensite de pixelisation doit etre comprise entre 1 et 10.\n");
                    }
                } else if (grayOutputImage != NULL) {
                    int pixelizeIntensity;
                    printf("Entrez l'intensite de pixelisation (1-10) : ");
                    scanf("%d", &pixelizeIntensity);

                    if (pixelizeIntensity >= 1 && pixelizeIntensity <= 10) {
                        grayOutputImage = pixelizeImage(grayOutputImage, pixelizeIntensity);
                        printf("Pixelisation noir et blanc appliquee avec succes.\n");
                    } else {
                        fprintf(stderr, "L'intensite de pixelisation doit etre comprise entre 1 et 10.\n");
                    }
                } else {
                    fprintf(stderr, "Aucune image chargee pour appliquer la pixelisation.\n");
                }
                break;

            case 8:
                if (grayOutputImage != NULL) {
                    grayOutputImage = applyNegative(grayOutputImage);
                    printf("Effet negatif applique avec succes (noir et blanc).\n");
                } else if (colorOutputImage != NULL) {
                    colorOutputImage = applyNegativeColor(colorOutputImage);
                    printf("Effet negatif applique avec succes (couleur).\n");
                } else {
                    fprintf(stderr, "Aucune image chargee pour appliquer l'effet negatif.\n");
                }
                break;

            case 9:
                printf("Entrez l'intensite de contraste (0.1-2.0) : ");
                scanf("%lf", &contrastIntensity);

                if (contrastIntensity >= 0.1 && contrastIntensity <= 2.0) {
                    if (grayOutputImage != NULL) {
                        grayOutputImage = adjustContrast(grayOutputImage, contrastIntensity);
                        printf("Contraste ajuste avec succes (PGM).\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = adjustContrastColor(colorOutputImage, contrastIntensity);
                        printf("Contraste ajuste avec succes (PPM).\n");
                    } else {
                        fprintf(stderr, "Aucune image chargee pour ajuster le contraste.\n");
                    }
                } else {
                    fprintf(stderr, "L'intensite de contraste doit etre comprise entre 0.1 et 2.0.\n");
                }
                break;

            case 10:
                printf("Entrez la valeur d'ajustement de luminosite (-255 à 255) : ");
                scanf("%d", &brightnessDelta);

                if (brightnessDelta >= -255 && brightnessDelta <= 255) {
                    if (grayOutputImage != NULL) {
                        grayOutputImage = adjustBrightness(grayOutputImage, brightnessDelta);
                        printf("Luminosite ajustee avec succes (PGM).\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = adjustBrightnessColor(colorOutputImage, brightnessDelta);
                        printf("Luminosite ajustee avec succes (PPM).\n");
                    } else {
                        fprintf(stderr, "Aucune image chargee pour ajuster la luminosite.\n");
                    }
                } else {
                    fprintf(stderr, "La valeur d'ajustement de luminosite doit etre entre -255 et 255.\n");
                }
                break;

            case 11:
                printf("Entrez la valeur de seuil (0-255) : ");
                scanf("%hhu", &threshold);

                if (threshold <= 255 && threshold >= 0) {
                    if (grayOutputImage != NULL) {
                        grayOutputImage = thresholdGray(grayOutputImage, threshold);
                        printf("Seuillage applique avec succes (PGM).\n");
                    } else if (colorOutputImage != NULL) {
                        colorOutputImage = thresholdColor(colorOutputImage, threshold);
                        printf("Seuillage applique avec succes (PPM).\n");
                    } else {
                        fprintf(stderr, "Aucune image chargee pour appliquer le seuillage.\n");
                    }
                } else {
                    fprintf(stderr, "La valeur de seuil doit etre entre 0 et 255.\n");
                }
                break;

            case 12:
                // Mise à l'echelle de l'image
                if (grayOutputImage != NULL || colorOutputImage != NULL) {
                    float scale;
                    printf("Entrez le facteur d'agrandissement (entre 1.0 et 5.0) ou de dezoom (entre 0.1 et 1.0) : ");
                    scanf("%f", &scale);

                    if ((scale >= 1.0 && scale <= 5.0) || (scale >= 0.1 && scale < 1.0)) {
                        if (grayOutputImage != NULL) {
                            GrayImage* scaledImage = scaleGray(grayOutputImage, scale);
                            free(grayOutputImage->data);
                            free(grayOutputImage);
                            grayOutputImage = scaledImage;
                        } else if (colorOutputImage != NULL) {
                            ColorImage* scaledImage = scaleColor(colorOutputImage, scale);
                            free(colorOutputImage->r);
                            free(colorOutputImage->g);
                            free(colorOutputImage->b);
                            free(colorOutputImage);
                            colorOutputImage = scaledImage;
                        }

                        printf("Image mise à l'echelle avec succes.\n");
                    } else {
                        fprintf(stderr, "Erreur : Le facteur de mise à l'echelle doit etre entre 1.0 et 5.0 pour l'agrandissement ou entre 0.1 et 1.0 pour le dezoom.\n");
                    }
                } else {
                    fprintf(stderr, "Aucune image chargee pour la mise à l'echelle.\n");
                }
                break;

            case 13:
                // Quitter le programme
                break;

            default:
                fprintf(stderr, "Choix non valide. Veuillez reessayer.\n");
                break;
        }

    } while (choice != 13);

    // Liberer la memoire
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
