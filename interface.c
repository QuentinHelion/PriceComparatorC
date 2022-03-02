#include <stdio.h>
#include <stdlib.h> 
#include <gtk/gtk.h>
#include "main.c"

void addArti(GtkWidget *widget, gpointer data);
void selectHistorical(GtkWidget *widget, gpointer data);
void menu(GtkWidget *widget, gpointer data);
void exitApp(GtkWidget *widget, gpointer data);
void refPrice(GtkWidget *widget, gpointer data);
void getArticle(GtkBuilder *widget, gpointer data);
void test(GtkBuilder *widget, char *data);
void showHistorical(GtkWidget *widget, gpointer data);


int main(int argc, char **argv){

  printf("starting...\n");   

  // ======= GTK PART =======
  GtkBuilder *gtkBuilder;
  GtkWidget *window;

  GtkWidget *butAdd;
  GtkWidget *butShow;
  GtkWidget *butRefresh;
  GtkWidget *butExit;


  gtk_init(&argc, &argv);

  gtkBuilder = gtk_builder_new();  

  gtkBuilder = gtk_builder_new_from_file("interface.glade"); // récupération du glade
  window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "MainWindow")); // récupération ta fenêtre
  gtk_builder_connect_signals(gtkBuilder, NULL);

  butAdd = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "addArticle"));
  butShow = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "showHistory"));
  butRefresh = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "refreshPrice"));
  butExit = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "exit"));

  g_signal_connect(butAdd, "clicked", G_CALLBACK(addArti), (gpointer)window);
  g_signal_connect(butShow, "clicked", G_CALLBACK(selectHistorical), (gpointer)window);
  g_signal_connect(butRefresh, "clicked", G_CALLBACK(refPrice), (gpointer)window);
  g_signal_connect(butExit, "clicked", G_CALLBACK(exitApp), (gpointer)window); 
  

  // g_signal_connect(butAdd, "clicked", add(), (gpointer)window);
  // g_signal_connect(butShow, "clicked", show(), (gpointer)window);
  // g_signal_connect(butRefresh, "clicked", refresh(), (gpointer)window);

  gtk_widget_show_all(window);
  gtk_main();

  return EXIT_SUCCESS;
}

void refPrice(GtkWidget *widget, gpointer data){
  // ======= SQL PART =======
  MYSQL *conn;

  conn = mysql_init(NULL);

  // Connection
  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DB, 0, NULL, 0)) {
      printf("error: %s\n", mysql_error(conn));
  }

  refreshAllPrice(conn);
}


void menu(GtkWidget *widget, gpointer data){

  GtkBuilder *gtkBuilder;
  GtkWidget *window;

  GtkWidget *butAdd;
  GtkWidget *butShow;
  GtkWidget *butRefresh;
  GtkWidget *butExit;

  gtk_widget_destroy(data);

  gtkBuilder = gtk_builder_new();
  gtkBuilder = gtk_builder_new_from_file("interface.glade");
  window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "MainWindow"));
  gtk_builder_connect_signals(gtkBuilder, NULL);

  butAdd = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "addArticle"));
  butShow = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "showHistory"));
  butRefresh = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "refreshPrice"));
  butExit = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "exit"));

  g_signal_connect(butAdd, "clicked", G_CALLBACK(addArti), (gpointer)window);
  g_signal_connect(butShow, "clicked", G_CALLBACK(selectHistorical), (gpointer)window); 
  g_signal_connect(butRefresh, "clicked", G_CALLBACK(refPrice), (gpointer)window);
  g_signal_connect(butExit, "clicked", G_CALLBACK(exitApp), (gpointer)window); 

  gtk_widget_show_all(window);
}

void exitApp(GtkWidget *widget, gpointer data){
  gtk_main_quit ();
}

void test(GtkBuilder *widget, char *data){
  printf("test: %s\n",data);

}


