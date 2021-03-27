/**
 * @file lib.h
 * @author Taras Kuts
 * @date 8 September
 * @brief File contains declarations and implementation for all functions that use Server
 * This file has implementation for all functions which involve in the creating client.For example  function  for creating socket or function for creating connect
 * And all functions who are responsible for the server logic is here.
 *
 */
#ifndef LIB_H
#define LIB_H

int socketCreate (int socketType);/**<This function serves to create socket
**@param Socket
*@return SocketID
*/
int connectCreate (int socket, const char * ip, int port);/**<This function serves to create connection with klient
**@param Socket,ip,port
*/
void recieveData (int socket);/**<This function serves to send and receive data
**@param Socket
*/
int connectionClose (int socket);/**<This function serves to close connection if the situation requires it
**@param Socket
*/
int portBinding (int socket, int port);/**<This function serves to get IP  of the server
**@param Socket,port
*@return Binded or not
*/
int listenClient (int socket, int backlog);/**<This function serves to listen client ,is he connected or not
**@param Socket,backLoG
*@return listening
*/
void acceptClients (int socket, void (*func)());/**<This function serves to accept new  connections if the situation requires it
**@param Socket,func
*/
void authorization();/**<This function serves to create authorization,finally in this function we will who was connected(Admin,Egzamenator,Student)
*and send this information to the client
*
*/
char checkLogin();/**<This function serves to check login
*/
void getStudList(int socket);/**<This function serves to get the list of student and send to client
*@param socket
*/
void getAllList();/**<This function serves to get all lists of student and send to client
*
*/
void getExList(int socket);/**<This function serves to get the list of exams and send to client
*@param socket
*/
void getGroupList(int socket);/**<This function serves to get the list of student in selected group and send to client
*@param socket
*/
void getStudGroupList(int socket, char* groupName);
/**<This function serves to get the list of student and send to client
*@param socket,group name
*/
void getRightAnswers(int socket, char* examName);/**<This function serves to get the list of right answers
*@param socket,name of the exam
*/
void getStudResult(int socket, char* studLogin);/**<This function serves to get the result of the exam and send it to the client
*@param socket,login of the student
*/
void checkStudExam(int socket, char* examName);/**<This function serves to check exam
*@param socket,examName
*/
long hash(char *str);
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <stdio_ext.h>
#include "lib.h"
#define PORT 13664
#define MAX_BUFFER 1024
#define AUTH_BUF 24
//FUNC DECLARATION

