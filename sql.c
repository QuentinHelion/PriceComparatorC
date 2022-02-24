#include <stdio.h>
#include <stdlib.h> 
#include <mysql/mysql.h>
#include <string.h>

struct articleStruct{
	char title[255];
	char descr[255];
	char enebaURL[255];
	char igURL[255];
};

struct priceStruct{
	int idArticle;
	int price;
	char site[255];
};

int insertPrice(struct priceStruct sitePrice, MYSQL *conn){

	MYSQL_RES *res;

	char request[4096] = "INSERT INTO website(idArticle, price, site) VALUES (";
	char strnum[20];

	sprintf(strnum, "%d", sitePrice.idArticle); 
	strcat(request, strnum);
	strcat(request, ",");
	sprintf(strnum, "%d", sitePrice.price); 
	strcat(request, strnum);
	strcat(request, ",'");
	strcat(request, sitePrice.site);
	strcat(request, "');");


	printf("request: %s\n", request);

	if (mysql_query(conn, request )) {
		fprintf(stderr, "%s\n", mysql_error(conn));
    	return EXIT_FAILURE;
  	}

	res = mysql_use_result(conn);
	mysql_free_result(res);
	
	printf("Succefuly insert\n");
	return EXIT_SUCCESS;
}


int insertArticle(struct articleStruct article, MYSQL *conn){

	char request[4096] = "INSERT INTO article(title, descr, enebaURL, igURL) VALUES (";

	strcat(request, "'");
	strcat(request, article.title);
	strcat(request, "', '");
	strcat(request, article.descr);
	strcat(request, "', '");
	strcat(request, article.enebaURL);
	strcat(request, "', '");
	strcat(request, article.igURL);
	strcat(request, "'); ");

	if (mysql_query(conn, request )) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}



	printf("Succefuly insert\n");
	return EXIT_SUCCESS;
}


int newArticle(struct articleStruct article, MYSQL *conn){

	printf("\n========oOo========\n");
	printf("Nom de l'article: ");
	scanf("%s",article.title);
	printf("Description: ");
	scanf("%s",article.descr);
	printf("Liens ('NC' si l'article n'est pas sur le site)\n");
	printf("Eneba: ");
	scanf("%s",article.enebaURL);
	printf("Instant gaming: ");
	scanf("%s",article.igURL);

	insertArticle(article, conn);

	return EXIT_SUCCESS;
}


int showHistory(char *idArticle, MYSQL *conn){

	MYSQL_RES *res;
	MYSQL_ROW row;

	char request[512] = "SELECT price, priceDate, site FROM website  WHERE idArticle = "; 
	strcat(request, idArticle);
	strcat(request, " ORDER BY priceDate DESC;");

	if (mysql_query(conn, request)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}
    
    res = mysql_use_result(conn);

	printf("\n\n\n");
    printf(" Prix  | Date     | Site\n");

    while((row = mysql_fetch_row(res)) != NULL)
        printf("%s€ | %s   | %s    \n", row[0],row[1],row[2]);

    mysql_free_result(res);

	return EXIT_SUCCESS;
}