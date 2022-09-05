#include "./HeaderFiles/bigHeader.h"

void ProcessCommand(char *CmdStr)
{
    // First tokenise the string.
    // we need to deal with other de-limiters like ; & | etc. For Phase 1 however, only one command in the string.
    // So delim using space

    char **TokenArr = (char**)calloc(5,sizeof(char*));
  
    // the string can have all the delimiters
    const char delimPtr[3] = " \n";

    // StrTok makes changes in the original string itself. Replaces the delimiters with NULL.
    // So the original string itself is modified after strtok!!!
    // So hence, each of the pointers in TokenArr are not malloced! They are pointing to the og string...

    TokenArr[0] = strtok(CmdStr,delimPtr);

    if (TokenArr[0] == NULL)
        return;

    char *temp;

    int count = 1;
    while(temp = strtok(NULL,delimPtr))
    {
        TokenArr[count] = temp;
        count++;
    }

///////////////
    
    if(strcmp(TokenArr[0],"cd") == 0)
        HandleCD(TokenArr,count);
    
    else if (strcmp(TokenArr[0],"echo") == 0)
    {
        // deal with echo
        for (int i = 1; i<count-1; i++)
            printf("%s ",TokenArr[i]);
        printf("%s",TokenArr[count-1]);
    }
    else if (strcmp(TokenArr[0],"pwd") == 0)
        printf("%s\n",CurrLocation);

    // else if (strcmp(TokenArr[0],"ls") == 0)
    //     HandleLS(TokenArr,count);
    
    else
    {
        if (strcmp(TokenArr[count-1], "&") == 0)
        {
            // memset(TokenArr[count-1],'\0',1);
            // free(TokenArr[count-1]);
            TokenArr[count-1] = NULL;
            count--;
            command_bg(TokenArr, count);
        }
        else
            command_fg(TokenArr, count);
    }    
/////////////
    free(TokenArr);
}

int main()
{
    // Get username and Hostname and create TerminalName    
    setTerminalDisplay();
    // Place of running the code is our root (~).
    InitLocations();

    initBGqueue();

    size_t InpLen = 100;

    char *UserInpBuff;

    signal(SIGCHLD,ChildDone);

    while(1)
    {
        UserInpBuff = (char*)malloc(InpLen);
        TerminalPrompt();
        getline(&UserInpBuff,&InpLen,stdin);
    
        // Tokenise the command.
        ProcessCommand(UserInpBuff);

        // ig this resets the string?
        // UserInpBuff[0] = '\0';
        free(UserInpBuff);
    }
}
