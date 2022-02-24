#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "curl.c"
#include "instantgaming.c"
#include "eneba.c"

// ====== PROTOTYPES =======
int refreshAllPrice(MYSQL *conn);
int getEnebaPrice(int idArticle, char *url, MYSQL *conn);
int getIGPrice(int idArticle, char *url, MYSQL *conn);


int getEnebaPrice(int idArticle, char *url, MYSQL *conn){

    curl(url, "eneba");

    struct priceStruct priceEneba;

    priceEneba.idArticle = idArticle;
    priceEneba.price = searchEnebaPrice("eneba.txt");
    strcpy(priceEneba.site, "eneba.com");    

    insertPrice(priceEneba, conn);

    return EXIT_SUCCESS;
}

int getIGPrice(int idArticle, char *url, MYSQL *conn){

    curl(url, "instant-gaming");

    struct priceStruct priceIG;

    priceIG.idArticle = idArticle;
    priceIG.price = searchIGPrice("instant-gaming.txt");
    strcpy(priceIG.site, "instant-gaming.com");    

    insertPrice(priceIG, conn);

    return EXIT_SUCCESS;
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
			getEnebaPrice(idAricle, row[1], conn);
		} else {
			printf("non");
		}

        if(row[2][0] != 'N' && row[2][1] != 'C'){
			printf("curl");
			printf("liens: %s\n",row[2]);
            if(row[1][0] == 'N' && row[1][1] == 'C'){
                mysql_free_result(res);
            }
			getIGPrice(idAricle, row[2], conn);
		} else {
			printf("non");
		}

		printf("\n=========\n");
	}

	return EXIT_SUCCESS;
}

int selectHistory(MYSQL *conn){
    MYSQL_RES *res;
	MYSQL_ROW row;

    char *choice;

	if (mysql_query(conn, "SELECT id, title, descr FROM article;")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}
    
    res = mysql_use_result(conn);

    printf("id | Titre    | Description\n");

    while((row = mysql_fetch_row(res)) != NULL)
        printf("%s | %s   | %s \n", row[0],row[1],row[2]);

    mysql_free_result(res);

    printf("Quel article voulez-vous voir ? (id)   ");
    scanf("%s",choice);
    
    showHistory(choice, conn);

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
                selectHistory(conn);
                break;
            case 3: 
                refreshAllPrice(conn);
                break;
            case 4: 
                
                break;


            default:

                break;
        }
    }

    mysql_free_result(res);
    mysql_close(conn);
    return EXIT_SUCCESS;
}