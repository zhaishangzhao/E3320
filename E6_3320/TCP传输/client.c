#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define  MAX_DATA_BLOCK_SIZE 8192
void error_exit(const char* msg, int val);
void send_file(const char* file_name, const char* ip, u_short port);
int main(int argc, char** argv) {
	u_short port;
	if (argc == 3) {
		send_file(argv[1], argv[2], 8888);
	}
	else if (argc == 4) {
		port = (u_short)atoi(argv[1]);
		if (port == 0) {
			error_exit("�Ƿ��������˿�", -1);
		}
		else {
			send_file(argv[1], argv[2], port);
		}
	}
	else {
		error_exit("��������", -1);
	}
	return 0;
}
void error_exit(const char* msg, int val) {
	if (msg) {
		printf("%s\n\n", msg);
	}
	printf("ʹ�÷�ʽ�� ft_client<�ļ���><������IP��ַ>[�������˿�]\n");
	printf("�������˿��ǿ�ѡ������Ĭ��Ϊ8888\n\n");
	exit(val);
}
void send_file(const char* file_name, const char* ip, u_short port) {

	WSADATA wsaData;
	SOCKET s;
	FILE* fp;
	struct sockaddr_in server_addr;
	char data[MAX_DATA_BLOCK_SIZE];
	int i;
	int ret;
	fp = fopen(file_name, "rb");
	if (fp == NULL) {
		printf("�޷����ļ�\n");
		return;
	}
	WSAStartup(0x202, &wsaData);
	s = socket(AF_INET, SOCK_STREAM, 0);
	memset((void*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);
	if (connect(s, (struct sockaddr*) & server_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
		printf("���ӷ�����ʧ��\n");
		fclose(fp);
		closesocket(s);
		WSACleanup();
		return;
	}
	printf("�����ļ���������\n");
	send(s, file_name, strlen(file_name), 0);
	send(s, "\0", 1, 0);
	printf("�����ļ�����");
	for (;;) {
		memset((void*)data, 0, sizeof(data));
		i = fread(data, 1, sizeof(data), fp);
		if (i == 0) {
			printf("\n���ͳɹ�\n");
			break;
		}
		ret = send(s, data, i, 0);
		putchar('.');
		if (ret == SOCKET_ERROR) {
			printf("\n����ʧ�ܣ��ļ����ܲ�����\n");
			break;
		}
	}
	fclose(fp);
	closesocket(s);
	WSACleanup();


}

