
/*�ڰ���*/
#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<fstream>
#include<conio.h>
using namespace std;
#define MIN -9999999
#define MAX 0xffffff

int board[9][9]; //����״̬��-1�գ� 0�ף� 1�ڣ� 2����
int prev_board[9][9]; //������ 
int a, b, chess, turn; //ѡ��
int ai_x, ai_y, prev_x, prev_y;
int value[9][9] = {
	{  0,   0,   0,  0,  0,  0,  0,   0,   0},
	{0,500,-25,10,5,5,10,-25,500},
    {0,-25,-45,1,1,1,1,-45,-25},
    {0,10,1,3,2,2,3,1,10},
    {0,5,1,2,1,1,2,1,5},
    {0,5,1,2,1,1,2,1,5},
    {0,10,1,3,2,2,3,1,10},
    {0, -25,-45,1,1,1,1,-45,-25},
    {0,500,-25,10,5,5,10,-25,500}
};

void show_menu();//�˵� 
void rule(); //��Ϸ���� 
void game(); //��Ϸ���� 
void load(); //���� 
void save(); //�浵 
int count(int);//��ĳ����ɫ�����Ӽ���
void player(int);//��һغ� 
void update(int, int, int);//��ת���ӣ��������� 
void alter_loc(int); //������λ�� 
void undo_loc(); // ��������λ�ñ�Ϊ��λ 
void AI(int); 
pair<pair<int, int>, int> minimax(int color, int depth, int alpha, int beta); 
int cal_score(int); //���������� 
void print_board(); 
 
int main(){
    //��ʾ�˵�
	show_menu();
	return 0;
} 

void save(){
	ofstream outfile("data.txt");
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			outfile << board[i][j] <<' ';
		}
	}
	outfile << b << ' ' << chess << ' ' << ai_x << ' ' << ai_y << ' ' << turn; 
	outfile.close();
}

void load(){
	ifstream infile("data.txt");
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			infile >> board[i][j];
		}
	}
	infile >> b >> chess >> ai_x >> ai_y >> turn;
	 
	infile.close();
}
void show_menu(){
	system("cls");
	cout <<"======================================================"<< endl;
    cout <<"===================     �ڰ���     ==================="<< endl;
    cout <<"===================   1.�µ���Ϸ   ==================="<< endl;
    cout <<"===================   2.��Ϸ����   ==================="<< endl;
    cout <<"===================   3.��ȡ�浵   ==================="<< endl;
    cout <<"===================   4.�˳���Ϸ   ==================="<< endl;
    cout <<"======================================================"<< endl;
    cout << "������ѡ��"; 
    cin >> a;
    switch(a){
    	case 1: 
    		system("cls");
    		//��ʼ������ 
    		memset(board, -1, sizeof(board));
    		board[5][4] = board[4][5] = 1; board[4][4] = board[5][5] = 0;
    		
    		cout << "��ѡ����Ϸ����" << endl;
			cout << "1.�˻���ս  2.˫����Ϸ" << endl;
			cout << "���룺"; 
			cin >> b;
			
			system("cls"); 
			if(b == 1){
				cout << "����ѡ�õ�������ɫ" << endl;
				cout << "0.��  1.��" << endl;
				cout << "���룺"; 
				cin >> chess;
				turn = chess;
				if(chess == 1){
					board[4][3] = board[3][4] = board[6][5] = board[5][6] = 2;
				}
			}
			else if(b == 2){
				cout << "Ϊ��֤��Ϸ�Ĺ�ƽ��"<<endl;
				cout << "˫����Ϸ�ݲ�֧�ֻ���o(�i�n�i)o"  << endl; 
				cout << "�ڰ��壬��������" <<endl;
				system("pause");
			}
			game();
    		break;
    	case 2:
    		rule();
    		break;
    	case 3:
    		load();
    		game();
    		break;
    	case 4:
    		exit(1);
    		break;
	}
}

