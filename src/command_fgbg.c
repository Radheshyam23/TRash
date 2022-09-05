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
