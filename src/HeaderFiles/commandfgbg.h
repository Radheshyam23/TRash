#ifndef figBig
#define figBig

void command_fg(char **TokenArr, int count);
void command_bg(char **TokenArr, int count);
void ChildDone(int signal);

typedef struct BGqueue
{
    int pid;
    char *name;
    struct BGqueue *next;
}BGQ;
BGQ Start;

void initBGqueue();

void addBG(int pid, char* name);
void completeBG(int pid);
char* RetrieveBGName(int pid);
void HandlePinfo(char **TokenArr, int count);

#endif