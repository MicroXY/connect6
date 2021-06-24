#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <bits/stdc++.h>
#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define TSS_DEEP 6 

// ����ȫ�����֣������������ز���
#define ALERTSCORE 10000
#define LEN 5
#define DEEP 4
#define FLAG 5
using namespace std;
struct Point{ //��ṹ
	int x,y;
	int score;
};
struct Step{ //���ṹ
	Point first,second;
	int value;
};
struct pos//FLAG��������������еĽṹ��
{
	int x;
	int y;
	int flag;//�洢����
	int mask;//����λ0���ţ�1����
};

struct expos//FLAG��������������еĽṹ��
{
	int x;
	int y;
	int flag;//�洢��ɫ
	int index;//�洢��FlagArray�е�����λ
	int score;
};

int Board[19][19];//�洢������Ϣ����Ԫ��ֵΪ BLACK, WHITE, EMPTY ֮һ
static Step moves;
// ˫����
int myScore[2][7] = {{0, 3, 200, 300, 600, 600, 10000000},{0, 3, 240, 380, 6000, 6000, 10000000}};
int enemyScore[2][7] = {{0, 3, 240, 380, 6000, 6000, 10000000},{0, 3, 200, 300, 600, 600, 10000000}};

bool cmp(const Point &a,const Point &b)
{
	if (a.score<0 && b.score<0)
	{
		return abs(a.score) >abs(b.score);
	}
	else if (a.score>0 && b.score<0)
	{
		return false;
	}
	else if (a.score<0 && b.score>0)
	{
		return true;
	}
	else
	{
		return a.score > b.score;
	}
}
int** CreatMap(void)//����19*19�ľ���
{
	int** map=new int*[19];
	for (int i = 0; i < 19; ++i)
	{
		map[i]=new int[19];
	}
	//init_map(map,19,19);
	return map;
}
void DeleteMap(int** map,const int high=19,const int wide=19)
{
	for (int i = 0; i < high; ++i)
	{
		delete[] map[i];
	}
	delete[] map;
}
void CopyMap(int** map)//������ͼ��Ϣ��һ������
{
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			map[i][j]=Board[i][j];
		}
	}
}
pos* define_past(int** map, const int high, const int wide,const int pCount)
{
    int temp = 0;
    pos *past = new pos[pCount];
    for(int i = 0; i < high; ++i){
        for(int j = 0; j < wide; ++ j){
            if(map[i][j] == BLACK){
                past[temp].x = i;
                past[temp].y = j;
                past[temp ++].flag = BLACK;
            }
            else if(map[i][j] == WHITE){
                past[temp].x = i;
                past[temp].y = j;
                past[temp ++].flag = WHITE;
            }
        }
    }
    return past;
}
int TSS_divide_map(int** map,int& count_pieces)//�Ե�ͼ���л��֣������ֲ����е�ѹ���ȽϷ�Χ
{
	int count=0;
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			//�Ƿ�õ�������
			if (map[i][j]==BLACK || map[i][j]==WHITE)
			{
				count_pieces++;
				//�ڸõ�������8����������3��
				int x_left  =((i-3)>=0 )?i-3:0;
				int x_right =((i+3)<=18)?i+3:18;

				int y_top   =((j-3)>=0 )?j-3:0;
				int y_bottom=((j+3)<=18)?j+3:18;
				//����ɨ��
				for (int ii = x_left; ii <= x_right; ++ii)
				{
					if (map[ii][j]==EMPTY)
					{
						//�Ծֲ��������
						map[ii][j]=FLAG;
						count++;
					}
				}
				//����ɨ��
				for (int jj = y_top; jj <= y_bottom; ++jj)
				{
					if (map[i][jj]==EMPTY)
					{
						//�Ծֲ��������
						map[i][jj]=FLAG;
						count++;
					}
				}
				//�����ϵ�����
				for (int ii = x_left , jj = y_top ;(ii<=x_right && jj <= y_bottom); ++ii,++jj)
				{
					if (map[ii][jj]==EMPTY)
					{
						//�Ծֲ��������
						map[ii][jj]=FLAG;
						count++;
					}
				}
				//�����ϵ�����
				for (int ii = x_left, jj = y_bottom;(ii<=x_right && jj>=y_top ); ++ii,--jj )
				{
					if (map[ii][jj]==EMPTY)
					{
						//�Ծֲ��������
						map[ii][jj]=FLAG;
						count++;
					}
				}
			}
		}
	}
	return count;
}
int divide_map(int** map)//�Ե�ͼ���л��֣������ֲ����е�ѹ���ȽϷ�Χ
{
	int count=0;
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			//�Ƿ�õ�������
			if (map[i][j]==BLACK || map[i][j]==WHITE)
			{
				//�ڸõ�������8����������3��
				int x_left  =((i-3)>=0 )?i-3:0;
				int x_right =((i+3)<=18)?i+3:18;

				int y_top   =((j-3)>=0 )?j-3:0;
				int y_bottom=((j+3)<=18)?j+3:18;
				//����ɨ��
				for (int ii = x_left; ii <= x_right; ++ii)
				{
					if (map[ii][j]==EMPTY)
					{
						//�Ծֲ��������
						map[ii][j]=FLAG;
						count++;
					}
				}
				//����ɨ��
				for (int jj = y_top; jj <= y_bottom; ++jj)
				{
					if (map[i][jj]==EMPTY)
					{
						//�Ծֲ��������
						map[i][jj]=FLAG;
						count++;
					}
				}
				//�����ϵ�����
				for (int ii = x_left , jj = y_top ;(ii<=x_right && jj <= y_bottom); ++ii,++jj)
				{
					if (map[ii][jj]==EMPTY)
					{
						//�Ծֲ��������
						map[ii][jj]=FLAG;
						count++;
					}
				}
				//�����ϵ�����
				for (int ii = x_left, jj = y_bottom;(ii<=x_right && jj>=y_top ); ++ii,--jj )
				{
					if (map[ii][jj]==EMPTY)
					{
						//�Ծֲ��������
						map[ii][jj]=FLAG;
						count++;
					}
				}
			}
		}
	}
	return count;
}
bool antiAttack(int **map, const int high, const int wide, int computerSide){

	//����������10

	// 0:��������
	// 1:����
	// 2:����
	// 3:����
	// 4:����
	// 5:����
	// 6:����
	// 7:������
	// 8:���
	// 9:�߶�

	int* way=new int[7];
	for (int i = 0; i < 7; ++i)
	{
		way[i]=0;
	}

	//�ӵ�ǰ����ĸ�����ɨ��


	//�ӵ�ǰ�����ɨ��LEN���ĵ�
	for (int i = 0; i < high; ++i)
	{
		for (int j = 0; j <= wide-6; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = j; k < j+6 ; ++k)
			{
			    if(map[i][k] == FLAG || map[i][k] == EMPTY){
                    continue_num ++;
			    }
			    if(map[i][k] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[i][k] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}

		}
	}

	//�ӵ�ǰ������ɨ��LEN���ĵ�
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = 0; j < wide; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = i; k < i+6 ; ++k)//������Խ���������ɨ�跽�򲻶Ե����⣿
			{
			    if(map[k][j] == FLAG || map[k][j] == EMPTY){
                    continue_num ++;
			    }
			    if(map[k][j] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[k][j] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}
		}
	}

	//�ӵ�ǰ������ϵ�����ɨ��LEN���ĵ�
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = 0; j <= wide-6; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = 0; k < 6 ; ++k)
			{
			    if(map[i + k][j + k] == FLAG || map[i + k][j + k] == EMPTY){
                    continue_num ++;
			    }
			    if(map[i + k][j + k] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[i + k][j + k] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}
		}
	}

	//�ӵ�ǰ������ϵ�����ɨ��LEN���ĵ�
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = LEN-1; j < wide; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = 0; k < 6 ; ++k)
			{
			    if(map[i + k][j - k] == FLAG || map[i + k][j - k] == EMPTY){
                    continue_num ++;
			    }
			    if(map[i + k][j - k] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[i + k][j - k] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}
		}
	}

	// 0:��������
	// 1:����
	// 2:����
	// 3:����
	// 4:����
	// 5:����
	// 6:����
	// 7:������
	// 8:���
	// 9:�߶�
	if(way[4] >= 1 || way[5] >= 1){
		return true;
	}
	delete[] way;
	return false;

}
int caculateFlag(int** map){
	int count_flag = 0;
	for(int i = 0; i < 19; i ++){
		for(int j = 0; j < 19; j ++){
			if(map[i][j] == FLAG){
				count_flag++;
			}
		}
	}
	return count_flag;
}
pos* TSS_creat_flag_array(int** map,const int high,const int wide,const int len)//������չ�ı��flag�������
{
	//pos����
	pos* FlagArray=new pos[len];
	//��ʼ��pos����
	for (int i = 0; i < len; ++i)
	{
		FlagArray[i].x=0;
		FlagArray[i].y=0;
		FlagArray[i].flag=0;
		FlagArray[i].mask=0;
	}
	int count=0;
	for (int i = 0; i < high; ++i)
	{
		for (int j = 0; j < wide; ++j)
		{
			if (map[i][j]==FLAG)//���ֳ�����FLAG��
			{
				FlagArray[count].x=i;
				FlagArray[count].y=j;
				FlagArray[count].flag=FLAG;
				++count;
			}
		}
	}
	return FlagArray;
}