void rule(){
	system("cls");
    cout << "�ڰ�����Ϸ����\n" << endl;
    cout <<"1����ֿ�ʼʱ����λ��44��55������λ��54��45" << endl;
    cout <<"2���ڷ����У�˫���������塣"<< endl;
    cout <<"3��һ���Ϸ����岽��������һ���ո�������һ�����ӣ����ҷ�ת����һ���������ӡ�"<< endl;
    cout <<"4�������µ����������������е�ͬɫ���Ӽ䣬�Է�����ס���������Ӷ�Ҫ��ת�����������Ǻ��żУ����żУ�����б�żС���ס��λ���ϱ���ȫ���Ƕ��ֵ����ӣ������пո�"<< endl;
    cout <<"5��һ������������������Ϸ��壬�κα���ס�����Ӷ����뱻��ת������������Ȩѡ��ȥ��ĳ�����ӡ�"<< endl;
    cout <<"6���������ٷ�ת�˶��ֵ�һ�����ӣ�����Ͳ������ӡ����һ��û�кϷ��岽��Ҳ����˵�������µ�������������ٷ�ת���ֵ�һ�����ӣ�������һ��ֻ����Ȩ���������Ķ��ּ�������ֱ�����кϷ��岽���¡�"<< endl;
    cout <<"7�����һ��������һ���Ϸ��岽���£����ͱ������ӣ�������Ȩ��"<< endl;
    cout <<"8����ֳ�����ȥ��ֱ��������������˫�����޺Ϸ��岽���¡�"<< endl;
    cout <<"============================================================================="<< endl;
    system("pause");
    show_menu();
}

void print_board(){
	system("cls");
	int i, j;
	cout << "    1   2   3   4   5   6   7   8" << endl;
	cout << " ���������Щ������Щ������Щ������Щ������Щ������Щ������Щ�������" << endl;
	for (i = 1; i <= 8; i++)
	{
		cout << i << "��";
		for (j = 1; j <= 8; j++)
		{
			switch (board[i][j])
			{
			case -1:cout << "   ��"; break;
			case 0:cout << " �� ��"; break;
			case 1:cout << " �� ��"; break;
			case 2:cout << " �� ��"; break;
			}
		}
		if (i == 1) 	 cout << "	================";
		else if (i == 2) cout << "	==== 1.���� ====";
		else if(i == 3)  cout << "	==== 3.�浵 ====";
		else if(i == 4)  cout << "	================";
		cout << endl;
		if (i == 8)    cout << " ���������ة������ة������ة������ة������ة������ة������ة�������" << endl;
		else if (i == 1)    cout << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������	====  ѡ��  ====" << endl;
		else if (i == 2)    cout << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������	==== 2.��ͣ ====" << endl;
		else if (i == 3) cout << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������	==== 4.�˳� ====" << endl;
		else    cout << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������" << endl;
	}
	cout << endl;
	int cnt1 = count(1), cnt0 = count(0);
	cout << "        �ڣ�" << cnt1 << "        �ף�"  << cnt0 << endl;
	cout << "************************************" << endl;
	cout << endl;
	
	char c = _getch();
	switch(c){
		case '1':{
			memcpy(board, prev_board, sizeof(board));
			ai_x = prev_x, ai_y = prev_y;
			print_board();
			break;
		}
		case '2':{
			cout << "��Ϸ��ͣ" << endl; 
			system("pause");
			break;
		}
		case '3':{
			save();
			break;
		}
		case '4':
			exit(1);
	}
}

int count(int color){
	int cnt = 0;
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			if(board[i][j] == color)++cnt;
		}
	}	
	return cnt;
} 

