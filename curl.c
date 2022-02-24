int curl(char *url, char *plateforme){  
	char cmd[1028] = "curl -o ";

	strcat(cmd, plateforme);
	strcat(cmd, ".txt ");
	strcat(cmd, url);

	system(cmd);

	return EXIT_SUCCESS;
}