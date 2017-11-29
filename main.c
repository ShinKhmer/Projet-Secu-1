#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// VEUILLEZ MODIFIER LA TAILLE EN OCTET
#define SIZE 50000000  // 5 MO
#define NUM_MATRIX 4
// VEUILLEZ MODIFIER S'IL VOUS PLAIT LES FICHIERS PRESENTS DANS LE DOSSIER
#define MATRICE "Fichiers/matrice.txt"
#define TEXT "Fichiers/texte.txt"
#define TEXT_ENCRYPTED "Fichiers/texte.txtc"
#define TEXT_DECRYPTED "Fichiers/texte_decrypted.txt"
#include "functions.h"


int main(int argc, char **argv)
{

    FILE* explore = NULL;
    FILE* explore_matrix = NULL;
    FILE* encrypt = NULL;
    FILE* decrypt = NULL;
    char *line = NULL;
    char *text = NULL;
    int *matrix = NULL;
    int *index = NULL;
    int *bin = NULL;
    char *hexa = NULL;
    int **sub_messages = NULL;
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
        printf("\n===================================================\n");
        printf("============= APPLICATION DE CRYPTAGE =============\n");
        printf("===================================================\n");

        printf("\n1 - Cryptage\n");
        printf("\n2 - Decryptage\n");
        printf("\n9 - Quitter programme\n");

        printf("\nVotre choix:\n");
        choice = 0;
        scanf("%d", &choice);

        switch( choice ){

            case 1:
                        // SEARCH MATRIX
                        printf("\n\n==================== MATRICE ====================\n");

                        line = malloc( sizeof(char) * SIZE );
                        line[0] = '\0';                         // init line

                        explore_matrix = fopen(MATRICE, "r");
                        matrix = malloc( sizeof(int) * size_matrix );
                        init(matrix, size_matrix);

                        read_matrix( explore_matrix, line, matrix, size_matrix );
                        print_tab_int( matrix, size_matrix, size_matrix / NUM_MATRIX );

                        // FILE POINTER
                        explore = fopen(TEXT, "r");     // Open file / r+: read and write


                            if( explore != NULL ){
                                line[0] = '\0';

                                text = malloc( sizeof(char) * SIZE );
                                text[0] = '\0';


                                hexa = malloc( sizeof(char) * SIZE * 8 / 4 );
                                hexa[0] = '\0';

                                printf("\n\n============== CONTENU DU FICHIER ===============\n");

                                // FILE RECOVERY
                                read_file( explore, line, text );      // Function read and put all the lines read into one char chain

                                //printf("%s", text);
                                size_text = strlen(text);


                                // CONVERT CHAR TO BINARY
                                bin = malloc( sizeof(int) * size_text * 8 );
                                init(bin, size_text * 8);                    // Possibilité d'optimiser le SIZE

                                convert_char_to_bin( text, bin, size_text );
                                printf("\nLecture du fichier %s", TEXT);
                                printf( "\n\nTaille texte: %d\n\n", strlen(text) );


                                //printf("\n\n============= CARACTERES EN BINAIRE =============\n");

                                // CONVERT BINARY TO HEXA => NOT NECCESSARY
                                convert_bin_to_hexa( bin, hexa, size_text * 2 );

                                // Uncomment line 108 for having more informations
                                //print_result( text, bin, hexa, size_text );



                                // CALCULATE SUB MESSAGES   => Uncomment line 109 and 119 for having more informations

                                //printf("\n\n==================== SOUS-MESSAGES ====================\n");

                                sub_messages = malloc( sizeof(int *) * size_text * 2 );
                                for( i = 0; i < size_text * 2; i++ ){
                                    sub_messages[i] = malloc( sizeof(int) * size_matrix / NUM_MATRIX);
                                }

                                init_double( sub_messages, size_text * 2, size_matrix / NUM_MATRIX );

                                calculate_sub_message( bin, matrix, sub_messages, size_text, size_matrix, size_initial_matrix );
                                //print_tab_int_double( sub_messages, size_text * 2, size_matrix / NUM_MATRIX );



                                //printf("\n\n================== MESSAGE FINAL ==================\n");
                                final_result = malloc( sizeof(char) * (size_text * 2 + 1) );              // +1 for \0
                                final_result[0] = '\0';

                                final_message( sub_messages, final_result, size_text * 2, size_matrix / NUM_MATRIX );
                                printf("\n\n");


                                // ECRITURE
                                encrypt = fopen( TEXT_ENCRYPTED, "w" );
                                if( encrypt != NULL ){
                                    fwrite( final_result, sizeof(char), size_text * 2 + 1, encrypt );

                                    fprint("\nLe fichier %s a bien ete cree\n", TEXT_ENCRYPTED);
                                }else{
                                    printf("\nProblème lors de l'ouverture de texte.txtc\n");
                                }



                                // FULL FREE !
                                free(final_result);
                                for( i = 0; i < (size_text * 2) ; i++ ){
                                    free(sub_messages[i]);
                                }
                                free(sub_messages);
                                free(matrix);
                                free(hexa);
                                free(bin);
                                free(text);
                                free(line);

                                fclose(encrypt);
                                fclose(explore_matrix);         // Close file
                                fclose(explore);
                            }
                            else{
                                printf("\nImpossible d'ouvrir le fichier !\n");     // text = NULL => print error
                            }


                        break;
            case 2:     // DECRYPTAGE
                        printf("\n====================================================\n");
                        printf("==================== DECRYPTAGE ====================\n");
                        printf("====================================================\n");


                        // SEARCH MATRIX
                        printf("\n\n==================== MATRICE ====================\n");

                        explore_matrix = fopen(MATRICE, "r");

                        matrix = malloc( sizeof(int) * size_matrix );
                        init(matrix, size_matrix);

                        index = malloc( sizeof(int) * size_initial_matrix );
                        init( index, size_initial_matrix );

                        line = malloc( sizeof(char) * SIZE );
                        line[0] = '\0';

                        read_matrix( explore_matrix, line, matrix, size_matrix );
                        print_tab_int( matrix, size_matrix, size_matrix / NUM_MATRIX );

                        matrix_order( matrix, index, size_matrix / NUM_MATRIX, size_initial_matrix);


                        // open file encrypted
                        encrypt = fopen( TEXT_ENCRYPTED, "r" );

                        if( encrypt != NULL ){

                            line[0] = '\0';                         // init line

                            text = malloc( sizeof(char) * SIZE );
                            text[0] = '\0';

                            size_text2 = 0;


                            // READ FILE DECRYPT

                            read_file( encrypt, line, text);

                            size_text2 = strlen(text);


                            //printf("\nAffichage du fichier:\n%s\n", text);



                            // printf("\n\n==================== TEXTE EN BINAIRE ====================\n");          // optionnel

                            bin = malloc( sizeof(int) * size_text2 * 8 );
                            init(bin, size_text2 * 8);

                            convert_char_to_bin( text, bin, size_text2 );
                            // Uncomment line 216 for more informations
                            //print_tab_int( bin, size_text2 * 8, 8 );


                            printf("\n\n===================== TEXTE  =====================\n");          // optionnel


                            // DECRYPT => TAKE THE 4 COLUMNS OF THE IDENTITY MATRIX

                            bin_decrypted = malloc( sizeof(int *) * size_text2 * 2 );
                            for( i = 0; i < size_text2 * 2; i++ ){
                                bin_decrypted[i] = (int *)malloc( sizeof(int) * ( size_matrix / NUM_MATRIX ) );
                            }
                            init_double( bin_decrypted, size_text2 * 2, size_matrix / NUM_MATRIX );

                            final_bin( bin_decrypted, bin, index, size_text2 / 2, size_matrix / NUM_MATRIX );   // The correct bin for having the right text

                            // Uncomment line 233 for more informations
                            //print_tab_int_double( bin_decrypted, size_text2 / 2, size_matrix / NUM_MATRIX );


                            final_result = malloc( sizeof(char) * (size_text2 + 1) );              // +1 for \0
                            final_result[0] = '\0';

                            final_message( bin_decrypted, final_result, size_text2 / 2, size_matrix / NUM_MATRIX );
                            printf( "\n\nTaille texte: %d", strlen(final_result) );


                            // ECRITURE
                            decrypt = fopen( TEXT_DECRYPTED, "w" );
                            if( decrypt != NULL ){
                                fwrite( final_result, sizeof(char), size_text2 / 2 + 1, decrypt );
                                printf("\n\nLe fichier %s a bien ete cree.\n", TEXT_DECRYPTED);
                            }else{
                                printf("\nProbleme lors de l'ouverture de texte.txtc\n");
                            }



                            free(final_result);
                            for( i = 0; i < size_text2 * 2 ; i++ ){
                                free(bin_decrypted[i]);
                            }
                            free(index);
                            free(matrix);
                            free(bin);
                            free(text);
                            free(line);
                            fclose(decrypt);
                            fclose(encrypt);
                            fclose(explore_matrix);
                        }

                        break;

            case 9:     printf("\nAu revoir !\n");
                        break;

            default:    printf("\nVeuillez faire un choix.\n");
                        break;
        }

    }while( choice != 9 );

    return 0;
}



