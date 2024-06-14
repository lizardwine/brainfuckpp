#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_SIZE 65536
#define MEMORY_SIZE 65536
#define FUCNTION_STORAGE_SIZE 64
#define MAX_FUNCTIONS 32
#define FUCNTION_SIZE 4096
#define CELL_SIZE 16
#define MAX_IMPORTS 32

int global_memory[MEMORY_SIZE];
int global_memory_position = 0;
int functions_memory[FUCNTION_STORAGE_SIZE][MEMORY_SIZE];
int functions_memory_position[FUCNTION_STORAGE_SIZE];
char functions_storage[FUCNTION_STORAGE_SIZE][FUCNTION_SIZE];
char already_imported[MAX_IMPORTS][BUFFER_SIZE];
int imports = 0;
int functions = 0;
int std_functions = 0;
const char *syntax = "+-<>.,:;[]{}()@!$&=%#SO";
int using_stdlib = 0;
char *main_content;
char *path;
char *filename;

void split_path(const char *path, char *directory, char *filename) {
    // Encuentra la última ocurrencia de '/'
    const char *last_slash = strrchr(path, '/');

    if (last_slash != NULL) {
        // Copia la parte del directorio
        size_t dir_length = last_slash - path + 1;
        strncpy(directory, path, dir_length);
        directory[dir_length] = '\0'; // Añade el terminador nulo

        // Copia la parte del archivo
        strcpy(filename, last_slash + 1);
    } else {
        // Si no hay '/', entonces no hay directorio, solo el nombre del archivo
        directory[0] = '\0';
        strcpy(filename, path);
    }
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result;
    char *ins;
    char *tmp;
    int len_rep;
    int len_with;
    int len_front;
    int count;
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL;
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

char *get_content(char *filename){
    char *content = (char *)malloc(MAX_SIZE);
    ssize_t bytes_read;
    int fd;
    fd = open(filename, O_RDONLY);
    if(fd == -1){
        printf("\nNo such file: %s\n", filename);
        exit(1);
    }
    while((bytes_read = read(fd, content, MAX_SIZE)) > 0){
        content = realloc(content, MAX_SIZE + bytes_read);
    }
    close(fd);
    return content;
}

char *optimize(char *content){
   char *new_content = (char *)malloc(MAX_SIZE);
   int j = 0;
   int parentesis = 0;
   for (int i = 0; i < strlen(content); i++){
       if(strchr(syntax, content[i]) == NULL && parentesis == 0){
           continue;
       }
       if (content[i] == '#'){
           for (int k = i; k < strlen(content); k++){
               if (content[k] == '\n'){
                   i = k;
                   break;
               }
           }
           continue;
       }
       if (content[i] == '('){
           parentesis++;   
       }
       if (content[i] == ')'){
           parentesis--;
       }
       new_content[j] = content[i];
       j++;
   }
   return new_content;
}

int executor(int function){
    int *memory;
    int *memory_position;
    int global = 1;
    int offset = 0;
    int loop_stack[MEMORY_SIZE];
    int loop_stack_ptr = 0;
    char *content = (char *)malloc(MAX_SIZE);

    memory = global_memory;
    memory_position = &global_memory_position;
    content = main_content;
    if (function != -1){;
        memory = functions_memory[function];
        memory_position = &functions_memory_position[function];
        content = functions_storage[function];
    }
    
    int i = 0;
    for(i = 0; i < strlen(content); i++){

        switch (content[i])
        {
        case '~':
            // TODO: Implement label's system
            break;
        case '|':
            // TODO: Implement goto's system
            int label = memory[*memory_position];
            
            break;
        case '(':
            // Get filename which is in ()
            char *filename = (char *)malloc(BUFFER_SIZE);
            int end = i + 1;
            for (int j = 1; j < BUFFER_SIZE; j++){
                if (content[i + j] == ')'){
                    filename[j - 1] = '\0';
                    end = i + j;
                    break;
                }
                filename[j - 1] = content[i + j];
            }
            if (strlen(filename) == 0){
                break;
            }
            // Check if already imported
            int imported = 0;
            for (int j = 0; j < MAX_IMPORTS; j++){
                if (strcmp(already_imported[j], filename) == 0){
                    imported = 1;
                    break;
                }
            }
            if (imported){
                break;
            }

            for (int j = 0; j < strlen(filename); j++){
                already_imported[imports][j] = filename[j];
            }
            already_imported[imports][strlen(filename)] = '\0';
            // Read file
            char *absolute_path = (char *)malloc(BUFFER_SIZE);
            sprintf(absolute_path, "%s%s", path, filename);
            char *file = (char *)malloc(BUFFER_SIZE);
            char *aux_directory = (char *)malloc(BUFFER_SIZE);

            split_path(absolute_path, aux_directory, file);
            if (strcmp(file, "std.bf") == 0){
                using_stdlib = 1;
            }
            char *new_content = get_content(absolute_path);
            new_content = optimize(new_content);
            if (strlen(new_content) + strlen(main_content) > MAX_SIZE){
                printf("\nFile too long\n");
                exit(1);
            }
            // Realocate main_content
            char *orig = main_content;
            char *rep = (char *)malloc(BUFFER_SIZE + 2);
            sprintf(rep, "(%s)", filename);
            char *with = (char *)malloc(MAX_SIZE);
            sprintf(with, "(%s)%sO", filename, new_content);
            main_content = str_replace(orig, rep, with);
            i = end;
            content = main_content;
            imports++;
            break;
        case '@':
            if (function == -1){
                printf("\n@ can only be used in function\n");
                exit(1);
            }
            if (global == 1){ // in local case
                memory = global_memory;
                memory_position = &global_memory_position;
            }
            else{ // in global case
                memory = functions_memory[function];
                memory_position = &functions_memory_position[function];
            }
            global = -global;
            break;
        case '$':
            if (function == -1){
                printf("\n$ can only be used in function\n");
                exit(1);
            }

            if (global == 1){
                memory[*memory_position] = global_memory[global_memory_position];
            }
            else{
                global_memory[global_memory_position] = functions_memory[function][functions_memory_position[function]];
            }
            break;
        case '>':
            if (*memory_position + 1 < MEMORY_SIZE){
                *memory_position += 1;
            }
            break;
        case '<':
            if (*memory_position - 1 >= 0){
                *memory_position -= 1;
            }
            break;
        case '=':
            *memory_position = memory[*memory_position];
            if (*memory_position >= MEMORY_SIZE){
                *memory_position = MEMORY_SIZE - 1;
            }
            if (*memory_position < 0){
                *memory_position = 0;
            }
            break;
        case '&':
            memory[*memory_position] = *memory_position;
            break;
        case '+':
            if (memory[*memory_position] + 1 > pow(2, CELL_SIZE) - 1){
                memory[*memory_position] = 0;
                break;
            }
            memory[*memory_position] += 1;
            break;
        case '-':
            if (memory[*memory_position] - 1 < 0){
                memory[*memory_position] = pow(2, CELL_SIZE) - 1;
                break;
            }
            memory[*memory_position] -= 1;
            break;
        case '.':
            putchar(memory[*memory_position]);
            fflush(stdout);
            break;
        case ':':
            printf("%d", memory[*memory_position]);
            fflush(stdout);
            break;
        case ',':
            memory[*memory_position] = getchar();
            getchar();
            break;
        case ';':
            int input;
            scanf("%d", &input);
            memory[*memory_position] = input;
            break;
        case '[':
            if (memory[*memory_position] == 0) {
                int loop_count = 1;
                while (loop_count > 0) {
                    i++;
                    if (content[i] == '[') loop_count++;
                    else if (content[i] == ']') loop_count--;
                }
            } else {
                loop_stack[loop_stack_ptr++] = i;
            }
            break;
        case ']':
            if (memory[*memory_position] != 0) {
                i = loop_stack[loop_stack_ptr - 1];
            } else {
                loop_stack_ptr--;
            }
            break;
        case '{':

            int *current_functions = &functions;
            offset = 0;
            if (content[i + 1] == 'S'){
                current_functions = &std_functions;
                offset = MAX_FUNCTIONS;
                i += 1;
            }

            if (*current_functions >= MAX_FUNCTIONS){
                printf("\nToo many functions\n");
                exit(1);
            }
            int k = 0;
            for(int j = 1; j < FUCNTION_SIZE; j++){
                if (j == FUCNTION_SIZE - 1 && content[i + j] != '}'){
                    printf("\nFunction: %d too long \n", *current_functions + offset);
                    exit(1);
                }
                if (content[i + j] == '}'){
                    i = i + j;
                    break;
                }
                if (strchr(syntax, content[i + j]) == NULL){
                    continue;
                }
                functions_storage[*current_functions + offset][k] = content[i + j];
                k++;
            }
            (*current_functions) += 1;
            break;
        case '!':
            int function_id = memory[*memory_position];
            offset = using_stdlib == 1 ? 0 : MAX_FUNCTIONS;
            if (function_id == 0){
                break;
            }
            if (function_id > FUCNTION_STORAGE_SIZE - offset){
                printf("\nInvalid function\n");
                exit(1);
            }
            int result = executor(function_id - 1);
            if (result){
                memory[*memory_position] = result;
            }
            for (int j = 0; j < MEMORY_SIZE; j++){
                functions_memory[function_id - 1][j] = 0;
            }
            functions_memory_position[function_id - 1] = 0;
            break;
        case '%':
            return memory[*memory_position];
        default:
            break;
        }
    }
    return memory[*memory_position];
}
int main(int argc, char **argv){
    char buffer[BUFFER_SIZE];
    main_content = (char *)malloc(MAX_SIZE);
    if (argc < 2){
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }
    path = (char *)malloc(MAX_SIZE);
    filename = (char *)malloc(MAX_SIZE);
    split_path(argv[1], path, filename);

    main_content = get_content(argv[1]);
    main_content = optimize(main_content);
    executor(-1);
    printf("\n");
    return 0;
}