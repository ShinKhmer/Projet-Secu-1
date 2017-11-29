#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 500
#define NUM_MATRIX 4
#include "functions.h"


int main(int argc, char **argv)
{

    FILE* explore = NULL;
    FILE* explore_matrix = NULL;
    FILE* encrypt = NULL;
    char *line = NULL;
    char *text = NULL;
    int *matrix = NULL;
    int *bin = NULL;
    char *hexa = NULL;
    int **sub_messages = NULL;
    char *hexa2 = NULL;
    unsigned char *final_result = NULL;
    int **bin_decrypted = NULL;


    int i = 0;
    int j = 0;
    int choice = 0;
    int size_text = 0;
    int size_text2 = 0;
    int size_matrix = NUM_MATRIX*8;     // A modifier
    int size_initial_matrix = NUM_MATRIX;


    do{
        printf("\n\n==================== APPLICATION DE CRYPTAGE ====================\n\n");

        printf("\n1 - Cryptage\n");
        printf("\n2 - Decryptage\n");
        printf("\n9 - Quitter programme\n");


        choice = 0;
        scanf("%d", &choice);

        switch( choice ){

            line = malloc( sizeof(char) * SIZE );
            line[0] = '\0';                         // init line

            case 1:
                        // SEARCH MATRIX
                        printf("\n==================== MATRICE ====================");

                        explore_matrix = fopen("matrice.txt", "r");
                        matrix = malloc( sizeof(int) * size_matrix );
                        init(matrix, size_matrix);

                        read_matrix( explore_matrix, line, matrix, size_matrix );
                        print_tab_int( matrix, size_matrix, size_matrix / NUM_MATRIX );

                        // FILE POINTER
                        explore = fopen("texte.txt", "r");     // Open file / r+: read and write


                            if( explore != NULL ){

                                line[0] = '\0';                         // init line

                                text = malloc( sizeof(char) * SIZE );
                                text[0] = '\0';


                                hexa = malloc( sizeof(char) * SIZE * 8 / 4 );
                                hexa[0] = '\0';

                                printf("\n==================== FILE CONTENT ====================\n");

                                // FILE RECOVERY
                                read_file( explore, line, text );      // Function read and put all the lines read into one char chain

                                printf("%s", text);
                                size_text = strlen(text);


                                // CONVERT CHAR TO BINARY
                                bin = malloc( sizeof(int) * size_text * 8 );
                                init(bin, size_text * 8);                    // Possibilité d'optimiser le SIZE

                                convert_char_to_bin( text, bin, size_text );
                                printf( "\n\nTaille texte: %d\n\n", strlen(text) );


                                printf("\n==================== BINARY CONTENT ====================\n");

                                // CONVERT BINARY TO HEXA => NOT NECCESSARY
                                convert_bin_to_hexa( bin, hexa, size_text * 2 );

                                print_result( text, bin, hexa, size_text );



                                // CALCULATE SUB MESSAGES

                                printf("\n==================== SUB-MESSAGES BIN ====================\n");

                                sub_messages = malloc( sizeof(int *) * size_text * 2 );
                                for( i = 0; i < size_text * 2; i++ ){
                                    sub_messages[i] = malloc( sizeof(int) * size_matrix / NUM_MATRIX);
                                }

                                init_double( sub_messages, size_text * 2, size_matrix / NUM_MATRIX );

                                calculate_sub_message( bin, matrix, sub_messages, size_text, size_matrix, size_initial_matrix );
                                print_tab_int_double( sub_messages, size_text * 2, size_matrix / NUM_MATRIX );


                                // CONVERT TO HEXA FOR HAVING ASCII CHAR

                                printf("\n==================== SUB-MESSAGES HEXA ====================\n");          // optionnel

                                hexa2 = malloc( sizeof(int) * size_text * 2 * 2 );
                                hexa2[0] = '\0';

                                convert_bin_to_hexa2( sub_messages, hexa2, size_text);

                                print_tab_char( hexa2, size_text * 2 * 2, 2 );


                                printf("\n==================== FINAL MESSAGE ====================\n");
                                final_result = malloc( sizeof(char) * (size_text * 2 + 1) );              // +1 for \0
                                final_result[0] = '\0';

                                final_message( sub_messages, final_result, size_text * 2, size_matrix / NUM_MATRIX );



                                // ECRITURE
                                encrypt = fopen( "texte.txtc", "w" );
                                if( encrypt != NULL ){
                                    fwrite( final_result, sizeof(char), size_text * 2 + 1, encrypt );
                                }else{
                                    printf("Problème lors de l'ouverture de texte.txtc");
                                }


                                // FULL FREE !
                                free(final_result);
                                free(hexa2);
                                for( i = 0; i < (size_text * 2) ; i++ ){
                                    free(sub_messages[i]);
                                }
                                free(sub_messages);

                                free(matrix);
                                free(hexa);
                                free(bin);
                                free(text);

                                fclose(encrypt);
                                fclose(explore_matrix);         // Close file
                                fclose(explore);
                            }
                            else{
                                printf("Impossible d'ouvrir le fichier !");     // text = NULL => print error
                            }


                        break;
            case 2:     // DECRYPTAGE

                        printf("\n\n\n\==================== DECRYPTAGE ====================\n");          // optionnel

                        encrypt = fopen( "texte.txtc", "r" );

                        if( encrypt != NULL ){

                            line[0] = '\0';                         // init line

                            text = malloc( sizeof(char) * SIZE );
                            text[0] = '\0';

                            size_text2 = 0;


                            // READ FILE DECRYPT

                            read_file( encrypt, line, text);

                            size_text2 = strlen(text);



                            printf("\nAffichage du fichier:\n%s\n", text);


                            // DECRYPT => TAKE THE 4 COLUMNS OF THE IDENTITY MATRIX

                            printf("\n==================== TEXTE EN BINAIRE ====================\n");          // optionnel

                            bin = malloc( sizeof(int) * size_text2 * 8 );
                            init(bin, size_text2 * 8);

                            convert_char_to_bin( text, bin, size_text2 );
                            print_tab_int( bin, size_text2 * 8, 8 );

                            bin_decrypted = (int *)malloc( sizeof(int *) * size_text2 * 2 );
                            for( i = 0; i < size_text2 * 2; i++ ){
                                bin_decrypted[i] = (int *)malloc( sizeof(int) * ( size_matrix / NUM_MATRIX ) );
                            }
                            init_double( bin_decrypted, size_text2 * 2, size_matrix / NUM_MATRIX );
                            //matrix_order();

                            printf("\n==================== TEXT  ====================\n");          // optionnel

                            int cnt = 0;

                            // PRINT TEXT DECRYPTED => NEED A FUNCTION
                            for( i = 0; i < size_text2 / 2; i++ ){
                                for( j = 0; j < size_matrix / NUM_MATRIX; j++ ){
                                    if( j < 4 ){
                                        bin_decrypted[i][j] = bin[j+cnt];
                                    }
                                    else{
                                        bin_decrypted[i][j] = bin[j + 4 + cnt];
                                    }
                                }
                                cnt += 16;
                            }

                            printf("ORO\n");
                            print_tab_int_double( bin_decrypted, size_text2 / 2, size_matrix / NUM_MATRIX );


                            final_result = malloc( sizeof(char) * (size_text2 + 1) );              // +1 for \0
                            final_result[0] = '\0';

                            final_message( bin_decrypted, final_result, size_text2 / 2, size_matrix / NUM_MATRIX );


                            free(final_result);
                            for( i = 0; i < size_text2 * 2 ; i++ ){
                                free(bin_decrypted[i]);
                            }
                            free(bin);
                            free(text);
                            fclose(encrypt);
                        }

                        free(line);
                        break;

            case 9:     printf("\nAu revoir !\n");
                        break;

            default:    break;
        }

    }while( choice != 9 );

    return 0;
}



