Telegrambot на си + Arduino

"http://www.pvsm.ru/programmirovanie/263237"

На хабре пост закрыт.
================================

Бота писал для своего "безумного дома" (выкладываю упрощённую версию), может ещё кому-то пригодится. Работает через webhook (понадобится белый ip).

Суть заключается в следующем: вы пишите боту команду, он переправляет её на сервер, сервер оправляет команду ардуине, ардуина выполняет команду и возвращает статус серверу, и наконец сервер отправляет статус боту.

Наглядный код для ардуины
char reciv_buf[16] = {0,};
byte d13 = 0;

void setup()
{
  Serial.begin(57600);
  pinMode(13, OUTPUT);
}
 
void loop()
{
    int s = 0;
    memset(reciv_buf, 0, 16);
    while(Serial.available())         
     {
        delay(1);
        reciv_buf[s] = Serial.read();  
        if(reciv_buf[s] == '\n')         
          {
            reciv_buf[s] = 0;       
            if(strstr(reciv_buf, "vkld13") != NULL)
             { 
               digitalWrite(13, HIGH);
               d13 = 1;
               Serial.print("OK:vkld13");
               Serial.print('\n');
             }
     
            if(strstr(reciv_buf, "otkld13") != NULL)
             { 
               digitalWrite(13, LOW);
               d13 = 0;
               Serial.print("OK:otkld13");
               Serial.print('\n');
             }   

          }
          
        s++;
        
     } // END while
        
} // END LOOP




Команды придумываете сами, вписываете их туда где — «vkld13» и т.д., а потом отправляете их боту.

Так же можно послать боту букву t, на что он ответит словом TEST.

Не знаю нужно ли выкладывать бинарник, так как у меня, например, это хозяйство работает на роутере.

Исходник бота для Линукса
#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <unistd.h>
#include <openssl/err.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <netdb.h>
#include <time.h>
#include <sys/wait.h>
#include <termios.h>
#include <fcntl.h> 

#define BREADSIZE 2048
#define AREADSIZE 128

int fd;
int port = 0;
char token[64] = {0,};
char mkpatch[32] = {0,};
unsigned long int baudrate = 0;
char bRead[AREADSIZE] = {0,};


void error_log(char *my_error) 
{ 
   time_t t;
   time(&t);
   FILE *f;
   f = fopen("ErrTelebot.log", "a"); 
   if(f == NULL)
    {
      printf("Error open ErrTelebot.log.\n");
      exit(0);
    }

   fprintf(f, "%s", ctime( &t));
   fprintf(f, "Error: %s\n\n", my_error);
   printf("Error: %s Write to ErrTelebot.log.\n", my_error);
   fclose(f);
   exit(0);
}


void read_conf()
{  
   FILE *mf;
   char str[64] = {0,};
   char *restr;
   mf = fopen ("Telebot.conf","r");
   if(mf == NULL) error_log("mf.");
   printf ("Open config file.\n");
   while(1)
    {
      restr = fgets(str, sizeof(str), mf);
      if(restr == NULL)
       {
         if(feof(mf) != 0) break; 
         else error_log("read from config file.");
       }

      if(strstr(str,"port=") != NULL) { port = atoi(strstr(str, "port=") + 5); printf("Port:%d_END\n", port); }
      char *p;
      if((p = strstr(str,"token=")) != NULL) 
       {
         int index = p - str;
         int i = 0;
         int ot = index + 6;
         for(; i <= 62; i++)
          {
            token[i] = str[ot];
            ot++;
            if(token[i] == '\n') 
             {
               token[i] = '\0';
               printf("Token:%s_END\n", token);
               break;
             }
          }
       }

      char *mkp;
      if((mkp = strstr(str,"mkpatch=")) != NULL) 
       {
         int index = mkp - str;
         int i = 0;
         int ot = index + 8;
         for(; i <= 14; i++)
          {
            mkpatch[i] = str[ot];
            ot++;
            if(mkpatch[i] == '\n') 
             {
               mkpatch[i] = '\0';
               printf("Mkpatch:%s_END\n", mkpatch);
               break;
             }
          }
       }

      char *sp;
      if((sp = strstr(str,"baudrate=")) != NULL) 
       {
         char mkspeed[8] = {0,};
         int index = sp - str;
         int i = 0;
         int ot = index + 9;
         for(; i <= 7; i++)
          {
            mkspeed[i] = str[ot];
            ot++;
            if(mkspeed[i] == '\n') 
             {
               baudrate = strtoul(mkspeed, NULL, 0);
               printf("Baudrate:%lu_END\n", baudrate);
               break;
             }
          }
       }

    } // END while

   printf ("Close config file.\n");
   if(fclose(mf) == EOF) error_log("mf EOF.");
} 


