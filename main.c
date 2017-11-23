#include <stdio.h>
#include <stdlib.h>
#define SIZE 100

int main(int argc, char **argv)
{

    FILE* explore = NULL;
    char *text = NULL;

    int i = 0;


    text = malloc( sizeof(char) * SIZE );


    // File recovery
    explore = fopen("texte.txt", "r+");    // Open file / r+: read and write

    if( explore != NULL ){


        // INSTRUCTIONS
        while( fgets( text, SIZE, explore) != NULL ){
            printf( "%s\n", text );
            printf( "%d", strlen(text) );
        }


        // ECRITURE
        //fputs( 'A', text );



        fclose(explore);   // Close file
    }
    else{
        printf("Impossible d'ouvrir le fichier !");     // text = NULL => print error
    }







    return 0;
}