void game(){
	if(b == 1){//�˻���ս 
		while(1){
			if(turn){
				alter_loc(chess);
				print_board();
				memcpy(prev_board, board, sizeof(board));
				cout << "��һ��AI����λ�ã�" << ai_x <<' ' << ai_y << endl;
				player(chess); 
			}
			else{
				AI(1-chess);
			}
			if((count(1) + count(0) == 64) || !count(0) || !count(1))break;
			turn = 1-turn;
		}
	}
	else if(b == 2){
		board[4][3] = board[3][4] = board[6][5] = board[5][6] = 2;
		while(1){
			print_board();
			player(1);
			if(count(0)+count(1) == 64 || !count(0) || !count(1))break;
			alter_loc(0);
			print_board();
			player(0);
			if(count(0)+count(1) == 64 || !count(0) || !count(1))break;
			alter_loc(1);
		}
	}
	
	print_board();
	cout << endl;
	if(count(1) == count(0)) cout << "                ƽ��" <<endl;
	else if(count(1) > count(0)) cout << "                ����ʤ��" << endl;
	else cout << "                ����ʤ��" << endl;  
	cout << endl;
	system("pause");
}

void player(int color){
	int cnt2 = count(2);
	if(cnt2 == 0){
		cout << "���غ����ӿ��䣬��һ������" << endl;
		system("pause");
		return;
	}
	
	if(color)cout << "����Ļغ�" << endl;
	else cout << "����Ļغ�" << endl;
	cout << "����������λ�ã�";
	int x, y;
	while(1){
		cin >> x >> y;
		if(board[x][y] == 2)break;
		cout << "���Ϸ���λ�ã�ֻ�С�����������" << endl;
		cout << "���������룺";
	} 
	
	board[x][y] = color;
	undo_loc();
	update(x, y, color);
}
void undo_loc(){
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			if(board[i][j] == 2)board[i][j] = -1;
		}
	}
}
void update(int x, int y, int chess){
	int i, j;
	int flag = 0;//��¼�м�Է����ӵ�����
	//�������
	if (y >= 2)
	{
		for (i = y - 1; i >= 1; i--)
		{
			if (board[x][i] == -1)    break;
			else if (board[x][i] == 1 - chess)    flag++;
			else if (board[x][i] == chess)
			{
				if (flag == 0)    break;
				else
				{
					for (j = i + 1; j <= y - 1; j++)    board[x][j] = chess;
					break;
				}
			}
		}
	}
	//���Ҽ���
	flag = 0;
	if (y <= 7)
	{
		for (i = y + 1; i <= 8; i++)
		{
			if (board[x][i] == -1)    break;
			else if (board[x][i] == 1 - chess)    flag++;
			else if (board[x][i] == chess)
			{
				if (flag == 0)    break;
				else
				{
					for (j = i - 1; j >= y + 1; j--)    board[x][j] = chess;
					break;
				}
			}
		}
	}
	//���ϼ���
	flag = 0;
	if (x >= 2)
	{
		for (i = x - 1; i >= 1; i--)
		{
			if (board[i][y] == -1)    break;
			else if (board[i][y] == 1 - chess)    flag++;
			else if (board[i][y] == chess)
			{
				if (flag == 0)    break;
				else
				{
					for (j = i + 1; j <= x - 1; j++)    board[j][y] = chess;
					break;
				}
			}
		}
	}
	//���¼���
	flag = 0;
	if (x <= 7)
	{
		for (i = x + 1; i <= 8; i++)
		{
			if (board[i][y] == -1)    break;
			else if (board[i][y] == 1 - chess)    flag++;
			else if (board[i][y] == chess)
			{
				if (flag == 0)    break;
				else
				{
					for (j = i - 1; j >= x + 1; j--)    board[j][y] = chess;
					break;
				}
			}
		}
	}
	//�����ϼ���
	flag = 0;
	if (x >= 2 && y >= 2)
	{
		for(i = x-1, j = y-1; i >= 1 && j >= 1; --i, --j){
			if(board[i][j] == -1)break;
			else if(board[i][j] == 1 - chess) ++flag;
			else if(board[i][j] == chess){
				if(flag == 0)break;
				else{
					for(int k = i+1, p = j+1; k <= x-1 && p <= y-1; ++k, ++p)
						board[k][p] = chess;
					break;
				}
			} 
		}
	}
	//�����¼���
	flag = 0;
	if (x <= 7 && y <= 7)
	{
		for(i = x+1, j = y+1; i <= 8 && j <= 8; ++i, ++j){
			if(board[i][j] == -1)break;
			else if(board[i][j] == 1 - chess) ++flag;
			else if(board[i][j] == chess){
				if(flag == 0)break;
				else{
					for(int k = x+1, p = y+1; k <= i-1 && p <= j-1; ++k, ++p)
						board[k][p] = chess;
					break;
				}
			} 
		}
	}
	//�����ϼ���
	flag = 0;
	if (x >= 2 && y <= 7)
	{
		for(int i = x-1, j = y+1; i >= 1 && j <= 8; --i, ++j){
			if(board[i][j] == -1)break;
			else if(board[i][j] == 1 - chess) ++flag;
			else if(board[i][j] == chess){
				if(flag == 0)break;
				else{
					for(int k = x-1, p = y+1; k >= i+1 && p <= j-1; --k, ++p)
						board[k][p] = chess;
					break;
				}
			} 
		}
	}
	//�����¼���
	flag = 0;
	if (y >= 2 && x <= 7)
	{
		for(int i = x+1, j = y-1; i <= 8 && j >= 1; ++i, --j){
			if(board[i][j] == -1)break;
			else if(board[i][j] == 1 - chess) ++flag;
			else if(board[i][j] == chess){
				if(flag == 0)break;
				else{
					for(int k = x+1, p = y-1; k <= i-1 && p >= j+1; ++k, --p)
						board[k][p] = chess;
					break;
				}
			} 
		}
	}
}