Point* creat_flag_array(int** map,const int high,const int wide, int len)//������չ�ı��flag�������
{
	len++;
	//pos����
	Point* FlagArray=new Point[len];
	FlagArray[0].score=len;
	//��ʼ��pos����
	for (int i = 1; i < len; ++i)
	{
		FlagArray[i].x=0;
		FlagArray[i].y=0;
		FlagArray[i].score=0;
	}
	int count=1;
	for (int i = 0; i < high; ++i)
	{
		for (int j = 0; j < wide; ++j)
		{
			if (map[i][j]==FLAG)//���ֳ�����FLAG��
			{
				FlagArray[count].x=i;
				FlagArray[count].y=j;
				++count;
				map[i][j]=EMPTY;
			}
		}
	}
	return FlagArray;
}

//ȫ�����
int CalScore(int** map,const int computerSide,int baseScore)
{
	int model = 0;
    if(baseScore>ALERTSCORE)
        model = 1;
    //��ʼ����ͼ������
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19; j++)
        {
            if(map[i][j]==BLACK)
                map[i][j] = 1;
            else if(map[i][j]==WHITE)
                map[i][j] = 7;
            else
                map[i][j] = 0;
        }
    }
    int myRoad[7];
    int enemyRoad[7];
    memset(myRoad,0,sizeof(myRoad));
    memset(enemyRoad,0,sizeof(enemyRoad));
    //ˮƽ����
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19-5; j++)
        {
			//ֱ�Ӽ���ȡ��index�ٶȿ�
            int number = map[i][j]+map[i][j+1]+map[i][j+2]+map[i][j+3]+map[i][j+4]+map[i][j+5];
            //�쳣�������
			if(number==0||(number>6 && number%7!=0))
                continue;
            if(number<7)//ȫ��
            {
                if(computerSide==BLACK)
                    myRoad[number]++;
                else
                    enemyRoad[number]++;
            }
            else
            {
                if(computerSide==BLACK)
                    enemyRoad[number/7]++;
                else
                    myRoad[number/7]++;
            }
        }
    }
    // ��ֱ����
    for(int i=0; i<19-5; i++)
    {
        for(int j=0; j<19; j++)
        {
            int number = map[i][j]+map[i+1][j]+map[i+2][j]+map[i+3][j]+map[i+4][j]+map[i+5][j];
            if(number==0||(number>6 && number%7!=0))
                continue;
            if(number<7)
            {
                if(computerSide==BLACK)
                    myRoad[number]++;
                else
                    enemyRoad[number]++;
            }
            else
            {
                if(computerSide==BLACK)
                    enemyRoad[number/7]++;
                else
                    myRoad[number/7]++;
            }
        }
    }
    // ����-����
    for(int i=0; i<19-5; i++)
    {
        for(int j=0; j<19-5; j++)
        {
            int number = map[i][j]+map[i+1][j+1]+map[i+2][j+2]+map[i+3][j+3]+map[i+4][j+4]+map[i+5][j+5];
            if(number==0||(number>6&&number%7!=0))
                continue;
            if(number<7)
            {
                if(computerSide==BLACK)
                    myRoad[number]++;
                else
                    enemyRoad[number]++;
            }
            else
            {
                if(computerSide==BLACK)
                    enemyRoad[number/7]++;
                else
                    myRoad[number/7]++;
            }
        }
    }
    // ����-����
    for(int i=0; i<19-5; i++)
    {
        for(int j=5; j<19; j++)
        {
            int number = map[i][j]+map[i+1][j-1]+map[i+2][j-2]+map[i+3][j-3]+map[i+4][j-4]+map[i+5][j-5];
            if(number==0||(number>6 && number%7!=0))
                continue;
            if(number<7)
            {
                if(computerSide==BLACK)
                    myRoad[number]++;
                else
                    enemyRoad[number]++;
            }
            else
            {
                if(computerSide==BLACK)
                    enemyRoad[number/7]++;
                else
                    myRoad[number/7]++;
            }
        }
    }
    // ���
    int score = 0;
    for(int i=1; i<7; i++)
    {
        score += myRoad[i]*myScore[model][i]-enemyRoad[i]*enemyScore[model][i];
    }
    // ��ԭ
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19; j++)
        {
            if(map[i][j]==1)
                map[i][j] = BLACK;
            else if(map[i][j]==7)
                map[i][j] = WHITE;
            else
                map[i][j] = EMPTY;
        }
    }
    return score;
}

