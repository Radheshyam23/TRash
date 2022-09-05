#include "./HeaderFiles/bigHeader.h"

char* GetRelativePath()
{
    char *RelPath = (char*)malloc(60);
    strcpy(RelPath,":~");
    if (strcmp(CurrLocation, RootLocation) != 0)
    {
        // Checking if the CurrLocation contains the RootLocation.
        // It is present.
        if(strstr(CurrLocation,RootLocation))
            strncpy(&RelPath[2],&CurrLocation[strlen(RootLocation)],strlen(CurrLocation)-strlen(RootLocation));
 
        // It isn't present. Then we display the absolute path instead of the relative path
        else
        {
            memset(&RelPath[1],'\0',1);
            strcat(RelPath,CurrLocation);
        }
    }
    // timeAppend();
    // if (processTime >= 1)
    // {
    //     strcat(RelPath, "took ");
    //     strcat(RelPath,)
    // }
    // also reset processTime to 0.
    strcat(RelPath,">\0");
    return RelPath;
}

void cdDotDot()
{
    // printf("Entered dot dot\n");
    char *SaveCurrLoc = (char*)malloc(60);
    strcpy(SaveCurrLoc,CurrLocation);

    char ch;
    int i;
    
    // printf("CurrLoc %s, len %ld, PrevLoc %s\n",CurrLocation,strlen(CurrLocation),PrevLocation);
    
    for (i=strlen(CurrLocation)-1; i>=0; i--)
    {
        if (CurrLocation[i] == '/')
            break;
    }

    // printf("Last slash: %d\n",i);

    if (i!=0)
    {
        memset(CurrLocation,'\0',strlen(CurrLocation));
        // printf("%s\n",CurrLocation);
        strncpy(CurrLocation,SaveCurrLoc,i);
        strcpy(PrevLocation,SaveCurrLoc);
        chdir(CurrLocation);
    }

    // printf("CurrLoc %s, PervLoc %s\n",CurrLocation,PrevLocation);
}

void PrevDir()
{
    char temp[60];
    strcpy(temp,PrevLocation);
    strcpy(PrevLocation,CurrLocation);
    strcpy(CurrLocation,temp);
    chdir(CurrLocation);
}

void GoToLocation(char *Location)
{
    printf("Location: %s\n",Location);

    DIR *Dir;
    Dir = opendir(Location);
    closedir(Dir);

    printf("opendir done\n");

    // Location doesnt exist
    if (Dir == NULL)
    {
        printf("No loc :(\n");
        return;
    }        
    // Get the full path of the location and store in CurrLocation
    printf("Read full path\n");
    char *FullPath = realpath(Location,NULL);
    printf("Full path got\n");
    if (FullPath == NULL)
    {
        printf("No path\n");
        return;
    }

    printf("Path is: %s\n",FullPath);

    // Update PrevLocation
    strcpy(PrevLocation,CurrLocation);
    strcpy(CurrLocation,FullPath);
    chdir(CurrLocation);
}

void HandleCD(char **TokenArr, int count)
{
        // Invalid
        if (count > 2)
            return;
        
        // going to root directory
        if (count == 1)
        {
            strcpy(PrevLocation,CurrLocation);
            strcpy(CurrLocation,RootLocation);
            chdir(CurrLocation);
        }
        
        else if (strcmp(TokenArr[1],"~") == 0)
        {
            strcpy(PrevLocation,CurrLocation);
            strcpy(CurrLocation,RootLocation);
            chdir(CurrLocation);
        }

        else if (strcmp(TokenArr[1],"..") == 0)
            cdDotDot();
        else if (strcmp(TokenArr[1], "-") == 0)
            PrevDir();
        else if (strcmp(TokenArr[1], ".") == 0)
            strcpy(PrevLocation,CurrLocation);
        else
            GoToLocation(TokenArr[1]);
        // else if the path provided as TokenArr[1] exists go there

        // PENDING!!

    }