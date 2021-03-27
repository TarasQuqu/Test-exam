/**
 * @file Client.h
 * @author Taras Kuts
 * @date 8 September
 * @brief This header has definiton for all functions which involve in the creating client.For example  function  for creating socket or function for creating connect
 * And all functions by which all logic of the Client is written
 *
 *
 *
 */
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
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define ECHO_SERVER_PORT 7
#define MAX1024 1024
#define MAX 24
#define PORT 13664
int socketCreate (int socketType); /**<This function serves to create socket
**@param Socket
*@return SocketID
*/
int connectCreate (int socket, const char * ip, int port);/**<This function serves to create connection with server
**@param Socket
*/
void dataSendRecieve (int socket);/**<This function serves to send and receive data
**@param Socket
*/
int connectionClose (int socket);/**<This function serves to close connection if the situation requires it
**@param Socket
*/
void authorization(int socket);/**<This function serves to create authorization,finally in this function we will who was connected(Admin,Egzamenator,Student)
**@param Socket
*/
void adminPanel(int socket);/**<This function serves to create Admin panel and all actions that the administrator can perform
**@param Socket
*/
void temp(char* buf);/**<This function serves to delete Enter in the buffer
**@param buf
*/
void Add(int socket);/**<This function serves to add smth for Admin
**@param Socket
*/
void CreateGroup(int socket);/**<This function serves to create group for Admin
**@param Socket
*/
void egzaminatorPanel(int socket);/**<This function serves to create Admin panel and all actions that the administrator can perform
**@param Socket
*/
void SetTimeOfTest(int socket);/**<This function serves to set time of the test that the egzaminator can set
**@param Socket
*/
void rightAnswer(int socket);/**<This function serves to get right answers for the current test
**@param Socket
*/
void ChooseGroupOfStudents(int socket);/**<This function serves to choose group of the students
**@param Socket
*/
void AddingAnswers(int socket);/**<This function serves to adding answers , its for egzaminator
**@param Socket
*/
void AddToGroup(int socket);/**<This function serves to add to the group its for Admin
**@param Socket
*/
void PermissionGroup(int socket);/**<This function serves to get information this group has permisson for exam or not
**@param Socket
*/
void ChooseExamForStudent(int socket);/**@brief This function serves to check thi next things(1.Is the exam which entering student at general exist
*2.Is the student in the group which gets permission for the passing this exam  if yes for both questions he can
start his exam and enter answers and all answers will send to the server where they will check.
*@param socket
*/
void studentPanel(int socket);/**<This function serves to create student panel and all actions that the student can perform
**@param Socket
*/
int getch (void);/**<This function serves to help in writing password that will be unreadable.For example "******" */
void Stars(char* buff);/**<This function serves to create unreadable password.
**@param buff
*@return Char that was entered
*/
long hash(char *str);/**<This function serves to hashing the passwords.
**@param string
*/
