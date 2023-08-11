#pragma once

#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define COMM_BRAKE  		'I'//停止
#define COMM_FORWARD		'A'//前进
#define COMM_BACK  			'B'//后退
#define COMM_LEFT  			'C'//左转
#define COMM_RIGHT 			'D'//右转

int Send(char data);