char login[AUTH_BUF];/**This array serves for containing logins */
char password[AUTH_BUF];/**This array serves for containing passwords */
char permission[1];/**This array serves for giving permission */
int isClientLoginIn = 0;/**This array serves for checking login of client */
char errorMsg[MAX_BUFFER] = "error";/**This array serves for containing errors */
char okMsg[MAX_BUFFER] = "ok";/**This array serves for containing "ok" */
char studentList[MAX_BUFFER];/**This array serves for containing list of the student */
char accsList[MAX_BUFFER];/**This array serves for containing list of account */
char examsList[MAX_BUFFER];/**This array serves for containing list of exams */
char groupList[MAX_BUFFER];/**This array serves for containing list of groups */
char studGroupList[MAX_BUFFER];/**This array serves for containing list of student in groups */
char rightAnswersExam[MAX_BUFFER];/**This array serves for containing right answers */
char studResult[MAX_BUFFER];/**This array serves for containing result of the exam */
//CLIENT&SERVER:
int socketCreate (int socketType)
{
    int socketId;

    socketId = socket(AF_INET, socketType, 0);

    if (socketId == -1)
    {
        fprintf(stderr, "BLAD: nie moge utworzyc socketu: %s\n", strerror(errno));
        fflush(stdout);
        exit(EXIT_FAILURE);
        return socketId;
    }

    printf("1) Socket stworzony %d\n", socketId);

    return socketId;
}
//SERVER
int portBinding (int socket, int port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(port);
    memset( &( address.sin_zero ), '\0', 8 );

    int binded = bind(socket, (struct sockaddr *) & address, sizeof(struct sockaddr));

    if (binded == -1)
    {
        fprintf(stderr, "BLAD: nie moge przesluchac portu %d: %s\n", port, strerror(errno));
        fflush(stdout);
        exit(EXIT_FAILURE);
        return binded;
    }

    printf("2) Sluchanie portu %d\n", port);

    return binded;
}
int listenClient (int socket, int backlog)
{
    int listening = listen(socket, backlog);

    if (listening == -1)
    {
        fprintf(stderr, "BLAD: nie moge sluchac socketu %d z backlogiem %d: %s\n", socket, backlog, strerror(errno));
        fflush(stdout);
        exit(EXIT_FAILURE);
        return listening;
    }

    printf("3) sluchanie socketu %d z backlogiem %d\n", socket, backlog);

    return listening;
}
void acceptClients (int socket, void (*func)())
{
    int acceptedSocket;
    struct sockaddr_in address;
    socklen_t address_len = sizeof(struct sockaddr_in);
    struct in_addr clientIp;
    char * ip;
    unsigned short port;


    while (1==1)
    {
        acceptedSocket = accept(socket, (struct sockaddr *) & address, & address_len);
        clientIp = address.sin_addr;
        ip = inet_ntoa(clientIp);
        port = ntohs(address.sin_port);

        if (acceptedSocket == -1)
        {
            fprintf(stderr, "BLAD: nie moge zaakceptowac klienta %s:%d: %s\n", ip, port, strerror(errno));
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
        pid_t pid = fork();
        if(pid < 0)
        {
            printf("Nie moge utworzyc nowego procesu");
            fflush(stdout);
        }
        if(pid == 0)
        {
            func(acceptedSocket, socket); //Nowy proces obrobki klienta
        }
        else
        {

        }
    }
}
long hash(char *str)
    {
        long hash = 5381;
        char c;

        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }
void newConnection(int socket)
{
    char buff[MAX_BUFFER];
    int n;
    for(;;)
    {
        bzero(buff,MAX_BUFFER);
        if(isClientLoginIn == 0)
        {
            authorization(socket);
            printf("isClientLogin: %d\n", isClientLoginIn);
            fflush(stdout);
        }
        else
        {
            printf("Permission: %s\n", permission);
            fflush(stdout);
            bzero(buff, sizeof(buff));
            n = read(socket,buff,sizeof(buff));
            if(n == 0)
            {
                exit(0);
            }
            printf("Command:%s, byte:%d\n", buff,n);
            fflush(stdout);
            if(*permission == '1') //dla admina
            {
                printf("%s zalogowany jako admin\n", login);
                fflush(stdout);
                if (strcmp(buff, "logout") == 0)
                {
                    printf("logout\n");
                    fflush(stdout);
                    memset(login,'0',AUTH_BUF);
                    memset(password,'0',AUTH_BUF);
                    isClientLoginIn = 0;
                }
                if (strcmp("add", buff) == 0)
                {
                    printf("add\n");
                    fflush(stdout);
                    char newLogin[MAX_BUFFER];
                    char newPassword[MAX_BUFFER];
                    char newPermission[MAX_BUFFER];
                    long passHash;
                    int newPermissionInt = -1;
                    n = read(socket,newLogin,sizeof(newLogin));
                    printf("newLogin: %s", newLogin);
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    n = read(socket,newPassword,sizeof(newPassword));

                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    passHash = hash(newPassword);
                    sprintf(newPassword, "%ld", passHash);
                    printf("newPass in hash: %s", newPassword);
                    fflush(stdout);
                    n = read(socket,newPermission,sizeof(newPassword));
                    printf("newPermission: %s", newPermission);
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    newPermissionInt = atoi(newPermission);
                    if(newPermissionInt < 1 && newPermissionInt > 3)
                    {
                        write(socket,errorMsg,sizeof(errorMsg));
                    }
                    else
                    {
                        printf("OpenFile\n");
                        fflush(stdout);
                        FILE * f;
                        if ( ( f = fopen("Users.txt", "a") ) != NULL )
                        {
                            printf("File openned\n");
                            fflush(stdout);
                            char newLineUser[MAX_BUFFER * 3];
                            sprintf(newLineUser,"%s %s %d\n", newLogin, newPassword, newPermissionInt);
                            printf("newLineUser: %s", newLineUser);
                            fflush(stdout);
                            int result = fputs(newLineUser,f);
                            if(result)
                            {
                                write(socket,okMsg,sizeof(okMsg));
                            }
                            else
                            {
                                write(socket,errorMsg,sizeof(errorMsg));
                            }
                            fclose(f);

                        }
                        else
                        {
                            fprintf(stderr, "can't open file Users.txt\n");
                            fflush(stdout);
                            write(socket,errorMsg,sizeof(errorMsg));
                            break;
                        }
                    }


                }
                if (strcmp("remove", buff) == 0)
                {
                    printf("remove\n");
                    fflush(stdout);
                    FILE * f;
                    if ( (f = fopen ("Users.txt","r")) != NULL )
                    {
                        char loginTemp[MAX_BUFFER];
                        char pattern [MAX_BUFFER];
                        int n = read(socket,loginTemp,sizeof(loginTemp));
                        if(n < 0)
                        {
                            write(socket,errorMsg,sizeof(errorMsg));
                            break;
                        }
                        fseek (f,0,SEEK_END);
                        char storyData[ftell (f)+1];
                        char * ptr = storyData;
                        *ptr = 0;
                        fseek (f,0,SEEK_SET);
                        fflush(stdout);
                        __fpurge(stdin);
                        while ( fgets (pattern,MAX_BUFFER,f) )
                        {
                            fflush(stdout);
                            if ( (n = (strncmp (loginTemp,pattern,strlen(loginTemp) - 1))) != 0 )
                            {
                                strcat (ptr,pattern);
                                ptr += strlen (pattern);
                            }
                            fflush(stdout);
                            __fpurge(stdin);
                        }

                        fclose (f);
                        f = fopen ("Users.txt","w");
                        fputs (storyData,f);
                        fclose (f);
                        write(socket,okMsg,sizeof(okMsg));
                    }
                    else
                    {
                        fprintf(stderr, "can't open file Users.txt\n");
                        fflush(stdout);
                        write(socket,errorMsg,sizeof(errorMsg));
                        break;
                    }

                }
                if (strcmp("studList", buff) == 0)
                {
                    printf("studList\n");
                    fflush(stdout);
                    getStudList(socket);
                    write(socket,studentList,sizeof(studentList));
                }
                if (strcmp("allList", buff) == 0)
                {
                    printf("allList\n");
                    fflush(stdout);
                    getAllList();
                    write(socket,accsList,sizeof(accsList));
                }
                if (strcmp("examsList", buff) == 0)
                {
                    printf("examsList\n");
                    fflush(stdout);
                    getExList(socket);
                    write(socket,examsList,sizeof(examsList));
                }
                if (strcmp("createGroup", buff) == 0)
                {
                    printf("CreateGroup\n");
                    fflush(stdout);
                    FILE * f;
                    char groupName[MAX_BUFFER];
                    fflush(stdout);
                    n = read(socket,groupName,sizeof(groupName));
                    printf("groupName: %s, byte:%d\n", groupName, n);
                    fflush(stdout);
                    if(n < 0)
                    {
                        write(socket,errorMsg,sizeof(errorMsg));
                        break;
                    }
                    char PATH[MAX_BUFFER * 2] = "tests/";
                    //strcat(PATH,groupName);
                    //strcat(PATH,".txt");
                    sprintf(PATH,"tests/%s.txt",groupName);
                    printf("%s", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "a")) != NULL)
                    {
                        write(socket,okMsg,sizeof(okMsg));
                        fclose(f);
                    }
                    else
                    {
                        write(socket,errorMsg,sizeof(errorMsg));
                    }
                    sprintf(PATH,"tests/list.txt");
                    printf("%s\n", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "a")) != NULL)
                    {
                        fputs(groupName,f);
                        fputs("\n",f);
                        //write(socket,okMsg,sizeof(okMsg));
                        fclose(f);
                    }
                    else
                    {
                        //write(socket,errorMsg,sizeof(errorMsg));
                        fclose(f);
                    }
                }
                if (strcmp("addToGroup", buff) == 0)
                {
                    getStudList(socket);
                    printf("%s", studentList);
                    fflush(stdout);
                    printf("addToGroup\n");
                    fflush(stdout);
                    FILE * f = NULL;
                    int result;
                    int cnt = 0;
                    int j = 0;
                    char groupName[MAX_BUFFER];
                    fflush(stdout);
                    n = read(socket,groupName,sizeof(groupName));
                    fflush(stdout);
                    if(n < 0)
                    {
                        write(socket,errorMsg,sizeof(errorMsg));
                        break;
                    }
                    printf("Add groupName: %s\n", groupName);
                    fflush(stdout);
                    char PATH[MAX_BUFFER * 2] = "tests/";
                    sprintf(PATH,"tests/%s.txt",groupName);
                    printf("PATH: %s\n", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "a")) != NULL)
                    {
                        n = read(socket,buff,sizeof(buff));
                        if(n < 0)
                        {
                            write(socket,errorMsg,sizeof(errorMsg));
                            break;
                        }
                        cnt++;

                        printf("Add studentToAdd: %s", buff);
                        fflush(stdout);
                        while(strcmp(buff,"stop!\n") != 0 && cnt < 21)
                        {
                            for(int i = 0; i < (int)strlen(studentList); i++)
                            {
                                __fpurge(stdin);
                                fflush(stdout);
                                if(buff[j] == studentList[i])
                                {
                                    j++;
                                    if(buff[j] == '\n')
                                    {
                                        result = fputs(buff,f);
                                        if(result >= 0)
                                        {
                                            write(socket,okMsg,sizeof(okMsg));
                                            printf("Add studentDodany: %s\n", okMsg);
                                            fflush(stdout);
                                            cnt++;
                                            j = 0;
                                        }
                                        else
                                        {
                                            write(socket,errorMsg,sizeof(errorMsg));
                                            printf("Add studentDodany: %s", errorMsg);
                                            fflush(stdout);
                                            j=0;
                                        }
                                        break;
                                    }
                                }
                                else
                                {
                                    j = 0;
                                    for(; studentList[i] != '\n'; i++)
                                    {
                                        printf("skip for studentList[i]:%c\n",studentList[i]);
                                        fflush(stdout);
                                    }
                                    if(i == (int)strlen(studentList) - 1)
                                        write(socket,errorMsg,sizeof(errorMsg));
                                }

                            }
                            n = read(socket,buff,sizeof(buff));
                            if(n < 0)
                            {
                                write(socket,errorMsg,sizeof(errorMsg));
                                break;
                            }
                            printf("Add studentToAdd: %s\n", buff);
                            fflush(stdout);
                        }
                        fclose(f);
                    }
                    else
                    {
                        write(socket,errorMsg,sizeof(errorMsg));
                    }

                }
            }
            if(*permission == '2') //dla egzaminatora
            {
                printf("%s zalogowany jako egzaminator", login);
                fflush(stdout);
                if (strcmp("studList", buff) == 0)
                {
                    printf("studList\n");
                    fflush(stdout);
                    getStudList(socket);
                    write(socket,studentList,sizeof(studentList));
                }
                if (strcmp("allList", buff) == 0)
                {
                    printf("allList\n");
                    fflush(stdout);
                    getAllList();
                    write(socket,accsList,sizeof(accsList));
                }
                if (strcmp("examsList", buff) == 0)
                {
                    printf("examsList\n");
                    fflush(stdout);
                    getExList(socket);
                    write(socket,examsList,sizeof(examsList));
                }
                if (strcmp("groupsList", buff) == 0)
                {
                    printf("groupsList\n");
                    fflush(stdout);
                    getGroupList(socket);
                    write(socket,groupList,sizeof(groupList));
                }
                if (strcmp("logout", buff) == 0)
                {
                    printf("logout\n");
                    fflush(stdout);
                    memset(login,'0',AUTH_BUF);
                    memset(password,'0',AUTH_BUF);
                    isClientLoginIn = 0;
                }
                if (strcmp("createExam", buff) == 0)
                {
                    char NameExam[MAX_BUFFER];
                    char NameExamNL[MAX_BUFFER];
                    char questionCnt[MAX_BUFFER];
                    char Question[MAX_BUFFER];
                    char DifferentAnswers[MAX_BUFFER];
                    char RightAnswer[MAX_BUFFER];
                    char groupForExam[MAX_BUFFER];
                    int qCnt = 0;
                    char answersCnt[MAX_BUFFER];
                    int aCnt;
                    n = read(socket,NameExam,sizeof(NameExam));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    printf("CreateExam\n");
                    fflush(stdout);
                    FILE * f;
                    fflush(stdout);
                    printf("NameExam: %s, byte:%d\n", NameExam, n);
                    fflush(stdout);
                    if(n < 0)
                    {
                        write(socket,errorMsg,sizeof(errorMsg));
                        break;
                    }
                    strcpy(NameExamNL,NameExam);
                    NameExamNL[strlen(NameExamNL)] = '\n';
                    char PATH[MAX_BUFFER * 2] = "exams/";
                    //strcat(PATH,groupName);
                    //strcat(PATH,".txt");
                    sprintf(PATH,"exams/%s.txt",NameExam);
                    printf("%s\n", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "w")) != NULL)
                    {
                        fputs(NameExamNL,f);
                        //write(socket,okMsg,sizeof(okMsg));
                        fclose(f);
                    }
                    else
                    {
                        //write(socket,errorMsg,sizeof(errorMsg));
                        fclose(f);
                    }
                    n = read(socket,questionCnt,sizeof(questionCnt));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    qCnt = questionCnt[0] - '0';
                    for(int i = 0; i < qCnt; i++)
                    {
                        printf("qCnt: %d", qCnt);
                        fflush(stdout);
                        n = read(socket,Question,sizeof(Question));
                        fflush(stdout);
                        if(n < 0)
                        {
                            printf("Error read from socket");
                            fflush(stdout);
                            break;
                        }
                        sprintf(PATH,"exams/%s_questions.txt",NameExam);
                        printf("%s\n", PATH);
                        fflush(stdout);
                        if((f = fopen(PATH, "a")) != NULL)
                        {
                            if(i == 0)
                            {
                                fputs(questionCnt,f);
                            }
                            fputs(Question,f);
                            //write(socket,okMsg,sizeof(okMsg));
                            fclose(f);
                        }
                        else
                        {
                            //write(socket,errorMsg,sizeof(errorMsg));
                            fclose(f);
                        }
                        n = read(socket,answersCnt,sizeof(answersCnt));
                        fflush(stdout);
                        if(n < 0)
                        {
                            printf("Error read from socket");
                            fflush(stdout);
                            break;
                        }
                        aCnt = answersCnt[0] - '0';
                        printf("aCnt: %d", aCnt);
                        fflush(stdout);
                        for(int j = 0; j < aCnt; j++)
                        {
                            n = read(socket,DifferentAnswers,sizeof(DifferentAnswers));
                            fflush(stdout);
                            if(n < 0)
                            {
                                printf("Error read from socket");
                                fflush(stdout);
                                break;
                            }
                            sprintf(PATH,"exams/%s_answers.txt",NameExam);
                            printf("%s\n", PATH);
                            fflush(stdout);
                            if((f = fopen(PATH, "a")) != NULL)
                            {
                                if(j == 0)
                                {
                                    fputs(answersCnt,f);
                                }

                                fputs(DifferentAnswers,f);
                                //write(socket,okMsg,sizeof(okMsg));
                                fclose(f);
                            }
                            else
                            {
                                //write(socket,errorMsg,sizeof(errorMsg));
                                fclose(f);
                            }
                        }
                        n = read(socket,RightAnswer,sizeof(RightAnswer));
                        fflush(stdout);
                        if(n < 0)
                        {
                            printf("Error read from socket");
                            fflush(stdout);
                            break;
                        }
                        sprintf(PATH,"exams/%s_rightAnswers.txt",NameExam);
                        printf("%s\n", PATH);
                        fflush(stdout);
                        if((f = fopen(PATH, "a")) != NULL)
                        {
                            fputs(RightAnswer,f);
                            //write(socket,okMsg,sizeof(okMsg));
                            fclose(f);
                        }
                        else
                        {
                            //write(socket,errorMsg,sizeof(errorMsg));
                            fclose(f);
                        }
                    }
                    getGroupList(socket);
                    write(socket,groupList,sizeof(groupList));
                    n = read(socket,groupForExam,sizeof(groupForExam));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    printf("GroupForExam: %s\n", groupForExam);
                    if(strcmp(groupForExam, "err") == 0)
                    {
                        bzero(PATH,sizeof(PATH));
                        sprintf(PATH,"exams/%s_rightAnswers.txt",NameExam);
                        if((f = fopen(PATH, "w")) != NULL)
                        {
                            fclose(f);
                        }
                        bzero(PATH,sizeof(PATH));
                        sprintf(PATH,"exams/%s_answers.txt",NameExam);
                        if((f = fopen(PATH, "w")) != NULL)
                        {
                            fclose(f);
                        }
                        bzero(PATH,sizeof(PATH));
                        sprintf(PATH,"exams/%s_questions.txt",NameExam);
                        if((f = fopen(PATH, "w")) != NULL)
                        {
                            fclose(f);
                        }
                    }
                    else
                    {
                    sprintf(PATH,"exams/%s.txt",NameExam);
                    printf("%s\n", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "a")) != NULL)
                    {
                        fputs(groupForExam,f);
                        fputs("\n",f);
                        //write(socket,okMsg,sizeof(okMsg));
                        fclose(f);
                    }
                    sprintf(PATH,"exams/list.txt");
                    printf("%s\n", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "a")) != NULL)
                    {
                        fputs(NameExamNL,f);
                        //write(socket,okMsg,sizeof(okMsg));
                        fclose(f);
                    }
                    else
                    {
                        //write(socket,errorMsg,sizeof(errorMsg));
                        fclose(f);
                    }
                    }

                }
                if (strcmp("chooseGroupToExam", buff) == 0)
                {
                    printf("chooseGroupToExam\n");
                    char NameExam[MAX_BUFFER];
                    char NameGroup[MAX_BUFFER];
                    char NameExamNL[MAX_BUFFER];
                    n = read(socket,NameExam,sizeof(NameExam));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    n = read(socket,NameGroup,sizeof(NameGroup));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    FILE * f;
                    fflush(stdout);
                    printf("NameExam: %s, byte:%d\n", NameExam, n);
                    fflush(stdout);
                    if(n < 0)
                    {
                        write(socket,errorMsg,sizeof(errorMsg));
                        break;
                    }
                    strcpy(NameExamNL,NameExam);
                    NameExamNL[strlen(NameExamNL)] = '\n';
                    char PATH[MAX_BUFFER * 2] = "exams/";
                    //strcat(PATH,groupName);
                    //strcat(PATH,".txt");
                    sprintf(PATH,"exams/%s.txt",NameExam);
                    printf("%s\n", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "a")) != NULL)
                    {
                        fputs(NameGroup,f);
                        //write(socket,okMsg,sizeof(okMsg));
                        fclose(f);
                    }
                    else
                    {
                        //write(socket,errorMsg,sizeof(errorMsg));
                        fclose(f);
                    }
                }
            }
            if(*permission == '3') //dla studenta
            {
                printf("%s zalogowany jako student\n", login);
                fflush(stdout);
                if (strcmp("logout", buff) == 0)
                {
                    printf("logout\n");
                    fflush(stdout);
                    memset(login,'0',AUTH_BUF);
                    memset(password,'0',AUTH_BUF);
                    isClientLoginIn = 0;
                    *permission = '0';
                }
                if (strcmp("checkResults", buff) == 0)
                {
                    printf("checkResults\n");
                    fflush(stdout);
                    char studLogin[AUTH_BUF];
                    n = read(socket,studLogin,sizeof(studLogin));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    getStudResult(socket,studLogin);
                    write(socket,studResult,sizeof(studResult));
                }
                if (strcmp("chooseExam", buff) == 0)
                {
                    char examName[MAX_BUFFER];
                    char pattern[MAX_BUFFER];
                    char isExamStart[MAX_BUFFER];
                    char storyData[MAX_BUFFER] = {0};
                    char examQuestions[MAX_BUFFER];
                    char examAnswers[MAX_BUFFER];
                    char * ptr = storyData;
                    FILE * f;
                    fflush(stdout);
                    printf("chooseExam");
                    fflush(stdout);
                    getExList(socket);
                    write(socket,examsList,sizeof(examsList));
                    n = read(socket,examName,sizeof(examName));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }

                    char PATH[MAX_BUFFER*2];
                    sprintf(PATH,"exams/%s.txt",examName);
                    printf("%s\n", PATH);
                    fflush(stdout);
                    if((f = fopen(PATH, "r")) != NULL)
                    {
                        printf("exam openned \n");
                        fflush(stdout);
                        fgets(pattern, MAX_BUFFER, f);
                        fgets(pattern, MAX_BUFFER, f);
                        fclose(f);
                    }
                    else
                    {
                        printf("exam not openned \n");
                        fflush(stdout);
                        //write(socket,errorMsg,sizeof(errorMsg));
                        fclose(f);
                    }
                    pattern[strlen(pattern) - 1] = '\0';
                    printf("Patter: %s \n", pattern);
                    fflush(stdout);
                    getStudGroupList(socket, pattern);
                    write(socket,login,sizeof(login));
                    write(socket,studGroupList,sizeof(studGroupList));
                    n = read(socket,isExamStart,sizeof(isExamStart));
                    fflush(stdout);
                    if(n < 0)
                    {
                        printf("Error read from socket");
                        fflush(stdout);
                        break;
                    }
                    printf("After read, %s\n",isExamStart);
                    fflush(stdout);
                    if(strcmp(isExamStart,"no") == 0)
                    {
                        printf("Student not allowed to exam\n");
                        break;
                    }
                    sprintf(PATH,"exams/%s_questions.txt",examName);
                    memset(pattern,'\0',MAX_BUFFER);
                    if((f = fopen(PATH, "r")) != NULL)
                    {
                        while (fgets(pattern, MAX_BUFFER, f) != NULL)
                        {
                            strcat(ptr,pattern);
                            ptr += strlen(pattern);
                            fflush(stdout);
                            __fpurge(stdin);
                        }
                        fclose(f);
                        strcpy(examQuestions,storyData);
                    }
                    else
                    {
                        printf("file not open");
                        fflush(stdout);
                        fclose(f);
                    }
                    printf("ExamQuestions: %s\n",examQuestions);
                    fflush(stdout);
                    memset(PATH,'\0', strlen(PATH));
                    write(socket,examQuestions,sizeof(examQuestions));
                    sprintf(PATH,"exams/%s_answers.txt",examName);
                    memset(storyData,'\0',MAX_BUFFER);
                    memset(pattern,'\0',MAX_BUFFER);
                    ptr = storyData;
                    if((f = fopen(PATH, "r")) != NULL)
                    {
                        while (fgets(pattern, MAX_BUFFER, f) != NULL)
                        {
                            strcat(ptr,pattern);
                            ptr += strlen(pattern);
                            fflush(stdout);
                            __fpurge(stdin);
                        }
                        fclose(f);
                        strcpy(examAnswers,storyData);
                    }
                    else
                    {
                        fclose(f);
                    }
                    printf("examAnswers: %s\n",examAnswers);
                    fflush(stdout);
                    write(socket,examAnswers,sizeof(examAnswers));
                    checkStudExam(socket,examName);

                }
            }
        }
    }
}

