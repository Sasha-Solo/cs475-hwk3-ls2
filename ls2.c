#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

// TODO: function definitions here for ls2
/**
 * Opens the directory given by path and recursively show all files’ name and size in bytes
 *
 * @param	*argument pointer to the command line argument 
 * @param   counter for indents 
 */
void modeOne(char *argument, int counter){

	char newIndent[500];
	char entryName[50];

    DIR *stream; //directory stream
	struct dirent *entry; //points to the struct, struct has information about the entry 
	struct stat statStruct; //info about the file passed in

    stream = opendir(argument); //open the directory stream

    while((entry = readdir(stream)) != NULL){ //while there are still files/dirs to read...
        
		chdir(argument); //change to the directory passed in
		
		//puts info about the file/dir we are looking at into the struct
        lstat(entry->d_name, &statStruct); 
        
		strcpy(entryName, entry->d_name);

		//check if we have a file or dir
        if (S_ISDIR(statStruct.st_mode)){
			//check if we are on current or parent dir and ignore
			if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0){
				continue;
			}
			
			strcat(entryName, "/ (directory)");
			newIndent[0] = '\0';
			for (int i = 0; i < counter; i++){
				strcat(newIndent, INDENT);
			}
			printf("%s", newIndent);
			//strcat(newIndent, entryName);
			printf("%s\n", entryName); //print if it is a directory
			entryName[0] = '\0'; //clear entryName
			
			modeOne(entry->d_name, counter + 1); //recurse to find subdirs
	
        }
        else if (S_ISREG(statStruct.st_mode)){

			char buffer[500]; 
			sprintf(buffer, " (%ld bytes)", statStruct.st_size);

			//printf("buffer is %s\n", buffer);
			
			strcat(entryName, buffer);
			//printf("entry name is %s\n", entryName);
			newIndent[0] = '\0';
			//printf("%d", counter);
			for (int i = 0; i < counter; i++){
				strcat(newIndent, INDENT);
			}

			printf("%s", newIndent);

			//strcat(newIndent, entryName);

			printf("%s\n", entryName); //print the size
        }
    }
	chdir(".."); //need to jump out of the directory that we are currently in and continue
    closedir(stream);
}

/**
 * Only shows files that match the optional-file that the user gives, and all directories
 * and subdirectories the files are contained inside
 *
 * @param	*argument pointer to the command line argument 
 * @param   *filename the file to match files with
 * @param   *s2 pointer to the stack
 * @param   counter2  for indents 
 * @result  integer representing if file was found or not 
 */
int modeTwo(char *argument, char *filename, stack_t *s2, int counter2){
	char newIndent[500];
	int truth = 0; //keeps track of if we need to add dirs/subdirs, set to false initially

	DIR *stream; //directory stream
	struct dirent *entry; //points to the struct, struct has information about the entry 
	struct stat statStruct; //info about the file passed in

    stream = opendir(argument); //open the directory stream

    while((entry = readdir(stream)) != NULL){ //while there are still files/dirs to read...
        
		chdir(argument); //change to the directory passed in
		//puts info about the file/dir we are looking at into the struct
        lstat(entry->d_name, &statStruct); 

		if (S_ISDIR(statStruct.st_mode)){
			//check if we are on current or parent dir and ignore
			if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0){
				continue;
			}

			int m2 = modeTwo(entry->d_name, filename, s2, counter2 + 1); //recurse to find subdirs
			
			if (m2 == 1){
				truth = 1;

				strcat(entry->d_name, "/ (directory)");
				
				newIndent[0] = '\0';
				for (int i = 0; i < counter2; i++){
					strcat(newIndent, INDENT);
				}
			
				//push dirs onto stack
				char* space = (char*) malloc(strlen(entry->d_name) + 1 + strlen(newIndent)); //make space for dir
				memset(space, 0, strlen(entry->d_name) + 1 +  strlen(newIndent));
				strcat(space, newIndent);
				strcat(space, entry->d_name); //puts dir into the allocated memory block

				push(s2, space); //push onto stack
			}
    	}

	else if (S_ISREG(statStruct.st_mode)){
		if (strcmp(entry->d_name, filename) == 0){ //if we found the file we want...

			char buffer[500]; 
			sprintf(buffer, " (%ld bytes)", statStruct.st_size);
			strcat(entry->d_name, buffer);

			newIndent[0] = '\0';
			for (int i = 0; i < counter2; i++){
				strcat(newIndent, INDENT);
			}

			char* space = (char*) malloc(strlen(entry->d_name) + 1 + strlen(newIndent)); //make space for filename
			memset(space, 0, strlen(entry->d_name) + 1 + strlen(newIndent));
			strcat(space, newIndent);
			strcat(space, entry->d_name); //puts filename into the allocated memory block

			push(s2, space); //push onto stack
			truth = 1; //if we found the file, set truth to true
		}
    }
	
}   //end while

	chdir(".."); //need to jump out of the directory that we are currently in and continue

    closedir(stream);
	
	return truth;
} //end method