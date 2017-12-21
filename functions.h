
// Last change: change 48

void read_file( FILE *file, char* text ){

    long i = 0;
    if( text != NULL ){
        text[0] = '\0';


        while( !feof(file) ){
            text[i] = fgetc(file);
            i++;
        }
        text[i] = '\0';
        printf("\nLe fichier a bien ete lu!\n" );



    }else{
        printf("L'allocation de la variable text n'a pas fonctionné.\n");
    }
}




void init_tab( int *tab, long size_tab ){
    long i = 0;

    for( i = 0; i < size_tab; i++ ){
        tab[i] = 0;
    }
}

void init_double( int **tab, long line, int columns ){
    int i = 0;
    int j = 0;

    for( i = 0; i < line; i++ ){
        for( j = 0; j < columns; j++ ){
            tab[i][j] = 0;
        }
    }
}


void convert_char_to_bin( char *text, int *bin, long size_tab ){
    long i = 0;
    int j = 0;
    int mem = 0;
    long cnt = 0;

    for( i = 0; i < size_tab ; i++ ){
        mem = text[i];

        if( mem < 0 )           // for extended ASCII
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

}

void convert_bin_to_hexa( int *bin, char *hexa, int size_tab ){
    int i = 0;
    int j = 0;
    int cnt = 0;
    int result = 0;

    for( i = 0; i < size_tab ; i++ ){

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

void convert_bin_to_hexa_double( int **bin, char *hexa, int size_tab ){
    int i = 0;
    int j = 0;
    int k = 0;
    int cnt = 0;
    int counter = 0;
    int result = 0;

    for( i = 0; i < size_tab * 2 ; i++ ){       // number of line of 8 bits
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


void read_matrix( FILE *file, char *line, int *tab, int size_tab ){     // Penser à mettre des erreurs lorsqu'il y a des colonnes identiques & colonne à 0
    int i = 0;
    int cnt = 0;                // count number


    if( file != NULL ){

        while( fgets( line, size_tab, file ) != NULL ){
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

void calculate_sub_message( int *binary, int *matrix, int **sub_messages, long size_text, int size_matrix, int size_initial_matrix ){
    long i = 0;
    int j = 0;
    int k = 0;
    int cnt = 0;
    int result = 0;

    for( i = 0; i < size_text * 2; i++ ){        // number of sub messages

        for( j = 0; j < size_matrix / NUM_MATRIX; j++ ){       // number of columns in the matrix

            result = 0;
            for( k = 0; k < size_initial_matrix; k++ ){         // calculate initial matrix's line with matrix's column
                result += binary[k+cnt] * (matrix[k * 8 + j]);
            }

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


void final_message( int **bin, char *text_encrypted, long size_lines, int size_matrix_column ){        // bin to char ASCII
    long i = 0;
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
    }
}






void matrix_order( int *matrix, int *index, int size_tab, int size_id_matrix){
    int i = 0;

    for( i = 0; i < size_tab; i++){
        if( matrix[i] == 1 && matrix[i + 8] == 0 && matrix[i + 16] == 0 && matrix[i + 24] == 0 )
            index[0] = i;

        if( matrix[i] == 0 && matrix[i + 8] == 1 && matrix[i + 16] == 0 && matrix[i + 24] == 0 )
            index[1] = i;

        if( matrix[i] == 0 && matrix[i + 8] == 0 && matrix[i + 16] == 1 && matrix[i + 24] == 0 )
            index[2] = i;

        if( matrix[i] == 0 && matrix[i + 8] == 0 && matrix[i + 16] == 0 && matrix[i + 24] == 1 )
            index[3] = i;
    }

    printf("\nOrdre des colonnes de la matrice identite:\n");
    print_tab_int( index, size_id_matrix, 0 );

}


void final_bin( int ** bin_decrypted, int *bin, int *index, long lines, int columns ){
    long i = 0;
    int j = 0;
    int cnt = 0;

    for( i = 0; i < lines; i++ ){
        for( j = 0; j < columns; j++ ){
            if( j < 4 ){
                bin_decrypted[i][j] = bin[index[j]+cnt];
                if( j == 3 )
                    cnt += 8;
            }
            else{
                bin_decrypted[i][j] = bin[index[j-4]+cnt];
            }
        }
        cnt += 8;
    }

}












void print_tab_char( char *tab, int size_tab, int separate ){
    int i = 0;

    for( i = 0; i < size_tab; i++ ){
        if( i != 0 && i % separate == 0 )
            printf(" ");

        printf("%c", tab[i]);
    }

    printf("\n\n");

}

void print_tab_int( int *tab, int size_tab, int separate ){
    int i = 0;
    int cnt = 0;

    for( i = 0; i < size_tab; i++ ){
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

