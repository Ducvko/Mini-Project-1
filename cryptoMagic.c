#include "cryptoMagic.h"
#include <windows.h>
#include <dirent.h>
#include <errno.h>
#include <shlwapi.h>

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
 *      -O proceeding file path is the output filepath 
*/


int encr = 0;
int decr = 0;

int keep = 0;

int alt_out = 0;
char *alt_path;
char *abs_path;

FILE *file;
char *file_name;
char *extension;

FILE *file_out;



int main(int argc, char *argv[]) {

    if (!check_param(argc, argv)) exit (-1);

    if (encr) {
        file_out = fopen(strcat("output\\", file_name), "w");
        if (encrypt()) {
            if (!alt_out) printf("Encryption Successful!\nOutput file at: %s\\output\\%s", _pgmptr, strcat(file_name, "crp"));
            else printf("Encryption Successful!\nOutput file at: %s\\%s", alt_path, strcat(file_name, "crp"));
        } else {
            exit(-1);
        }
    } else if (decr) {
        if (decrypt()) {
            printf("Decryption Successful!\nOutput file at: %s", );
        } else {
            exit(-1);
        }
    }

    return 0;
}

int check_param(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {

        char *saveptr1;
        char *saveptr2;
        char *saveptr3;

        if (strcmp(argv[i], "-E") == 0) encr = 1;
        else if (strcmp(argv[i], "-D") == 0) decr = 1;

        if (strcmp(argv[i], "-KEEP_ORIG")) keep = 1;

        if (strcmp(argv[i], "-O") == 0) alt_out = 1;

        if (alt_out) {
            if (strcmp(argv[i-1], "-O")) {
                alt_path = argv[i];
                if (PathIsRelative(alt_path)) {
                    _fullpath(abs_path, alt_path, _MAX_PATH);
                } else {
                    abs_path = alt_path;
                }
            }
        }

        if (encr == 1 && decr == 1) {
            printf("You cannot specify the encrypt (-E) and decrypt (-D) parameter in the same command!\n\n");
            return 0;
        }

        if (file == NULL) {
            file = fopen(argv[i], "r");
            printf("file opened\n");
            if (file != NULL) {
                printf("file valid\n");
                printf("file_name: %s\n", argv[i]);

                char *file_segment = strtok_r(argv[i], "\\", &saveptr1);
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
                            file_name = sub_segment;
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
                                extension = sub_segment;
                                break;
                            }
                        }
                        sub_segment = strtok_r(NULL, ".", &saveptr2);
                    }
                }

                printf("%s", file_name);
            }   
        }
    }
    if (!encr && !encr) {
        encr = 1;
    }

    if (alt_out) {
        DIR *test_dir = opendir(abs_path);
        if (test_dir) {
            printf("Alternate output directory exists! now selected as output path.\n");
        } else if (ENOENT == errno) {
            printf("Valid directory was provided however did not exist, attempting to create...");
            if (!mkdir(abs_path)) printf("\nSuccessfully created directory at %s\n", abs_path);
        } else {
            printf("Provided alternate output path is either not formatted correctly or not input correctly (alternate filepath must follow the delaration of '-O')\nContinuing with default filepath");
        }
    }

    return 1;
}

int encrypt() {
    int result = 0;



    return result;
}

int decrypt() {
    int result = 0;

    return result;
}