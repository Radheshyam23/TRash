#include "./HeaderFiles/bigHeader.h"

void retrieveHistory()
{
    // printf("Retrieval...\n");

    myHist.total = 20;
    myHist.currIndex = 0;
    myHist.startIndex = 0;
    myHist.History = (char**)calloc(myHist.total,sizeof(char*));

    char *fileAddr = (char*)malloc(100);
    strcpy(fileAddr,RootLocation);
    strcat(fileAddr,"/history.txt");

    FILE *histFile = fopen(fileAddr,"r");

    // if there was indeed a file
    if (histFile)
    {
        // read each line and store into history array.
        size_t tempSize = 0, count;
        char *tempLine = NULL;

        while(count = getline(&tempLine,&tempSize,histFile) > 0)
        {
            // getline will allocate memory for tempLine by default if tempSize = 0 and tempLine = NULL
            myHist.History[myHist.currIndex] = tempLine;
            // if (myHist.History[myHist.currIndex][count - 1] == '\n')
            //     myHist.History[myHist.currIndex][count - 1] = '\0';
            myHist.currIndex++;
            tempLine = NULL;
            tempSize = 0;
        }
        fclose(histFile);
    }
}

void addHistory(char *command)
{

    if (myHist.currIndex >= myHist.total)
    {
        // printf("History full\n");
        myHist.currIndex = 0;
        myHist.startIndex++;
    }

    // printf("start:%d curr:%d\n",myHist.startIndex,myHist.currIndex);
    if (myHist.currIndex - 1 >= 0)
    {
        // printf("e1\n");
        if (myHist.History[myHist.currIndex - 1] && (strcmp(myHist.History[myHist.currIndex - 1],command) == 0))
        {
            // printf("e2\n");
            return;
        }
        // printf("end e1\n");
    }
    else
    {
        // printf("e3\n");
        if (myHist.History[myHist.total - 1] && (strcmp(myHist.History[myHist.total - 1],command) == 0))
        {
            // printf("e4\n");
            return;
        }
    }

    // if the history is full, then delete the first element and shift all the elements to the left.
    
    // overwrite the 1st history element if > 20
    

    if((myHist.currIndex != 0) && (myHist.currIndex == myHist.startIndex))
    {
        if (myHist.startIndex == myHist.total-1)
            myHist.startIndex = 0;
        else
            myHist.startIndex++;
    }

    
    if (myHist.History[myHist.currIndex])
        free(myHist.History[myHist.currIndex]);
    
    myHist.History[myHist.currIndex] = (char*)malloc(strlen(command)*sizeof(char));
    strcpy(myHist.History[myHist.currIndex],command);
    // printf("Added to history: %s",myHist.History[myHist.currIndex]);
    myHist.currIndex++;    
    // printf("start: %d curr: %d\n",myHist.startIndex,myHist.currIndex);
}

void dispHistory()
{
    // printf("In dispHistory\n");
    // printf("%d %d\n",myHist.currIndex, myHist.startIndex);

    if (myHist.currIndex <= myHist.startIndex)
    {
        for (int i = myHist.startIndex; i < myHist.total; i++)
            printf("%s",myHist.History[i]);
        for(int i=0; i < myHist.currIndex; i++)
            printf("%s",myHist.History[i]);
    }
    else
    {
        for (int i = myHist.startIndex; i < myHist.currIndex; i++)
            printf("%s",myHist.History[i]);
    }
}

void cacheHistory()
{
    // printf("In cache\n");

    char *fileAddr = (char*)malloc(100);
    strcpy(fileAddr,RootLocation);
    strcat(fileAddr,"/history.txt");
    
    int cacheFD = open(fileAddr,O_WRONLY | O_CREAT , 0644);

    // printf("FD: %d\n",cacheFD);

    // FILE *histFile = fopen(fileAddr,"w");

    // if(!histFile)
    // {
    //     printf("File not opened\n");
    //     return;
    // }

    // printf("File opened\n");
 
    if (myHist.currIndex <= myHist.startIndex)
    {
        for (int i = myHist.startIndex; i < myHist.total; i++)
            {//printf("%s",myHist.History[i]);
            // fprintf(histFile,"%s",myHist.History[i]);
            write(cacheFD,myHist.History[i],strlen(myHist.History[i]));
            }
        for(int i=0; i < myHist.currIndex; i++)
            {//printf("%s",myHist.History[i]);
            // fprintf(histFile,"%s",myHist.History[i]);
            write(cacheFD,myHist.History[i],strlen(myHist.History[i]));
            
            }
    }
    else
    {
        for (int i = myHist.startIndex; i < myHist.currIndex; i++)
           {//printf("%s",myHist.History[i]);
            // fprintf(histFile,"%s",myHist.History[i]);
            write(cacheFD,myHist.History[i],strlen(myHist.History[i]));
            }
    }
    // fclose(histFile);
    close(cacheFD);
}