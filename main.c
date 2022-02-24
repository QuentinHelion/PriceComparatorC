#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "curl.c"
// #include "sql.c"
#include "eneba.c"

// ====== PROTOTYPES =======
int curlEnebaPrice(int idArticle, char *url, MYSQL *conn);
int refreshAllPrice(MYSQL *conn);



int curlEnebaPrice(int idArticle, char *url, MYSQL *conn){

    curl(url, "eneba");

    struct priceStruct priceEneba;

    priceEneba.idArticle = idArticle;
    priceEneba.price = searchPrice("eneba.txt");
    strcpy(priceEneba.site, "eneba.com");    

    insertPrice(priceEneba, conn);

    return EXIT_SUCCESS;
}

int test(MYSQL *conn){
    struct priceStruct sitePrice;
    sitePrice.idArticle = 9;
    sitePrice.price = 10;
    strcpy(sitePrice.site, "eneba.com");
    insertPrice(sitePrice, conn);

    return 0;
}


int refreshAllPrice(MYSQL *conn){
	MYSQL_RES *res;
	MYSQL_ROW row;

	if (mysql_query(conn, "SELECT id, enebaURL, igURL FROM article;")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	res = mysql_use_result(conn);

	printf("result: \n");
	while((row = mysql_fetch_row(res)) != NULL){
        int idAricle = atoi(row[0]);
		printf("id: %d \n", idAricle);
		printf("enebaURL: %s \n", row[1]);
		printf("igURL: %s \n", row[2]);

		if(row[1][0] != 'N' && row[1][1] != 'C'){
			printf("curl");
			printf("liens: %s\n",row[1]);
            mysql_free_result(res);
			curlEnebaPrice(idAricle, row[1], conn);
		} else {
			printf("non");
		}

		printf("\n=========\n");
	}

	return EXIT_SUCCESS;
}



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
    struct priceStruct sitePrice;

    while(programLoop){
        choice = 0;

        printf("\n========oOo========\n");
        printf("1: Ajouter un nouvel article\n");
        printf("2: Voir l'historique d'un article\n");
        printf("3: Actualiser le prix des articles\n");
        printf("4: Test\n");
        printf("0: Quitter\n");
        printf("Que voulez-vous faire ?\n");

        scanf("%hd",&choice);


        switch (choice){
            case 0: 
                programLoop = 0;
                break;
            case 1: 
                newArticle(article,conn);
                break;
            
            case 2:
                curlEnebaPrice(6,"https://www.eneba.com/fr/steam-subnautica-steam-key-global",conn);
                break;
            case 3: 
                refreshAllPrice(conn);
                break;
            case 4: 
                test(conn);
                break;


            default:

                break;
        }
    }

    mysql_free_result(res);
    mysql_close(conn);
    return EXIT_SUCCESS;
}