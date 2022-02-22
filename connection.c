/* Simple C program that connects to MySQL Database server*/
#include <stdio.h>
#include <stdlib.h> 
#include <mysql/mysql.h>
#include <string.h>


int selectSQL(char *request, MYSQL *conn, MYSQL_RES *res ){

  MYSQL_ROW row;
  printf("Request: %s\n",request);
  printf("Sending SQL Request...\n");

  if (mysql_query(conn, request)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    return EXIT_FAILURE;
  }


  res = mysql_use_result(conn);

  printf("result: \n");
  while((row = mysql_fetch_row(res)) != NULL)
    printf("%s \n", row[0]);

  printf("SQL Request reponse succes !\n\n");

  return EXIT_SUCCESS;
}


// int insertSQL(char *request, MYSQL *conn ){

//   printf("Request: %s\n",request);
//   printf("Sending SQL Request...\n");

//   if (mysql_query(conn, request)) {
//     fprintf(stderr, "%s\n", mysql_error(conn));
//     return EXIT_FAILURE;
//   } else {
//     printf("SQL Request reponse succes !\n");
//   }
  

//   return EXIT_SUCCESS;
// }

 struct articleStruct{
    char title[255];
    char descr[255];
    char enebaURL[255];
    char igURL[255];
};

int insertArticle(struct articleStruct article, MYSQL *conn){
  
  printf("title: %s\n", article.title);
  printf("description: %s\n", article.descr);
  printf("enebaURL: %s\n", article.enebaURL);
  printf("igURL: %s\n", article.igURL);

  char request[4096] = "INSERT INTO article(title, descr, enebaURL, igURL) VALUES (";

  strcat(request, article.title);
  strcat(request, ", ");
  strcat(request, article.descr);
  strcat(request, ", ");
  strcat(request, article.enebaURL);
  strcat(request, ", ");
  strcat(request, article.igURL);
  strcat(request, "); ");

  printf("request: %s\n",request);

  if (mysql_query(conn, request )) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    return EXIT_FAILURE;
  }


  printf("Succefuly insert\n");
  return EXIT_SUCCESS;
}




int main(int argc, char **argv) {

  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  char *server = "localhost";
  char *user = "root";
  char *password = "root";
  char *database = "PCC";

  conn = mysql_init(NULL);

  printf("starting...\n");   

  /* Connect to database */
  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
    printf("error: %s\n", mysql_error(conn));
    return EXIT_FAILURE;
  } else {
    printf("Connection success !\n");
  }

  // printf("1\n");

  // if(mysql_query(conn, "SHOW TABLES;")) {
  //   printf("error: %s\n", mysql_error(conn));
  //   return EXIT_FAILURE;
  // } else {
  //   printf("Request success !\n");
  // }

  // printf("2\n");

  




 

  struct articleStruct article;


  strcpy(article.title, "'titre'");
  strcpy(article.descr, "'description'");
  strcpy(article.enebaURL, "'url eneba'");
  strcpy(article.igURL, "'url IG'");

  // char request;
  insertArticle(article, conn);
  
  // row = mysql_fetch_row(res);
  // printf("%s",row[0]);
  
  printf("=============\n");
  /* output table name */

  selectSQL("SHOW TABLES;", conn, res);

  printf("MySQL Tables in mysql database:\n");
  // res = mysql_use_result(conn);

  while((row = mysql_fetch_row(res)) != NULL)
    printf("%s \n", row[0]);


  /* close connection */
  mysql_free_result(res);
  mysql_close(conn);
  return EXIT_SUCCESS;
}