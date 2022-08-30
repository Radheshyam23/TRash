#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>

char TerminalName[60];
char CurrLocation[60];
char RootLocation[60];

void setTerminalDisplay()
{
    int nameLen = 30;
    char *HostName = (char*)malloc(nameLen*sizeof(char));
    char *UserName = (char*)malloc(nameLen*sizeof(char));

    gethostname(HostName,nameLen);
    UserName = getlogin();

    strcat(UserName,"\0");
    strcat(HostName,"\0");

    TerminalName[0] = '<';
    strcat(TerminalName,UserName);
    strcat(TerminalName,"@");
    strcat(TerminalName,HostName);
    strcat(TerminalName,":~");

    // free(UserName);
    // free(HostName);
}

char* GetRelativePath()
{
    char *RelPath = (char*)malloc(60);
    if (strcmp(CurrLocation, RootLocation) == 0)
    {
        strcpy(RelPath,">\0");
    }
    else
    {
        // Checking if the CurrLocation contains the RootLocation.
        // It is present.
        if(strstr(CurrLocation,RootLocation))
            strncpy(RelPath,&CurrLocation[strlen(RootLocation)],strlen(CurrLocation)-strlen(RootLocation));
 
        // It isn't present. Then we display the absolute path instead of the relative path
        else
            strcpy(RelPath,CurrLocation);
        
        strcat(RelPath,">\0");
    }
    return RelPath;
}

void TerminalPrompt()
{
    // Find the current directory and print:
    // <username@hostname:~path_to_cwd>
    // TerminalName path_to_cwd>
    char *RelativePath = GetRelativePath();
    printf("%s%s",TerminalName,RelativePath);
}

void InitLocations()
{
    // Get the cwd and store it in root.
    getcwd(RootLocation,60);
    // also store it as the currentLocation
    strcpy(CurrLocation,RootLocation);

    // testing:
    // strcat(CurrLocation,"/temp");
    // strcpy(CurrLocation,"/home/radheshyam/Desktop/");
}

void ProcessCommand(char *CmdStr)
{
    // First tokenise the string.
    // we need to deal with other de-limiters like ; & | etc. For Phase 1 however, only one command in the string.
    // So delim using space

    char **TokenArr = (char**)malloc(sizeof(char*)*5);
  
    // the string can have all the delimiters
    const char delimPtr[3] = " \n";

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
    // if count exceeds 5, need to realloc

    
    if(strcmp(TokenArr[0],"cd") == 0)
    {
        // deal with cd

        // Invalid
        if (count > 2)
            return;
        
        // going to root directory
        if (count == 1)
            strcpy(CurrLocation,RootLocation);
        
        else if (TokenArr[1] == '~')
            strcpy(CurrLocation,RootLocation);

        else if (TokenArr[1] == '..')
            ;

        // PENDING!!

    }
    else if (strcmp(TokenArr[0],"echo") == 0)
    {
        // deal with echo
        for (int i = 1; i<count-1; i++)
            printf("%s ",TokenArr[i]);
        printf("%s",TokenArr[count-1]);
    }
    else if (strcmp(TokenArr[0],"pwd") == 0)
    {
        printf("%s\n",CurrLocation);
    }
    
}

int main()
{
    // Get username and Hostname and create TerminalName    
    setTerminalDisplay();
    // Place of running the code is our root (~).
    InitLocations();

    size_t InpLen = 100;
    char *UserInpBuff = (char*)malloc(InpLen);

    while(1)
    {
        TerminalPrompt();
        getline(&UserInpBuff,&InpLen,stdin);
    
        // Tokenise the command.
        ProcessCommand(UserInpBuff);

        // ig this resets the string?
        UserInpBuff[0] = '\0';
    }
}
