# include "./HeaderFiles/bigHeader.h"

void initBGqueue()
{
    Start.pid = -1;
    Start.name = (char*) malloc(6);
    strcpy(Start.name,"Start");
    Start.next = NULL;
}

void addBG(int pid, char* name)
{
    BGQ* NewProcess = (BGQ*)malloc(sizeof(BGQ));
    NewProcess->name = (char*)malloc(strlen(name));
    strcpy(NewProcess->name,name);
    NewProcess->pid = pid;
    NewProcess->next = NULL;

    if (Start.next != NULL)
        NewProcess->next = Start.next;

    Start.next = NewProcess;
}

void completeBG(int pid)
{
    if (Start.next == NULL)
        return;
    BGQ *temp = Start.next;
    BGQ *prev = &Start;

    while(temp != NULL)
    {
        if (temp->pid == pid)
        {
            if(temp->next == NULL)
                prev->next = NULL;
            else
                prev->next = temp->next;
            free(temp);
            break;
        }
    }
}

char* RetrieveBGName(int pid)
{
    BGQ *temp = Start.next;
    char *BGname = NULL;

    while(temp != NULL)
    {
        // printf("BG ll not empty\n");
        if (temp->pid == pid)
        {
            BGname = temp->name;
            // printf("Entry found!: %s\n",temp->name);
            break;
        }
    }
    return BGname;
}

void command_fg(char **TokenArr, int count)
{
    // printf("In Foreground:\n");
    pid_t pid;

    // Created a new process
    pid = fork();

    // The child process returns 0
    if(pid == 0)
    {
        int x = execvp(TokenArr[0],TokenArr);
        if (x == -1)
        {
            perror("<execvp>");
            exit(0);
            return;
        }
    }
    // fork didn't work
    else if(pid < 0)
    {
        perror("<fork>");
    }
    // Parent process returns the pid of the child process
    else
    {
        int stat;
        long startTime = time(NULL);
        waitpid(pid,&stat,WUNTRACED);
        long endTime = time(NULL);
        long timeTaken = endTime - startTime;
        if (timeTaken >= 1)
            printf("Time: %ld\n",timeTaken);
        // processTime = endTime - startTime;
    }
}

void command_bg(char **TokenArr, int count)
{
    pid_t pid;

    // Created a new process
    pid = fork();

    // The child process returns 0
    if(pid == 0)
    {
        int x = execvp(TokenArr[0],TokenArr);
        if (x == -1)
        {
            perror("<execvp>");
            exit(0);
            return;
        }
        exit(0);
        // Nothing after execvp will run in the original code...
    }
    // fork didn't work
    else if(pid < 0)
    {
        perror("<fork>");
    }
    // Parent process returns the pid of the child process
    else
    {
        printf("%d\n",pid);
        // printf("\e[4;32m%d\e[0m\n", pid);
        // printf("Adding to LL: %d %s\n",pid,TokenArr[0]);
        addBG(pid,TokenArr[0]);
    }    
}

void ChildDone(int signal)
{
    // printf("Process dead\n");
    int pid;
    int status;
    while ((pid = waitpid(-1, &status , WNOHANG)) > 0)
    {
        char *processName = RetrieveBGName(pid);
        if (WIFEXITED(status))
            printf("\n%s with pid = %d exited normally\n",processName,pid);
        else
            printf("\n%s with pid = %d exited abnormally\n",processName,pid);
        completeBG(pid);
        TerminalPrompt();
    }
}

void HandlePinfo(char **TokenArr, int count)
{
    char *pid = (char*)malloc(10);
    if (count == 1)
        // pid = itoa(getpid());
        sprintf(pid,"%d",getpid());
    else if (count == 2)
        strcpy(pid,TokenArr[1]);
    else
    {
        printf("Too many arguments\n");
        return;
    }

    char *processDetsFile = (char*)malloc(20);
    strcpy(processDetsFile,"/proc/");
    strcat(processDetsFile,pid);
    strcat(processDetsFile,"/stat");

    // FILE *filePtr = fopen(processDetsFile,'r');
    int inpFD = open(processDetsFile,O_RDONLY);

    if (inpFD == -1)
    {
        printf("Process doesn't exist\n");
        return;
    }

    char contents[500];
    read(inpFD,contents,500);
    close(inpFD);

    char **statTokens = (char**)calloc(53,sizeof(char*));
    const char delimPtr[2] = " ";

    statTokens[0] = strtok(contents,delimPtr);

    char *temp;
    count = 1;
    while(temp = strtok(NULL,delimPtr))
    {
        statTokens[count] = temp;
        count++;
    }

    printf("pid: %s\n",pid);
    printf("process status: %s",statTokens[2]);
    if (strcmp(statTokens[7], statTokens[0]) == 0)
        printf("+");
    printf("\n");
    printf("memory: %s\n",statTokens[22]);
}

int BGcompare(const void *a, const void *b)
{
    // printf("Whats the fkn issue?\n");
    BGQ **BG1 = (BGQ**)a;
    BGQ **BG2 = (BGQ**)b;

    // printf("Ptrs made\n");

    // printf("BG1: %s\n",(*BG1)->name);
    // printf("BG2: %s\n",(*BG2)->name);
    // printf("diff: %d\n",strcmp((*BG1)->name,(*BG2)->name));
    return strcmp((*BG1)->name,(*BG2)->name);
}

void DispAllBG()
{
    // printf("Entered\n");
    BGQ* ptr = Start.next;

    // printf("Got ptr\n");

    BGQ **BGarr = (BGQ**)malloc(100*sizeof(BGQ*));

    // printf("Got BGArr created\n");

    int i=0;
    while(ptr != NULL)
    {
        BGarr[i++] = ptr;
        ptr = ptr->next;
    }

    // printf("Array ready %d\n",i);

    // Now sort BGarr based on the process name.

    // printf("%s\n",BGarr[0]->name);
    // printf("%s\n",BGarr[1]->name);

    qsort(BGarr,i,sizeof(BGQ*),BGcompare);

    // printf("Sorted\n");

    // printf("Size: %d\n",i);

    for (int k=0; k<i; k++)
    {
        char pid[10];
        char *processDetsFile = (char*)malloc(20);
        strcpy(processDetsFile,"/proc/");
        sprintf(pid,"%d",BGarr[k]->pid);
        strcat(processDetsFile,pid);
        strcat(processDetsFile,"/stat");

        // FILE *filePtr = fopen(processDetsFile,'r');
        int inpFD = open(processDetsFile,O_RDONLY);

        if (inpFD == -1)
            continue;
        
        char contents[500];
        read(inpFD,contents,500);
        close(inpFD);

        char **statTokens = (char**)calloc(53,sizeof(char*));
        const char delimPtr[2] = " ";

        statTokens[0] = strtok(contents,delimPtr);

        char *temp;
        int count = 1;
        while(temp = strtok(NULL,delimPtr))
        {
            statTokens[count] = temp;
            count++;
        }

        printf("[%d] ",BGarr[k]->pid);      // should print job ID
        if (strcmp(statTokens[2],"R") == 0)
            printf("Running ");
        else
            printf("Stopped ");

        printf("%s ",BGarr[k]->name);
        printf("[%d]\n",BGarr[k]->pid);      
    }

    free(BGarr);
}