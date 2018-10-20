/*
==================================================================================
Tittle: Program ConsoleApplication24: Echo Server 
Date: August/2017
Information: This program is a server app that talks to the client program (ConsoleApplication24)
Author: Rajinder Yadav
Date: Sept 5, 2007
=================================================================================
*/

#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <windows.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
// Initialize WinSock2.2 DLL
// low word = major, highword = minor
WSADATA wsaData = {0};
WORD wVer = MAKEWORD(2,2);
int nRet = WSAStartup( wVer, &wsaData );
if( nRet == SOCKET_ERROR ) {
// WSAGetLastError()
cout << "Failed to init Winsock library" << endl;
return -1;
}
cout << "Starting server" << endl;
// name a socket
WORD WSAEvent = 0;
WORD WSAErr = 0;
// open a socket
//
// for the server we do not want to specify a network address
// we should always use INADDR_ANY to allow the protocal stack
// to assign a local IP address
SOCKET hSock = {0};
hSock = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
if( hSock == INVALID_SOCKET ) {
cout << "Invalid socket, failed to create socket" << endl;
return -1;
}
// name socket
sockaddr_in saListen = {0};
saListen.sin_family = PF_INET;
saListen.sin_port = htons( 10000 );
saListen.sin_addr.s_addr = htonl( INADDR_ANY );
// bind socket's name
nRet = bind( hSock, (sockaddr*)&saListen, sizeof(sockaddr) );
if( nRet == SOCKET_ERROR ) {
cout << "Failed to bind socket" << endl;
//shutdown( hSock );
closesocket( hSock );
return -1;
}
while( true )
{
cout << "Listening for connections" << endl;
// listen
nRet = listen( hSock, 5 ); // connection backlog queue set to 10
if( nRet == SOCKET_ERROR )
{
int nErr = WSAGetLastError();
if( nErr == WSAECONNREFUSED ) {
cout << "Failed to listen, connection refused" << endl;
}
else {
cout << "Call to listen failed" << endl;
}
closesocket( hSock );
return -1;
}
// connect
sockaddr_in saClient = {0};
int nSALen = sizeof( sockaddr );
SOCKET hClient = accept( hSock, (sockaddr*)&saClient, &nSALen );
if( hClient == INVALID_SOCKET ) {
cout << "Invalid client socket, connection failed" << endl;
closesocket( hSock );
return -1;
}
cout << "Connection estabilished" << endl;
// process data
char wzRec[512] = {0};
int nLeft = 512;
int iPos = 0;
int nData = 0;
do
{
nData = recv( hClient, &wzRec[iPos], nLeft, 0 );
if( nData == SOCKET_ERROR ) {
cout << "Error receiving data" << endl;
memset( &wzRec, 0, sizeof( wzRec ) );
break;
}
nLeft -= nData;
iPos += nData;
} while( nLeft > 0 );
cout << "Data Received: " << wzRec << endl;
// echo data back to client
iPos = 0;
nLeft = 512;
do
{
nData = send( hClient, &wzRec[iPos], nLeft, 0 );
if( nData == SOCKET_ERROR ) {
cout << "Error sending data" << endl;
break;
}
nLeft -= nData;
iPos += nData;
} while( nLeft > 0 );
// close client connection
closesocket( hClient );
hClient = 0;
// perform a lowercase comparison
if( _stricmp( wzRec, "!shutdown" ) == 0 ) {
break;
}
// clear data buffer
memset( &wzRec, 0, sizeof( wzRec ) );
} // loop
cout << "Shutting down the server" << endl;
}
