#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100


int main(int argc, char **argv)
{

    FILE* explore = NULL;
    char *line = NULL;
    char *text = NULL;



    int i = 0;
    int cnt = 0;


    line = malloc( sizeof(char) * SIZE );
    line[0] = '\0';                         // init line

    text = malloc( sizeof(char) * SIZE );

    // File pointer
    explore = fopen("texte.txt", "r+");     // Open file / r+: read and write

    if( explore != NULL ){

        // FILE RECOVERY
        read_file( explore, line, text, SIZE);
        /*cnt = 0;                            // Each line read will make cnt++
        while( fgets( line, SIZE, explore) != NULL ){
            printf( "%s\n", line );
            if(cnt == 0){                   // 1st line => copy
                strcpy( text, line );
            }
            else{                           // next lines => concat
                strcat( text, line );
            }
            printf( "%d\n", strlen(line) );
            cnt++;
        }*/
        printf( "Texte final: %s\n", text );
        printf( "Taille texte: %d\n", strlen(text) );


        // CONVERT BINARY TO HEXA
        /*for( i = 0; i < strlen(text); i++ ){
            tab[i]
        }*/





        // ECRITURE
        //fputs( 'A', text );



        fclose(explore);   // Close file
    }
    else{
        printf("Impossible d'ouvrir le fichier !");     // text = NULL => print error
    }




    free(text);
    free(line);

    return 0;
}


void read_file( char *file, char *line, char* text, int size ){

    int cnt = 0;                            // Each line read will make cnt++

    if( text != NULL ){
    text[0] = '\0';

        while( fgets( line, SIZE, file) != NULL ){
            printf( "%s\n", line );
            if(cnt == 0){                   // 1st line => copy
                strcpy( text, line );
            }
            else{                           // next lines => concat
                strcat( text, line );
            }
            printf( "%d\n", strlen(line) );
            cnt++;
        }
    }else{
        printf("L'allocation de la variable text n'a pas fonctionné.");
    }

}


