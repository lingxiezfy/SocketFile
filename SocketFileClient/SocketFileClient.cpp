// SocketFileClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <winsock.h>
#pragma comment (lib,"ws2_32.lib")

using namespace std;

#define NO_FLAGS_SET 0
#define MAXBUFLEN 512
#define PORT (u_short) 44965
#define DEST_IP_ADDR "127.0.0.1" //Server address
INT main(VOID)
{
	WSADATA Data;
	SOCKADDR_IN destSockAddr;
	SOCKET destSocket;
	unsigned long destAddr;
	int status;
	int numsnt;

	/* initialize the Windows Socket DLL */
	status = WSAStartup(MAKEWORD(1, 1), &Data);
	if (status != 0) {
		cerr << "ERROR: WSAStartup unsuccessful" << endl;
		WSACleanup();
		cerr << "Exit ..." << endl;
		Sleep(2000);
		return 1;
	}
	/* convert IP address into in_addr form */
	destAddr = inet_addr(DEST_IP_ADDR);
	/* copy destAddr into sockaddr_in structure */
	memcpy(&destSockAddr.sin_addr,
		&destAddr, sizeof(destAddr));
	/* specify the port portion of the address */
	destSockAddr.sin_port = htons(PORT);
	/* specify the address family as Internet */
	destSockAddr.sin_family = AF_INET;

	/* create a socket */
	destSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (destSocket == INVALID_SOCKET)
	{
		cerr << "ERROR: socket unsuccessful" << endl;
		status = WSACleanup();
		if (status == SOCKET_ERROR)
			cerr << "ERROR: WSACleanup unsuccessful"<< endl;
		cerr << "Exit ..." << endl;
		Sleep(2000);
		return 1;
	}

	cout << "Trying to connect to IP Address: "<< DEST_IP_ADDR << endl;

	/* connect to the server */
	status = connect(destSocket,(LPSOCKADDR)&destSockAddr,sizeof(destSockAddr));
	if (status == SOCKET_ERROR)
	{
		cerr << "ERROR: connect unsuccessful" << endl;
		status = closesocket(destSocket);
		if (status == SOCKET_ERROR)
			cerr << "ERROR: closesocket unsuccessful"
			<< endl;
		status = WSACleanup();
		if (status == SOCKET_ERROR)
			cerr << "ERROR: WSACleanup unsuccessful"
			<< endl;
		cerr << "Exit ..." << endl;
		Sleep(2000);
		return 1;
	}
	cout << "Connected...\n" << endl;
	numsnt = send(destSocket, "1",sizeof("1"), NO_FLAGS_SET);
	Sleep(2000);
	if (numsnt != 0) {
		char buffer[MAXBUFLEN];
		ofstream out("download\\2_temp.txt", ios_base::out | ios_base::binary);
		memset(buffer, 0, sizeof(buffer));
		while (recv(destSocket, buffer, MAXBUFLEN-10, NO_FLAGS_SET)) {
			int count = 0;
			count = strlen(buffer);
			out.write(buffer, count);       //写入那个字节的数据  
			cout << count << " " << buffer << endl;
			memset(buffer, 0, sizeof(buffer));
			Sleep(50);
		}
		out.flush();
		out.close();
	}
	status = closesocket(destSocket);
	if (status == SOCKET_ERROR)
		cerr << "ERROR: closesocket unsuccessful"
		<< endl;
	status = WSACleanup();
	if (status == SOCKET_ERROR)
		cerr << "ERROR: WSACleanup unsuccessful"
		<< endl;
	//保留界面
	int n;
	cin >> n;
	return 0;
}

