#ifndef TWIXTSOCKET_H
#define TWIXTSOCKET_H

#include <winsock2.h>
#include <iostream>

#define IPADDRESS "127.0.0.1"
#define PORT 60070

using namespace std;


class TwixtSocket{
public:
	TwixtSocket();

	static bool socketConnect();
	static void envia(char *s, int len);
	static void recebe(char *ans);
	static void quit();
};
#endif

