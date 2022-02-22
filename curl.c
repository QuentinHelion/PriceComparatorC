int curl(char *url, char *plateforme){  
  char cmd[1028] = "curl -o ";

  strcat(cmd, plateforme);
  strcat(cmd, ".txt ");
  strcat(cmd, url);

  // printf("%s\n",cmd);
	system(cmd);

  return 0;
}