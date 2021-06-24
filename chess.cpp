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

// 返回全局评分，决定进攻防守参数
#define ALERTSCORE 10000
#define LEN 5
#define DEEP 4
#define FLAG 5
using namespace std;
struct Point{ //点结构
	int x,y;
	int score;
};
struct Step{ //步结构
	Point first,second;
	int value;
};
struct pos//FLAG和已下棋的数组中的结构体
{
	int x;
	int y;
	int flag;//存储棋形
	int mask;//屏蔽位0开放，1屏蔽
};

struct expos//FLAG和已下棋的数组中的结构体
{
	int x;
	int y;
	int flag;//存储颜色
	int index;//存储在FlagArray中的索引位
	int score;
};

int Board[19][19];//存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一
static Step moves;
// 双参数
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
int** CreatMap(void)//创建19*19的矩阵
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
void CopyMap(int** map)//拷贝地图信息到一个矩阵
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
int TSS_divide_map(int** map,int& count_pieces)//对地图进行划分，画出局部可行点压缩比较范围
{
	int count=0;
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			//是否该点有棋子
			if (map[i][j]==BLACK || map[i][j]==WHITE)
			{
				count_pieces++;
				//在该点棋子向8个方向衍生3格
				int x_left  =((i-3)>=0 )?i-3:0;
				int x_right =((i+3)<=18)?i+3:18;

				int y_top   =((j-3)>=0 )?j-3:0;
				int y_bottom=((j+3)<=18)?j+3:18;
				//纵向扫描
				for (int ii = x_left; ii <= x_right; ++ii)
				{
					if (map[ii][j]==EMPTY)
					{
						//对局部点做标记
						map[ii][j]=FLAG;
						count++;
					}
				}
				//横向扫描
				for (int jj = y_top; jj <= y_bottom; ++jj)
				{
					if (map[i][jj]==EMPTY)
					{
						//对局部点做标记
						map[i][jj]=FLAG;
						count++;
					}
				}
				//从左上到左下
				for (int ii = x_left , jj = y_top ;(ii<=x_right && jj <= y_bottom); ++ii,++jj)
				{
					if (map[ii][jj]==EMPTY)
					{
						//对局部点做标记
						map[ii][jj]=FLAG;
						count++;
					}
				}
				//从右上到右下
				for (int ii = x_left, jj = y_bottom;(ii<=x_right && jj>=y_top ); ++ii,--jj )
				{
					if (map[ii][jj]==EMPTY)
					{
						//对局部点做标记
						map[ii][jj]=FLAG;
						count++;
					}
				}
			}
		}
	}
	return count;
}
int divide_map(int** map)//对地图进行划分，画出局部可行点压缩比较范围
{
	int count=0;
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			//是否该点有棋子
			if (map[i][j]==BLACK || map[i][j]==WHITE)
			{
				//在该点棋子向8个方向衍生3格
				int x_left  =((i-3)>=0 )?i-3:0;
				int x_right =((i+3)<=18)?i+3:18;

				int y_top   =((j-3)>=0 )?j-3:0;
				int y_bottom=((j+3)<=18)?j+3:18;
				//纵向扫描
				for (int ii = x_left; ii <= x_right; ++ii)
				{
					if (map[ii][j]==EMPTY)
					{
						//对局部点做标记
						map[ii][j]=FLAG;
						count++;
					}
				}
				//横向扫描
				for (int jj = y_top; jj <= y_bottom; ++jj)
				{
					if (map[i][jj]==EMPTY)
					{
						//对局部点做标记
						map[i][jj]=FLAG;
						count++;
					}
				}
				//从左上到左下
				for (int ii = x_left , jj = y_top ;(ii<=x_right && jj <= y_bottom); ++ii,++jj)
				{
					if (map[ii][jj]==EMPTY)
					{
						//对局部点做标记
						map[ii][jj]=FLAG;
						count++;
					}
				}
				//从右上到右下
				for (int ii = x_left, jj = y_bottom;(ii<=x_right && jj>=y_top ); ++ii,--jj )
				{
					if (map[ii][jj]==EMPTY)
					{
						//对局部点做标记
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

	//棋形总数有10

	// 0:六连或长连
	// 1:活五
	// 2:眠五
	// 3:活四
	// 4:眠四
	// 5:活三
	// 6:眠三
	// 7:朦胧三
	// 8:活二
	// 9:眠二

	int* way=new int[7];
	for (int i = 0; i < 7; ++i)
	{
		way[i]=0;
	}

	//从当前点的四个方向扫描


	//从当前点横向扫描LEN长的点
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

	//从当前点纵向扫描LEN长的点
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = 0; j < wide; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = i; k < i+6 ; ++k)//可能有越界问题或者扫描方向不对的问题？
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

	//从当前点从左上到右下扫描LEN长的点
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

	//从当前点从右上到左下扫描LEN长的点
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

	// 0:六连或长连
	// 1:活五
	// 2:眠五
	// 3:活四
	// 4:眠四
	// 5:活三
	// 6:眠三
	// 7:朦胧三
	// 8:活二
	// 9:眠二
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
pos* TSS_creat_flag_array(int** map,const int high,const int wide,const int len)//建立扩展的标记flag点的数组
{
	//pos数组
	pos* FlagArray=new pos[len];
	//初始化pos数组
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
			if (map[i][j]==FLAG)//划分出来的FLAG点
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

Point* creat_flag_array(int** map,const int high,const int wide, int len)//建立扩展的标记flag点的数组
{
	len++;
	//pos数组
	Point* FlagArray=new Point[len];
	FlagArray[0].score=len;
	//初始化pos数组
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
			if (map[i][j]==FLAG)//划分出来的FLAG点
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

//全局算分
int CalScore(int** map,const int computerSide,int baseScore)
{
	int model = 0;
    if(baseScore>ALERTSCORE)
        model = 1;
    //初始化地图，数组
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
    //水平方向
    for(int i=0; i<19; i++)
    {
        for(int j=0; j<19-5; j++)
        {
			//直接计算取得index速度快
            int number = map[i][j]+map[i][j+1]+map[i][j+2]+map[i][j+3]+map[i][j+4]+map[i][j+5];
            //异常情况忽略
			if(number==0||(number>6 && number%7!=0))
                continue;
            if(number<7)//全黑
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
    // 竖直方向
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
    // 左上-右下
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
    // 右上-左下
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
    // 算分
    int score = 0;
    for(int i=1; i<7; i++)
    {
        score += myRoad[i]*myScore[model][i]-enemyRoad[i]*enemyScore[model][i];
    }
    // 还原
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
    // 水平两点
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
    // 竖直两点
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
    // 左上-右下
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
    // 右上-左下
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

    //cout<<"算分中"<<endl;
    // 算分
    int scoreme = 0;
	int scoreth = 0;
    for(int i=1; i<7; i++)
    {
        scoreme += myRoad[i]*myScore[model][i];
		scoreth += enemyRoad[i]*enemyScore[model][i];
    }
    // 还原
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
    // 水平两点
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
    //cout<<"水平两点计算完毕"<<endl;
    /*-----------------------*/
    // 竖直两点
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
    //cout<<"竖直两点计算完毕"<<endl;
    /*-----------------------*/
    // 左上-右下
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
    //cout<<"右下两点计算完毕"<<endl;
    /*-----------------------*/
    // 右上-左下
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
    //cout<<"左下两点计算完毕"<<endl;
    //cout<<"算分中"<<endl;
    // 算分
    int score = 0;
    for(int i=1; i<7; i++)
    {
        score += myRoad[i]*myScore[model][i]-enemyRoad[i]*enemyScore[model][i];
    }
    // 还原
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
    // 八个方向依次为 上，下，左，右，左上，左下，右上，右下
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
						// 路上不能有不同颜色的棋子
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

//alpah-beta树
int AlphaBetaTree(int** map,int depth,int alpha,int beta,int computerSide,int baseScore)
{
	int pp=connect6(map,19,19,computerSide);
	if(depth==0 ||pp==1 || pp==-1)//到达叶节点
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
			//如果游戏没有结束
			// return CalScore(map,computerSide,baseScore);
			return baseScore;
		}
	}
	else if(depth%2==0)//MAX
	{
		//选出较好的10个点
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


		//从中再选出评分高的10组点
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
				int tempScore=baseScore+b-a;   //获取目前局面评分
				int score=AlphaBetaTree(map,depth-1,alpha,beta,computerSide,tempScore);
				map[pointarray[i].x][pointarray[i].y]=EMPTY;
				map[pointarray[j].x][pointarray[j].y]=EMPTY;//还原落子
				if(score>alpha)
				{
					alpha=score;
					if(depth==DEEP)
					{
						//获取最佳点
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
		//选出较好的10个点
		Point* pointarray=GetBestPoint1(map,1-computerSide,100000);
		// Reversal(map);
		//从中再选出评分高的10组点
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
				int tempScore=baseScore-(b-a);   //获取目前局面评分
				int score=AlphaBetaTree(map,depth-1,alpha,beta,computerSide,tempScore);
				map[pointarray[i].x][pointarray[i].y]=EMPTY;
				map[pointarray[j].x][pointarray[j].y]=EMPTY;//还原落子
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
    // 两种棋子的分数
    //计算分数的方法：检查路的数量，检查的方向分为四个方向，分别是横向，纵向，左对角线，右对角线
	//每个方向从离所下棋子5个距离的点开始，沿着该方向检查6个点，观察有多少路，每检查一次向所下棋子前进一格作为新起点，一直前进到所下棋子处为止
	int score = 0;
    //棋形数目统计
    int pattern[7];
    // 初始化棋形数目为0
    memset(pattern, 0, sizeof(pattern));
    // 四个方向依次为 横向，纵向，左对角线，右对角线
    int dx[4] = {0, 1, 1, 1};
    int dy[4] = {1, 0, 1, -1};
	//开始检查
      int tempDirection = direction; //决定目前检查的方向

	  int xx = row -  dx[tempDirection]*5; //检查起始点
	  int yy = column - dy[tempDirection] * 5; //检查起始点
	  int maxRoad = 0; //找到该方向上包括中心点的最大的路，据此来评分
	  for(int offset = 0; offset < 6; offset ++){
	  	if(offset != 0){
          xx = xx + dx[tempDirection]; //起始点在每次检查一遍后移一位
          yy = yy + dy[tempDirection]; //起始点在每次检查一遍后移一位
          }
          if(xx < 0 || xx > 18 || yy < 0 || yy > 18){
          	continue;
		  }
          int check_x = xx;  //检查的点
          int check_y = yy;  //检查的点
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
    int count = 0; //计算FlagArray数组中flag值为0以及mask值为0的元素的个数
	expos** array=new expos*[5];
	for(int i = 0; i < 5; i ++){
		array[i] = new expos[2];
	}
			for (int _i = 0; _i < count_flag; ++_i)
			{
				//选取第一个自己下的点
				if (FlagArray[_i].flag==FLAG )
				{
					temp_map[FlagArray[_i].x][FlagArray[_i].y]=computerSide;
					FlagArray[_i].flag=computerSide;
					int row1 = FlagArray[_i].x;
					int column1 = FlagArray[_i].y;
					for (int _j = _i+1; _j < count_flag; ++_j)
					{
						//选取第二个自己下的点
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
	//开始检查
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
    // 两种棋子的分数
    int myScore, enemyScore;
    myScore = enemyScore = 0;
    //棋形数目统计
    int myPartten[7];
    int enemyPartten[7];
    // 初始化棋形数目为0
    memset(enemyPartten, 0, sizeof(enemyPartten));
    memset(myPartten, 0, sizeof(myPartten));
    // 八个方向依次为 上，下，左，右，左上，左下，右上，右下
    int dx[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[8] = {-1, 1, 0, 0, -1, 1, -1, 1};

    // 遍历棋子
    for (int pos = 0; pos<_count; pos++) {
        // 该点棋子颜色
        int color = past[pos].flag;
        // 统计全局分数
        // 中心坐标
        int x = past[pos].x;
        int y = past[pos].y;
        // 遍历八个方向 xx,yy 新的坐标点，cnt ==>路数
        int xx, yy, cnt;
        for (int i = 0; i < 8; i++) {
            // 初始化
            cnt = 0;
            // 延展长度LEN
			xx = x;
			yy = y;
            for (int j = 0; j < 6; j++) {
            	if(j != 0){
                xx = xx + dx[i];
                yy = yy + dy[i];
                }
                // 越界，直接跳过
                if (xx > 18 || xx < 0 || yy > 18 || yy < 0) {
                    break;
                }
                // 路上不能有不同颜色的棋子
                if (map[xx][yy] == 1 - color) {
                	cnt = 0;
                    break;
                }
                // 统计路数
                if (map[xx][yy] == color) {
                    cnt++;

                }
            }

            // 更新路数
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
    // 统计全局评分
}
int firstXX;
int firstYY;
int secondXX;
int secondYY;
int TSStree(int** same_map,pos* FlagArray,int count_flag,int count_pieces,int depth,int computerSide,Step temppoint)
{
	pos *past = define_past(same_map, 19, 19, count_pieces);

	int nowResult = overTSS(same_map, 19, 19, computerSide, past, count_pieces,depth);
	if ((depth==0 || nowResult == 1 ) && depth != TSS_DEEP)//到达最底层，对棋盘评分
	{
		//将有棋子的点放入线性数组里面
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
		//分割FlagArray数组,将里面的点逐个评分选出己方评分最大的10个点的数组并排序
		//如果不够10个那么直接将不足的点全部放入数组中返回，并把isenough=0;
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

	//从当前点的四个方向扫描


	//从当前点横向扫描LEN长的点
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

	//从当前点纵向扫描LEN长的点
	for (int i = 0; i <= high-6; ++i)
	{
		for (int j = 0; j < wide; ++j)
		{
		    int continue_num = 0;
		    int my_piece_num = 0;

			for (int k = i; k < i+6 ; ++k)//可能有越界问题或者扫描方向不对的问题？
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

	//从当前点从左上到右下扫描LEN长的点
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

	//从当前点从右上到左下扫描LEN长的点
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
		//对棋形评分
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
	Step step;//临时步结构
	char message[256];//通信消息缓冲
    int computerSide;//己方执棋颜色
    int start=0;//对局开始标记
	srand(int(time(0)));
	//初始化地图(创建，拷贝)
    int** map=CreatMap();
    int** TSS_map = CreatMap();
    int** temp_map = CreatMap();
	//此处放置初始化代码
    //...
	while (1)	//程序主循环
	{
		fflush(stdout);//不要删除此语句，否则程序会出问题
		scanf("%s", message);//获取平台命令消息
        //分析命令
		if (strcmp(message, "name?") == 0)//向对战平台发送队名
		{
			fflush(stdin);
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
			/*******/		printf("name CM6\n");		/**只修改令狐冲，不要删除name空格****/
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
		}
		else if (strcmp(message, "new") == 0)//建立新棋局
		{
			int i, j;
			scanf("%s", message);//获取己方执棋颜色

			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //执黑
			else  computerSide = WHITE;   //执白

			for (i = 0; i<19; ++i)   //初始化棋局
				for (j = 0; j<19; ++j)
					Board[i][j] = EMPTY;
			// baseScore=0;//新开棋局初始化基本评分为0
			start = 1;

			if (computerSide == BLACK)
			{
				/**********生成第一手着法，并保存在step结构中，落子坐标为(step.first.x,step.first.y)**********/
				/****************************在下方填充代码，并替换我的示例代码******************************/


				step.first.x = 9;
				step.first.y = 9;


				/******************************在上面填充第一步行棋代码*******************************************/

				Board[step.first.x][step.first.y] = computerSide;//处理己方行棋
				printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//输出着法
			}
		}
		else if(strcmp(message,"move")==0)//行棋,本程序核心
		{
			scanf("%s", message);//获取对手行棋着法
            fflush(stdin);
			step.first.x=message[0]-'A';		step.first.y=message[1]-'A';
			step.second.x=message[2]-'A';		step.second.y=message[3]-'A';
            //处理对手行棋
			Board[step.first.y][step.first.x] = 1 - computerSide;
			if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.y][step.second.x] = 1 - computerSide;

            /**********************************************************************************************************/
			/***生成落子的坐标，保存在step结构中，第1子下在(step.first.x,step.first.y)，第2子下在(step.first.x,step.first.y)*****/
			/**************************************在下方填充代码，并替换我的示例代码*****************************************/
			//初始化结构
				//初始化行走的步结构
				moves.first.x=-1;
				moves.first.y=-1;
				moves.second.x=-1;
				moves.second.y=-1;

				//InitMap(map,0);
				CopyMap(map);
				//限制落子点
			//while循环开始之前初始化hash表(alpha-beta hash,TSS hash)
			//
			//


			//此处是TTS决策代码
				//如果TTS有决策那么执行其决策
           if(antiAttack(map, 19, 19, computerSide)){
           	CopyMap(temp_map);
           	int count_pieces = 0;
			int count_flag  =TSS_divide_map(temp_map,count_pieces);//返回值：对多少个点做了flag标记
			pos* FlagArray=TSS_creat_flag_array(temp_map,19,19,count_flag);
				int max_score = -100000;
				for (int _i = 0; _i < count_flag; ++_i)
			{
				//选取第一个自己下的点
				if (FlagArray[_i].flag==FLAG )
				{
					temp_map[FlagArray[_i].x][FlagArray[_i].y]=computerSide;
					FlagArray[_i].flag=computerSide;
					for (int _j = _i+1; _j < count_flag; ++_j)
					{
						//选取第二个自己下的点
						if (FlagArray[_j].flag==FLAG )
						{
							temp_map[FlagArray[_j].x][FlagArray[_j].y]=computerSide;
							FlagArray[_j].flag=computerSide;
							int score_me =fakeGrade(temp_map,19,19,computerSide,0);//对自己的棋形评分
							int score_amy=fakeGrade(temp_map,19,19,1 - computerSide,1);//对敌人的棋形评分
							int score    = score_me-score_amy;
							//自己评分大于敌人评分，且两者差值较大，则选则该点
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
			int count_pieces=0;//记录下了棋点的个数
			int count_flag  = TSS_divide_map(TSS_map,count_pieces);//返回值：对多少个点做了flag标记
			pos* FlagArray;
		int result = TSStree(TSS_map, FlagArray, count_flag, count_pieces, 6, computerSide, temp_p);
		//分割FlagArray数组,将里面的点逐个评分选出己方评分最大的10个点的数组并排序
		//如果不够10个那么直接将不足的点全部放入数组中返回，并把isenough=0;
		if(result == 1 && !antiAttack(map, 19, 19, 1 - computerSide)){
			moves.first.x = firstXX;
			moves.first.y = firstYY;
			moves.second.x = secondXX;
			moves.second.y = secondYY;
		}
		else{
				//否则采用普通的决策树
					//计算一个全局baseScore
					int baseScore=CalScore(map,computerSide,0);
					//传入决策树
						//如果baseScore>alertscore
					int score=AlphaBetaTree(map,DEEP,-10000000,10000000,computerSide,baseScore);
						// 防止没有决策结果时候，采用随机方法
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
			//返回极大极小评分坐标
			//生成第1子落子位置step.first.x和step.first.y
			step.first.x =moves.first.x;
			step.first.y =moves.first.y;
			Board[step.first.x][step.first.y] = computerSide;
			//生成第2子落子位置step.second.x和step.second.y
			step.second.x =moves.second.x;
			step.second.y =moves.second.y;
			Board[step.second.x][step.second.y] = computerSide;
			/*****************************************在上面填充代码******************************************************/
			/**********************************************************************************************************/

			printf("move %c%c%c%c\n",step.first.y+'A',step.first.x+'A',step.second.y+'A',step.second.x+'A');//输出着法
		}
        else if (strcmp(message, "error") == 0)//着法错误
        {
            fflush(stdin);
        }
		else if (strcmp(message, "end") == 0)//对局结束
		{
            fflush(stdin);
			start = 0;
		}
		else if (strcmp(message, "quit") == 0)//退出引擎
		{
            fflush(stdin);
			printf("Quit!\n");
			break;
		}
	}
    DeleteMap(map);

	return 0;
}
