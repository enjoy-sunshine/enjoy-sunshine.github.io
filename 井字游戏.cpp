//write by zhouyi
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;
//定义4种情况 人机对弈
enum plays { Computer, Human, Draw, None };
//赢的情况
const int iWin[8][3] = { {0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{2,4,6},{0,4,8} };
//井字游戏类
class ttt
{
public:
	//构造方法
	ttt()
	{
		//轮到谁走，_p = 1，人走，否则电脑走
		_p = rand() % 2;
		reset();
	}
	//游戏函数
	void play()
	{
		//res表示最终结果，初始为平局
		int res = Draw;
		while (true)
		{
			//给出3*3的格子
			drawGrid();
			while (true)
			{
				//轮到人走
				if (_p) getHumanMove();
				//电脑走
				else getComputerMove();
				//根据人和电脑下的棋子，画出3*3的格子
				drawGrid();
				//判断谁赢了
				res = checkVictory();
				//已经可以分出胜负，跳出内循环
				if (res != None) break;
				//计数器加一，判断下一步谁走
				++_p %= 2;
			}
			//人赢了
			if (res == Human)
				cout << "You win!";
			//电脑赢了
			else if (res == Computer)
				cout << "You lose!";
			//平局
			else
				cout << "It's a draw!";
			cout << endl << endl;
			string r;
			//是否再玩一局
			cout << "play again(Y/N)?";
			cin >> r;
			//选择不玩了就跳出外循环，结束游戏
			if (r != "Y" && r != "y")
				return;
			//计数器加一
			++_p %= 2;
			//重置棋盘
			reset();
		}
	}

private:
	//棋盘初始化状态
	void reset()
	{
		for (int x = 0; x < 9; x++)
			_field[x] = None;
	}
	//给格子分别赋值
	void drawGrid()
	{
		//清屏
		system("cls");
		//定义初始坐标
		COORD c = { 0,2 };
		//设置控制台光标位置
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		//棋盘初始状态
		cout << " 1 | 2 | 3 " << endl;
		cout << "---+---+---" << endl;
		cout << " 4 | 5 | 6 " << endl;
		cout << "---+---+---" << endl;
		cout << " 7 | 8 | 9 " << endl << endl << endl;
		//棋子计数器
		int f = 0;
		for (int y = 0; y < 5; y += 2)//纵坐标
			for (int x = 1; x < 11; x += 4)//横坐标
			{
				//格子被用过
				if (_field[f] != None)
				{
					//光标位置坐标
					COORD c = { x,y + 2 };
					//设置控制台光标位置
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);//GetStdHandle函数获得句柄
					//判断下棋子的是否为电脑，是输出X，否则输出O
					string o = _field[f] == Computer ? "X" : "O";
					cout << o;
				}
				//棋子个数加一
				f++;
			}
		c.Y = 9;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	}
	//判断当前棋局是否可以判断输赢
	int checkVictory()
	{
		//如果有一行一列或者是对角，那么就表示已经赢了
		for (int i = 0; i < 8; i++)
		{
			if (_field[iWin[i][0]] != None &&
				_field[iWin[i][0]] == _field[iWin[i][1]] && _field[iWin[i][1]] == _field[iWin[i][2]])
			{
				//赢的状态是人还是电脑取决于_field[iWin[i][0]]的值
				return _field[iWin[i][0]];
			}
		}
		//n表示棋子个数
		int n = 0;
		for (int f = 0; f < 9; f++)
		{
			//如果当前格子已经有棋子，i+=1
			if (_field[f] != None)
				n++;
		}
		//如果已经有9个格子有棋子，那么就是一个平局状态
		if (n == 9)
			return Draw;
		//None表示还可以继续下棋子，未分出胜负
		return None;
	}
	//人下棋
	void getHumanMove()
	{
		int m;
		cout << "Enter your move(1-9):";
		//开始死循环
		while (true)
		{
			m = 0;
			//输入要下棋子的编号
			cin >> m;
			//所下棋子地方已经有了棋子，表示无效输入
			if (_field[m - 1] != None)
				cout << "Invalid move.Try again!" << endl;
			//否则为有效输出，跳出循环
			else
				break;
		}
		//标记为人下的棋子
		_field[m - 1] = Human;
	}
	//电脑下棋
	void getComputerMove()
	{
		int move = 0;
		do
		{
			//随机取值
			move = rand() % 9;
		}
		//只要格子里没有棋子
		while (_field[move] != None);
		//对赢的状态枚举
		for (int i = 0; i < 8; i++)
		{
			//try1、try2和try3表示可能赢的3个格子坐标
			int try1 = iWin[i][0], try2 = iWin[i][1], try3 = iWin[i][2];
			//如果try1和try2已经有棋子，并且下棋子的玩家是一样的，而且try3没有棋子
			if (_field[try1] != None && _field[try1] == _field[try2] && _field[try3] == None)
			{
				//电脑会将棋子下载try3这个格子上
				move = try3;
				//如果try1这个格子的棋子是电脑，那么电脑赢了，游戏结束
				if (_field[try1] == Computer)
					break;
			}
			//如果try1和try3已经有棋子，并且下棋子的玩家是一样的，而且try2没有棋子
			if (_field[try1] != None && _field[try1] == _field[try3] && _field[try2] == None)
			{
				//电脑会将棋子下载try2这个格子上
				move = try2;
				//如果try1这个格子的棋子是电脑，那么电脑赢了，游戏结束
				if (_field[try1] == Computer)
					break;
			}
			//如果try2和try3已经有棋子，并且下棋子的玩家是一样的，而且try1没有棋子
			if (_field[try2] != None && _field[try2] == _field[try3] && _field[try1] == None)
			{
				//电脑会将棋子下载try1这个格子上
				move = try1;
				//如果try2这个格子的棋子是电脑，那么电脑赢了，游戏结束
				if (_field[try2] == Computer)
					break;
			}
		}
		//将第move个格子标记为电脑下的棋子
		_field[move] = Computer;
	}
	//计数器，判断当前玩家是谁
	int _p;
	//格子状态
	int _field[9];
};

int main(int argc, char* argv[])
{
	//随机初始化
	srand(GetTickCount());
	//定义井字游戏的实例对象tic
	ttt tic;
	//开始进行游戏
	tic.play();
	return 0;
}