void child_kill() { printf("Child_kill.\n"); wait(NULL); } 

void SendMessage(char *chat_id, char *send_text) 
{
    char host[] = "api.telegram.org"; 
    char str[1024] = {0,};
    int lenstr = (int)strlen("/sendMessage HTTP/1.1\r\nHost: api.telegram.org\r\nContent-Type: application/json\r\nContent-Length: ");
    char json_str[128] = {0,};
    snprintf(json_str, 1 + 11 + (int)strlen(chat_id) + 9 + (int)strlen(send_text) + 2, "%s%s%s%s%s", "{\"chat_id\":", chat_id, ",\"text\":\"", send_text, "\"}");

//printf("Json_str:%s_END\n", json_str);
    int lenjson = (int)strlen(json_str);
    snprintf(str, 1 + 9 + (int)strlen(token) + lenstr + 3 + (int)strlen("\r\nConnection: close\r\n\r\n") + lenjson, "%s%s%s%d%s%s", "POST /bot", token, "/sendMessage HTTP/1.1\r\nHost: api.telegram.org\r\nContent-Type: application/json\r\nContent-Length: ", lenjson, "\r\nConnection: close\r\n\r\n", json_str);
    
//printf("\nSEND_POST:%d\n%s_END\n", (int)strlen(str), str);
    struct hostent *server; 
    struct sockaddr_in serv_addr;
    int sd = 0;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) error_log("socket in SM.");
    server = gethostbyname(host); 
    if (server == NULL) error_log("host in SM."); 
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(443);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length); 

    if(connect(sd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) error_log("connect.");
    SSL_CTX * sslctx = SSL_CTX_new(TLSv1_2_client_method());

    SSL * cSSL = SSL_new(sslctx);
    if(SSL_set_fd(cSSL, sd) == 0) error_log("SSL_set_fd in SM.");
    if(SSL_connect(cSSL) <= 0) error_log("SSL_connect in SM."); 
    int vsm = SSL_write(cSSL, str, (int)strlen(str));
    if(vsm <= 0)
     {
       SSL_free(cSSL);
       if(close(sd) == -1) error_log("close sd in SM.");
       error_log("vsm = SSL_write in SM.");            
     }

printf("\nSendMessage.\n");
    memset(str, 0, 1024); 
    int n = SSL_read(cSSL, str, 1022); 
    if(n <= 0)
     {
       SSL_free(cSSL);
       if(close(sd) == -1) error_log("close client_3.");
       printf("Err SSL_read in SM.\n");
     } 

//printf("\nReport_SM:%d\n%s\n", n, str);
    SSL_free(cSSL);
    SSL_CTX_free(sslctx);
    if(close(sd) == -1) error_log("close sd in SendMessage.");
}


void ardu_read_func(char *chat_id) 
 { 
   int i = 0;
   //int err_count1 = 0;
   int bytes = 0;
   memset(bRead, 0, AREADSIZE * sizeof(char));
   //counterr = 0;

   if((bytes = read(fd, bRead, AREADSIZE - 1)) == -1) error_log("Read_from_Arduino.");
   for(i = 0; i <= bytes; i++)
    {
      if(bRead[i] == '\n')
       {
         bRead[i] = 0; 
         break;
       }
    } 

   tcflush(fd, TCIFLUSH); 
printf("SM_from_Ardu:%s_%s\n\n", chat_id, bRead);
   SendMessage(chat_id, bRead);
  
 } // END ardu_read_func


