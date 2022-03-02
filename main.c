#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "instantgaming.c"
#include "eneba.c"

#define SERVER   "localhost"
#define USER     "root"
#define PASSWORD "root"
#define DB       "PCC"


// ====== PROTOTYPES =======
int refreshAllPrice(MYSQL *conn);
// int getEnebaPrice(int idArticle, char *url, MYSQL *conn);
// int getIGPrice(int idArticle, char *url, MYSQL *conn);
int curl(char *url, char *plateforme);


int getEnebaPrice(int idArticle, char *url){

        // ======= PARTIE SQL =======
    MYSQL *conn;
    conn = mysql_init(NULL);

    printf("starting...\n");   

    if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DB, 0, NULL, 0)) {
        printf("error: %s\n", mysql_error(conn));
        return EXIT_FAILURE;
    }

    printf("url: %s\n", url);

    // curl(url, "eneba");

    struct priceStruct priceEneba;

    priceEneba.idArticle = idArticle;
    priceEneba.price = searchEnebaPrice("eneba.txt");
    strcpy(priceEneba.site, "eneba.com");    

    insertPrice(priceEneba, conn);

    return EXIT_SUCCESS;
}

int getIGPrice(int idArticle, char *url){

    // curl(url, "instant-gaming");

    MYSQL *conn;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DB, 0, NULL, 0)) {
        printf("error: %s\n", mysql_error(conn));
        return EXIT_FAILURE;
    }

    struct priceStruct priceIG;

    priceIG.idArticle = idArticle;
    priceIG.price = searchIGPrice();
    printf("IGGGG: %d\n",searchIGPrice());
    strcpy(priceIG.site, "instant-gaming.com");    

    insertPrice(priceIG, conn);

    return EXIT_SUCCESS;
}

int curl(char *url, char *plateforme){  
	char cmd[2096] = "curl -o ";

	strcat(cmd, plateforme);
	strcat(cmd, ".txt \"");
	strcat(cmd, url);
	strcat(cmd, "\"");

	printf("%s\n",cmd);

	system(cmd);

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
        char enebaURL[255];
        char igURL[255];
        strcpy(enebaURL,row[1]);
        strcpy(igURL, row[2]);
		printf("id: %d \n", idAricle);
		printf("enebaURL: %s \n", enebaURL);
		printf("igURL: %s \n", igURL);

        // mysql_free_result(res);

		if(enebaURL[0] != 'N' && enebaURL[1] != 'C'){
			printf("curl\n");
            // char *url;
            // strcat(url, row[1]);
			printf("liens: %s\n", enebaURL );

            char cmd[2096] = "curl -o ";
            strcat(cmd, "eneba.txt ");
            strcat(cmd, enebaURL);
            printf("%s\n",cmd);

            system(cmd);

            
            // curl(url, "eneba");

			getEnebaPrice(idAricle, enebaURL);
		} else {
			printf("non");
		}

        if(igURL[0] != 'N' && igURL[1] != 'C'){
			printf("curl\n");
			printf("liens: %s\n",igURL);

            char cmd[2096] = "curl -o ";
            strcat(cmd, "instantgaming.txt ");
            strcat(cmd, igURL);
            printf("%s\n",cmd);
            system(cmd);

            // mysql_free_result(res);
			getIGPrice(idAricle, igURL);
		} else {
			printf("non");
		}

		printf("\n=========\n");
	}

    mysql_free_result(res);

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



// int main(int argc, char **argv){
//     // curl("https://www.eneba.com/fr/steam-subnautica-steam-key-global", "eneba");
//     short choice;
//     short programLoop = 1;


//     // ======= PARTIE SQL =======
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;

//     conn = mysql_init(NULL);

//     printf("starting...\n");   

//     if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DB, 0, NULL, 0)) {
//         printf("error: %s\n", mysql_error(conn));
//         return EXIT_FAILURE;
//     }

//     struct articleStruct article;
//     struct priceStruct sitePrice;

//     while(programLoop){
//         choice = 0;

//         printf("\n========oOo========\n");
//         printf("1: Ajouter un nouvel article\n");
//         printf("2: Voir l'historique d'un article\n");
//         printf("3: Actualiser le prix des articles\n");
//         printf("4: Test\n");
//         printf("0: Quitter\n");
//         printf("Que voulez-vous faire ?\n");

//         scanf("%hd",&choice);


//         switch (choice){
//             case 0: 
//                 programLoop = 0;
//                 break;
//             case 1: 
//                 newArticle(article,conn);
//                 break;
            
//             case 2:
//                 selectHistory(conn);
//                 break;
//             case 3: 
//                 refreshAllPrice(conn);
//                 break;
//             case 4: 
//                 // curl("https://www.eneba.com/fr/steam-subnautica-steam-key-global", "eneba");
//                 searchIGPrice();
//                 break;


//             default:

//                 break;
//         }
//     }

//     mysql_free_result(res);
//     mysql_close(conn);
//     return EXIT_SUCCESS;
// }