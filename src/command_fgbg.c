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
    NewProcess->name = name;
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
        if (temp->pid == pid)
        {
            BGname = temp->name;
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
        printf("Time: %ld\n",endTime-startTime);
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
            printf("\n%swith pid = %d exited normally\n",processName,pid);
        else
            printf("\n%swith pid = %d exited abnormally\n",processName,pid);
        completeBG(pid);
        // TerminalPrompt();
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