int OnePointScore(int** map, int computerSide,int x,int y,int model)
{
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19; j++)
        {
            if(map[i][j]==BLACK)
                map[i][j] = 1;
            else if(map[i][j]==WHITE)
                map[i][j] = 7;
            else
                map[i][j] = 0;
        }
    }
    //
    int myRoad[7];
    int enemyRoad[7];
    memset(myRoad,0,sizeof(myRoad));
    memset(enemyRoad,0,sizeof(enemyRoad));
    // ˮƽ����
    for(int i=y-5>0?y-5:0; i<=y && i+5<19; i++)
    {
        int number = map[x][i]+map[x][i+1]+map[x][i+2]+map[x][i+3]+map[x][i+4]+map[x][i+5];
        if(number==0||(number>6 && number%7!=0))
            continue;
        if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
    }
    /*-----------------------*/
    // ��ֱ����
    for(int i=x-5>0?x-5:0; i<=x && i+5<19; i++)
    {
        int number = map[i][y]+map[i+1][y]+map[i+2][y]+map[i+3][y]+map[i+4][y]+map[i+5][y];
        if(number==0||(number>6 && number%7!=0))
            continue;
        if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
    }
    /*-----------------------*/
    // ����-����
	for (int i =x,j=y,count=6; count>0 && i>=0 && j>=0 ; --i,--j,--count)
	{
		if (i+5>18 || j+5>18)
		{
			continue;
		}
		int number=map[i][j]+map[i+1][j+1]+map[i+2][j+2]+map[i+3][j+3]+map[i+4][j+4]+map[i+5][j+5];
		if(number==0||(number>6 && number%7!=0))
		{
            continue;
        }
		if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
	}

    /*-----------------------*/
    // ����-����
	for (int i =x,j=y,count=6; count>0 && i>=0 && j<19 ; --i,++j,--count)
	{
		if (i+5>18 || j-5<0)
		{
			continue;
		}
		int number=map[i][j]+map[i+1][j-1]+map[i+2][j-2]+map[i+3][j-3]+map[i+4][j-4]+map[i+5][j-5];
		if(number==0||(number>6 && number%7!=0))
		{
            continue;
        }
		if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
	}

    //cout<<"�����"<<endl;
    // ���
    int scoreme = 0;
	int scoreth = 0;
    for(int i=1; i<7; i++)
    {
        scoreme += myRoad[i]*myScore[model][i];
		scoreth += enemyRoad[i]*enemyScore[model][i];
    }
    // ��ԭ
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19; j++)
        {
            if(map[i][j]==1)
                map[i][j] = BLACK;
            else if(map[i][j]==7)
                map[i][j] = WHITE;
            else
                map[i][j] = EMPTY;
        }
    }
	int score= (scoreme>scoreth)?scoreme:scoreth;
    return score;
}

Point* GetBestPoint(int** map,int computerSide,int baseScore)
{
	int model=0;
	if(baseScore>ALERTSCORE)
        model = 1;
	int count=divide_map(map);
	Point* FlagArray=creat_flag_array(map,19,19,count);
	for(int i=1;i<FlagArray[0].score;++i)
	{
		map[FlagArray[i].x][FlagArray[i].y]=computerSide;
		int a=OnePointScore(map,computerSide,FlagArray[i].x,FlagArray[i].y,model);
		map[FlagArray[i].x][FlagArray[i].y]=1-computerSide;
		int b=OnePointScore(map,1-computerSide,FlagArray[i].x,FlagArray[i].y,1);
		FlagArray[i].score=(a>b)?a:b;
		map[FlagArray[i].x][FlagArray[i].y]=EMPTY;
	}
	sort(FlagArray+1,FlagArray+FlagArray[0].score,cmp);
	return FlagArray;
}

Point* GetBestPoint1(int** map,int computerSide,int baseScore)
{
	int model=0;
	if(baseScore>ALERTSCORE)
        model = 1;
	int count=divide_map(map);
	Point* FlagArray=creat_flag_array(map,19,19,count);
	for(int i=1;i<FlagArray[0].score;++i)
	{
		map[FlagArray[i].x][FlagArray[i].y]=computerSide;
		int a=OnePointScore(map,computerSide,FlagArray[i].x,FlagArray[i].y,1);
		FlagArray[i].score=a;
		map[FlagArray[i].x][FlagArray[i].y]=EMPTY;
	}
	sort(FlagArray+1,FlagArray+FlagArray[0].score,cmp);
	return FlagArray;
}

int PartScore(int** map,const int computerSide,Step temppoint,int model)
{
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19; j++)
        {
            if(map[i][j]==BLACK)
                map[i][j] = 1;
            else if(map[i][j]==WHITE)
                map[i][j] = 7;
            else
                map[i][j] = 0;
        }
    }
    int myRoad[7];
    int enemyRoad[7];
    memset(myRoad,0,sizeof(myRoad));
    memset(enemyRoad,0,sizeof(enemyRoad));
    // ˮƽ����
    int x = temppoint.first.x;
    int y = temppoint.first.y;
    for(int i=y-5>0?y-5:0; i<=y && i+5<19; i++)
    {
        int number = map[x][i]+map[x][i+1]+map[x][i+2]+map[x][i+3]+map[x][i+4]+map[x][i+5];
        if(number==0||(number>6 && number%7!=0))
            continue;
        if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
    }
    x = temppoint.second.x;
    y = temppoint.second.y;
	for(int i=y-5>0?y-5:0; i<=y && i+5<19; i++)
    {
		int xx = temppoint.first.x;
		int yy = temppoint.first.y;
		if(xx==x && (yy==i||yy==1+i||yy==2+i||yy==3+i||yy==4+i||yy==5+i))
            continue;
        int number = map[x][i]+map[x][i+1]+map[x][i+2]+map[x][i+3]+map[x][i+4]+map[x][i+5];
        if(number==0||(number>6 && number%7!=0))
            continue;
        if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
    }
    //cout<<"ˮƽ����������"<<endl;
    /*-----------------------*/
    // ��ֱ����
    x = temppoint.first.x;
    y = temppoint.first.y;
    for(int i=x-5>0?x-5:0; i<=x && i+5<19; i++)
    {
        int number = map[i][y]+map[i+1][y]+map[i+2][y]+map[i+3][y]+map[i+4][y]+map[i+5][y];
        if(number==0||(number>6 && number%7!=0))
            continue;
        if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
    }
    x = temppoint.second.x;
    y = temppoint.second.y;
	for(int i=x-5>0?x-5:0; i<=x && i+5<19; i++)
    {
		int xx = temppoint.first.x;
		int yy = temppoint.first.y;
        if( yy==y && ( xx==i||xx==1+i||xx==2+i||xx==3+i||xx==4+i||xx==5+i ))
            continue;
        int number = map[i][y]+map[i+1][y]+map[i+2][y]+map[i+3][y]+map[i+4][y]+map[i+5][y];
        if(number==0||(number>6 && number%7!=0))
            continue;
        if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
    }
    //cout<<"��ֱ����������"<<endl;
    /*-----------------------*/
    // ����-����
    x = temppoint.first.x;
    y = temppoint.first.y;
	for (int i =x,j=y,count=6; count>0 && i>=0 && j>=0 ; --i,--j,--count)
	{
		if (i+5>18 || j+5>18)
		{
			continue;
		}
		int number=map[i][j]+map[i+1][j+1]+map[i+2][j+2]+map[i+3][j+3]+map[i+4][j+4]+map[i+5][j+5];
		if(number==0||(number>6 && number%7!=0))
		{
            continue;
        }
		if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
	}
    x = temppoint.second.x;
    y = temppoint.second.y;
	for (int i =x,j=y,count=6; count>0 && i>=0 && j>=0 ; --i,--j,--count)
	{
        int xx = temppoint.first.x;
        int yy = temppoint.first.y;
		if ((i+5>18) || (j+5>18)||(yy==j&&xx==i)||(yy==1+j&&xx==1+i)||(yy==2+j&&xx==2+i)||(yy==3+j&&xx==3+i)||(yy==4+j&&xx==4+i)||(yy==5+j&&xx==5+i))
		{
			continue;
		}
		int number=map[i][j]+map[i+1][j+1]+map[i+2][j+2]+map[i+3][j+3]+map[i+4][j+4]+map[i+5][j+5];
		if(number==0||(number>6 && number%7!=0))
		{
            continue;
        }
		if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
	}
    //cout<<"��������������"<<endl;
    /*-----------------------*/
    // ����-����
    x = temppoint.first.x;
    y = temppoint.first.y;
	for (int i =x,j=y,count=6; count>0 && i>=0 && j<19 ; --i,++j,--count)
	{
		if (i+5>18 || j-5<0)
		{
			continue;
		}
		int number=map[i][j]+map[i+1][j-1]+map[i+2][j-2]+map[i+3][j-3]+map[i+4][j-4]+map[i+5][j-5];
		if(number==0||(number>6 && number%7!=0))
		{
            continue;
        }
		if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
	}
    x = temppoint.second.x;
    y = temppoint.second.y;
	for (int i =x,j=y,count=6; count>0 && i>=0 && j<19 ; --i,++j,--count)
	{
		int xx=temppoint.first.x;
		int yy=temppoint.first.y;
		if ((i+5>18) || (j-5<0)||(yy==j&&xx==i)||(yy==j-1&&xx==i+1)||(yy==j-2&&xx==i+2)||(yy==j-3&&xx==i+3)||(yy==j-4&&xx==i+4)||(yy==j-5&&xx==i+5))
		{
			continue;
		}
		int number=map[i][j]+map[i+1][j-1]+map[i+2][j-2]+map[i+3][j-3]+map[i+4][j-4]+map[i+5][j-5];
		if(number==0||(number>6 && number%7!=0))
		{
            continue;
        }
		if(number<7)
        {
            if(computerSide==BLACK)
                myRoad[number]++;
            else
                enemyRoad[number]++;
        }
        else
        {
            if(computerSide==BLACK)
                enemyRoad[number/7]++;
            else
                myRoad[number/7]++;
        }
	}
    //cout<<"��������������"<<endl;
    //cout<<"�����"<<endl;
    // ���
    int score = 0;
    for(int i=1; i<7; i++)
    {
        score += myRoad[i]*myScore[model][i]-enemyRoad[i]*enemyScore[model][i];
    }
    // ��ԭ
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19; j++)
        {
            if(map[i][j]==1)
                map[i][j] = BLACK;
            else if(map[i][j]==7)
                map[i][j] = WHITE;
            else
                map[i][j] = EMPTY;
        }
    }
    return score;
}