void checkStudExam(int socket, char* examName)
{
    int n;
    char studAnswers[MAX_BUFFER];
    char studLogin[AUTH_BUF];
    char aCnt[MAX_BUFFER];
    int aCntInt = 0;
    char studAnsw[MAX_BUFFER] = {'\0'};
    char rightAnsw[MAX_BUFFER] = {'\0'};
    int ra = 0;
    int sa = 0;
    int i = 0;
    int studMark = 0;
    char studMarkStr[MAX_BUFFER] = { '\0' };
    char result[MAX_BUFFER * 4];
    FILE *f;
    char PATH[MAX_BUFFER * 2];
    n = read(socket,studLogin,sizeof(studLogin));
    fflush(stdout);
    if(n < 0)
    {
        printf("Error read from socket");
        fflush(stdout);
    }
    n = read(socket,studAnswers,sizeof(studAnswers));
    fflush(stdout);
    if(n < 0)
    {
        printf("Error read from socket");
        fflush(stdout);
    }
    n = read(socket,aCnt,sizeof(aCnt));
    fflush(stdout);
    if(n < 0)
    {
        printf("Error read from socket");
        fflush(stdout);
    }
    aCntInt = aCnt[0] - '0';
    getRightAnswers(socket,examName);
    printf("studLogin %s\nstudAnswers %s\nrightAnswers %s\n",studLogin, studAnswers, rightAnswersExam);
    while(aCntInt > 0)
    {
        while(studAnswers[sa] != '\n')
        {
            studAnsw[i] = studAnswers[sa];
            i++;
            sa++;
        }
        i = 0;
        sa++;
        while(rightAnswersExam[ra] != '\n')
        {
            rightAnsw[i] += rightAnswersExam[ra];
            i++;
            ra++;
        }
        i = 0;
        ra++;
        printf("studAnsw %s\nrightAnsw %s\n",studAnsw, rightAnsw);
        fflush(stdout);
        if(strcmp(studAnsw,rightAnsw) == 0)
        {
            studMark++;
        }
        memset(studAnsw,'\0', strlen(studAnsw));
        memset(rightAnsw,'\0', strlen(rightAnsw));
        aCntInt--;
    }
    studMarkStr[0] = studMark + '0';
    write(socket,studMarkStr,sizeof(studMarkStr));
    sprintf(result,"%s %s/%s",examName, studMarkStr, aCnt);
    printf("\n%s",result);
    sprintf(PATH,"students/%s.txt",studLogin);
    if((f = fopen(PATH,"a")) != NULL)
    {
        fputs(result,f);
        fclose(f);
    }
    else
    {
        printf("Can't open file");
        fclose(f);
    }
}
void getStudResult(int socket, char* studLogin)
{
    memset(studResult,'\0', strlen(studResult));
    FILE * f;
    char PATH[MAX_BUFFER];
    sprintf(PATH,"students/%s.txt", studLogin);
    if ( ( f = fopen(PATH, "r") ) != NULL )
    {
        char pattern [MAX_BUFFER];
        char storyData[MAX_BUFFER] = {0};
        char * ptr = storyData;
        *ptr = 0;
        while ( fgets(pattern, MAX_BUFFER, f) )
        {
            strcat(ptr,pattern);
            ptr += strlen(pattern);
            fflush(stdout);
            __fpurge(stdin);

        }
        fclose(f);
        strcpy(studResult,storyData);
        fflush(stdout);
    }
    else
    {
        fprintf(stderr, "can't open file %s.txt\n", studLogin);
        fflush(stdout);
        write(socket,errorMsg,sizeof(errorMsg));
    }
}

