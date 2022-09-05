#include "./HeaderFiles/bigHeader.h"

void InitLocations()
{
    // Get the cwd and store it in root.
    getcwd(RootLocation,60);
    // also store it as the currentLocation
    strcpy(CurrLocation,RootLocation);
    strcpy(PrevLocation,CurrLocation);

    // testing:
    // strcat(CurrLocation,"/temp");
    // strcpy(CurrLocation,"/home/radheshyam/Desktop/");
}

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
    // strcat(TerminalName,":~");

    // free(UserName);
    // free(HostName);
}

void TerminalPrompt()
{
    // Find the current directory and print:
    // <username@hostname:~path_to_cwd>
    // TerminalName path_to_cwd>
    char *RelativePath = GetRelativePath();
    printf("%s%s",TerminalName,RelativePath);
}