int connect6(int** map,const int high,const int wide,const int computerSide)
{
    // �˸���������Ϊ �ϣ��£����ң����ϣ����£����ϣ�����
    int dx[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[8] = {-1, 1, 0, 0, -1, 1, -1, 1};
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if(map[i][j]!=EMPTY)
			{
				int color=map[i][j];
				int x=i;
    			int y=j;
				int xx,yy,cnt;
				for (int j = 0; j < 8; ++j)
				{
					cnt=0;
					xx=x;
					yy=y;
					for (int k = 0; k < LEN; ++k)
					{
						xx+=dx[j];
						yy+=dy[j];
						if (xx > 18 || xx < 0 || yy > 18 || yy < 0) {
							continue;
						}
						// ·�ϲ����в�ͬ��ɫ������
						if (map[xx][yy] != color) {
							cnt = 0;
							break;
						}
						else if (map[xx][yy]==color)
						{
							cnt++;
						}
					}
					if (cnt==5 && color==computerSide)
					{
						return 1;
					}
					else if (cnt==5 && color==1-computerSide)
					{
						return -1;
					}
				}
			}
		}
	}
    return 0;
}

void Reversal(int** map)
{
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map[i][j]==BLACK)
			{
				map[i][j]=WHITE;
			}
			else if (map[i][j]==WHITE)
			{
				map[i][j]=BLACK;
			}
		}
	}
}