void addArti(GtkWidget *widget, gpointer data){

  GtkBuilder *gtkBuilder;
  GtkWidget *window;

  GtkWidget *butEnter;
  GtkWidget *butReturn;
  
  gtk_widget_destroy(data);

  gtkBuilder = gtk_builder_new();
  gtkBuilder = gtk_builder_new_from_file("interface.glade");
  window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "addArticleWindow"));  
  gtk_builder_connect_signals(gtkBuilder, NULL);

  butEnter = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "enter"));
  butReturn = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "return"));
  
  g_signal_connect(butEnter, "clicked", G_CALLBACK(getArticle), (gpointer)gtkBuilder);
  g_signal_connect(butEnter, "clicked", G_CALLBACK(menu), (gpointer)window);
  g_signal_connect(butReturn, "clicked", G_CALLBACK(menu), (gpointer)window);


  gtk_widget_show_all(window); 
}

void getArticle(GtkBuilder *widget, gpointer data){
  MYSQL *conn;
  struct articleStruct artcl;

  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DB, 0, NULL, 0)) {
      printf("error: %s\n", mysql_error(conn));
  }

  GtkBuilder *gtkBuilder = data;
  // GtkWidget *window;
  GtkWidget *titl;
  GtkWidget *descrr;
  GtkWidget *urlEneba;
  GtkWidget *urlIG;

  titl = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "title"));
  descrr = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "descr"));
  urlEneba = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "enebaURL"));
  urlIG = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "igURL"));

  const gchar *dataEntry = gtk_entry_get_text(GTK_ENTRY(titl));
  printf("title: %s\n",dataEntry);
  strcpy(artcl.title, dataEntry);

  dataEntry = gtk_entry_get_text(GTK_ENTRY(descrr));
  printf("descr: %s\n",dataEntry);
  strcpy(artcl.descr, dataEntry);

  dataEntry = gtk_entry_get_text(GTK_ENTRY(urlEneba));
  printf("eneba: %s\n",dataEntry);
  strcpy(artcl.enebaURL, dataEntry);

  dataEntry = gtk_entry_get_text(GTK_ENTRY(urlIG));
  printf("IG: %s\n",dataEntry);
  strcpy(artcl.igURL, dataEntry);

  // DOC: https://hackthedeveloper.com/gtk-entry-user-input/
  
  insertArticle(artcl, conn);
}


void selectHistorical(GtkWidget *widget, gpointer data){

  GtkBuilder *gtkBuilder;
  GtkWidget *window;
  
  GtkWidget *textLabel;
  GtkWidget *idSearch;
  GtkWidget *butSend;
  GtkWidget *butReturn;

  MYSQL *conn;
  MYSQL_RES *res;
	MYSQL_ROW row;

  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DB, 0, NULL, 0)) {
      printf("error: %s\n", mysql_error(conn));
  }

  const gchar *request = "SELECT id, title FROM article"; 

	if (mysql_query(conn, request)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
    
  res = mysql_use_result(conn);
  
  char insert[4096] = " id  | Titre\n";

  while((row = mysql_fetch_row(res)) != NULL){
    strcat(insert, row[0]);
    strcat(insert,  " | ");
    strcat(insert, row[1]);
    strcat(insert,  "\n" );
  }

  mysql_free_result(res);

  printf("%s\n",insert);


  gtk_widget_destroy(data);

  gtkBuilder = gtk_builder_new();
  gtkBuilder = gtk_builder_new_from_file("interface.glade");
  window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "selectHistoryWindow"));  
  gtk_builder_connect_signals(gtkBuilder, NULL);
  
  idSearch = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "idToSearch"));
  butSend = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "send"));
  textLabel = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "artcList"));
  butReturn = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "return2"));

  gtk_label_set_text(GTK_LABEL(textLabel), insert);

  

  g_signal_connect(butSend, "clicked", G_CALLBACK(showHistorical), (gpointer)gtkBuilder); 

  // printf("%s\n",rep);

  g_signal_connect(butReturn, "clicked", G_CALLBACK(menu), (gpointer)window);  

  gtk_widget_show_all(window); 
}

