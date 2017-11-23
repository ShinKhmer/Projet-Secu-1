#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100


int main(int argc, char **argv)
{

    FILE* explore = NULL;
    char *line = NULL;
    char *text = NULL;
    int *bin = NULL;
    char *hexa = NULL;

    int size_text = 0;


    line = malloc( sizeof(char) * SIZE );
    line[0] = '\0';                         // init line

    text = malloc( sizeof(char) * SIZE );
    line[0] = '\0';

    bin = malloc( sizeof(int) * SIZE * 8 );
    init(bin);

    hexa = malloc( sizeof(char) * SIZE / 4 );
    hexa[0] = '\0';

    // File pointer
    explore = fopen("texte.txt", "r+");     // Open file / r+: read and write

    if( explore != NULL ){

        // FILE RECOVERY
        read_file( explore, line, text );      // Function read and put all the lines read into one char chain
        size_text = strlen(text);


        // CONVERT BINARY TO HEXA
        convert_char_to_bin( text, bin, size_text );
        printf( "\nTaille texte: %d\n", strlen(text) );

        print_bin( text, bin, size_text );





        // ECRITURE
        //fputs( 'A', text );



        fclose(explore);   // Close file
    }
    else{
        printf("Impossible d'ouvrir le fichier !");     // text = NULL => print error
    }


    free(hexa);
    free(bin);
    free(text);
    free(line);

    return 0;
}



void init( int *tab ){
    int i = 0;

    for( i = 0; i < SIZE * 8; i++ ){
        tab[i] = 0;
    }
}



void read_file( char *file, char *line, char* text ){

    int cnt = 0;                            // Each line read will make cnt++

    if( text != NULL ){
    text[0] = '\0';

        while( fgets( line, SIZE, file) != NULL ){
            if(cnt == 0){                   // 1st line => copy
                strcpy( text, line );
            }
            else{                           // next lines => concat
                strcat( text, line );
            }
            printf( "Taille ligne %d: %d\n", (cnt + 1), strlen(line) );
            cnt++;
        }
    }else{
        printf("L'allocation de la variable text n'a pas fonctionné.");
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

    /*for( i = 0; i < size * 8; i++ ){

        if( i == 0 ){
            printf("%c: ", text[cnt]);
            cnt++;
        }

        if( i != 0 && i % 8 == 0 ){
            printf("\n%c: ", text[cnt]);
            cnt++;
        }

        printf("%d" , bin[i]);
    }*/

}

void print_bin( char *text, int* bin, int size ){
    int cnt = 0;
    int i = 0;

    for( i = 0; i < size * 8; i++ ){

        if( i == 0 ){
            printf("%c: ", text[cnt]);
            cnt++;
        }

        if( i != 0 && i % 8 == 0 ){
            printf("\n%c: ", text[cnt]);
            cnt++;
        }

        printf("%d" , bin[i]);
    }
}