/////////////////////////////////////////// open_port /////////////////////////////////////////////////
void open_port()  
 {   
   fd = open(mkpatch, O_RDWR | O_NOCTTY); 
   if(fd == -1) error_log("open /dev/ttyXXX.");
   else  
     {  
       struct termios options;  
       tcgetattr(fd, &options);   

       switch(baudrate)
       {
        case 4800:       
          cfsetispeed(&options, B4800); 
          cfsetospeed(&options, B4800); 
        break;

        case 9600:       
          cfsetispeed(&options, B9600); 
          cfsetospeed(&options, B9600); 
        break;

        case 19200:       
          cfsetispeed(&options, B19200); 
          cfsetospeed(&options, B19200); 
        break;

        case 38400:       
          cfsetispeed(&options, B38400); 
          cfsetospeed(&options, B38400); 
        break;

        case 57600:       
          cfsetispeed(&options, B57600); 
          cfsetospeed(&options, B57600); 
        break;

        case 115200:       
          cfsetispeed(&options, B115200); 
          cfsetospeed(&options, B115200); 
        break;

        default: 
          error_log("baudrate_port.");
        break;
       }

       options.c_cflag |= (CLOCAL | CREAD); 
       options.c_iflag = IGNCR;
       options.c_cflag &= ~PARENB;  
       options.c_cflag &= ~CSTOPB;  
       options.c_cflag &= ~CSIZE;  
       options.c_cflag |= CS8;  
       options.c_cc[VMIN] = 1;  
       options.c_cc[VTIME] = 1;  
       options.c_lflag = ICANON;  
       options.c_oflag = 0;  
       options.c_oflag &= ~OPOST; 
       tcflush(fd, TCIFLUSH);
       tcsetattr(fd, TCSANOW, &options);  
     }  
 }


