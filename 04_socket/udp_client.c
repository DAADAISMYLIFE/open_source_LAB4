#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAXDATASIZE 100

int main(int argc, char *argv[]){
	int csock;
	char buf[MAXDATASIZE];
	struct sockaddr_in to_addr;
	struct sockaddr_in from_addr;
	int from_len;
	int len;
	int recv_len;

	if(argc != 3){
		fprintf(stderr, "Usage : udp_client <SERVER_IP> <ECHO STRING>\n");
		exit(1);
	}

	// 클라이언트 소켓 생성
	if((csock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("socket");
		exit(1);
	}

	// 서버 주소 설정
	memset(&to_addr, 0, sizeof(to_addr));
	to_addr.sin_family = AF_INET;
	to_addr.sin_addr.s_addr = inet_addr(argv[1]);
	to_addr.sin_port = htons(PORT);

	// 전송할 문자열 설정 및 전송
	memset(buf, 0, MAXDATASIZE);
	strcpy(buf, argv[2]);
	len = strlen(buf);

	if(sendto(csock, buf, len, 0, (struct sockaddr *)&to_addr, sizeof(to_addr))!=len){
		fprintf(stderr, "send failed...\n");
		exit(1);
	}
	

	// 응답 수신
	memset(buf, 0, MAXDATASIZE);
	from_len = sizeof(from_addr);
	if((recv_len = recvfrom(csock, buf, MAXDATASIZE, 0, (struct sockaddr *)&from_addr, &from_len))==-1){
		perror("recv");
		exit(1);
	}

	// 수신한 메시지 출력
	printf("Received : %s\n", buf);

	// 소켓 닫기
	close(csock);
	return 0;
}