void getRightAnswers(int socket, char* examName)
{
    FILE * f;
    char PATH[MAX_BUFFER];
    sprintf(PATH,"exams/%s_rightAnswers.txt", examName);
    if ( ( f = fopen(PATH, "r") ) != NULL )
    {
        char pattern [MAX_BUFFER];
        char storyData[MAX_BUFFER] = {0};
        char * ptr = storyData;
        *ptr = 0;
        while ( fgets(pattern, MAX_BUFFER, f) )
        {
            strcat(ptr,pattern);
            ptr += strlen(pattern);
            fflush(stdout);
            __fpurge(stdin);

        }
        fclose(f);
        strcpy(rightAnswersExam,storyData);
        fflush(stdout);
    }
    else
    {
        fprintf(stderr, "can't open file Users.txt\n");
        fflush(stdout);
        write(socket,errorMsg,sizeof(errorMsg));
    }
}

void getStudList(int socket)
{
    FILE * f;
    if ( ( f = fopen("Users.txt", "r") ) != NULL )
    {
        char pattern [MAX_BUFFER];
        char storyData[MAX_BUFFER] = {0};
        char * ptr = storyData;
        char loginTemp[MAX_BUFFER] = {0};
        *ptr = 0;
        while ( fgets(pattern, MAX_BUFFER, f) )
        {
            int i = 0;
            if(pattern[strlen(pattern) - 2] == '3')
            {
                while(pattern[i] != ' ')
                {
                    loginTemp[i] = pattern[i];
                    i++;
                    fflush(stdout);
                }
                loginTemp[i] = '\n';
                fflush(stdout);
                strcat(ptr,loginTemp);
                ptr += strlen(loginTemp);
                memset(loginTemp, '\0', strlen(loginTemp));
            }
            fflush(stdout);
            __fpurge(stdin);
        }
        fclose(f);
        strcpy(studentList,storyData);
        printf("StudentList:\n%s", studentList);
        fflush(stdout);
    }
    else
    {
        fprintf(stderr, "can't open file Users.txt\n");
        fflush(stdout);
        write(socket,errorMsg,sizeof(errorMsg));
    }
}
void getAllList(int socket)
{
    FILE * f;
    if ( ( f = fopen("Users.txt", "r") ) != NULL )
    {
        char pattern [MAX_BUFFER];
        char storyData[MAX_BUFFER] = {0};
        char * ptr = storyData;
        char loginTemp[MAX_BUFFER] = {0};
        *ptr = 0;
        while ( fgets(pattern, MAX_BUFFER, f) )
        {
            int i = 0;
            printf("%s", pattern);
            fflush(stdout);
            while(pattern[i] != ' ')
            {
                loginTemp[i] = pattern[i];
                i++;
            }
            loginTemp[i] = '\n';
            strcat(ptr,loginTemp);
            ptr += strlen(loginTemp);
            memset(loginTemp, '\0', strlen(loginTemp));
            fflush(stdout);
            __fpurge(stdin);

        }
        fclose(f);
        strcpy(accsList,storyData);
        printf("AccountList:\n%s", accsList);
        fflush(stdout);
    }
    else
    {
        fprintf(stderr, "can't open file Users.txt\n");
        fflush(stdout);
        write(socket,errorMsg,sizeof(errorMsg));
    }
}
void getExList(int socket)
{
    FILE * f;
    char PATH[MAX_BUFFER * 2];
    char name[MAX_BUFFER] = "list";
    sprintf(PATH,"exams/%s.txt",name);

    if ( ( f = fopen(PATH, "r") ) != NULL )
    {
        printf("PATH: %s\n",PATH);
        fflush(stdout);
        char pattern [MAX_BUFFER];
        char storyData[MAX_BUFFER] = {0};
        char * ptr = storyData;
        *ptr = 0;
        while ( fgets(pattern, MAX_BUFFER, f) )
        {
            strcat(ptr,pattern);
            ptr += strlen(pattern);
            fflush(stdout);
            __fpurge(stdin);

        }
        fclose(f);
        strcpy(examsList,storyData);
        printf("Exams:\n%s", examsList);
        fflush(stdout);
    }
    else
    {
        fprintf(stderr, "can't open file exams/list.txt\n");
        fflush(stdout);
        write(socket,errorMsg,sizeof(errorMsg));
    }
}
void getGroupList(int socket)
{
    FILE * f;
    char PATH[MAX_BUFFER * 2];
    char name[MAX_BUFFER] = "list";
    sprintf(PATH,"tests/%s.txt",name);

    if ( ( f = fopen(PATH, "r") ) != NULL )
    {
        printf("PATH: %s\n",PATH);
        fflush(stdout);
        char pattern [MAX_BUFFER];
        char storyData[MAX_BUFFER] = {0};
        char * ptr = storyData;
        *ptr = 0;
        while ( fgets(pattern, MAX_BUFFER, f) )
        {
            strcat(ptr,pattern);
            ptr += strlen(pattern);
            fflush(stdout);
            __fpurge(stdin);

        }
        fclose(f);
        strcpy(groupList,storyData);
        printf("Groups:\n%s", groupList);
        fflush(stdout);
    }
    else
    {
        fprintf(stderr, "can't open file tests/list.txt\n");
        fflush(stdout);
        write(socket,errorMsg,sizeof(errorMsg));
    }
}
void getStudGroupList(int socket, char* groupName)
{
    FILE * f;
    char PATH[MAX_BUFFER * 2];
    sprintf(PATH,"tests/%s.txt",groupName);

    if ( ( f = fopen(PATH, "r") ) != NULL )
    {
        printf("PATH: %s\n",PATH);
        fflush(stdout);
        char pattern [MAX_BUFFER];
        char storyData[MAX_BUFFER] = {0};
        char * ptr = storyData;
        *ptr = 0;
        while ( fgets(pattern, MAX_BUFFER, f) )
        {
            strcat(ptr,pattern);
            ptr += strlen(pattern);
            fflush(stdout);
            __fpurge(stdin);

        }
        fclose(f);
        strcpy(studGroupList,storyData);
        printf("Stud of groups:\n%s", studGroupList);
        fflush(stdout);
    }
    else
    {
        fprintf(stderr, "can't open file tests/%s.txt\n", groupName);
        fflush(stdout);
        write(socket,errorMsg,sizeof(errorMsg));
    }
}

