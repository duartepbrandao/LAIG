#ifndef TWIXTSOCKET_H
#define TWIXTSOCKET_H

#include <iostream>
#include <winsock2.h>

using namespace std;


#define IPADDRESS "127.0.0.1"
#define PORT 60070

class TwixtSocket{
public:
	TwixtSocket(char* gameMode);

	bool socketConnect();
	void envia(char *s, int len);
	void recebe(char *ans);
	void quit();
};
#endif

