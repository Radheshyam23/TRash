#include "./HeaderFiles/bigHeader.h"

void HandleLS(char **TokensArr, int count)
{
    int opt;
    optind = 1;
    
    // while ((opt = getopt(count, TokensArr, "al")) != -1)
    // {
    //     printf("%c\n",opt);
    // }
    DIR *dir;

    dir = opendir(CurrLocation);
    struct dirent *dp;
    while (dp = readdir(dir))
    {
        printf("%ld %s %d %c\n",dp->d_ino, dp->d_name, dp->d_reclen, dp->d_type);
    }
}