int main() 
{
    read_conf();

    open_port(); 
    sleep(2);
    tcflush(fd, TCIFLUSH);

    ////////////////////////////////////    SSL    //////////////////////////////////////////
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_library_init();
    SSL_CTX * sslctx = SSL_CTX_new(TLSv1_2_server_method());
    /////////////////////////////    READ certificate    ////////////////////////////////////
    if(SSL_CTX_use_certificate_file(sslctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) error_log("use_certificate_file.");
    if(SSL_CTX_use_PrivateKey_file(sslctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) error_log("use_PrivateKey_file.");
    if(!SSL_CTX_check_private_key(sslctx)) error_log("check_private_key.");
    ///////////////////////////////////    SERVER    ////////////////////////////////////////
    int sd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sd < 0) error_log("descriptor socket.");
    int one = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
 
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    s_addr.sin_port = htons(port);

    if(bind(sd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) error_log("binding.");

    if(listen(sd, 5) == -1) 
     {
       close(sd);
       error_log("listen.");
     }

    char read_buffer[BREADSIZE] = {0,};
    int client = 0;
  
    while(1) 
     {  
        printf("Wait connection.\n");
        memset(read_buffer, 0, BREADSIZE);

        client = accept(sd, NULL, NULL); 

        if(client == -1) 
         {
           printf("Not cl accept.\n");
           if(close(client) == -1) error_log("close client_1.");
           continue;
         }

//printf("OK_1.\n");
        SSL *ssl = SSL_new(sslctx);
        if(SSL_set_fd(ssl, client) == 0) error_log("SSL_set_fd.");

        int acc = SSL_accept(ssl); 
        if(acc <= 0)
         { 
            SSL_free(ssl);
            if(close(client) == -1) error_log("close client_2.");
            printf("Not SSL_accept.\n");
            continue;
         }

//printf("OK2_acc:%d\n", acc);
 
        /////////////////////// FORK ///////////////////////////
        pid_t pid;  
        signal(SIGCHLD, child_kill);  
        pid = fork();
        
        if(pid != 0) 
         { 
            SSL_free(ssl);
            if(close(client) == -1) error_log("close client_pid.");
            continue;
         }

        int n = SSL_read(ssl, read_buffer, BREADSIZE - 2); // first SSL_read
        if(n <= 0)
         {
            SSL_free(ssl);
            if(close(client) == -1) error_log("close client_3.");
            printf("Disconnection:%d\n", n);
            exit(0);           
         } 

//printf("First SSL_read:%d %s\n", n, read_buffer);
//printf("OK_3.\n");

        if(strstr(read_buffer, token) == NULL) 
         { 
            SSL_free(ssl);
            if(close(client) == -1) error_log("close client_4.");
            printf("Not valid POST.\n");
            exit(0);
         }

//printf("OK_4.\n");
        if(strstr(read_buffer, "Content-Type: application/json") == NULL) 
         {
            SSL_free(ssl);
            if(close(client) == -1) error_log("close client_5.");
            printf("Not json.\n");
            exit(0);
         }

//printf("OK_5.\n");

        int len = atoi(strstr(read_buffer, "Content-Length: ") + strlen("Content-Length: "));

        memset(read_buffer, 0, BREADSIZE); 
        int m = SSL_read(ssl, read_buffer, len);  // second SSL_read
        if(m <= 0)
         {
            SSL_free(ssl);
            if(close(client) == -1) error_log("close client_8.");
            error_log("m = SSL_read.");            
         }

//printf("\nSecond SSL_read:%d %s\n", m, read_buffer);

        char *p;
        //memset(chat_id, 0, 16);
        char chat_id[16] = {0,};
        if((p = strstr(read_buffer, "chat\":{\"id\":")) != NULL) 
         {
           int index = p - read_buffer;
           int i = 0;
           int ot = index + 12;
           for(; i <= 14; i++)
            {
              chat_id[i] = read_buffer[ot];
              ot++;
              if(chat_id[i] == ',') 
               {
                 chat_id[i] = 0;
                 printf("\nchat_id:%s_END\n", chat_id);
                 break;
               }
            }
         }


        char *q;
        char msg_text[64] = {0,};
        //long int comanda = 0;
        if((q = strstr(read_buffer, "text\":\"")) != NULL) 
         {
           int index = q - read_buffer;
           int i = 0;
           int ot = index + 7;
           for(; i <= 62; i++)
            {
              msg_text[i] = read_buffer[ot];
              ot++;
              if(msg_text[i] == '"') 
               {
                 msg_text[i] = 0;
                 printf("Text:%s_END\n", msg_text);
                 //comanda = strtol(msg_text, NULL, 0);
                 //printf("Comanda:%ld_END\n", comanda);
                 break;
               }
            }
         }

        int v = SSL_write(ssl, "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n", 38);

        if(v <= 0)
         {
           SSL_free(ssl);
           if(close(client) == -1) error_log("v = SSL_write.");
         }

        SSL_free(ssl);
        if(close(client) == -1) error_log("close client_6.");


        if(msg_text[0] == 't' && msg_text[1] == 0)
         { 
           SendMessage(chat_id, "TEST"); 
         }
      
        else
         {
           char to_Ardu[128] = {0,};
           snprintf(to_Ardu, strlen(msg_text) + strlen(mkpatch) + 11, "echo '%s' > %s", msg_text, mkpatch);
           printf("\nto_Ardu:%s_END\n", to_Ardu);
           system(to_Ardu);
           ardu_read_func(chat_id);
         }

        exit(0); 
  
    } // END while(1) 
 
   if(close(sd) == -1) error_log("close sd client_7.");
}


// gcc -Wall -Wextra telebot.c -o telebot -lcrypto -lssl



Конфигурационный файл должен называться — Telebot.conf и находиться в одной папке с сервером.

Содержимое:

port=443
token=313334553:EomqdVs-77i9XE0hABw3AHZj4YLK7g3FKqk
mkpatch=/dev/ttyUSB0
baudrate=57600

В конце строк не должно быть пробелов.

После знаков равно впишите входящий порт сервера (его же нужно указать при создании webhooka), свой токен, путь к ардуине и baudrate.

Для установки webhooka воспользуйтесь такой командой:

curl -F "url=https://223.245.126.153:443/313334553:EomqdVs-77i9XE0hABw3AHZj4YLK7g3FKqk" -F "certificate=@public.pem" https://api.telegram.org/313334553:EomqdVs-77i9XE0hABw3AHZj4YLK7g3FKqk/setWebhook

То есть так (как советует API), чтоб после адреса шёл токен. Поскольку токен знаете только вы и показывать его никому нельзя, то он будет служить признаком правильного запроса к серверу.

Часть кода заимствована из этой статьи, там же описано создание ключей (на тот случай, если вы не знаете как это делать).

Для использования на роутере, нужно установить библиотеку openssl.

На этом всё.