void alter_loc(int color){
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			if(board[i][j] == color){
				//����
				int flag = 0;
				if(j > 2){
					for(int k = j-1; k >= 1; --k){
						if(board[i][k] == 2 || board[i][k] == color || (board[i][k] == -1 && !flag))break;
						else if(board[i][k] == (1-color))++flag;
						else if(board[i][k] == -1 && flag){
							board[i][k] = 2;
							break;
						}
					}
				} 
				//����
				flag = 0;
				if(j < 7){
					for(int k = j+1; k <= 8; ++k){
						if(board[i][k] == 2 || board[i][k] == color || (board[i][k] == -1 && !flag))break;
						else if(board[i][k] == (1-color))++flag;
						else if(board[i][k] == -1 && flag){
							board[i][k] = 2;
							break;
						}
					}
				} 
				//����
				flag = 0;
				if(i > 2){
					for(int k = i-1; k >= 1; --k){
						if(board[k][j] == 2 || board[k][j] == color || (board[k][j] == -1 && !flag))break;
						else if(board[k][j] == (1-color))++flag;
						else if(board[k][j] == -1 && flag){
							board[k][j] = 2;
							break;
						}
					}
				} 
				//����
				flag = 0;
				if(i < 7){
					for(int k = i+1; k <= 8; ++k){
						if(board[k][j] == 2 || board[k][j] == color || (board[k][j] == -1 && !flag))break;
						else if(board[k][j] == (1-color))++flag;
						else if(board[k][j] == -1 && flag){
							board[k][j] = 2;
							break;
						}
					}
				}
				//����
				flag = 0;
				if(i > 2 && j > 2){
					for(int k = i-1, p = j-1; k >= 1 && p >= 1; --k, --p){
						if(board[k][p] == 2 || board[k][p] == color || (board[k][p] == -1 && !flag))break;
						else if(board[k][p] == (1-color))++flag;
						else if(board[k][p] == -1 && flag){
							board[k][p] = 2;
							break;
						}
					}
				}
				//����
				flag = 0;
				if(i < 7 && j < 7){
					for(int k = i+1, p = j+1; k <= 8 && p <= 8; ++k, ++p){
						if(board[k][p] == 2 || board[k][p] == color || (board[k][p] == -1 && !flag))break;
						else if(board[k][p] == (1-color))++flag;
						else if(board[k][p] == -1 && flag){
							board[k][p] = 2;
							break;
						}
					}
				}
				//����
				flag = 0;
				if(i < 7 && j > 2){
					for(int k = i+1, p = j-1; k <= 8 && p >= 1; ++k, --p){
						if(board[k][p] == 2 || board[k][p] == color || (board[k][p] == -1 && !flag))break;
						else if(board[k][p] == (1-color))++flag;
						else if(board[k][p] == -1 && flag){
							board[k][p] = 2;
							break;
						}
					}
				}
				//����
				flag = 0;
				if(i > 2 && j < 7){
					for(int k = i-1, p = j+1; k >= 1 && p <= 8; --k, ++p){
						if(board[k][p] == 2 || board[k][p] == color || (board[k][p] == -1 && !flag))break;
						else if(board[k][p] == (1-color))++flag;
						else if(board[k][p] == -1 && flag){
							board[k][p] = 2;
							break;
						}
					}
				}
			}
		}
	}
}

