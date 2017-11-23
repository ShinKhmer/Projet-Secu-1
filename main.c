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
    text[0] = '\0';


    // File recovery
    explore = fopen("texte.txt", "r+");    // Open file / r+: read and write

    if( explore != NULL ){

        // INSTRUCTIONS
        cnt = 0;    // Each line read will make cnt++
        while( fgets( line, SIZE, explore) != NULL ){
            printf( "%s\n", line );
            if(cnt == 0){
                strcpy( text, line );
            }
            else{
                strcat( text, line );
            }
            printf( "%d\n", strlen(line) );
            cnt++;
        }
        printf( "Texte final: %s\n", text );
        printf( "Taille texte: %d\n", strlen(text) );


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
