#include "systemcalls.h"
#include <sys/wait.h>
#include <stdlib.h>
       #include <sys/types.h>
       #include <unistd.h>
#include<fcntl.h>

#define _XOPEN_SOURCE

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/



bool do_system(const char *cmd)
{
int ret;
/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

ret =  system(cmd);
if (ret==-1){return false;}
if(! WIFEXITED(ret) )
return false;
    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


  int status;
    pid_t pid;
    int rt;

    //create a child process
    pid = fork();

    //fork fail condition
    if(pid == -1){
        perror("fork error");
        return false;
    }
    else if(pid == 0){//Child process
        
        rt = execv(command[0],command);
        if(rt == -1){
            perror("execv error");
            exit(-1); //same as false
        }

    }else{ //Parent process

        //wait for child process exit
        if(waitpid(pid,&status,0) == -1){
            perror("wait error");
            return false;
        }
        
        //check exit status after wait
        if( ! (WIFEXITED(status)) || WEXITSTATUS(status)){
            perror("wait error");
            return false;
        }
    }
va_end(args);
    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    int status;
    pid_t pid;
    int rt;
   int fd;

   fd =open(outputfile,O_WRONLY | O_CREAT | O_TRUNC,
S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);

    pid = fork();

    //fork fail condition
    if(pid == -1){
        perror("fork error");
        return false;
    }
    else if(pid == 0){//Child process
        dup2(fd,1);
        rt = execv(command[0],command);
        if(rt == -1){
            perror("execv error");
            exit(-1); //same as false
        }

    }else{ //Parent process

        //wait for child process exit
        if(waitpid(pid,&status,0) == -1){
            perror("wait error");
            return false;
        }
        
        //check exit status after wait
        if( ! (WIFEXITED(status)) ){
            perror("wait error");
            return false;
        }
    }


    va_end(args);

    return true;
}

