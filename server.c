#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[])
{

	if(argc < 2){

		fprintf(stderr, "Port not provided. Program terminated\n");
		exit(1);
	}

	int sockfd, newsockfd, portno, n;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Error opening Socket.");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));  //bzero clears any text or data whatever it is refering to
	                                                //here we are clearnig serv_addr, so that we can write correct data in it.

	portno = atoi(argv[1]);  //port number is provided in original (command line) arguments 2nd number.
	                         //1st argument is file name so argv[0] is file name.

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);  //host to network short (htons() function converts the unsigned short integer hostshort from host byte order to network byte order)


	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Binding Failed.");


	listen(sockfd, 5);   // listens who is trying to connect? who is on the same port?
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);  // from now on the communication will be on this newsockfd

	if(newsockfd < 0)
		error("Error on Accept");


	while(1)
	{

		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);  //a corresponding write function should be there in client
		if(n < 0)
			error("Error on reading.");
		printf("Client : %s\n", buffer);   // client message is stored in buffer and we are printing it.
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);  //fgets reads bytes from string

		n = write(newsockfd, buffer, strlen(buffer));
		if(n < 0)
			error("Error on Writing. ");

		int i = strcmp("Bye", buffer);
		if(i == 0)
			break;

	}

	close(newsockfd);
	close(sockfd);
	return 0;

}

