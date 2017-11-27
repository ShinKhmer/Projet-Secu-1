#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 500

#define NUM_MATRIX 4


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
    int size_text = 0;
    int size_text2 = 0;
    int size_matrix = NUM_MATRIX*8;     // A modifier
    int size_initial_matrix = NUM_MATRIX;

    // FILE POINTER
    explore = fopen("texte.txt", "r");     // Open file / r+: read and write

    // SEARCH MATRIX
    printf("\n==================== MATRICE ====================");
    line = malloc( sizeof(char) * SIZE );
    line[0] = '\0';                         // init line

    explore_matrix = fopen("matrice.txt", "r");
    matrix = malloc( sizeof(int) * size_matrix );
    init(matrix, size_matrix);

    read_matrix( explore_matrix, line, matrix, size_matrix );
    print_tab_int( matrix, size_matrix, size_matrix / NUM_MATRIX );


    if( explore != NULL ){

        line[0] = '\0';                         // init line

        text = malloc( sizeof(char) * SIZE );
        text[0] = '\0';


        hexa = malloc( sizeof(char) * SIZE * 8 / 4 );
        hexa[0] = '\0';

        // FILE RECOVERY
        read_file( explore, line, text );      // Function read and put all the lines read into one char chain

        printf("%s", text);
        size_text = strlen(text);


        // CONVERT CHAR TO BINARY
        bin = malloc( sizeof(int) * size_text * 8 );
        init(bin, size_text * 8);                    // Possibilité d'optimiser le SIZE

        convert_char_to_bin( text, bin, size_text );
        printf( "\nTaille texte: %d\n", strlen(text) );


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
            i = 0;
            while( final_result[i] != '\0' ){
                if( final_result[i] < 0 )
                    fputc( final_result[i], encrypt );           // +256 because extended ASCII are negatives
                else{
                    fputc( final_result[i], encrypt );
                }
                i++;
            }
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


    // ==================================================================================
    // DECRYPTAGE

        printf("\n\n\n\==================== DECRYPTAGE ====================\n");          // optionnel

    encrypt = fopen( "texte.txtc", "r+" );

    if( encrypt != NULL ){

        line[0] = '\0';                         // init line

        text = malloc( sizeof(char) * SIZE );
        text[0] = '\0';

        size_text2 = 0;


        // READ FILE DECRYPT

        read_file( encrypt, line, text);

        size_text2 = strlen(text);

        /*for(i = 0; i < size_text; i++ ){
            if( text[i] < 0 )
                text[i] += 256;

            printf("%d %c ", text[i]);
        }*/



        printf("\nAffichage du fichier:\n%s\n", text);


        // DECRYPT => TAKE THE 4 COLUMNS OF THE IDENTITY MATRIX

        printf("\n==================== TEXTE EN BINAIRE ====================\n");          // optionnel

        bin = malloc( sizeof(int) * size_text2 * 8 );
        init(bin, size_text2 * 8);

        convert_char_to_bin2( text, bin, size_text2 );
        print_tab_int( bin, size_text2 * 8, 8 );

        bin_decrypted = (int *)malloc( sizeof(int *) * size_text2 * 2 );
        for( i = 0; i < size_text2 * 2; i++ ){
            bin_decrypted[i] = (int *)malloc( sizeof(int) * ( size_matrix / NUM_MATRIX ) );
        }
        init_double( bin_decrypted, size_text2 * 2, size_matrix / NUM_MATRIX );
        //matrix_order();


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



        final_result = malloc( sizeof(char) * (size_text2 + 1) );              // +1 for \0
        final_result[0] = '\0';

        final_message( bin_decrypted, final_result, size_text2 / 2, size_matrix / NUM_MATRIX );
        printf( "MESSAGE DECODE:\n%s", final_result );


        free(final_result);
        for( i = 0; i < size_text2 * 2 ; i++ ){
            free(bin_decrypted[i]);
        }
        free(bin);
        free(text);
        fclose(encrypt);
    }


        free(line);

    return 0;
}



void init( int *tab, int size ){
    int i = 0;

    for( i = 0; i < size; i++ ){
        tab[i] = 0;
    }
}

void init_double( int **tab, int line, int columns ){
    int i = 0;
    int j = 0;

    for( i = 0; i < line; i++ ){
        for( j = 0; j < columns; j++ ){
            tab[i][j] = 0;
        }
    }
}