void authorization(int socket)
{
    int n;
    n = read(socket,login,sizeof(login));
    printf("%s\n", login);
    fflush(stdout);
    if(n < 0)
        printf("Error read from socket");
    n = read(socket,password,sizeof(password));
    printf("%s\n", password);
    fflush(stdout);
    for(int i = 0; i < AUTH_BUF; i++)
    {
        printf("%c", password[i]);
    }
    printf("\n");
    fflush(stdout);
    if(n < 0)
        printf("Error read from socket");
    if((*permission = checkLogin(login,password)) != '0')
    {
        isClientLoginIn = 1;
        printf("Permission: %s", permission);
        fflush(stdout);
        write(socket,permission,sizeof(permission));

    }
    else
    {
        isClientLoginIn = 0;
        memset(login,'0',AUTH_BUF);
        memset(password,'0',AUTH_BUF);
        *permission = '0';
        write(socket,permission,sizeof(permission));
    }
}

char checkLogin()
{
    FILE * f;
    char buf[MAX_BUFFER];
    char loginTemp[AUTH_BUF];
    char passwordTemp[AUTH_BUF];
    if ( ( f = fopen("Users.txt", "r") ) == NULL )
    {
        fprintf(stderr, "can't open file Users.txt\n");
        fflush(stdout);
        return '0';
    }
    while ( fgets(buf, MAX_BUFFER, f) )
    {
        memset(loginTemp,'\0',AUTH_BUF);
        memset(passwordTemp,'\0',AUTH_BUF);
        int i = 0;
        int j = 0;
        printf("%s\n", buf);
        fflush(stdout);
        while(buf[i] != ' ' && buf[i] != '\n' &&  buf[i] != '\0')
        {
            loginTemp[j] = buf[i];
            i++;
            j++;
        }
        loginTemp[j] = '\0';
        j = 0;
        i++;
        while(buf[i] != ' ' && buf[i] != '\n' &&  buf[i] != '\0')
        {
            passwordTemp[j] = buf[i];
            i++;
            j++;
        }
        passwordTemp[j] = '\0';
        i++;
        printf("%s - %s ... %s - %s\n",login,loginTemp,password,passwordTemp);
        fflush(stdout);
        if(strcmp(loginTemp,login) == 0 && strcmp(passwordTemp,password) == 0)
        {
            printf("%s \n", loginTemp);
            fflush(stdout);
            printf("%s", login);
            fflush(stdout);
            *permission = buf[i];
            fclose(f);
            return *permission;
        }
    }
    if ( ferror(f) )
    {
        fprintf(stderr, "error while reading file User.txt\n");
        fflush(stdout);
        return 1;
    }
    fclose(f);
    return '0';


}

#endif
