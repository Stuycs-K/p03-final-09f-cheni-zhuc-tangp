#ifndef UPLOAD_H
#define UPLOAD_H

long send_file(int socket, char * filepath);
char * recieve_file_to_memory(int socket, size_t file_size);
long receive_file(int socket, char * filepath, size_t file_size);
void parse_args( char * line, char ** arg_ary );
int get_arr_len(char ** arr);


#endif 