//alpah-beta��
int AlphaBetaTree(int** map,int depth,int alpha,int beta,int computerSide,int baseScore)
{
	int pp=connect6(map,19,19,computerSide);
	if(depth==0 ||pp==1 || pp==-1)//����Ҷ�ڵ�
	{
		if (pp==1)
		{
			return 100000;
		}
		else if (pp==-1)
		{
			return -100000;
		}
		else
		{
			//�����Ϸû�н���
			// return CalScore(map,computerSide,baseScore);
			return baseScore;
		}
	}
	else if(depth%2==0)//MAX
	{
		//ѡ���Ϻõ�10����
		Point* pointarray=GetBestPoint(map,computerSide,baseScore);
		// int len1=0;
		// Point* pointarray1=NULL;
		// if (depth==DEEP)
		// {
		// 	Reversal(map);
		// 	pointarray1=GetBestPoint(map,1-computerSide,baseScore);
		// 	Reversal(map);
		// 	len1=pointarray1[0].score;
		// }


		//������ѡ�����ָߵ�10���
		int len=pointarray[0].score;
		len=(len>10)?10:len;




		for (int i = 1; i < len	; i++)
		{

			for (int j = i+1; j < len; j++)
			{	int model=0;
				if(baseScore>ALERTSCORE)
					model = 1;
				Step tt;
				tt.first.x=pointarray[i].x;
				tt.first.y=pointarray[i].y;
				tt.second.x=pointarray[j].x;
				tt.second.y=pointarray[j].y;
				int a=PartScore(map,computerSide,tt,model);
				map[pointarray[i].x][pointarray[i].y]=computerSide;
				map[pointarray[j].x][pointarray[j].y]=computerSide;
				int b=PartScore(map,computerSide,tt,model);
				int tempScore=baseScore+b-a;   //��ȡĿǰ��������
				int score=AlphaBetaTree(map,depth-1,alpha,beta,computerSide,tempScore);
				map[pointarray[i].x][pointarray[i].y]=EMPTY;
				map[pointarray[j].x][pointarray[j].y]=EMPTY;//��ԭ����
				if(score>alpha)
				{
					alpha=score;
					if(depth==DEEP)
					{
						//��ȡ��ѵ�
						moves.first.x = pointarray[i].x;
						moves.first.y = pointarray[i].y;
						moves.second.x= pointarray[j].x;
						moves.second.y= pointarray[j].y;
					}
					if(alpha>=beta)
					{
						return beta;
					}
				}
			}
		}
		return alpha;
	}
	else//MIN
	{
		// Reversal(map);
		//ѡ���Ϻõ�10����
		Point* pointarray=GetBestPoint1(map,1-computerSide,100000);
		// Reversal(map);
		//������ѡ�����ָߵ�10���
		int len=pointarray[0].score;
		len=(len>10)?10:len;

		for (int i = 1; i < len	; i++)
		{
			for (int j = i+1; j < len; j++)
			{
				int model=0;
				if(baseScore>ALERTSCORE)
					model = 1;
				Step tt;
				tt.first.x=pointarray[i].x;
				tt.first.y=pointarray[i].y;
				tt.second.x=pointarray[j].x;
				tt.second.y=pointarray[j].y;
				int a=PartScore(map,1-computerSide,tt,model);
				map[pointarray[i].x][pointarray[i].y]=1-computerSide;
				map[pointarray[j].x][pointarray[j].y]=1-computerSide;
				int b=PartScore(map,1-computerSide,tt,model);
				int tempScore=baseScore-(b-a);   //��ȡĿǰ��������
				int score=AlphaBetaTree(map,depth-1,alpha,beta,computerSide,tempScore);
				map[pointarray[i].x][pointarray[i].y]=EMPTY;
				map[pointarray[j].x][pointarray[j].y]=EMPTY;//��ԭ����
				if(score<beta)
				{
					beta=score;
					if(alpha>=beta)
					{
						return alpha;
					}
				}
			}
		}
		return beta;
	}
	return -1;
}
bool TTSmaxJudge(int **map, int computerSide, int row, int column, int direction){
    // �������ӵķ���
    //��������ķ��������·�����������ķ����Ϊ�ĸ����򣬷ֱ��Ǻ���������Խ��ߣ��ҶԽ���
	//ÿ�����������������5������ĵ㿪ʼ�����Ÿ÷�����6���㣬�۲��ж���·��ÿ���һ������������ǰ��һ����Ϊ����㣬һֱǰ�����������Ӵ�Ϊֹ
	int score = 0;
    //������Ŀͳ��
    int pattern[7];
    // ��ʼ��������ĿΪ0
    memset(pattern, 0, sizeof(pattern));
    // �ĸ���������Ϊ ����������Խ��ߣ��ҶԽ���
    int dx[4] = {0, 1, 1, 1};
    int dy[4] = {1, 0, 1, -1};
	//��ʼ���
      int tempDirection = direction; //����Ŀǰ���ķ���

	  int xx = row -  dx[tempDirection]*5; //�����ʼ��
	  int yy = column - dy[tempDirection] * 5; //�����ʼ��
	  int maxRoad = 0; //�ҵ��÷����ϰ������ĵ������·���ݴ�������
	  for(int offset = 0; offset < 6; offset ++){
	  	if(offset != 0){
          xx = xx + dx[tempDirection]; //��ʼ����ÿ�μ��һ�����һλ
          yy = yy + dy[tempDirection]; //��ʼ����ÿ�μ��һ�����һλ
          }
          if(xx < 0 || xx > 18 || yy < 0 || yy > 18){
          	continue;
		  }
          int check_x = xx;  //���ĵ�
          int check_y = yy;  //���ĵ�
          int continueChess = 0;
          int meetEnemy = 0;
          int num = 0;
          for(num = 0; num < 6; num ++){
              if(num != 0){
              check_x += dx[tempDirection];
              check_y += dy[tempDirection];
              }
              if(check_x < 0 || check_x > 18 || check_y < 0 || check_y > 18){
                  continue;
              }
              if(map[check_x][check_y] == 1 - computerSide){
                  continueChess = 0;
                  meetEnemy = 1;
                  break;
              }
              if(map[check_x][check_y] == computerSide){
                  continueChess ++;
              }
          }
          if(continueChess > maxRoad){
            maxRoad = continueChess;
          }
          if(meetEnemy == 1){
           	offset += num;
           	xx = check_x;
			yy = check_y;
		  }
	  }
	if(maxRoad < 4){
		return false;
	}
	int count = 0;
	int meetSpace = 0;
	int nowX = row;
	int nowY = column;
	int sideCount = 0;
	for(int i = 0; i < 5; i ++){
		nowX = nowX +  dx[tempDirection];
		nowY = nowY +  dy[tempDirection];
		if(nowX < 0 || nowX > 18 || nowY < 0 || nowY > 18){
			break;
		}
		if(map[nowX][nowY] == EMPTY || map[nowX][nowY] == FLAG){
			if(meetSpace == 0){
				meetSpace = 1;
			}
			count ++;
		}
		if((map[nowX][nowY] != EMPTY && map[nowX][nowY] != FLAG) && meetSpace == 1){
			break;
		}
		if((map[nowX][nowY] == computerSide && meetSpace == 0)){
			sideCount ++;
		}
		if(map[nowX][nowY] == 1 - computerSide){
			break;
		}
	}
	if(count < 2){
		return false;
	}
	nowX = row;
	nowY = column;
	count = 0;
	meetSpace = 0;
	for(int i = 0; i < 5; i ++){
		nowX = nowX -  dx[tempDirection];
		nowY = nowY -  dy[tempDirection];
		if(nowX < 0 || nowX > 18 || nowY < 0 || nowY > 18){
			break;
		}
		if(map[nowX][nowY] == EMPTY || map[nowX][nowY] == FLAG){
			if(meetSpace == 0){
				meetSpace = 1;
			}
			count ++;
		}
		if((map[nowX][nowY] != EMPTY && map[nowX][nowY] != FLAG) && meetSpace == 1){
			break;
		}
		if((map[nowX][nowY] == computerSide && meetSpace == 0)){
			sideCount ++;
		}
		if(map[nowX][nowY] == 1 - computerSide){
			break;
		}
	}
	if(sideCount < 3){
		return false;
	}
	if(count < 2){
		return false;
	}
	return true;
}
int TPointNum;

