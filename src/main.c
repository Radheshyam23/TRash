#include "./HeaderFiles/bigHeader.h"

void ProcessCommand(char *CmdStr)
{
    // First tokenise the string.
    // we need to deal with other de-limiters like ; & | etc. For Phase 1 however, only one command in the string.
    // So delim using space
    
    // printf("#a\n");
    addHistory(CmdStr);
    // printf("a#\n");
    char **TokenArr = (char**)calloc(20,sizeof(char*));
  
    // the string can have all the delimiters
    const char delimPtr[3] = " \n";

    // StrTok makes changes in the original string itself. Replaces the delimiters with NULL.
    // So the original string itself is modified after strtok!!!
    // So hence, each of the pointers in TokenArr are not malloced! They are pointing to the og string...

    int RedirFlag = 0;

    TokenArr[0] = strtok(CmdStr,delimPtr);

    if (TokenArr[0] == NULL)
        return;
    else if (strcmp(TokenArr[0],"<") == 0 || strcmp(TokenArr[0],">") == 0 || strcmp(TokenArr[0],">>") == 0)
        RedirFlag = 1;

    char *temp;

    int count = 1;

    // printf("#b#\n");

    while(temp = strtok(NULL,delimPtr))
    {
        TokenArr[count] = temp;
        if (strcmp(TokenArr[count],"<") == 0 || strcmp(TokenArr[count],">") == 0 || strcmp(TokenArr[count],">>") == 0)
            RedirFlag = 1;
        count++;
    }
    
    // for (int i = 0; i < count; i++)
    //     printf("##%s##|",TokenArr[i]);
    // printf("\n");

    if (RedirFlag == 1)
        HandleRedirection(TokenArr,count);

    else if(strcmp(TokenArr[0],"cd") == 0)
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

    else if(strcmp(TokenArr[0],"exit") == 0)
    {
        cacheHistory();
        printf("Bye!\n");
        exit(0);
        return;
        // printf("bruhhhhhh\n");
    }

    else if(strcmp(TokenArr[0],"jobs") == 0)
    {
        // printf("In Jobs main\n");
        if (count > 1)
        {
            printf("Too many arguments\n");
            return;
        }        
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
    free(TokenArr);
}

int main()
{
    // Get username and Hostname and create TerminalName    
    setTerminalDisplay();
    // Place of running the code is our root (~).
    InitLocations();
    retrieveHistory();

    initBGqueue();

    size_t InpLen = 100;

    char *UserInpBuff;

    signal(SIGCHLD,ChildDone);

    // while(1)
    // {
    //     UserInpBuff = (char*)malloc(InpLen);
    //     TerminalPrompt();
    //     getline(&UserInpBuff,&InpLen,stdin);
    
    //     // Tokenise the command.
        // ProcessCommand(UserInpBuff);

    //     // ig this resets the string?
    //     // UserInpBuff[0] = '\0';
    //     free(UserInpBuff);
    // }

        
    char *inp = malloc(sizeof(char) * InpLen);
    char c;
    while (1) 
    {
        setbuf(stdout, NULL);
        enableRawMode();
        // printf("Prompt>");
        TerminalPrompt();
        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) 
        {
            if (iscntrl(c)) 
            {
                if (c == 10)    //newline
                {
                    inp[pt++] = c;
                    printf("%c", c);
                    break;
                }
                else if (c == 27) 
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } 
                else if (c == 127) // backspace 
                { 
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } 
                else if (c == 9) // TAB character
                {
                    inp[pt++] = c;
                    for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                        printf(" ");
                    }
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } 
            else 
            {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        // printf("\nInput Read: [%s]\n", inp);
        ProcessCommand(inp);
    }

    return 0;
}
