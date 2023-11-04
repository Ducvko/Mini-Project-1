#include "cryptoMagic.h`"

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
*/


int encr = 0;
int decr = 0;

int keep = 0;

FILE *file;
char *file_name = "";
char *extension;

char *saveptr1;
char *saveptr2;
char *saveptr3;


int main(int argc, char *argv[]) {

    check_param(argc, argv);

    return 0;
}

int check_param(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {

        if (strcmp(argv[i], "-E")) encr = 1;
        else if (strcmp(argv[i], "-D")) decr = 1;

        if (strcmp(argv[i], "-KEEP_ORIG")) keep = 1;

        if (encr && decr) {
            printf("You cannot specify the encrypt (-E) and decrypt (-D) parameter in the same command!\n\n");
            exit(-1);
        }

        if (file == NULL) {
            if (strcmp(argv[i-1], "-E") == 0 || strcmp(argv[i-1], "-D") == 0) {
                file = fopen(argv[i], "r");
                if (file != NULL) {
                    char *file_segment = strtok_r(argv[i], "\\", &saveptr1);

                    while (file_segment != NULL) {
                        char *temp_segment;
                        strcpy(temp_segment, file_segment);

                        char *curr_segment;
                        strcpy(curr_segment, file_segment);

                        char *next_segment = strtok_r(NULL, "\\", &saveptr1); // Looks at the future segment

                        char *sub_segment = strtok_r(temp_segment, ".", &saveptr2);

                        while (sub_segment != NULL) {
                            
                            if (next_segment != NULL) break;

                            if (strcmp(sub_segment, curr_segment) == 0) {
                                file_name = sub_segment;
                                break;                        
                            } else {
                                if (strcmp(sub_segment, "txt") != 0 && strcmp(sub_segment, "crp") != 0) {
                                    strcat(file_name, sub_segment);
                                    strcat(file_name, ".");
                                } else {
                                    extension = sub_segment;
                                    break;
                                }
                            }
                            sub_segment = strtok_r(NULL, ".", &saveptr2);
                        }
                    }
                }  
            } 
        }
        printf("%s", file_name);
    }
    if (!encr && !encr) {
        encr = 1;
    }

    return 0;
}

int encrypt(FILE* file) {
    int result;

    return result;
}

int decrypt(FILE* file) {
    int result;


    return result;
}