#include "cryptoMagic.h"
#include <windows.h>
#include <dirent.h>
#include <errno.h>

// #include <shlwapi.h>
// #pragma comment(link, "shlwapi.lib")

/*
 *  Required Parameters:
 * 
 *      foo.txt/foo.crp: The text file or .crp file to be encrypted or decrypted
 *
 *  Optional Parameters (Mutually Exclusive):
 * 
 *      -E: Encrypts text files
 *
 *      -D: Decrypts encrypted .crp file
 * 
 *      -KEEP_ORIG: Keeps the original file after finishing the operation
 * 
 *      -O proceeding file path is the output ABSOLUTE filepath 
*/


int encr = 0;
int decr = 0;

int keep = 0;

int alt_out = 0;
char *alt_path;

FILE *file;
char *file_name;
char extension[3];
char *out_name;
char *read_fp;

FILE *file_out;


int main(int argc, char *argv[]) {

    if (!check_param(argc, argv)) exit (-1);

    if (encr) {
        if (encrypt()) {
            printf("Encryption Successful!\nOutput file at: ");
            if (!alt_out) printf("%s\n", _pgmptr);
            else printf("%s\\%s\n", alt_path, out_name);
        } else {
            exit(-1);
        }
    } else if (decr) {
        if (decrypt()) {
            printf("Decryption Successful!\nOutput file at:");
            if (!alt_out) printf("%s\n", _pgmptr);
            else printf("%s\\%s\n", alt_path, out_name);
        } else {
            exit(-1);
        }
    }

    fclose(file);
    fclose(file_out);

    if (!keep) remove(read_fp);
    
    free(out_name);
    free(file_name);

    return 0;
}

int check_param(int argc, char* argv[]) {

    if (argc == 1) return -1;

    for (int i = 1; i < argc; ++i) {
        char *saveptr1;
        char *saveptr2;

        if (strcmp(argv[i], "-E") == 0) encr = 1;
        else if (strcmp(argv[i], "-D") == 0) decr = 1;

        if (strcmp(argv[i], "-KEEP_ORIG") == 0) keep = 1;

        if (strcmp(argv[i], "-O") == 0) alt_out = 1;

        if (alt_out) {
            if (strcmp(argv[i-1], "-O")) {
                alt_path = argv[i];
            }
        }

        if (encr == 1 && decr == 1) {
            printf("You cannot specify the encrypt (-E) and decrypt (-D) parameter in the same command!\n\n");
            return 0;
        }

        if (strcmp(argv[i-1], "-O") != 0) {
            if (file == NULL) {
                file = fopen(argv[i], "r");
                
                if (file != NULL) {
                    read_fp = argv[i];
                    char *temp_arg = malloc(strlen(argv[i]));
                    strcpy(temp_arg, argv[i]);
                    char *file_segment = strtok_r(temp_arg, "\\", &saveptr1);
                    file_name = malloc(strlen(file_segment)+1);

                    int first_segment = 0;
                    while (file_segment != NULL) {
                        char *temp_segment = (char *) malloc(strlen(file_segment) + 1);
                        strcpy(temp_segment, file_segment);

                        char *curr_segment = (char *) malloc(strlen(file_segment) + 1);
                        strcpy(curr_segment, file_segment);

                        file_segment = strtok_r(NULL, "\\", &saveptr1); // Looks at the future segment

                        char *sub_segment = strtok_r(temp_segment, ".", &saveptr2);

                        while (sub_segment != NULL) {
                            
                            if (file_segment != NULL) break;

                            if (strcmp(sub_segment, curr_segment) == 0) {
                                strcpy(file_name, sub_segment);
                                strcat(file_name, ".");
                                break;                        
                            } else {
                                if (strcmp(sub_segment, "txt") != 0 && strcmp(sub_segment, "crp") != 0) {
                                    if (!first_segment) {
                                        strcpy(file_name, sub_segment);
                                        first_segment = 1;
                                    } else {
                                        strcat(file_name, sub_segment);
                                    }
                                    strcat(file_name, ".");
                                } else {
                                    char *temp_extension = sub_segment;
                                    strcpy(extension, temp_extension);
                                    break;
                                }
                            }
                            sub_segment = strtok_r(NULL, ".", &saveptr2);
                        }
                        free(temp_segment);
                        free(curr_segment);
                    }
                    free(temp_arg);
                }   
            }
        }
    }

    if (!encr && !decr) {
        encr = 1;
    }

    out_name = (char *) malloc(strlen(file_name) + strlen(extension) + 1);
    strcpy(out_name, file_name);
    if (encr) strcat(out_name, "crp");
    else if (decr) strcat(out_name, "txt");

    char *fpath = (char *) malloc(strlen((strcat(_pgmptr, "\\output\\"), out_name)));
    fpath = strremove(_pgmptr, "cryptoMagic.exe\\");

    if (alt_out) {
        DIR *test_dir = opendir(alt_path);
        char * copy_path = (char *) malloc(strlen(alt_path) + 1);
        strcpy(copy_path, alt_path);
        if (test_dir) {
            printf("Alternate output directory exists! now selected as output path.\n");
            file_out = fopen(alt_path, "w");
        } else if (ENOENT == errno) {
            printf("Valid directory was provided however did not exist, attempting to create...");
            if (!mkdir(alt_path)) {
                printf("\nSuccessfully created directory at %s\n", alt_path);
                file_out = fopen(strcat(copy_path, out_name), "w");
            } else {
                printf("\nFailed to create directory at %s\nContinuing with default filepath %s\\output\\\n", alt_path, _pgmptr);
                alt_out = 0;
                file_out = fopen(strcat(fpath, out_name), "w");
            }
        } else {
            printf("Provided alternate output path is either not formatted correctly or not input correctly (alternate filepath must follow the delaration of '-O')\nContinuing with default filepath\n");
            file_out = fopen(strcat(fpath, out_name), "w");
        }
        free(copy_path);
    } else {
        file_out = fopen(strcat(fpath, out_name), "w");
    }

    return 1;
}

