// SocketFileServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <windows.h>
#include <iostream>
#include <fstream> 
#include <winsock.h>

#pragma comment (lib,"ws2_32.lib")

#define NO_FLAGS_SET 0

#define PORT (u_short) 44965
#define MAXBUFLEN 2

using namespace std;
INT main(VOID)
{
	WSADATA Data;
	SOCKADDR_IN serverSockAddr;
	SOCKADDR_IN clientSockAddr;
	SOCKET serverSocket;
	SOCKET clientSocket;
	int addrLen = sizeof(SOCKADDR_IN);
	int status;
	
	char buffer_send[MAXBUFLEN];

	/* initialize the Windows Socket DLL */
	status = WSAStartup(MAKEWORD(1, 1), &Data);
	/*��ʼ��Winsock DLL*/
	if (status != 0)
	cerr << "ERROR: WSAStartup unsuccessful" << endl;

	/* zero the sockaddr_in structure */
	memset(&serverSockAddr, 0, sizeof(serverSockAddr));
	/* specify the port portion of the address */
	serverSockAddr.sin_port = htons(PORT);
	/* specify the address family as Internet */
	serverSockAddr.sin_family = AF_INET;
	/* specify that the address does not matter */
	/*INADDR_ANY �ľ��庬���ǣ��󶨵�0.0.0.0����ʱ�������еĵ�ַ��������Ч��*/
	serverSockAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	/* create a socket */
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
		cerr << "ERROR: socket unsuccessful" << endl;

	/* associate the socket with the address */
	status = bind(serverSocket, (LPSOCKADDR)&serverSockAddr,
		sizeof(serverSockAddr));
	if (status == SOCKET_ERROR) {
		cerr << "ERROR: bind unsuccessful" << endl;
	}

	/* allow the socket to take connections */
		status = listen(serverSocket, 1);
		if (status == SOCKET_ERROR)
			cerr << "ERROR: listen unsuccessful" << endl;

	while (true)
	{
		cout << "Wait the connection..." << endl;
		/* accept the connection request when one is received */
		clientSocket = accept(serverSocket,
			(LPSOCKADDR)&clientSockAddr,
			&addrLen);
		
		char beginTag[3];
		recv(clientSocket, beginTag,3, NO_FLAGS_SET);
		int count = atoi(beginTag);
		
		if (count == 1) {
			cout << "Got the connection..." << endl;
			memset(beginTag, 0, 3);
			cout << count << endl;
			cout << "Begin Sending..." << endl;
			ifstream in("server_files\\test.txt", ios_base::in | ios_base::binary);
			if (!in)
			{
				cerr << "Open File Failure,Please Try Again!";
				continue;
			}
			while (!in.eof())
			{
				in.read(buffer_send, MAXBUFLEN);       //���ļ��ж�ȡ MAXBUFLEN ���ֽڵ����ݵ������� 
				send(clientSocket, buffer_send, MAXBUFLEN, NO_FLAGS_SET);
			}
			in.close();

			status = closesocket(clientSocket);
			if (status == SOCKET_ERROR)
				cerr << "ERROR: closesocket unsuccessful"<< endl;
			else
			{
				cout << "Finish Connectiong... " << endl;
			}
		}
		
	}
	status = WSACleanup();
	if (status == SOCKET_ERROR)
		cerr << "ERROR: WSACleanup unsuccessful"
		<< endl;
}


