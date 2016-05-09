#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;
void err_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
void main() {
  WSADATA wsaData;
  SOCKET clntSock;
  SOCKADDR_IN servAddr;

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    err_handling("WSAStartup Err");
  }

  clntSock = socket(PF_INET, SOCK_STREAM, 0);
  if (clntSock == INVALID_SOCKET) {
    err_handling("clntsock err");
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(9000);

  if (connect(clntSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
    err_handling("connect err");
  }

  int rcvlen;
  char message[BUFSIZ];
  while (1) {
    fgets(message, BUFSIZ, stdin);
    if (!strcmp(message, "Q\n")) {
      break;
    }

    send(clntSock, message, sizeof(message), 0);
    recv(clntSock, message, BUFSIZ, 0);
    cout << "message from : " << message << "\n";
  }
  closesocket(clntSock);
  WSACleanup();

  //전체흐름
  // WSAstartup으로 소켓버전, 라이브러리 초기화
  // 클라이언트 소켓 생성
  // 연결할 서버 주소 설정
  // 서버로 connect
  // recv로 메시지 받음
  // closesocket, wsacleanup으로 마무리
}