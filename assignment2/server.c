// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    printf("Check re-execute\n"); // check for re-execute. Since print before fork, if appear twice, re-exec succeeds.
    if (argc == 1) {    // original
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        //if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, 
        &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );

        if (bind(server_fd, (struct sockaddr *)&address,
        sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // process data
        pid_t pid = fork();

        if (pid == 0) { // child process
            printf("Child UID is: %d\n", (int)getpwnam("nobody"));
            setuid(getpwnam("nobody")->pw_uid);    //  drop its privileges to the “nobody” user using getpwnam
            
            char args_sock[100];
            sprintf(args_sock, "%d", new_socket);
            char args_original[100];
            sprintf(args_original, "%s", argv[0]);
            char *args[] = {args_original, args_sock, NULL};
            execvp(args[0], args);  // call re-exec
        } else if (pid < 0) {   // fork failed
            perror("Fork Error");
            exit(EXIT_FAILURE);
        } else {    // parent process
            wait(NULL); // wait for all its children to terminate
            printf("Parent end\n");
        }
    } else {    // the re-exec new_socket
        new_socket = atoi(argv[1]);
        valread = read(new_socket, buffer, 1024);
        printf("Read %d bytes: %s\n", valread, buffer);
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
    }
    
    

    return 0;
}
