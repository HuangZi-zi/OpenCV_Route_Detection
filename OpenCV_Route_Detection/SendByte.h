#pragma once

#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define COMM_BRAKE  		'I'//ֹͣ
#define COMM_FORWARD		'A'//ǰ��
#define COMM_BACK  			'B'//����
#define COMM_LEFT  			'C'//��ת
#define COMM_RIGHT 			'D'//��ת

int Send(char data);