expos** TSS_get_best_point(int** temp_map,pos* FlagArray,int count_flag,int& isenough, int computerSide, int totalNum)
{
    int count = 0; //����FlagArray������flagֵΪ0�Լ�maskֵΪ0��Ԫ�صĸ���
	expos** array=new expos*[5];
	for(int i = 0; i < 5; i ++){
		array[i] = new expos[2];
	}
			for (int _i = 0; _i < count_flag; ++_i)
			{
				//ѡȡ��һ���Լ��µĵ�
				if (FlagArray[_i].flag==FLAG )
				{
					temp_map[FlagArray[_i].x][FlagArray[_i].y]=computerSide;
					FlagArray[_i].flag=computerSide;
					int row1 = FlagArray[_i].x;
					int column1 = FlagArray[_i].y;
					for (int _j = _i+1; _j < count_flag; ++_j)
					{
						//ѡȡ�ڶ����Լ��µĵ�
						if (FlagArray[_j].flag==FLAG )
						{
							temp_map[FlagArray[_j].x][FlagArray[_j].y]=computerSide;
							FlagArray[_j].flag=computerSide;
							int row2 = FlagArray[_j].x;
							int column2 = FlagArray[_j].y;
							if((row1 != row2) && (column1 != column2) && (row1 - row2 != column1 - column2) && (row1 - row2 != column2 - column1)){
								temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
								FlagArray[_j].flag=FLAG;
								continue;
							}
							if(row1 == row2){
								if(column1 - column2 > 3 || column1 - column2 < -3){
									temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
									FlagArray[_j].flag=FLAG;
									continue;
								}
							}
							else if(column1 == column2){
								if(row1 - row2 > 3 || row1 - row2 < -3){
									temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
									FlagArray[_j].flag=FLAG;
									continue;
								}
							}
							else {
								if(row1 - row2 > 3 || row1 - row2 < -3){
									temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
									FlagArray[_j].flag=FLAG;
									continue;
								}
							}

							if(count >= totalNum){
								isenough = 1;
								temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
								FlagArray[_j].flag=FLAG;
								break;
							}
							int direction;
							if(row1 == row2){
								direction = 0;
							}
							else if(column1 == column2){
								direction = 1;
							}
							else if(row1 - row2 == column1 - column2){
								direction = 2;
							}
							else {
								direction = 3;
							}
							if(count >= totalNum){
								temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
								FlagArray[_j].flag=FLAG;
								break;
							}

							if(TTSmaxJudge(temp_map, computerSide, row1, column1, direction)){
								array[count][0].x = row1;
								array[count][0].y = column1;
								array[count][0].index = _i;
								array[count][1].x = row2;
								array[count][1].y = column2;
								array[count][1].index = _j;
								count ++;

							}


							temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
							FlagArray[_j].flag=FLAG;
						}
					}
					temp_map[FlagArray[_i].x][FlagArray[_i].y]=FLAG;
					FlagArray[_i].flag=FLAG;
				}
			}
	if(count < totalNum){
		isenough = 0;
		TPointNum = count;
	}
	return array;
}
int MinNode;
pos* TSSMinFind(int **map, int computerSide, int row1, int column1, int row2, int column2, int &isenough){
	pos* PointArray = new pos[4];
	int index = 0;
	int direction;
	if(row1 == row2){
		direction = 0;
	}
	else if(column1 == column2){
		direction = 1;
	}
	else if(row1 - row2 == column1 - column2){
		direction = 2;
	}
	else {
		direction = 3;
	}

    int dx[4] = {0, 1, 1, 1};
    int dy[4] = {1, 0, 1, -1};
	//��ʼ���
	int tempDirection = direction;
	int* count2 = new int[2];
	int count = 0;
	int meetSpace = 0;
	int nowX = row1;
	int nowY = column1;
	for(int i = 0; i < 6; i ++){
		nowX = nowX + dx[tempDirection];
		nowY = nowY + dy[tempDirection];
		if(nowX < 0 || nowX > 18 || nowY < 0 || nowY > 18){
			break;
		}
		if(map[nowX][nowY] == EMPTY || map[nowX][nowY] == FLAG){
			if(meetSpace == 0){
				meetSpace = 1;
			}

			count ++;
			if(count <= 2){
				PointArray[index].x = nowX;
				PointArray[index].y = nowY;
				index ++;
			}
		}
		if((map[nowX][nowY] != EMPTY && map[nowX][nowY] != FLAG) && meetSpace == 1){
			break;
		}
		if(map[nowX][nowY] == 1 - computerSide){
			break;
		}
	}
	count2[0] = count;
	count = 0;
	meetSpace = 0;
	nowX = row1;
	nowY = column1;
	for(int i = 0; i < 5; i ++){
		nowX = nowX -  dx[tempDirection];
		nowY = nowY -  dy[tempDirection];
		if(nowX < 0 || nowX > 18 || nowY < 0 || nowY > 18){
			break;
		}
		if(map[nowX][nowY] == EMPTY || map[nowX][nowY] == FLAG){
			if(meetSpace == 0){
				meetSpace = 1;
			}

			count ++;
			if(count <= 2){
				PointArray[index].x = nowX;
				PointArray[index].y = nowY;
				index ++;
			}
		}
		if((map[nowX][nowY] != EMPTY && map[nowX][nowY] != FLAG) && meetSpace == 1){
			break;
		}
		if(map[nowX][nowY] == 1 - computerSide){
			break;
		}
	}
	if(index < 4){
		isenough = 0;
		MinNode = index;
	}
	count2[1] = count;
	delete[] count2;
	return PointArray;
}
int overTSS(int **map, const int high, const int wide, const int computerSide, pos *past,int _count,int depth) {
	if(depth % 2 != 0){
		return 0;
	}
    // �������ӵķ���
    int myScore, enemyScore;
    myScore = enemyScore = 0;
    //������Ŀͳ��
    int myPartten[7];
    int enemyPartten[7];
    // ��ʼ��������ĿΪ0
    memset(enemyPartten, 0, sizeof(enemyPartten));
    memset(myPartten, 0, sizeof(myPartten));
    // �˸���������Ϊ �ϣ��£����ң����ϣ����£����ϣ�����
    int dx[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[8] = {-1, 1, 0, 0, -1, 1, -1, 1};

    // ��������
    for (int pos = 0; pos<_count; pos++) {
        // �õ�������ɫ
        int color = past[pos].flag;
        // ͳ��ȫ�ַ���
        // ��������
        int x = past[pos].x;
        int y = past[pos].y;
        // �����˸����� xx,yy �µ�����㣬cnt ==>·��
        int xx, yy, cnt;
        for (int i = 0; i < 8; i++) {
            // ��ʼ��
            cnt = 0;
            // ��չ����LEN
			xx = x;
			yy = y;
            for (int j = 0; j < 6; j++) {
            	if(j != 0){
                xx = xx + dx[i];
                yy = yy + dy[i];
                }
                // Խ�磬ֱ������
                if (xx > 18 || xx < 0 || yy > 18 || yy < 0) {
                    break;
                }
                // ·�ϲ����в�ͬ��ɫ������
                if (map[xx][yy] == 1 - color) {
                	cnt = 0;
                    break;
                }
                // ͳ��·��
                if (map[xx][yy] == color) {
                    cnt++;

                }
            }

            // ����·��
            if (computerSide == color) {
                myPartten[cnt]++;
            } else {
                enemyPartten[cnt]++;
            }

        }

        

    }
    if(myPartten[4] >= 1){
    	return 1;
	}
	return 0;
    // ͳ��ȫ������
}
int firstXX;
int firstYY;
int secondXX;
int secondYY;
int TSStree(int** same_map,pos* FlagArray,int count_flag,int count_pieces,int depth,int computerSide,Step temppoint)
{
	pos *past = define_past(same_map, 19, 19, count_pieces);

	int nowResult = overTSS(same_map, 19, 19, computerSide, past, count_pieces,depth);
	if ((depth==0 || nowResult == 1 ) && depth != TSS_DEEP)//������ײ㣬����������
	{
		//�������ӵĵ����������������
		delete[] past;
		if(nowResult == 1){

			return 1;
		}
		else {
			return 0;
		}
	}
	else if(depth%2==0)//max
	{
	    // pos * past=NULL;
		int isenough=1;
		//�ָ�FlagArray����,������ĵ��������ѡ��������������10��������鲢����
		//�������10����ôֱ�ӽ�����ĵ�ȫ�����������з��أ�����isenough=0;
		count_flag = TSS_divide_map(same_map, count_pieces);
		count_flag = caculateFlag(same_map);
		FlagArray = TSS_creat_flag_array(same_map,19,19,count_flag);

		expos** ArrayMe=TSS_get_best_point(same_map,FlagArray,count_flag,isenough,computerSide,5);
		int len=(isenough==1)? 5:TPointNum;
		for(int i = 0; i < len; i ++){
			int row1 = ArrayMe[i][0].x;
			int column1 = ArrayMe[i][0].y;
			int row2 = ArrayMe[i][1].x;
			int column2 = ArrayMe[i][1].y;
			same_map[row1][column1] = computerSide;
			same_map[row2][column2] = computerSide;
			temppoint.first.x = row1;
			temppoint.first.y = column1;
			temppoint.second.x = row2;
			temppoint.second.y = column2;
			FlagArray[ArrayMe[i][0].index].flag = computerSide;
			FlagArray[ArrayMe[i][1].index].flag = computerSide;
			int result = TSStree(same_map, FlagArray, count_flag, count_pieces + 2, depth - 1, computerSide, temppoint);
			FlagArray[ArrayMe[i][0].index].flag = FLAG;
			FlagArray[ArrayMe[i][1].index].flag = FLAG;
			same_map[row1][column1] = FLAG;
			same_map[row2][column2] = FLAG;
			if(depth == TSS_DEEP && result == 1){
				firstXX = row1;
				firstYY = column1;
				secondXX = row2;
				secondYY = column2;
			}
			if(result == 1){
			    delete[] FlagArray;
				return 1;
			}
		}
		delete[] FlagArray;
		return 0;


	}
	else//min
	{
	    // pos * past=NULL;
	    int isenough = 1;
	    int row1 = temppoint.first.x;
	    int column1 = temppoint.first.y;
	    int row2 = temppoint.second.x;
	    int column2 = temppoint.second.y;
	    pos* minArray = TSSMinFind(same_map, computerSide, row1, column1, row2, column2, isenough);
		count_flag = TSS_divide_map(same_map, count_pieces);
		count_flag = caculateFlag(same_map);
		FlagArray = TSS_creat_flag_array(same_map,19,19,count_flag);
		int len=(isenough==1)? 4:MinNode;
		for(int i = 0; i < len; i ++){
			int nowX = minArray[i].x;
			int nowY = minArray[i].y;
			if(same_map[nowX][nowY] == EMPTY || same_map[nowX][nowY] == FLAG){
				same_map[nowX][nowY] = 1 - computerSide;
				for(int j = 0; j < count_flag; j ++){
					int tempX = FlagArray[j].x;
					int tempY = FlagArray[j].y;
					if(tempX == nowX && tempY == nowY){
						FlagArray[j].flag = 1 - computerSide;
					}
				}
			}
		}
		int result = TSStree(same_map, FlagArray, count_flag, count_pieces + len, depth - 1, computerSide, temppoint);
		for(int i = 0; i < len; i ++){
			int nowX = minArray[i].x;
			int nowY = minArray[i].y;
			if(same_map[nowX][nowY] == 1 - computerSide){
				same_map[nowX][nowY] = FLAG;
				same_map[nowX][nowY] = FLAG;
				for(int j = 0; j < count_flag; j ++){
					int tempX = FlagArray[j].x;
					int tempY = FlagArray[j].y;
					if(tempX == nowX && tempY == nowY){
						FlagArray[j].flag = FLAG;
					}
				}
			}
		}
		delete[] FlagArray;
		if(result == 1){
			return 1;
		}

		return 0;

  }
}
int fakeGrade(int** map,const int high,const int wide,const int computerSide, int defend)
{


	int* way=new int[7];
	for (int i = 0; i < 7; ++i)
	{
		way[i]=0;
	}

	//�ӵ�ǰ����ĸ�����ɨ��


	//�ӵ�ǰ�����ɨ��LEN���ĵ�
	for (int i = 0; i < high; ++i)
	{
		for (int j = 0; j <= wide-6; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = j; k < j+6 ; ++k)
			{
			    if(map[i][k] == FLAG || map[i][k] == EMPTY){
                    continue_num ++;
			    }
			    if(map[i][k] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[i][k] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}

		}
	}

	//�ӵ�ǰ������ɨ��LEN���ĵ�
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = 0; j < wide; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = i; k < i+6 ; ++k)//������Խ���������ɨ�跽�򲻶Ե����⣿
			{
			    if(map[k][j] == FLAG || map[k][j] == EMPTY){
                    continue_num ++;
			    }
			    if(map[k][j] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[k][j] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}
		}
	}

	//�ӵ�ǰ������ϵ�����ɨ��LEN���ĵ�
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = 0; j <= wide-6; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = 0; k < 6 ; ++k)
			{
			    if(map[i + k][j + k] == FLAG || map[i + k][j + k] == EMPTY){
                    continue_num ++;
			    }
			    if(map[i + k][j + k] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[i + k][j + k] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}
		}
	}

	//�ӵ�ǰ������ϵ�����ɨ��LEN���ĵ�
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = 6-1; j < wide; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = 0; k < 6 ; ++k)
			{
			    if(map[i + k][j - k] == FLAG || map[i + k][j - k] == EMPTY){
                    continue_num ++;
			    }
			    if(map[i + k][j - k] == computerSide){
                    my_piece_num ++;
			    }
			    if(map[i + k][j - k] == 1 - computerSide){
                    my_piece_num = 0;
                    break;
			    }
			}
			switch(my_piece_num){
			    case 1: way[1] ++; break;
			    case 2: way[2] ++; break;
			    case 3: way[3] ++; break;
			    case 4: way[4] ++; break;
			    case 5: way[5] ++; break;
			    case 6: way[6] ++; break;
			}
		}
	}


	int score=0;
	for (int i = 1; i < 7; ++i)
	{
	    if(defend == 0){
		//����������
		switch(i)
		{
			case 1:score+=way[i]*1; break;
			case 2:score+=way[i]*30; break;
			case 3:score+=way[i]*80; break;
			case 4:score+=way[i]*300; break;
			case 5:score+=way[i]*350; break;
			case 6:score+=way[i]*100000; break;
		}
	    }
	    else {
		switch(i)
		{
			case 1:score+=way[i]*1; break;
			case 2:score+=way[i]*30; break;
			case 3:score+=way[i]*80; break;
			case 4:score+=way[i]*5000; break;
			case 5:score+=way[i]*5000; break;
			case 6:score+=way[i]*100000; break;
		}
	    }
	}
	delete[] way;
	return score;
}
int main()
{
	Step step;//��ʱ���ṹ
	char message[256];//ͨ����Ϣ����
    int computerSide;//����ִ����ɫ
    int start=0;//�Ծֿ�ʼ���
	srand(int(time(0)));
	//��ʼ����ͼ(����������)
    int** map=CreatMap();
    int** TSS_map = CreatMap();
    int** temp_map = CreatMap();
	//�˴����ó�ʼ������
    //...
	while (1)	//������ѭ��
	{
		fflush(stdout);//��Ҫɾ������䣬�������������
		scanf("%s", message);//��ȡƽ̨������Ϣ
        //��������
		if (strcmp(message, "name?") == 0)//���սƽ̨���Ͷ���
		{
			fflush(stdin);
			/***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
			/*******/		printf("name CM6\n");		/**ֻ�޸�����壬��Ҫɾ��name�ո�****/
			/***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
		}
		else if (strcmp(message, "new") == 0)//���������
		{
			int i, j;
			scanf("%s", message);//��ȡ����ִ����ɫ

			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //ִ��
			else  computerSide = WHITE;   //ִ��

			for (i = 0; i<19; ++i)   //��ʼ�����
				for (j = 0; j<19; ++j)
					Board[i][j] = EMPTY;
			// baseScore=0;//�¿���ֳ�ʼ����������Ϊ0
			start = 1;

			if (computerSide == BLACK)
			{
				/**********���ɵ�һ���ŷ�����������step�ṹ�У���������Ϊ(step.first.x,step.first.y)**********/
				/****************************���·������룬���滻�ҵ�ʾ������******************************/


				step.first.x = 9;
				step.first.y = 9;


				/******************************����������һ���������*******************************************/

				Board[step.first.x][step.first.y] = computerSide;//����������
				printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//����ŷ�
			}
		}
		else if(strcmp(message,"move")==0)//����,���������
		{
			scanf("%s", message);//��ȡ���������ŷ�
            fflush(stdin);
			step.first.x=message[0]-'A';		step.first.y=message[1]-'A';
			step.second.x=message[2]-'A';		step.second.y=message[3]-'A';
            //�����������
			Board[step.first.y][step.first.x] = 1 - computerSide;
			if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.y][step.second.x] = 1 - computerSide;

            /**********************************************************************************************************/
			/***�������ӵ����꣬������step�ṹ�У���1������(step.first.x,step.first.y)����2������(step.first.x,step.first.y)*****/
			/**************************************���·������룬���滻�ҵ�ʾ������*****************************************/
			//��ʼ���ṹ
				//��ʼ�����ߵĲ��ṹ
				moves.first.x=-1;
				moves.first.y=-1;
				moves.second.x=-1;
				moves.second.y=-1;

				//InitMap(map,0);
				CopyMap(map);
				//�������ӵ�
			//whileѭ����ʼ֮ǰ��ʼ��hash��(alpha-beta hash,TSS hash)
			//
			//


			//�˴���TTS���ߴ���
				//���TTS�о�����ôִ�������
           if(antiAttack(map, 19, 19, computerSide)){
           	CopyMap(temp_map);
           	int count_pieces = 0;
			int count_flag  =TSS_divide_map(temp_map,count_pieces);//����ֵ���Զ��ٸ�������flag���
			pos* FlagArray=TSS_creat_flag_array(temp_map,19,19,count_flag);
				int max_score = -100000;
				for (int _i = 0; _i < count_flag; ++_i)
			{
				//ѡȡ��һ���Լ��µĵ�
				if (FlagArray[_i].flag==FLAG )
				{
					temp_map[FlagArray[_i].x][FlagArray[_i].y]=computerSide;
					FlagArray[_i].flag=computerSide;
					for (int _j = _i+1; _j < count_flag; ++_j)
					{
						//ѡȡ�ڶ����Լ��µĵ�
						if (FlagArray[_j].flag==FLAG )
						{
							temp_map[FlagArray[_j].x][FlagArray[_j].y]=computerSide;
							FlagArray[_j].flag=computerSide;
							int score_me =fakeGrade(temp_map,19,19,computerSide,0);//���Լ�����������
							int score_amy=fakeGrade(temp_map,19,19,1 - computerSide,1);//�Ե��˵���������
							int score    = score_me-score_amy;
							//�Լ����ִ��ڵ������֣������߲�ֵ�ϴ���ѡ��õ�
							if (score > max_score)
							{
								max_score=score;
								moves.first.x =FlagArray[_i].x;
								moves.first.y =FlagArray[_i].y;
								moves.second.x =FlagArray[_j].x;
								moves.second.y =FlagArray[_j].y;
							}
							temp_map[FlagArray[_j].x][FlagArray[_j].y]=FLAG;
							FlagArray[_j].flag=FLAG;
						}
					}
					temp_map[FlagArray[_i].x][FlagArray[_i].y]=FLAG;
					FlagArray[_i].flag=FLAG;
				}
			}
			}
			else{
		int isenough=1;
			CopyMap(TSS_map);
			Step temp_p;
			temp_p.first.x=step.first.y;
			temp_p.first.y=step.first.x;
			temp_p.second.x=step.second.y;
			temp_p.second.y=step.second.x;
			int count_pieces=0;//��¼�������ĸ���
			int count_flag  = TSS_divide_map(TSS_map,count_pieces);//����ֵ���Զ��ٸ�������flag���
			pos* FlagArray;
		int result = TSStree(TSS_map, FlagArray, count_flag, count_pieces, 6, computerSide, temp_p);
		//�ָ�FlagArray����,������ĵ��������ѡ��������������10��������鲢����
		//�������10����ôֱ�ӽ�����ĵ�ȫ�����������з��أ�����isenough=0;
		if(result == 1 && !antiAttack(map, 19, 19, 1 - computerSide)){
			moves.first.x = firstXX;
			moves.first.y = firstYY;
			moves.second.x = secondXX;
			moves.second.y = secondYY;
		}
		else{
				//���������ͨ�ľ�����
					//����һ��ȫ��baseScore
					int baseScore=CalScore(map,computerSide,0);
					//���������
						//���baseScore>alertscore
					int score=AlphaBetaTree(map,DEEP,-10000000,10000000,computerSide,baseScore);
						// ��ֹû�о��߽��ʱ�򣬲����������
					}
				}
			if (moves.first.x==-1 && moves.first.y==-1)
			{
				moves.first.x=rand()%19;
				moves.first.y=rand()%19;
				while(map[moves.first.x][moves.first.y]!=EMPTY)
				{
					moves.first.x=rand()%19;
					moves.first.y=rand()%19;
				}
			}
			if (moves.second.x==-1 && moves.second.x==-1)
			{
				moves.second.x=rand()%19;
				moves.second.y=rand()%19;
				while(map[moves.second.x][moves.second.y]!=EMPTY)
				{
					moves.second.x=rand()%19;
					moves.second.y=rand()%19;
				}
			}
			//���ؼ���С��������
			//���ɵ�1������λ��step.first.x��step.first.y
			step.first.x =moves.first.x;
			step.first.y =moves.first.y;
			Board[step.first.x][step.first.y] = computerSide;
			//���ɵ�2������λ��step.second.x��step.second.y
			step.second.x =moves.second.x;
			step.second.y =moves.second.y;
			Board[step.second.x][step.second.y] = computerSide;
			/*****************************************������������******************************************************/
			/**********************************************************************************************************/

			printf("move %c%c%c%c\n",step.first.y+'A',step.first.x+'A',step.second.y+'A',step.second.x+'A');//����ŷ�
		}
        else if (strcmp(message, "error") == 0)//�ŷ�����
        {
            fflush(stdin);
        }
		else if (strcmp(message, "end") == 0)//�Ծֽ���
		{
            fflush(stdin);
			start = 0;
		}
		else if (strcmp(message, "quit") == 0)//�˳�����
		{
            fflush(stdin);
			printf("Quit!\n");
			break;
		}
	}
    DeleteMap(map);

	return 0;
}
