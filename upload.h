#ifndef UPLOAD_H
#define UPLOAD_H

long send_file(int socket, char * filepath);
int receive_file(int socket, char * filepath, size_t file_size);

#endif 
