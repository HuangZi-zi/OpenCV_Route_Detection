#pragma once

#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define COMM_BRAKE  		'E'//ֹͣ
#define COMM_FORWARD		'A'//ǰ��
#define COMM_BACK  			'B'//����
#define COMM_LEFT  			'C'//��ת
#define COMM_RIGHT 			'D'//��ת
#define COMM_SPIN_LEFT		'F'//����ת
#define COMM_SPIN_RIGHT		'G'//����ת
#define COMM_IR				'H'//��������Ѱ��
int Send(char data);
