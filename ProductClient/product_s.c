 #include <stdio.h>
 #include <pthread.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <arpa/inet.h>
 #include <string.h>
 #define LINE_SIZE 128
 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 FILE* file_fd;
 void check(int return_value, const char *str){
    if(return_value < 0){
        printf("error %s", str);
        exit(1);
    }
 }
 void* handler(void*arg1){
    int* ptr = (int*)arg1;
    char buff[LINE_SIZE];
    int readSize = recv(*ptr,buff, LINE_SIZE,0);
    check(readSize, "recv");
    buff[strcspn(buff, "\r\n")] = '\0';
    char *msg = "error";
    if(strncmp(buff,"POST",4) == 0){
        char *p = strchr(buff, ' ');
        if(p == NULL){
            perror("error request line");
            send(*ptr,msg,LINE_SIZE,0);
            close(*ptr);
            free(ptr);
            pthread_exit(NULL);
        }
        p++;
        pthread_mutex_lock(&mutex);
        if((file_fd = fopen("database.txt","a")) == NULL){
            perror("error_fopen");
            pthread_mutex_unlock(&mutex);
            close(*ptr);
            free(ptr);
            pthread_exit(NULL);
        }
        fwrite(p,strlen(p),1,file_fd);
        fputc('\n',file_fd);
        fclose(file_fd);
        pthread_mutex_unlock(&mutex);
        char *message = "Product successfully added\n";
        send(*ptr, message,LINE_SIZE,0);
        close(*ptr);
        free(ptr);
        pthread_exit(NULL);
    }else if(strcmp(buff,"GET ALL") == 0){
        pthread_mutex_lock(&mutex);
        if((file_fd = fopen("database.txt","r")) == NULL){
            perror("error_fopen");
            pthread_mutex_unlock(&mutex);
            close(*ptr);
            free(ptr);
            pthread_exit(NULL);
        }
        fseek(file_fd,0,SEEK_END);
        long size = ftell(file_fd);
        fseek(file_fd,0,SEEK_SET);
        char *ptr1 = (char*)malloc(size);
        fread(ptr1,size,1,file_fd);
        check(send(*ptr,ptr1,size,0),"send");
        fclose(file_fd);
        pthread_mutex_unlock(&mutex);
        free(ptr1);
        close(*ptr);
        free(ptr);
        pthread_exit(NULL);
    }else if(strncmp(buff,"GET",3) == 0){
        char *p = strchr(buff, ' ');
        if(p == NULL){
            perror("error request line");
            send(*ptr,msg,sizeof(LINE_SIZE),0);
            pthread_exit(NULL);
        }
        p++;
        pthread_mutex_lock(&mutex);
        if((file_fd = fopen("database.txt","r")) == NULL){
            perror("error_fopen");
            pthread_mutex_unlock(&mutex);
            close(*ptr);
            free(ptr);
            pthread_exit(NULL);
        }
        int found = 0;
        char buffer[LINE_SIZE+1];
        while(fgets(buffer,LINE_SIZE,file_fd) != NULL){
            buffer[LINE_SIZE] = '\0';
                if(strstr(buffer,p) != NULL){
                check(send(*ptr,buffer,LINE_SIZE,0),"send");
                found = 1;
                break;
            }
            memset(buffer,0,LINE_SIZE);
        }
        fclose(file_fd);
        pthread_mutex_unlock(&mutex);
        if(!found){
            char *message = "NOT FOUND\n";
            send(*ptr,message, LINE_SIZE,0);
        }
        close(*ptr);
        free(ptr);
        pthread_exit(NULL);
    }else{
        char *message = "Request line error\n";
        check(send(*ptr,message,LINE_SIZE,0),"send");
        close(*ptr);
        free(ptr);
        pthread_exit(NULL);
    }
 }
 int main(){
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    check(server_fd, "socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    check(bind(server_fd,(struct sockaddr*)&addr, sizeof(addr)), "bind");
    check(listen(server_fd,10),"listen");
    while(1){
        int client_fd = accept(server_fd,NULL,NULL);
        check(client_fd,"accept");
        int *fd = malloc(sizeof(int));
        *fd = client_fd;
        pthread_t t1;
        pthread_create(&t1,NULL,handler,(void*)fd);
        pthread_detach(t1);
    }
    close(server_fd);
    pthread_exit(NULL);    
 }