void read_file( char *file, char *line, char* text ){

    int i = 0;
    int cnt_line = 0;                            // Each line read will make cnt_line++

    if( text != NULL ){
    text[0] = '\0';

        while( fgets( line, SIZE, file) != NULL ){

            if(cnt_line == 0){                   // 1st line => copy
                strcpy( text, line );
            }
            else{                           // next lines => concat
                strcat( text, line );
            }


            printf( "Taille ligne %d: %d\n", (cnt_line + 1), strlen(line) );
            cnt_line++;
        }
    }else{
        printf("L'allocation de la variable text n'a pas fonctionné.");
    }


    while( text[i] != '\0' ){
        i++;
    }

    if( cnt_line == 1 ){
        while( text[i] != '\0' ){
            if( text[i] == '\n' )
                text[i] == '\0';
        }
    }


}

void convert_char_to_bin( char *text, int *bin, int size ){
    int i = 0;
    int j = 0;
    int mem = 0;
    int cnt = 0;

    for( i = 0; i < size ; i++ ){

        mem = text[i];

        for( j = 7; j >= 0; j-- ){
            if( mem % 2 == 0 ){
                bin[j+cnt] = 0;
            }else{
                bin[j+cnt] = 1;
            }
            mem /= 2;
        }
        cnt += 8;
    }

    cnt = 0;
}

void convert_char_to_bin2( char *text, int *bin, int size ){
    int i = 0;
    int j = 0;
    int mem = 0;
    int cnt = 0;

    for( i = 0; i < size ; i++ ){

        mem = text[i];

        if( mem < 0 )
            mem += 256;

        for( j = 7; j >= 0; j-- ){
            if( mem % 2 == 0 ){
                bin[j+cnt] = 0;
            }else{
                bin[j+cnt] = 1;
            }
            mem /= 2;
        }
        cnt += 8;
    }

    cnt = 0;
}

void convert_bin_to_hexa( int *bin, char *hexa, int size ){
    int i = 0;
    int j = 0;
    int cnt = 0;
    int result = 0;

    for( i = 0; i < size ; i++ ){

        // SUM 4 BITS
        result = 0;
        for( j = 0; j < 4; j++ ){
            if( bin[(j + cnt)] == 1 ){
                switch( j ){
                    case 0: result += 8;
                            break;
                    case 1: result += 4;
                            break;
                    case 2: result += 2;
                            break;
                    case 3: result += 1;
                            break;
                    default:
                            printf("Il y a une couille dans le pâté");
                            break;
                }
            }
        }

        // CONVERT TO HEXA
        if( result >= 0 && result <= 9 ){
            hexa[i] = result + 48;          // ASCII 0 to 10
        }
        else if( result >= 10 && result <= 15 ){
            hexa[i] = result + 55;          // ASCII A to F
        }
        else{
            printf("error");
        }

        cnt += 4;
    }
}

void convert_bin_to_hexa2( int **bin, char *hexa, int size ){
    int i = 0;
    int j = 0;
    int k = 0;
    int cnt = 0;
    int counter = 0;
    int result = 0;

    for( i = 0; i < size * 2 ; i++ ){       // number of line of 8 bits
        cnt = 0;
        for( j = 0; j < 2; j++ ){           // group of 4 bits in a line
            result = 0;
            for( k = 0; k < 4; k++ ){           // sum of 4 bits
                if( bin[i][(k + cnt)] == 1 ){
                    switch( k ){
                        case 0: result += 8;
                                break;
                        case 1: result += 4;
                                break;
                        case 2: result += 2;
                                break;
                        case 3: result += 1;
                                break;
                        default:
                                printf("Il y a une couille dans le pâté");
                                break;
                    }
                }
            }
            // CONVERT TO HEXA
            if( result >= 0 && result <= 9 ){
                hexa[counter] = result + 48;          // ASCII 0 to 10
            }
            else if( result >= 10 && result <= 15 ){
                hexa[counter] = result + 55;          // ASCII A to F
            }
            else{
                printf("error");
            }
            counter ++;
            cnt += 4;           // go to indice 4 of a line
        }
    }
}


