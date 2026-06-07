 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/time.h>
 #include <arpa/inet.h>
 #include <string.h>
 #define LINE_SIZE 128
 void check(int return_value, const char *str){
    if(return_value < 0){
        printf("error %s", str);
        exit(1);
    }
 }
 int main(){
    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    check(client_fd,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    if(inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr) <= 0){
        perror("inet_pton");
        exit(1);
    }
    if(connect(client_fd,(struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("connect");
        exit(1);
    }
    char request[LINE_SIZE-1] = {0};
    printf("Enter your request:\n");
    fgets(request,LINE_SIZE-1,stdin);
    check(send(client_fd,request,LINE_SIZE-1,0),"send");
    
    struct timeval time;
    time.tv_sec = 10;
    time.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO,&time,sizeof(time));
    // FILE *f;
    //  if((f = fopen("output.txt","a")) == NULL){
    //     perror("error_open");
    //     exit(1);
    //  }
    char buffer[LINE_SIZE];
    int read;
    while((read = recv(client_fd,buffer,LINE_SIZE,0)) > 0 ){
         buffer[read] = '\0';
        // fwrite(buffer,read,1,f);
        // memset(buffer,0,LINE_SIZE);
        fputs(buffer,stdout);
        memset(buffer,0,LINE_SIZE);
    }
    // fputc('\0',f);
    
    close(client_fd);
    // fclose(f);
    return 0;

 }