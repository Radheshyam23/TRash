#ifndef commandHistory
#define commandHistory

struct HistStruct
{
    char **History;  //array of 20 strings having history of 20 commands
    int currIndex;
    int startIndex;
    int total;
}myHist;

void retrieveHistory();
void addHistory(char *command);
void dispHistory();
void cacheHistory();

#endif