/*A simple server in the internet domain using TCP
The port nu1mber is passed as an argument */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<sstream>
#include<vector>
#include<fstream>
#include<iostream>

using namespace std;

struct table {
    string major;
    int value;
    int value2;
};

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    ifstream file;
    string major;
    string line;
    string temp;
	string say;
	string notFound;
    int v1, v2;
    int index = 0;
	bool found = false;
    vector<table> storage;

	//read file into table
    file.open("Salaries.txt");
    if (file.is_open()) {
        while (getline(file, line)) {

            stringstream linestream(line);
            getline(linestream, major, '\t');
            linestream >> v1 >> v2;

            storage.push_back(table());
            storage[index].major = major;
            storage[index].value = v1;
            storage[index].value2 = v2;
            index++;
        }

    }
    file.close();

		int sockfd, newsockfd, portno;
		socklen_t clilen;
		char buffer[256];
		struct sockaddr_in serv_addr, cli_addr;
		int n;
		if (argc < 2) {
			fprintf(stderr, "ERROR, no port provided\n");
			exit(1);
		}
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
			error("ERROR opening socket");

			bzero((char *)&serv_addr, sizeof(serv_addr));
			portno = atoi(argv[1]);
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = INADDR_ANY;
			serv_addr.sin_port = htons(portno);
			if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0)
				error("ERROR on binding");
		
			listen(sockfd, 5);


			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if (newsockfd < 0)
				error("ERROR on accept");
		while (1) {
			found = false;
			bzero(buffer, 256);
			n = read(newsockfd, buffer, 256);
			if (buffer[0] == '\n') {
				break;
			}
			if (n < 0) error("ERROR reading from socket");

			printf("Here is the message: %s\n", buffer);


			//testing
			temp = buffer;
			temp.resize(temp.length() - 1);
			// cout << temp << "\t" << temp.length()<<endl;
			for (int i = 0; i < storage.size(); i++) {
				if (temp == storage[i].major) {
					//cout << "Major: " << storage[i].major << "\tSalary: " << storage[i].value << endl;
					index = i;
					found = true;
					break;
				}
			}
			if (found == true) {
				say = "The average early career pay for a " + temp + " major is $" + to_string(storage[index].value) + "\nThe corresponding mid-career pay is $" + to_string(storage[index].value2);
				for (int i = 0; i < say.length(); i++) {
					buffer[i] = say[i];
				}
				//cout << "buffer----------------" << endl;
				//for (int i = 0; i < 256; i++) {
				//	cout << buffer[i];
			//	}
			//	cout << "end-----------------" << endl;
			}

			else {
				notFound = "That major is not in the table";
				for (int i = 0; i < notFound.length(); i++) {
					buffer[i] = notFound[i];
				}
			
			}

			n = write(newsockfd, buffer, 256);
			if (n < 0) error("ERROR writing to socket");

		
		}
	
	return 0;
}
