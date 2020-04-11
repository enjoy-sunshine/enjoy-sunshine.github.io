#include <windows.h>
#include <iostream>
#include <string>
using namespace std;
//����4����� �˻�����
enum plays { Computer, Human, Draw, None };
//Ӯ�����
const int iWin[8][3] = { {0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{2,4,6},{0,4,8} };
//������Ϸ��
class ttt
{
public:
	//���췽��
	ttt()
	{
		//�ֵ�˭�ߣ�_p = 1�����ߣ����������
		_p = rand() % 2;
		reset();
	}
	//��Ϸ����
	void play()
	{
		//res��ʾ���ս������ʼΪƽ��
		int res = Draw;
		while (true)
		{
			//����3*3�ĸ���
			drawGrid();
			while (true)
			{
				//�ֵ�����
				if (_p) getHumanMove();
				//������
				else getComputerMove();
				//�����˺͵����µ����ӣ�����3*3�ĸ���
				drawGrid();
				//�ж�˭Ӯ��
				res = checkVictory();
				//�Ѿ����Էֳ�ʤ����������ѭ��
				if (res != None) break;
				//��������һ���ж���һ��˭��
				++_p %= 2;
			}
			//��Ӯ��
			if (res == Human)
				cout << "You win!";
			//����Ӯ��
			else if (res == Computer)
				cout << "You lose!";
			//ƽ��
			else
				cout << "It's a draw!";
			cout << endl << endl;
			string r;
			//�Ƿ�����һ��
			cout << "play again(Y/N)?";
			cin >> r;
			//ѡ�����˾�������ѭ����������Ϸ
			if (r != "Y" && r != "y")
				return;
			//��������һ
			++_p %= 2;
			//��������
			reset();
		}
	}

private:
	//���̳�ʼ��״̬
	void reset()
	{
		for (int x = 0; x < 9; x++)
			_field[x] = None;
	}
	//�����ӷֱ�ֵ
	void drawGrid()
	{
		//����
		system("cls");
		//�����ʼ����
		COORD c = { 0,2 };
		//���ÿ���̨���λ��
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		//���̳�ʼ״̬
		cout << " 1 | 2 | 3 " << endl;
		cout << "---+---+---" << endl;
		cout << " 4 | 5 | 6 " << endl;
		cout << "---+---+---" << endl;
		cout << " 7 | 8 | 9 " << endl << endl << endl;
		//���Ӽ�����
		int f = 0;
		for (int y = 0; y < 5; y += 2)//������
			for (int x = 1; x < 11; x += 4)//������
			{
				//���ӱ��ù�
				if (_field[f] != None)
				{
					//���λ������
					COORD c = { x,y + 2 };
					//���ÿ���̨���λ��
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);//GetStdHandle������þ��
					//�ж������ӵ��Ƿ�Ϊ���ԣ������X���������O
					string o = _field[f] == Computer ? "X" : "O";
					cout << o;
				}
				//���Ӹ�����һ
				f++;
			}
		c.Y = 9;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	}
	//�жϵ�ǰ����Ƿ�����ж���Ӯ
	int checkVictory()
	{
		//�����һ��һ�л����ǶԽǣ���ô�ͱ�ʾ�Ѿ�Ӯ��
		for (int i = 0; i < 8; i++)
		{
			if (_field[iWin[i][0]] != None &&
				_field[iWin[i][0]] == _field[iWin[i][1]] && _field[iWin[i][1]] == _field[iWin[i][2]])
			{
				//Ӯ��״̬���˻��ǵ���ȡ����_field[iWin[i][0]]��ֵ
				return _field[iWin[i][0]];
			}
		}
		//n��ʾ���Ӹ���
		int n = 0;
		for (int f = 0; f < 9; f++)
		{
			//�����ǰ�����Ѿ������ӣ�i+=1
			if (_field[f] != None)
				n++;
		}
		//����Ѿ���9�����������ӣ���ô����һ��ƽ��״̬
		if (n == 9)
			return Draw;
		//None��ʾ�����Լ��������ӣ�δ�ֳ�ʤ��
		return None;
	}
	//������
	void getHumanMove()
	{
		int m;
		cout << "Enter your move(1-9):";
		//��ʼ��ѭ��
		while (true)
		{
			m = 0;
			//����Ҫ�����ӵı��
			cin >> m;
			//�������ӵط��Ѿ��������ӣ���ʾ��Ч����
			if (_field[m - 1] != None)
				cout << "Invalid move.Try again!" << endl;
			//����Ϊ��Ч���������ѭ��
			else
				break;
		}
		//���Ϊ���µ�����
		_field[m - 1] = Human;
	}
	//��������
	void getComputerMove()
	{
		int move = 0;
		do
		{
			//���ȡֵ
			move = rand() % 9;
		}
		//ֻҪ������û������
		while (_field[move] != None);
		//��Ӯ��״̬ö��
		for (int i = 0; i < 8; i++)
		{
			//try1��try2��try3��ʾ����Ӯ��3����������
			int try1 = iWin[i][0], try2 = iWin[i][1], try3 = iWin[i][2];
			//���try1��try2�Ѿ������ӣ����������ӵ������һ���ģ�����try3û������
			if (_field[try1] != None && _field[try1] == _field[try2] && _field[try3] == None)
			{
				//���ԻὫ��������try3���������
				move = try3;
				//���try1������ӵ������ǵ��ԣ���ô����Ӯ�ˣ���Ϸ����
				if (_field[try1] == Computer)
					break;
			}
			//���try1��try3�Ѿ������ӣ����������ӵ������һ���ģ�����try2û������
			if (_field[try1] != None && _field[try1] == _field[try3] && _field[try2] == None)
			{
				//���ԻὫ��������try2���������
				move = try2;
				//���try1������ӵ������ǵ��ԣ���ô����Ӯ�ˣ���Ϸ����
				if (_field[try1] == Computer)
					break;
			}
			//���try2��try3�Ѿ������ӣ����������ӵ������һ���ģ�����try1û������
			if (_field[try2] != None && _field[try2] == _field[try3] && _field[try1] == None)
			{
				//���ԻὫ��������try1���������
				move = try1;
				//���try2������ӵ������ǵ��ԣ���ô����Ӯ�ˣ���Ϸ����
				if (_field[try2] == Computer)
					break;
			}
		}
		//����move�����ӱ��Ϊ�����µ�����
		_field[move] = Computer;
	}
	//���������жϵ�ǰ�����˭
	int _p;
	//����״̬
	int _field[9];
};

int main(int argc, char* argv[])
{
	//�����ʼ��
	srand(GetTickCount());
	//���微����Ϸ��ʵ������tic
	ttt tic;
	//��ʼ������Ϸ
	tic.play();
	return 0;
}