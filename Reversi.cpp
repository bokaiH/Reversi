
/*黑白棋*/
#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<fstream>
#include<conio.h>
using namespace std;
#define MIN -9999999
#define MAX 0xffffff

int board[9][9]; //棋盘状态：-1空， 0白， 1黑， 2可下
int prev_board[9][9]; //悔棋用 
int a, b, chess, turn; //选项
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

void show_menu();//菜单 
void rule(); //游戏规则 
void game(); //游戏过程 
void load(); //读档 
void save(); //存档 
int count(int);//对某种颜色的棋子计数
void player(int);//玩家回合 
void update(int, int, int);//翻转棋子，更新棋盘 
void alter_loc(int); //可落子位置 
void undo_loc(); // 将可落子位置变为空位 
void AI(int); 
pair<pair<int, int>, int> minimax(int color, int depth, int alpha, int beta); 
int cal_score(int); //计算局面分数 
void print_board(); 
 
int main(){
    //显示菜单
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
    cout <<"===================     黑白棋     ==================="<< endl;
    cout <<"===================   1.新的游戏   ==================="<< endl;
    cout <<"===================   2.游戏规则   ==================="<< endl;
    cout <<"===================   3.读取存档   ==================="<< endl;
    cout <<"===================   4.退出游戏   ==================="<< endl;
    cout <<"======================================================"<< endl;
    cout << "请输入选择："; 
    cin >> a;
    switch(a){
    	case 1: 
    		system("cls");
    		//初始化棋盘 
    		memset(board, -1, sizeof(board));
    		board[5][4] = board[4][5] = 1; board[4][4] = board[5][5] = 0;
    		
    		cout << "请选择游戏类型" << endl;
			cout << "1.人机对战  2.双人游戏" << endl;
			cout << "输入："; 
			cin >> b;
			
			system("cls"); 
			if(b == 1){
				cout << "你想选用的棋子颜色" << endl;
				cout << "0.白  1.黑" << endl;
				cout << "输入："; 
				cin >> chess;
				turn = chess;
				if(chess == 1){
					board[4][3] = board[3][4] = board[6][5] = board[5][6] = 2;
				}
			}
			else if(b == 2){
				cout << "为保证游戏的公平性"<<endl;
				cout << "双人游戏暂不支持悔棋o(╥﹏╥)o"  << endl; 
				cout << "黑白棋，启动！！" <<endl;
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
    cout << "黑白棋游戏规则：\n" << endl;
    cout <<"1．棋局开始时黑棋位于44和55，白棋位于54和45" << endl;
    cout <<"2．黑方先行，双方交替下棋。"<< endl;
    cout <<"3．一步合法的棋步包括：在一个空格新落下一个棋子，并且翻转对手一个或多个棋子。"<< endl;
    cout <<"4．新落下的棋子与棋盘上已有的同色棋子间，对方被夹住的所有棋子都要翻转过来。可以是横着夹，竖着夹，或是斜着夹。夹住的位置上必须全部是对手的棋子，不能有空格。"<< endl;
    cout <<"5．一步棋可以在数个方向上翻棋，任何被夹住的棋子都必须被翻转过来，棋手无权选择不去翻某个棋子。"<< endl;
    cout <<"6．除非至少翻转了对手的一个棋子，否则就不能落子。如果一方没有合法棋步，也就是说不管他下到哪里，都不能至少翻转对手的一个棋子，那他这一轮只能弃权，而由他的对手继续落子直到他有合法棋步可下。"<< endl;
    cout <<"7．如果一方至少有一步合法棋步可下，他就必须落子，不得弃权。"<< endl;
    cout <<"8．棋局持续下去，直到棋盘填满或者双方都无合法棋步可下。"<< endl;
    cout <<"============================================================================="<< endl;
    system("pause");
    show_menu();
}

void print_board(){
	system("cls");
	int i, j;
	cout << "    1   2   3   4   5   6   7   8" << endl;
	cout << " ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
	for (i = 1; i <= 8; i++)
	{
		cout << i << "│";
		for (j = 1; j <= 8; j++)
		{
			switch (board[i][j])
			{
			case -1:cout << "   │"; break;
			case 0:cout << " ● │"; break;
			case 1:cout << " ○ │"; break;
			case 2:cout << " · │"; break;
			}
		}
		if (i == 1) 	 cout << "	================";
		else if (i == 2) cout << "	==== 1.悔棋 ====";
		else if(i == 3)  cout << "	==== 3.存档 ====";
		else if(i == 4)  cout << "	================";
		cout << endl;
		if (i == 8)    cout << " └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
		else if (i == 1)    cout << " ├───┼───┼───┼───┼───┼───┼───┼───┤	====  选项  ====" << endl;
		else if (i == 2)    cout << " ├───┼───┼───┼───┼───┼───┼───┼───┤	==== 2.暂停 ====" << endl;
		else if (i == 3) cout << " ├───┼───┼───┼───┼───┼───┼───┼───┤	==== 4.退出 ====" << endl;
		else    cout << " ├───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
	}
	cout << endl;
	int cnt1 = count(1), cnt0 = count(0);
	cout << "        黑：" << cnt1 << "        白："  << cnt0 << endl;
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
			cout << "游戏暂停" << endl; 
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
	if(b == 1){//人机对战 
		while(1){
			if(turn){
				alter_loc(chess);
				print_board();
				memcpy(prev_board, board, sizeof(board));
				cout << "上一步AI落子位置：" << ai_x <<' ' << ai_y << endl;
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
	if(count(1) == count(0)) cout << "                平局" <<endl;
	else if(count(1) > count(0)) cout << "                黑棋胜！" << endl;
	else cout << "                白棋胜！" << endl;  
	cout << endl;
	system("pause");
}

void player(int color){
	int cnt2 = count(2);
	if(cnt2 == 0){
		cout << "本回合无子可落，另一方落子" << endl;
		system("pause");
		return;
	}
	
	if(color)cout << "黑棋的回合" << endl;
	else cout << "白棋的回合" << endl;
	cout << "请输入落子位置：";
	int x, y;
	while(1){
		cin >> x >> y;
		if(board[x][y] == 2)break;
		cout << "不合法的位置，只有·处才能输入" << endl;
		cout << "请重新输入：";
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
	int flag = 0;//记录中间对方棋子的数量
	//向左检索
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
	//向右检索
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
	//向上检索
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
	//向下检索
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
	//向左上检索
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
	//向右下检索
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
	//向右上检索
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
	//向左下检索
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
				//向左
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
				//向右
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
				//向上
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
				//向下
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
				//左上
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
				//右下
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
				//左下
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
				//右上
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
		cout << "本回合无子可落，另一方落子" << endl;
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

