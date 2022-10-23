

struct mystring{
	char str[30];
};
struct setting{
	int boardc, xc, oc, bsize; // mau sac o co, co X, co O mac dinh = 15, kich thuoc o co;
};

void FixConsoleWindow();
void TextColor(int color);
void GameRun(int loadflag);
void LoadScr();
void DrawBoard(int x, int y, int width, int height, int flag, int curPosX, int curPosY);