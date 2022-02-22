#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl.c"
#include "sql.c"


int main(int argc, char **argv){
    // curl("https://www.eneba.com/fr/steam-subnautica-steam-key-global", "eneba");
    short choice;
    short programLoop = 1;


    // ======= PARTIE SQL =======
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "localhost";
    char *user = "root";
    char *password = "root";
    char *database = "PCC";

    conn = mysql_init(NULL);

    printf("starting...\n");   

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("error: %s\n", mysql_error(conn));
        return EXIT_FAILURE;
    }

    struct articleStruct article;

    while(programLoop){
        choice = 0;
        printf("\n========oOo========\n");
        printf("1: Ajouter un nouvel article\n");
        printf("2: Voir l'historique d'un article\n");
        printf("Que voulez-vous faire ?\n");

        scanf("%hd",&choice);


        switch (choice){
            case 1: 
                insertArticle(article,conn);
                break;
            
            default:

                break;
        }

    }


    mysql_free_result(res);
    mysql_close(conn);
    return EXIT_SUCCESS;
}