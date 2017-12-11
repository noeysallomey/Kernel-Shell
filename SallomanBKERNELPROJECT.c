#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/stat.h>


void ClearScreen(){ //clearscreen method
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition( hStdOut, homeCoords );
}

int main(){
    char input[100];
    int CONTINUE_FLAG = 1;

    const char delimeter[2] = " ";
    char *tokens;

    char current_directory_name[100];
    if(getcwd(current_directory_name, sizeof(current_directory_name))!= NULL){
        //print
    }else{
        perror("Error in getting current directory\n");
    }

    do{

        printf ("%s>", current_directory_name);
        scanf ("%[^\n]s", input);//ask input here
        getchar();

        tokens = strtok(input, delimeter); //tokenize the input
        tokens = strlwr(tokens);

        if(strcmp("cd", tokens) == 0 || strcmp("chdir", tokens) == 0){ //cd function
            //cd();
            tokens = strtok(NULL, delimeter);
            if(tokens == NULL){                                 // if user just typed "cd" print current directory
                printf ("%s\n", current_directory_name);
            }else if(strcmp("..", tokens) == 0){                // if user typed "cd .." go one directory up
                chdir("..");
                if(getcwd(current_directory_name, sizeof(current_directory_name))!= NULL){
                    //print something
                }else{
                    perror("Error in getting current directory\n");
                }
            }else{                                              // else if user typed "cd [directory]" go to the directory
                char directory[strlen(tokens)];
                strcpy(directory, tokens);
                while(tokens != NULL){
                    tokens = strtok(NULL, delimeter);
                    if(tokens != NULL){
                        strcat(directory, " ");
                        strcat(directory, tokens);
                    }
                }
                chdir(directory);

                if(getcwd(current_directory_name, sizeof(current_directory_name))!= NULL){ // if directory is valid
                    //print something
                }else{
                    perror("Error in getting current directory\n");
                }
            }

        }else if(strcmp("cd..", tokens) == 0 || strcmp("chdir..", tokens) == 0){ // this is if user typed "cd.." without the space after cd
            chdir("..");
            if(getcwd(current_directory_name, sizeof(current_directory_name))!= NULL){ // if directory is valid
                    //print something
                }else{
                    perror("Error in getting current directory\n");
                }

        }else if(strcmp("cls", tokens) == 0){
            //cls();
            ClearScreen();                                          //calls function clearscreen

        }else if(strcmp("cmd", tokens) == 0){                       // create new instance of a command prompt
            //cmd();
            strcat(current_directory_name, "\\");
            strcat(current_directory_name, "proj.exe");
            ShellExecute(NULL, "open", current_directory_name, NULL, NULL, SW_SHOWDEFAULT);

        }else if(strcmp("copy", tokens) == 0){                      // copies file to another location if file_name already exists on the target location, overwrites it
            //copy();
            tokens = strtok(NULL, ", ");
            while(tokens != NULL){

                char file_name[strlen(tokens)];
                strcpy(file_name, tokens);

                tokens = strtok(NULL, ", ");
                char location[strlen(tokens)];
                strcpy(location, tokens);

                FILE *source, *target;
                char ch;

                source = fopen(file_name, "r");                     // read file
                if (source == NULL){

                    printf(" Invalid file name.\n");
                    exit(EXIT_FAILURE);
                }else{
                    chdir(location);
                    target = fopen(file_name, "w");                 //write file in the specified directory
                    while ((ch=fgetc(source)) != EOF ){
                        fputc(ch, target);
                    }
                    fclose(source);
                    fclose(target);
                    chdir(current_directory_name);
                    printf("Successfully copied %s\n", file_name);
                }
                tokens = strtok(NULL, ", ");
            }

        }else if(strcmp("date", tokens) == 0){                      // displays date
            //date();
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("%d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

        }else if(strcmp("del", tokens) == 0){                       // deletes file in the current directory, can delete multiple files at the same time
            //del();
            tokens = strtok(NULL, ", ");
            if(tokens != NULL){
                    printf("heya\n");
                while(tokens != NULL){
                    int delete_this;

                    delete_this = remove(tokens);

                    tokens = strtok(NULL, ", ");

                    if(delete_this == 1){
                        printf("Error. Could not delete file .\n", tokens);
                    }
                }
            }else{
                printf("Specify the file please.");
            }

        }else if(strcmp("dir", tokens) == 0){                       //displays files in the current directory
            //dir();
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir (current_directory_name)) != NULL) {
              while ((ent = readdir (dir)) != NULL) {               // print all the files and directories within directory
                printf ("%s\n", ent->d_name);
              }
              closedir (dir);
            } else {
              perror ("Error.");                                    //could not open directory
            }

        }else if(strcmp("mkdir", tokens) == 0){                     //make a new directory under the current directory
            //mkdir();
            tokens = strtok(NULL, delimeter);

            char directory[strlen(tokens)];
            strcpy(directory, tokens);
            while(tokens != NULL){
                tokens = strtok(NULL, delimeter);
                if(tokens != NULL){
                    strcat(directory, " ");
                    strcat(directory, tokens);
                }
            }

            char *new_directory = directory;
            mkdir(new_directory);

        }else if(strcmp("move", tokens) == 0){                      //move file(s) from one directory to another
            //move();

            tokens = strtok(NULL, ", ");
            while(tokens != NULL){

                char file_name[strlen(tokens)];
                strcpy(file_name, tokens);

                tokens = strtok(NULL, ", ");
                char location[strlen(tokens)];
                strcpy(location, tokens);

                FILE *source, *target;
                char ch;

                source = fopen(file_name, "r");                     // reads file
                if (source == NULL){
                    printf(" Invalid file name.\n");
                    exit(EXIT_FAILURE);
                }else{
                    chdir(location);
                    target = fopen(file_name, "w");                 //writes file in the new directory
                    while ((ch=fgetc(source)) != EOF ){
                        fputc(ch, target);
                    }
                    fclose(source);
                    fclose(target);
                    chdir(current_directory_name);
                    remove(file_name);                              //delete original file
                    printf("Successfully moved %s\n", file_name);
                }
                tokens = strtok(NULL, ", ");
            }

        }else if(strcmp("rename", tokens) == 0){                    //renames the file
            //rename();

            tokens = strtok(NULL, ", ");
            while(tokens != NULL){

                char file_name[strlen(tokens)];
                strcpy(file_name, tokens);

                tokens = strtok(NULL, ", ");
                char new_name[strlen(tokens)];
                strcpy(new_name, tokens);

                FILE *source;
                char ch;

                source = fopen(file_name, "r");                     //reads file to check if its an existing file
                if (source == NULL){
                    printf(" Invalid file name.\n");
                    exit(EXIT_FAILURE);
                }else{
                    fclose(source);                                 //closes file to rename it
                    rename(file_name, new_name);                    //renames file
                    printf("Successfully renamed %s to %s\n", file_name, new_name);
                }
                tokens = strtok(NULL, ", ");
            }

        }else if(strcmp("rmdir", tokens) == 0){                     //removes a directory in current directory
            //rmdir();
            tokens = strtok(NULL, delimeter);

            char directory[strlen(tokens)];
            strcpy(directory, tokens);
            while(tokens != NULL){
                tokens = strtok(NULL, delimeter);
                if(tokens != NULL){
                    strcat(directory, " ");
                    strcat(directory, tokens);
                }
            }

            char *new_directory = directory;
            rmdir(new_directory);                                   //removes directory

        }else if(strcmp("time", tokens) == 0){                      //displays time
            //time();
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("%d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

        }else if(strcmp("type", tokens) == 0){                      //reads and displays the content of  atext file
            //type();
            tokens = strtok(NULL, delimeter);

            FILE *file;
            char temp[1000];

            char file_directory[strlen(tokens)];
            strcpy(file_directory, tokens);

            while(tokens != NULL){
                tokens = strtok(NULL, delimeter);
                if(tokens != NULL){
                    strcat(file_directory, " ");
                    strcat(file_directory, tokens);
                }
            }

            FILE *ptr_file;
            char buffer[1000];

            ptr_file = fopen(file_directory,"r");

            if (!ptr_file)
                printf("Error.");

            while (fgets(buffer,1000, ptr_file)!=NULL)
                printf("%s",buffer);

            printf("\n\n");

            fclose(ptr_file);
        }else if(strcasecmp("exit", tokens) == 0){
            CONTINUE_FLAG = 0;
        }

    }while(CONTINUE_FLAG); //continue while user havent typed "exit" yet


}

