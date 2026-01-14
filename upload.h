#ifndef UPLOAD_H
#define UPLOAD_H 

long send_file(int socket, char * filepath);
int receive_file(int socket, char * filepath, size_t file_size);
void parse_args( char * line, char ** arg_ary );
int get_arr_len(char ** arr);

#endif 