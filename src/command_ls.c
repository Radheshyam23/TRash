#include "./HeaderFiles/bigHeader.h"

int CompFiles(const void *a, const void *b)
{
    return strcasecmp(*(const char**)a, *(const char**)b);
}

void HandleLS(char **TokensArr, int count)
{
    int opt;
    optind = 0;     // This is some inbuilt variable in getopt?? 
                    // We need to reset it to 0 to reuse getopt.

    int lFlag = 0, aFlag =0;
    int nonFlag = 0;

    // get the flags
    while ((opt = getopt(count, TokensArr, "al")) != -1)
    {
        // if unrecognised flag, it returns ?
        if (opt == '?')
            return;
        if (opt == 'l')
            lFlag = 1;
        else if (opt == 'a')
            aFlag = 1;
    }

    DIR *dir;
    struct dirent *FileDets;

    
    for (int i=1; i<count; i++)
    {
        if (TokensArr[i][0] != '-')
            nonFlag ++;
    }

    // Basically, no path specified.
    if (nonFlag == 0)
    {
        char *dirPath = (char*)malloc(60);
        strcpy(dirPath,CurrLocation);
        TokensArr[count] = dirPath;
        count++;
    }

    // basically there may be multiple directories in the parameters...
    for (int i=1; i<count; i++)
    {
        if (TokensArr[i][0] == '-')
            continue;
        
        dir = opendir(TokensArr[i]);
        if (dir == NULL)
        {
            printf("Invalid directory/file %s??\n",TokensArr[i]);
            continue;
        }

        char **FilesList = (char**)malloc(100*sizeof(char*));
        int FolderSize = 0; 
        int FileCount = 0;
        // handle realloc also...

        // Collect file names first.
        while((FileDets = readdir(dir)) != NULL)
        {
            if (aFlag == 0 && FileDets->d_name[0] == '.')
                continue;

            // Get list of all file names
            // Calculate the total size
            // Then for loop printing for each file

            char *FileName = (char*)malloc(100);
            strcpy(FileName,CurrLocation);
            strcat(FileName,"/");
            strcat(FileName,FileDets->d_name);
            // strcpy(FileName,FileDets->d_name);
            FilesList[FileCount] = FileName;
            FileCount++;

            struct stat fileStat;
            lstat(FileName,&fileStat);

            // FolderSize += fileStat.st_blocks; 
            FolderSize += fileStat.st_blocks;  
            // check the block size. 
        }
        
        if (nonFlag > 1)
            printf("%s:\n",TokensArr[i]);

        // printf("OG array list %d:\n",FolderSize);
        // for (int i=0; i<FileCount; i++)
        //     printf("%s ",FilesList[i]);
        // printf("\n");

        // Sort the files array.
        qsort(FilesList,FileCount,sizeof(const char*),CompFiles);

        int printOffset = strlen(CurrLocation) + 1;

        // Now print the files
        if (lFlag == 1)
        {
            // Print folder size
            // print details

            printf("total %d\n",FolderSize/2);
            
            struct stat fileStat;

            for (int i=0; i<FileCount; i++)
            {
                lstat(FilesList[i],&fileStat);
                printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                printf(" %ld ",fileStat.st_nlink);
                // struct passwd *pw = getpwuid(fileStat.st_uid);
                // struct group  *gr = getgrgid(fileStat.st_gid);

                printf("%s %s ",getpwuid(fileStat.st_uid)->pw_name,getgrgid(fileStat.st_gid)->gr_name);
                printf("%ld ",fileStat.st_size);
                char *time = ctime(&fileStat.st_mtime);
                time[strlen(time)-1] = '\0';
                printf("%s ",time);
                printf("%s\n",&FilesList[i][printOffset]);
            }

        }
        else
        {
            for (int i=0; i<FileCount; i++)
                printf("%s  ",&FilesList[i][printOffset]);
            printf("\n");
        }
    }
}