int cal_score(int color){
	int score = 0;
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			if(board[i][j] == color){
				score += value[i][j];
			}
			else if(board[i][j] = 1 - color){
				score -= value[i][j];
			}
		}
	}
	/*int my_moves = 0, op_moves = 0;
	alter_loc(color);
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			if(board[i][j] == 2)my_moves++;
		}
	} 
	undo_loc();
	
	alter_loc(1-color);
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			if(board[i][j] == 2)op_moves++;
		}
	} 
	undo_loc();*/
	
	return score;
}

pair<pair<int, int>, int> minimax(int color, int depth, int alpha, int beta){//minimax 
    if(depth > 7)return make_pair(make_pair(0, 0), cal_score(color));
    
    alter_loc(color);
    if(count(2) == 0){
    	alter_loc(1-color);
    	if(count(2) == 0)return make_pair(make_pair(0, 0), cal_score(color));
    	else undo_loc();
    	return minimax(1-color, 1+depth, alpha, beta);
	}
	int tmp[9][9];
	int maxx = MIN;
	int mini = MAX;
	int x = 0, y = 0;
	memcpy(tmp, board, sizeof(board));
	for(int i = 1; i <= 8; ++i){
		for(int j = 1; j <= 8; ++j){
			if(board[i][j] == 2){
				board[i][j] = color;
				undo_loc();
				update(i, j, color);
				pair<pair<int, int>, int> p = minimax(1-color, depth+1, alpha, beta);
				memcpy(board, tmp, sizeof(tmp));
				
				if(color == (1-chess)){
					if(p.second > alpha){
						if(p.second > beta)
							return make_pair(make_pair(i, j), p.second);
						alpha = p.second;
					}
					if(p.second > maxx){
						maxx = p.second;
						x = i;
						y = j;
					}
				}
				else{
					if(p.second < beta){
						if(p.second < alpha)
							return make_pair(make_pair(i, j), p.second);
						beta = p.second;
					}
					if(p.second < mini){
						mini = p.second;
						x = i; 
						y = j;
					}
				}
			}
		}
	}
	undo_loc();
	if(color == (1-chess))
		return make_pair(make_pair(x, y), maxx);
	else 
		return make_pair(make_pair(x, y), mini);
}
void AI(int color){
	alter_loc(color);
	print_board();
	prev_x = ai_x, prev_y = ai_y;
	int cnt2 = count(2);
	if(cnt2 == 0){
		cout << "���غ����ӿ��䣬��һ������" << endl;
		system("pause");
		return; 
	}
	pair<pair<int, int>, int> p;
	p = minimax(color, 0, MIN, MAX);
	ai_x = p.first.first;
	ai_y = p.first.second;
	board[ai_x][ai_y] = color;
	undo_loc();
	update(ai_x, ai_y, color);
	
}

