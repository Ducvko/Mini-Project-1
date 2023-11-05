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
char *abs_path;

FILE *file;
char *file_name;
char extension[3];
char *out_name;

FILE *file_out;


int main(int argc, char *argv[]) {

    if (!check_param(argc, argv)) exit (-1);

    if (encr) {
        if (encrypt()) {
            printf("Encryption Successful!\nOutput file at: ");
            if (!alt_out) printf("%s\\output\\%s", _pgmptr, out_name);
            else printf("%s\\%s", abs_path, out_name);
        } else {
            exit(-1);
        }
    } else if (decr) {
        if (decrypt()) {
            printf("Decryption Successful!\nOutput file at:");
            if (!alt_out) printf("%s\\output\\%s", _pgmptr, out_name);
            else printf("%s\\%s", abs_path, out_name);
        } else {
            exit(-1);
        }
    }

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

        if (strcmp(argv[i], "-KEEP_ORIG")) keep = 1;

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

        if (file == NULL) {
            file = fopen(argv[i], "r");
            if (file != NULL) {

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
            }   
        }
    }
    if (!encr && !encr) {
        encr = 1;
    }

    out_name = (char *) malloc(strlen(file_name) + strlen(extension) + 1);
    strcpy(out_name, file_name);
    if (strcmp(extension, "txt") == 0) strcat(out_name, "crp");
    else strcat(out_name, "txt");

    if (alt_out) {
        DIR *test_dir = opendir(abs_path);
        char * copy_path = (char *) malloc(strlen(abs_path) + 1);
        strcpy(copy_path, abs_path);
        if (test_dir) {
            printf("Alternate output directory exists! now selected as output path.\n");
            file_out = fopen(abs_path, "w");
        } else if (ENOENT == errno) {
            printf("Valid directory was provided however did not exist, attempting to create...");
            if (!mkdir(abs_path)) {
                printf("\nSuccessfully created directory at %s\n", abs_path);
                file_out = fopen(strcat(copy_path, out_name), "w");
            } else {
                printf("\nFailed to create directory at %s\nContinuing with default filepath %s\\output\\\n", abs_path, _pgmptr);
                alt_out = 0;
                file_out = fopen(strcat(strcat(_pgmptr, "\\output\\"), out_name), "w");
            }
        } else {
            printf("Provided alternate output path is either not formatted correctly or not input correctly (alternate filepath must follow the delaration of '-O')\nContinuing with default filepath");
        }
        free(copy_path);
    } else {
        printf("%s", strcat(strcat(_pgmptr, "\\output\\"), out_name));
        file_out = fopen(strcat(strcat(_pgmptr, "\\output\\"), out_name), "w");
    }

    return 1;
}

int encrypt() {
    int result = 0;

    char output_array[255];

    char curr_line[120];


    while (fgets(curr_line, sizeof(curr_line), file) != NULL) {
        int first_put = 1;
        int EOL = min(min(indexOf(curr_line, (char) '\r'), indexOf(curr_line, (char) '\n')), strlen(curr_line));
        for (int i = 0; i <= EOL; ++i) {
            char encrypted[2];
            if (curr_line[i] == '\t') {
                encrypted[0] = 'T';
                encrypted[1] = 'T';
            } else if (curr_line[i] != '\n') {
                int let_ascii = ((int) curr_line[i]) - 16;
                if (let_ascii < 32) let_ascii = (let_ascii - 32) + 144;

                sprintf(encrypted, "%2X", let_ascii);
            }

            if (first_put){ 
                strcpy(output_array, encrypted);
                first_put = 0;
            } else {
                strcat(output_array, encrypted);
            } 
        }

        fputs(output_array, file_out);
        
    }



    return result;
}

int decrypt() {
    int result = 0;
    // output_array = (char *) malloc(121);


    // free(output_array);
    return result;
}

int indexOf(const char * str, const char toFind) {
    int i = 0;

    while(str[i] != '\0')
    {
        if(str[i] == toFind)
            return i;
        i++;
    }
    // Return -1 as character not found
    return INT_MAX;
}