int encrypt() {
    int result = 1;

    char curr_line[120];

    while (fgets(curr_line, sizeof(curr_line), file) != NULL) {
        
        for (int i = 0; i < strlen(curr_line); ++i) {
            
            if (curr_line[i] == '\t') {
                if (fputs("TT", file_out) == EOF) printf("Error putting to file\n"), result = 0;
                continue;
            } else if (curr_line[i] == '\n' || curr_line[i] == '\r') {
                if (fputc(curr_line[i], file_out) == EOF) printf("Error putting to file\n"), result = 0;
                continue;
            } else if (curr_line[i] != '\n' && curr_line[i] != '\r') {
                char *encrypted = (char *) malloc(2);
                int let_ascii = ((int) curr_line[i]) - 16;
                if (let_ascii < 32) let_ascii = (let_ascii - 32) + 144;

                sprintf(encrypted, "%2X", let_ascii);
                if (fputs(encrypted, file_out) == EOF) printf("Error putting to file\n"), result = 0; 
                free(encrypted);
            }
        }
    }

    return result;
}

int decrypt() {
    int result = 1;

    char curr_line[255];

    
    while (fgets(curr_line, sizeof(curr_line), file) != NULL) {
        char *stop_str;
        char sub_str[2];
        for (int i = 0; i < strlen(curr_line); i += 2) {

            sub_str[0] = curr_line[i];
            sub_str[1] = curr_line[i+1];

            if (strcmp(sub_str, "TT") == 0) {
                if (fputc('\t', file_out) == EOF) printf("Error putting to file\n"), result = 0;
            } else if (curr_line[i] == '\n' || curr_line[i] == '\r') {
                if (fputc(curr_line[i], file_out) == EOF) printf("Error putting to file\n"), result = 0;
            } else if (curr_line[i] != '\n' && curr_line[i] != '\r' && curr_line[i + 1] != '\n' && curr_line[i + 1] != '\r') {

                int ascii = strtol(sub_str, &stop_str, 16);

                ascii += 16;

                if (ascii > 127) ascii = (ascii - 144) + 32;

                fputc(ascii, file_out);
            }
        }
    }

    return result;
}

char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}