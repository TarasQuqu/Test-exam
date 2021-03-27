/**
 * @file Client.c
 * @author Taras Kuts
 * @date 8 September
 * @brief File contains implementation for all functions that are in file Client.h
 * This file has implementation for all functions which involve in the creating client.For example  function  for creating socket or function for creating connect
 * And all functions by which all logic of the Client is written
 *
 */
#include "Client.h"
#include <termios.h>
#include <unistd.h>
#include <stdio_ext.h>

char RightAnswer[1024];/**<This array serves for containing right answers*/
char ListGroups[1024] = { '\0' };/**<This array serves for containing list of groups*/
char EnteredGroup[1024] = {'\0'};/**<This array serves for containing entered group*/
char temp1[1024] = {'0'};/**<This array serves for helping as "temp" array*/
int strCmpTest;/**<This int  serves for checking name of the group*/
int numQuestions;/**<This int serves for counting questions*/
char NameExam[1024];/**<This array serves for containing names of exam*/
char Question[1024];/**<This array serves for containing questions*/
int connectResult;/**the parametr which will be show connection result */
int ch;/**This in return in the end of the function*/
char c;/**<This char serves to check exist string*/
int counter = 0;/**<This int  serves for counting students*/
long hash = 5381;/**<This parametr serves to set max value of hash*/
char examsList[1024];/**<This array serves for containing list of exams*/
char StudList[1024];/**<This array serves for containing list of students*/
char AllList[1024];/**<This array serves for containing all lists*/
char groupList[1024];/**<This array serves for containing list of groups*/
char login[24];/**<This array serves for containing list of logins*/
char DifferentAnswers[1024];/**<This array serves for containing answers */
int countAnswrs;/**<This int serves for counting answers*/
char questions[1024];/**This array serves for containing questions */
char answers[1024];/**This array serves for containing answers */
char rightAnswers[1024];/**This array serves for containing right answers */
char exam[1024];/**This array serves for containing exams */
char ListExams[1024];/**This array serves for containing list of exams */
int main (int argc, char *argv[])
{
    char ip[24] = "127.0.0.1";/**This array serves for IP address*/
    int port = PORT;
    int socketId;

    socketId = socketCreate(SOCK_STREAM);

    connectCreate(socketId,ip, port);

    authorization(socketId);

    connectionClose(socketId);

    //struct itimerval tval;

    return 0;
}
long hash(char *str)
    {
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }
int getch (void)
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}
/**@brief This function serves to clean buffs */
void fflushstdin ()
{
    int c;
    while ((c = fgetc (stdin)) != EOF && c != '\n');
}
/**@brief This function serves to create unreadable password.
*@param buffer
*/
void Stars(char* buff)
{
    char c = ' ';S
    int k = 0;
    {
        buff[k]=getch();
        __fpurge(stdin);
        c=buff[k];
        if(c==24)
            break;
        else
            printf("*");
        k++;
    }
    while(c != '\n');
}
/**@brief This function serves to create socket
*@param socket type
*/
int socketCreate (int socketType)
{
    int socketId;

    socketId = socket(AF_INET, socketType, 0);

    if (socketId == -1)
    {
        fprintf(stderr, "BLAD: nie moge utworzyc socketu: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
        return socketId;
    }

    printf("Socket stworzony %d\n", socketId);
    fflush(stdout);

    return socketId;
}
int connectCreate (int socket, const char * ip, int port)
{
    struct sockaddr_in address;


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);
    memset( &( address.sin_zero ), '\0', 8 );
    printf("Nawiazywane polaczenie z %s na porcie %d ...\n", ip, port);
    fflush(stdout);
    connectResult = connect(socket, (struct sockaddr *) & address, sizeof(struct sockaddr));

    if (connectResult == -1)
    {
        fprintf(stderr, "BLAD: nie moge utworzyc poloczenie %d: %s\n", socket, strerror(errno));
        fflush(stdout);
        exit(EXIT_FAILURE);
        return connectResult;
    }


    return connectResult;
}
int connectionClose (int socket)
{
    int connectionCloseStatus = close(socket);

    if (connectionCloseStatus == -1)
    {
        fprintf(stderr, "BLAD: nie moge zamknac polaczenie z socket %d: %s\n", socket, strerror(errno));
        fflush(stdout);
        exit(EXIT_FAILURE);
        return connectionCloseStatus;
    }

    printf("4) Zamkniecie polaczenie z socket %d\n", socket);
    fflush(stdout);

    return connectionCloseStatus;
}
void temp(char* buff)
{
    int i;
    for ( i = 0; i < MAX; i++ )
    {
        if ( buff[i] == '\n' )
        {
            buff[i] = '\0';
            break;
        }
    }
}
void authorization(int socket)
{
    while(1==1)
    {

        long passHash;
        char buff[24];
        printf("Enter your login:");
        fflush(stdout);
        fflush(NULL);
        fgets( login, 24, stdin );
        __fpurge(stdin);
        temp(login);
        write(socket, login, sizeof(login));
        printf("Enter your password:");
        fflush(stdout);
        Stars(buff);
        //Stars(buff);
        __fpurge(stdin);
        temp(buff);
        passHash = hash(buff);
        sprintf(buff, "%ld", passHash);
        write(socket, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(socket, buff, 1);
        printf("perm: %s", buff);
        if(strcmp(buff, "1")==0)
        {

            bzero(buff, sizeof(buff));
            adminPanel(socket);
        }
        if(strcmp(buff, "2")==0)
        {

            bzero(buff, sizeof(buff));
            egzaminatorPanel(socket);
        }
        if(strcmp(buff, "3")==0)
        {
            bzero(buff, sizeof(buff));
            studentPanel(socket);

        }
    }
}
void adminPanel(int socket)
{
    char buff[1024];
    char choose;
    while(1==1)
    {

        printf("Wybierz opcje:\n1-Create group\n2-logout\n3-Remove\n4-Add \n5-Add to group\n6-StudList\n7-AllList\n8-ExamsList\n");
        fflush(stdout);
        choose = getchar();
        __fpurge(stdin);
        printf("\n");
        switch (choose)
        {
        case '1':
            sprintf(buff,"createGroup");
            temp(buff);
            write(socket, buff, sizeof(buff));
            CreateGroup(socket);
            break;
        case '2':
            sprintf(buff,"logout");
            temp(buff);
            write(socket, buff, sizeof(buff));
            printf("\nLogout\n");
            fflush(stdout);
            return;
            break;
        case '3':
            sprintf(buff,"remove");
            fflush(stdout);
            temp(buff);
            write(socket, buff, sizeof(buff));

            printf("Enter login");
            fflush(stdout);
            fflush(stdin);
            fgets( buff, MAX, stdin );
            __fpurge(stdin);
            temp(buff);
            write(socket, buff, sizeof(buff));

            if(strncmp(buff, "ok", 2)==0)
            {
                bzero(buff, sizeof(buff));
                printf("OK!");
                fflush(stdout);
            }
            if(strncmp(buff, "error", 5)==0)
            {
                bzero(buff, sizeof(buff));
                printf("Error!");
                fflush(stdout);
            }
            break;
        case '4':
            sprintf(buff,"add");
            temp(buff);
            write(socket, buff, sizeof(buff));
            temp(buff);
            Add(socket);

            if(strncmp(buff, "ok", 2)==0)
            {
                bzero(buff, sizeof(buff));
                printf("OK!\n");
                fflush(stdout);
            }
            if(strncmp(buff, "error", 5)==0)
            {
                bzero(buff, sizeof(buff));
                printf("Error!\n");
                fflush(stdout);
            }
            break;
        case '5':
            sprintf(buff,"addToGroup");
            fflush(stdout);
            temp(buff);
            write(socket, buff, sizeof(buff));
            temp(buff);
            AddToGroup(socket);
            if(strncmp(buff, "ok", 2)==0)
            {
                bzero(buff, sizeof(buff));
                printf("OK!\n");
                fflush(stdout);
            }
            if(strncmp(buff, "error", 5)==0)
            {
                bzero(buff, sizeof(buff));
                printf("Error!\n");
                fflush(stdout);
            }
            break;
        case '6':
            sprintf(buff,"studList");
            fflush(stdout);
            temp(buff);
            write(socket, buff, sizeof(buff));
            bzero(StudList, sizeof(StudList));
            read(socket,StudList,sizeof(StudList));
            printf("%s",StudList);
            break;
        case '7':
            sprintf(buff,"allList");
            fflush(stdout);
            temp(buff);
            write(socket,buff,sizeof(buff));
            bzero(AllList, sizeof(AllList));
            read(socket,AllList,sizeof(AllList));
            printf("%s",AllList);
            break;
        case '8':
            sprintf(buff,"examsList");
            fflush(stdout);
            temp(buff);
            write(socket,buff,sizeof(buff));
            bzero(examsList, sizeof(examsList));
            read(socket,examsList,sizeof(examsList));
            printf("%s",examsList);
            break;
        }



    }

}
void CreateGroup(int socket)
{
    fflush(stdin);
    char buff[1024];
    printf("Enter the name of group\n");
    fflush(stdout);
    fgets( buff, 1024, stdin );
    __fpurge(stdin);
    fflush(stdin);
    temp(buff);
    write(socket, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(socket, buff, 1024);
    if(strncmp(buff, "error", 5)==0)
    {
        bzero(buff, sizeof(buff));
        printf("Error,try again!\n");
        fflush(stdout);
    }
    if(strncmp(buff, "ok", 2)==0)
    {
        bzero(buff, sizeof(buff));
        printf("OK!\n");
        fflush(stdout);
    }
}
void Add(int socket)
{
    char buff[1024];
    printf("Enter login\n");
    fflush(stdout);
    fgets( buff, 1024, stdin );
    __fpurge(stdin);
    temp(buff);
    write(socket, buff, sizeof(buff));

    printf("Enter password\n");
    fflush(stdout);
    fgets( buff, 1024, stdin );
    __fpurge(stdin);
    temp(buff);
    write(socket, buff, sizeof(buff));

    printf("Enter permission\n");
    fflush(stdout);
    fgets( buff, 1024, stdin );
    __fpurge(stdin);
    temp(buff);
    write(socket, buff, sizeof(buff));
    read(socket, buff, 1024);

    if(strncmp(buff, "error", 5)==0)
    {
        bzero(buff, sizeof(buff));
        printf("Error,try again!\n");
        fflush(stdout);
    }
    if(strncmp(buff, "ok", 2)==0)
    {
        bzero(buff, sizeof(buff));
        printf("OK!\n");
        fflush(stdout);
    }
}
void AddToGroup(int socket)
{
    char buff[1024];
    printf("Enter *stop!* if you want to stop\n");
    fflush(stdout);
    printf("Enter group name:");
    fflush(stdout);
    fgets(buff, 1024, stdin );
    __fpurge(stdin);
    temp(buff);
    write(socket, buff, sizeof(buff));
    printf("Enter login of student:");
    fflush(stdout);
    fgets(buff, 1024, stdin );
    __fpurge(stdin);
    while(counter<21 && strcmp(buff,"stop!\n") != 0)
    {
        write(socket, buff, sizeof(buff));
        read(socket,buff,sizeof(buff));
        printf("message:%s", buff);
        if(strncmp(buff, "ok", 2)==0)
        {
            printf("Student accepted in group!\n");
            fflush(stdout);
            counter++;
        }
        else
        {
            printf("User not a student!\n");
            fflush(stdout);
        }
        printf("Enter login of student:");
        fflush(stdout);
        fgets(buff, 1024, stdin );
        __fpurge(stdin);
    }
    if(strcmp(buff,"stop!\n") == 0)
    {
        write(socket, buff, sizeof(buff));
    }
}
void egzaminatorPanel(int socket)
{
    char buff[1024];
    char choose = '0';

    while(1==1)
    {
        printf("Hello!Creating exam is started\n");
        fflush(stdout);
        printf("Wybierz opcje:\n1 - Start to create test\n2 - Choose group\n3-logout\n4-StudList\n5-AllList\n6-ExamsList");
        fflush(stdout);
        choose = getchar();
        __fpurge(stdin);
        switch (choose)
        {
        case '1':
            sprintf(buff,"createExam");
            //temp(buff);
            write(socket, buff, sizeof(buff));
            printf("Enter name of the exam\n");
            fflush(stdout);
            fgets( NameExam, 1024, stdin );
            __fpurge(stdin);
            temp(NameExam);
            fflush(stdin);
            write(socket, NameExam, sizeof(buff));

            printf("How many question you want?\n");
            fflush(stdout);
            scanf("%d",&numQuestions);
            __fpurge(stdin);
            //temp(buff);
            bzero(buff, sizeof(buff));
            buff[0] = numQuestions + '0';
            buff[0] = '\n';
            write(socket, buff, sizeof(buff));

            for(int i=0; i<numQuestions; i++)
            {
                printf("Enter question:");
                fflush(stdout);
                fgets( Question, 1024, stdin );
                __fpurge(stdin);
                //temp(Question);
                fflush(stdin);
                write(socket, Question, sizeof(Question));
                AddingAnswers(socket);
                rightAnswer(socket);
            }
            PermissionGroup(socket);
            break;
        case '2':
            ChooseGroupOfStudents(socket);
            break;
        case '3':
            sprintf(buff,"logout");
            temp(buff);
            write(socket, buff, sizeof(buff));
            printf("\nLogout\n");
            fflush(stdout);
            return;
            break;
        case '4':
            sprintf(buff,"studList");
            fflush(stdout);
            temp(buff);
            write(socket, buff, sizeof(buff));

            read(socket,StudList,sizeof(StudList));
            printf("%s",StudList);
            break;
        case '5':
            sprintf(buff,"allList");
            fflush(stdout);
            temp(buff);
            write(socket,buff,sizeof(buff));

            read(socket,AllList,sizeof(AllList));
            printf("%s",AllList);
            break;
        case '6':
            sprintf(buff,"examsList");
            fflush(stdout);
            temp(buff);
            write(socket,buff,sizeof(buff));

            read(socket,examsList,sizeof(examsList));
            printf("%s",examsList);
            break;
        }
    }

}
/**@brief This function serves to give permission that gets group and then this group has permission for the selected exam
**@param socket
*/

void PermissionGroup(int socket)
{
    int exist = 0;
    int i = 0;
    int j = 0;
    read(socket,ListGroups,sizeof(ListGroups));
    printf("%s",ListGroups);
    fflush(stdout);
    printf("Enter group which gets permission for the this exam:");
    fflush(stdout);
    fgets( EnteredGroup,1024, stdin );
    __fpurge(stdin);
    EnteredGroup[strlen(EnteredGroup) - 1] = '\0';
    while(exist != 1)
    {
        while(ListGroups[i]!='\n')
        {
            temp1[j] = ListGroups[i];
            printf("temp1C: %c - listGroup: %c \n", temp1[j], ListGroups[i]);
            i++;
            j++;
        }
        i++;
        //temp1[j-1] = '\0';
        j = 0;
        fflush(stdout);
        if((strCmpTest = strcmp(EnteredGroup,temp1)) == 0)
        {
            exist = 1;
        }
        printf("strCmpTest: %d\n", strCmpTest);
        fflush(stdout);
        memset(temp1, '\0', strlen(temp1));
        if(i == strlen(ListGroups))
            break;
    }
    if(exist == 0)
    {
        char error[1024] = "err";
        printf("Your entered group doesn't exist\n");
        fflush(stdout);
        write(socket,error,sizeof(error));
        return;
    }
    else
    {
        write(socket,EnteredGroup, sizeof(EnteredGroup));
    }
}
void AddingAnswers(int socket)
{
    char buff[1024];

    printf("How many answers you want to add?\n");
    fflush(stdout);
    scanf("%d",&countAnswrs);
    __fpurge(stdin);
    //temp(buff);

    while(countAnswrs > 5)
    {
        printf("Enter number of the answers less than 6:");
        fflush(stdout);
        printf("How many answers you want to add?\n");
        fflush(stdout);
        scanf("%d",&countAnswrs);
        __fpurge(stdin);
        //temp(buff);
    }
    bzero(buff, sizeof(buff));
    buff[0] = countAnswrs + '0';
    buff[1] = '\n';
    write(socket, buff, sizeof(buff));
    for(int i=0; i<countAnswrs; i++)
    {
        printf("%d. Enter answers:",i);
        fflush(stdout);
        fgets(DifferentAnswers, 1024, stdin );
        __fpurge(stdin);
        //temp(DifferentAnswers);
        fflush(stdin);
        write(socket, DifferentAnswers, sizeof(DifferentAnswers));
    }
}
void rightAnswer(int socket)
{

    printf("Enter number of right answer:");
    fflush(stdout);
    fgets( RightAnswer, 1024, stdin );
    __fpurge(stdin);
    //temp(RightAnswer);
    fflush(stdin);
    write(socket, RightAnswer, sizeof(RightAnswer));
}

void ChooseGroupOfStudents(int socket)
{
    char buff[1024];
    printf("Exams list:\n");
    fflush(stdout);
    sprintf(buff,"examsList");
    write(socket,buff,sizeof(buff));
    read(socket,examsList,sizeof(examsList));
    printf("%s", examsList);
    fflush(stdout);
    printf("Groups list:\n");
    fflush(stdout);
    sprintf(buff,"groupsList");
    write(socket,buff,sizeof(buff));
    read(socket,groupList,sizeof(groupList));
    printf("%s", groupList);
    fflush(stdout);
    write(socket, buff, sizeof(buff));
    sprintf(buff,"chooseGroupToExam");
    write(socket, buff, sizeof(buff));
    printf("Choose exam:");
    fflush(stdout);
    fgets(buff, 1024, stdin );
    __fpurge(stdin);
    temp(buff);
    write(socket, buff, sizeof(buff));
    printf("Choose group:");
    fflush(stdout);
    fgets(buff, 1024, stdin );
    __fpurge(stdin);
    write(socket, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
}
/**@brief This function serves to open student panel and give all opportunities that has student.
*For example (Start to create test,Choose croup,logout,Studlist,examLlist)
*@param socket
*/
void studentPanel(int socket)
{
    char buf[1024];
    char choose = '0';
    while(1==1)
    {
        memset(buf,'\0',strlen(buf));
        printf("Wybierz opcje:\n1-Choose exam\n2-Check your results\n3-logout\n");
        fflush(stdout);
        choose = getchar();
        __fpurge(stdin);
        printf("\n");
        switch (choose)
        {
        case '1':
            sprintf(buf,"chooseExam");
            write(socket, buf, sizeof(buf));
            ChooseExamForStudent(socket);
            break;
        case '2':
            sprintf(buf,"checkResults");
            write(socket, buf, sizeof(buf));
            write(socket, login, sizeof(login));
            read(socket,buf,sizeof(buf));
            printf("Your exam results:\n%s", buf);
            break;
        case '3':
            sprintf(buf,"logout");
            write(socket, buf, sizeof(buf));
            printf("\nLogout\n");
            fflush(stdout);
            return;
            break;
        }



    }
}
void ChooseExamForStudent(int socket)
{
    int i=0;
    char Login[24];
    char ListStudent[1024];
    char tempListExams[1024] = { '\0' };
    char tempQuestions[1024];
    char tempAnswers[1024];
    char *ptr = rightAnswers;
    int exist = 0;
    int exist2 = 0;
    int k=0;
    int j=0;
    int y = 0;
    int qCntInt;
    int aCntInt;
    char tempListStudent[1024];
    read(socket,ListExams,sizeof(ListExams));
    printf("%s", ListExams);
    fflush(stdout);
    printf("Enter the exam which you want:");
    fflush(stdout);
    fgets(exam, 1024, stdin);
    __fpurge(stdin);
    temp(exam);
    write(socket, exam, sizeof(exam));
    char NO[1024];
    char YES[1024];
    int strCmpTest = 0;
    while(!exist2)
    {
        while(ListExams[i]!='\n')
        {
            tempListExams[j] = ListExams[i];
            i++;
            j++;
        }
        i++;
        j = 0;
        printf("exam: %s\n examFromList: %s\n", exam,tempListExams);
        fflush(stdout);
        if((strCmpTest = strcmp(exam,tempListExams)) == 0)
        {
            exist2 = 1;
        }
        printf("strCmpTest: %d\n", strCmpTest);
        fflush(stdout);
        memset(tempListExams, '\0', strlen(tempListExams));
    }
    if(exist2 == 0)
    {
        printf("The exam doesn't exist\n");
        fflush(stdout);
    }
    else
    {
        read(socket,Login,sizeof(Login));
        read(socket,ListStudent,sizeof(ListStudent));
        printf("%s", ListStudent);
        fflush(stdout);
        while(!exist)
        {
            while(ListStudent[k]!='\n')
            {
                tempListStudent[k] = ListStudent[y];
                k++;
                y++;
            }
            k++;
            y=0;
            if(strcmp(tempListStudent, Login) == 0)
            {
                exist = 1;
            }
        }
    }
    if(exist==0)
    {
        printf("You are not on this list\n");
        fflush(stdout);
        sprintf(NO,"no");
        temp(NO);
        write(socket, NO, sizeof(NO));
    }
    else
    {
        printf("Lets go,%s \n ",Login);
        sprintf(YES,"yes");
        temp(YES);
        write(socket,YES,sizeof(YES));
        read(socket,questions,sizeof(questions));
        read(socket,answers,sizeof(answers));
        i = 0;
        while(questions[i]!='\n')
        {
            tempQuestions[i] = questions[i];
            i++;
        }
        printf("%d  %s\n",i,tempQuestions);
        fflush(stdout);
        qCntInt = tempQuestions[0] - '0';
        i++;
        int z = 0;
        int x = 0;
        k = 0;
        while(k < qCntInt)
        {
            //memset(tempQuestions,'\0', strlen(tempQuestions));
            while(questions[i]!='\n')
            {
                tempQuestions[z] = questions[i];
                i++;
                z++;
            }
            i++;
            z = 0;
            printf("%s\n",tempQuestions);
            fflush(stdout);
            memset(tempAnswers,'\0', strlen(tempAnswers));
            while(answers[j]!= '\n')
            {
                tempAnswers[x] = answers[j];
                j++;
                x++;
            }
            j++;
            x = 0;
            aCntInt = tempAnswers[0] - '0';
            while(y < aCntInt)
            {
                memset(tempAnswers,'\0', strlen(tempAnswers));
                while(answers[j]!= '\n')
                {
                    tempAnswers[x] = answers[j];
                    j++;
                    x++;
                }
                j++;
                x = 0;
                printf("\t%d)%s\n",y + 1,tempAnswers);
                y++;
            }
            y = 0;
            char pattern[1024] = {'\0'};

            int checkAnswer = 0;
            printf("Enter your answer: ");
            do
            {
                if(strlen(pattern) > 0)
                {
                    printf("This number is out of range\nEnter your answer:");
                    fflush(stdout);
                }
                memset(pattern,'\0',strlen(pattern));
                fgets(pattern,1024, stdin);
                __fpurge(stdin);
                checkAnswer = atoi(pattern);
            }
            while(checkAnswer < 1 || checkAnswer > aCntInt);
            strcat(ptr,pattern);
            ptr += strlen(pattern);
            k++;
        }
        printf("rightAnsw: %s", rightAnswers);
        fflush(stdout);
        temp(Login);
        write(socket,Login,sizeof(Login));
        write(socket,rightAnswers,sizeof(rightAnswers));
        char temptemp[1024];
        temptemp[0] = qCntInt + '0';
        write(socket,temptemp,sizeof(temptemp));
    }
}
