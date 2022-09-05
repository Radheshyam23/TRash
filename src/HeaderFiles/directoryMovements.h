#ifndef DirMove
#define DirMove

void cdDotDot();
char* GetRelativePath();
void PrevDir();
void GoToLocation(char *Location);
void HandleCD(char **TokenArr, int count);

#endif