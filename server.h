#include "DBHandler.h"
#include <charconv>
#include <iostream>
#include <list>
#include <netinet/in.h>
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define MESSAGE_LENGTH 1024
#define PORT 7777

void server(Data handler, std::shared_ptr<bool> activeSession);
