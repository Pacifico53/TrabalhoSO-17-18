#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include "command.h"

Command* parse_file(int fd,int* i);
ssize_t read_line(int fd, char *buf, size_t nbyte);

#endif