void print_result( char *text, int* bin, char *hexa, int size ){
    int cnt = 0;
    int i = 0;

    for( i = 0; i < size * 8; i++ ){

        if( i == 0 ){                                           // print first char
            printf("%c: ", text[cnt]);
        }

        if( i != 0 && i % 8 == 0 ){                             // print hexa, return to the line then print char
            printf(" => %c %c\n%c: ", hexa[cnt*2], hexa[cnt*2+1], text[cnt+1]);
            cnt++;
        }

        printf("%d", bin[i]);                                   // print binary

        if( i == size * 8 - 1 )                                 // print last hexa
            printf(" => %c %c ", hexa[cnt*2], hexa[cnt*2+1] );
    }

    printf("\n\n");
}

void read_matrix( char *file, char *line, int *tab, int size ){     // Penser à mettre des erreurs lorsqu'il y a des colonnes identiques & colonne à 0
    int i = 0;
    int cnt = 0;                // count number


    if( file != NULL ){

        while( fgets( line, size, file ) != NULL ){
            // Browse line to search number
            i = 0;
            while( i < strlen(line) && ( line[i] != '\0' || line[i] != '\n' ) ){
                if( line[i] == 48 || line[i] == 49 ){               // 0 or 1 in ASCII
                    tab[cnt] = line[i] - 48;
                    cnt++;
                }
                i++;
            }
        }
    }
}

void calculate_sub_message( int *binary, int *matrix, int **sub_messages, int size_text, int size_matrix, int size_initial_matrix ){
    int i = 0;
    int j = 0;
    int k = 0;
    int cnt = 0;
    int result = 0;

    for( i = 0; i < size_text * 2; i++ ){        // number of sub messages

        for( j = 0; j < size_matrix / NUM_MATRIX; j++ ){       // number of columns in the matrix

            result = 0;
            for( k = 0; k < size_initial_matrix; k++ ){     // calculate initial matrix's line with matrix's column
                result += binary[k+cnt] * (matrix[k * 8 + j]);
                /*f( i == 0 && j == 3 )
                    printf("bin: %d x matrix: %d = %d \n", binary[k+cnt], (matrix[k * 8 + j]), result);*/
            }
            /*if( i == 0 && j == 3 )
                printf("result: %d\n", result);*/

            if( result % 2 == 0 ){
                sub_messages[i][j] = 0;
            }
            else if( result % 2 == 1 ){
                sub_messages[i][j] = 1;
            }
            else
                printf("Il y a un probleme dans la matrice, il faut appeler Neo !");
        }

        cnt += 4;
    }



}


void final_message( int **bin, char *text_encrypted, int size_lines, int size_matrix_column ){        // bin to char ASCII
    int i = 0;
    int j = 0;
    int result = 0;

    for( i = 0; i < size_lines; i++ ){
        result = 0;
        for( j = 0; j < size_matrix_column; j++ ){
            if( bin[i][j] == 1 ){
                switch( j ){
                    case 0:     result += 128;
                                break;
                    case 1:     result += 64;
                                break;
                    case 2:     result += 32;
                                break;
                    case 3:     result += 16;
                                break;
                    case 4:     result += 8;
                                break;
                    case 5:     result += 4;
                                break;
                    case 6:     result += 2;
                                break;
                    case 7:     result += 1;
                                break;
                    default:    printf("Il y a un problème !");
                                break;
                }
            }
        }

        text_encrypted[i] = result;
        printf("%c ", text_encrypted[i]);
    }

    text_encrypted[size_lines] = '\0';
}


void matrix_order( int *matrix, int size ){
    int i = 0;
    int j = 0;

    for( i = 0; i < size; i++){
    }

}













void print_tab_char( char *tab, int size, int separate ){
    int i = 0;

    for( i = 0; i < size; i++ ){
        if( i != 0 && i % separate == 0 )
            printf("   ");

        printf("%c", tab[i]);
    }
}

void print_tab_int( int *tab, int size, int separate ){
    int i = 0;
    int cnt = 0;

    for( i = 0; i < size; i++ ){
        if( separate != 0 ){
            if( i % separate == 0){
                printf("\n%d - ", cnt);
                cnt++;
            }
        }
        printf("%d ", tab[i]);
    }

    printf("\n\n");
}

void print_tab_int_double( int **tab, int sizei, int sizej ){
    int i = 0;
    int j = 0;

    for( i = 0; i < sizei; i++ ){
        for( j = 0; j < sizej; j++ ){
            if( j % 8 == 0 )
                printf("%d: ", i);
            printf("%d ", tab[i][j]);
        }
        printf("\n");
    }
}