void showHistorical(GtkWidget *widget, gpointer data){
  MYSQL *conn;
  MYSQL_RES *res;
	MYSQL_ROW row;

  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DB, 0, NULL, 0)) {
      printf("error: %s\n", mysql_error(conn));
  }


  GtkBuilder *gtkBuilder;
  GtkWidget *window;

  GtkWidget *idSearch;
  GtkWidget *butEnter;
  GtkWidget *butReturn;

  GtkWidget *enebaHisto;
  GtkWidget *igHisto;
  GtkWidget *articleInfo;
  

  idSearch = GTK_WIDGET(gtk_builder_get_object(data, "idToSearch"));
  const gchar *dataEntry = gtk_entry_get_text(GTK_ENTRY(idSearch));
  char rep[5];
  strcpy(rep, dataEntry);


  gtkBuilder = gtk_builder_new();
  gtkBuilder = gtk_builder_new_from_file("interface.glade");
  window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "showHistoryWindow"));  
  gtk_builder_connect_signals(gtkBuilder, NULL);

  enebaHisto  =  GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "enebaHistorical"));
  igHisto     =     GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "igHistorical"));
  articleInfo = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "infoArticle"));

  char requestInfo[512] = "SELECT title, descr FROM article WHERE id = "; 
	strcat(requestInfo, rep);
	// strcat(requestEneba, " && site = \"eneba.com\" ORDER BY priceDate DESC;");

  printf("%s\n", requestInfo);

	if (mysql_query(conn, requestInfo)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
    
  res = mysql_use_result(conn);
  char insertInfo[4096] = "==";
  while((row = mysql_fetch_row(res)) != NULL){
    strcat(insertInfo, row[0]);
    strcat(insertInfo, "==\nDescription: ");
    strcat(insertInfo, row[1]);
  }

  printf("%s\n", insertInfo);


  gtk_label_set_text(GTK_LABEL(articleInfo), insertInfo);

  mysql_free_result(res);


	char requestEneba[512] = "SELECT price, priceDate, site FROM website  WHERE idArticle = "; 
	strcat(requestEneba, rep);
	strcat(requestEneba, " && site = \"eneba.com\" ORDER BY priceDate DESC;");

  // printf("%s\n", request);

	if (mysql_query(conn, requestEneba)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
    
  res = mysql_use_result(conn);

  char insertEneba[4096] = "========ENEBA======== \nPrix  | Date     | Site\n";

  // strcpy(insert," Prix  | Date     | Site\n");

  while((row = mysql_fetch_row(res)) != NULL){
    // printf("%s€ | %s   | %s    \n", row[0],row[1],row[2]);
    strcat(insertEneba, row[0]);
    strcat(insertEneba, "€ | ");
    strcat(insertEneba, row[1]);
    strcat(insertEneba, " | ");
    strcat(insertEneba, row[2]);
    strcat(insertEneba, "\n");
  }

  mysql_free_result(res);

  
  char requestIG[512] = "SELECT price, priceDate, site FROM website  WHERE idArticle = "; 
	strcat(requestIG, rep);
	strcat(requestIG, " && site = \"instant-gaming.com\" ORDER BY priceDate DESC;");

	if (mysql_query(conn, requestIG)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
  res = mysql_use_result(conn);

  char insertIG[4096] = "========INSTANT-GAMING======== \nPrix  | Date     | Site\n";

  // strcpy(insert," Prix  | Date     | Site\n");

  while((row = mysql_fetch_row(res)) != NULL){
    // printf("%s€ | %s   | %s    \n", row[0],row[1],row[2]);
    strcat(insertIG, row[0]);
    strcat(insertIG, "€ | ");
    strcat(insertIG, row[1]);
    strcat(insertIG, " | ");
    strcat(insertIG, row[2]);
    strcat(insertIG, "\n");
  }

  mysql_free_result(res);

  printf("%s\n", insertEneba);
  printf("%s\n", insertIG);


  gtk_label_set_text(GTK_LABEL(enebaHisto), insertEneba);
  gtk_label_set_text(GTK_LABEL(igHisto), insertIG);

  gtk_widget_show_all(window); 
}