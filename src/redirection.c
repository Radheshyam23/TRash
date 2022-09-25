#include "./HeaderFiles/bigHeader.h"

void HandleRedirection(char **TokenArr, int count)
{

    pid_t pid = fork();

    if (pid == 0)
    {
        int SaveIn = dup(STDIN_FILENO);
        int SaveOut = dup(STDOUT_FILENO);

        // Check if there is a redirection symbol and replace the buffers
        for (int i=0; i<count; i++)
        {
            if (strcmp(TokenArr[i],"<") == 0)
            {
                if (i == count-1)
                {
                    printf("No input file\n");
                    exit(0);
                    return;
                }
                int inpFileFD = open(TokenArr[i+1], O_RDONLY);
                // change StandInp to inpFile
                dup2(inpFileFD, STDIN_FILENO);

                close(inpFileFD);
            }
            else if(strcmp(TokenArr[i],">") == 0)
            {
                if (i == count-1)
                {
                    printf("No output file name given\n");
                    exit(0);
                    return;
                }
                int opFileFD = open(TokenArr[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

                // change StandInp to inpFile
                dup2(opFileFD, STDOUT_FILENO);

                close(opFileFD);
            }
            else if(strcmp(TokenArr[i],">>") == 0)
            {
                if (i == count-1)
                {
                    printf("No output file name given\n");
                    exit(0);
                    return;
                }
                int opFileFD = open(TokenArr[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);

                // change StandInp to inpFile
                dup2(opFileFD, STDOUT_FILENO);

                close(opFileFD);
            }
            else
                continue;

            // remove tokens i and i+1
            for(int k = i; k<count-2; k++)
                TokenArr[k] = TokenArr[k+2];
            
            count -= 2;
            i-=1;
        }

        // Execute the command

        if(strcmp(TokenArr[0],"cd") == 0)
            HandleCD(TokenArr,count);
        
        else if (strcmp(TokenArr[0],"echo") == 0)
        {
            // deal with echo
            for (int i = 1; i<count-1; i++)
                printf("%s ",TokenArr[i]);
            printf("%s\n",TokenArr[count-1]);
        }
        else if (strcmp(TokenArr[0],"pwd") == 0)
            printf("%s\n",CurrLocation);

        else if (strcmp(TokenArr[0],"ls") == 0)
            HandleLS(TokenArr,count);

        else if(strcmp(TokenArr[0],"pinfo") == 0)
            HandlePinfo(TokenArr,count);
        
        else if(strcmp(TokenArr[0],"history") == 0)
            dispHistory();
        
        else if(strcmp(TokenArr[0],"jobs") == 0)
        {
            // printf("In Jobs main\n");
            if (count > 1)
                printf("Too many arguments\n");        
            else
                DispAllBG();
        }
        else
        {
            if (strcmp(TokenArr[count-1], "&") == 0)
            {
                // memset(TokenArr[count-1],'\0',1);
                // free(TokenArr[count-1]);
                TokenArr[count-1] = NULL;
                count--;
                command_bg(TokenArr, count);
                // printf("Created a bg process\n");
            }
            else
            {
                command_fg(TokenArr, count);
                // printf("Created a FG proc\n");
            }
        }

        // Restore the StandardInp and StandardOp files
        dup2(SaveIn, STDIN_FILENO);
        dup2(SaveOut, STDOUT_FILENO);
        exit(0);
    }

    else if (pid < 0)
    {
        perror("<Fork:Redirection>");
    }

    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}