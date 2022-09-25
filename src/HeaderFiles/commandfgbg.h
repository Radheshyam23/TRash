#ifndef figBig
#define figBig

void command_fg(char **TokenArr, int count);
void command_bg(char **TokenArr, int count);
void ChildDone(int signal);

typedef struct BGqueue
{
    int pid;
    int jobID;
    char *name;
    struct BGqueue *next;
}BGQ;
BGQ Start;

// typedef struct jobQ
// {
//     int pid;
//     char *name;
//     int jobID;
// }jobQ;

void initBGqueue();

void addBG(int pid, char* name);
void completeBG(int pid);
char* RetrieveBGName(int pid);
void HandlePinfo(char **TokenArr, int count);
void DispAllBG();

#endif