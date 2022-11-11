#include <iostream>
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <cstring>

using namespace std;

const int MAX = 1e3 + 5;
int prop[MAX][MAX];
//This 2-dimension array is used to store the randomly generated properties on the map that can be gained by the player.
int bombNum = 0,bombUsed = 0;
//bombNum is used to record the new planted bombs and bombUsed is to record the already exploded bombs.
//Both of the values are used to simulate the process of the explosion.

struct player{
    int x, y;        //x,y represent the present position of the player.
    int totalNum;    //totalNum represents the number of all the bombs the player can use (which can be increased by gaining properties).
    int settledNum;  //settledNum represents the number of the bombs that have been planted on the map right now.
    int bombTime;    //bombTime represents the time that the bombs need to explode (can be changed by gaining props).
    int health;      //health represents the present health value of the player.
    int powerLv;     //powerLv represents the level of the power of the bombs (can be changed by gaining props).
}
player[MAX];

struct explosion{
    int x, y;        //record the position where the explosion happens
    int time;        //record the time of the explosion
    int maker;       //record the player who made this explosion
}
explosion[MAX];
//This test function is used to test whether the present position is occupied by an explosion.

bool test(int x,int y)
{
    for (int i=bombUsed; i<=bombNum && bombNum!=0;i++)
    {
        if (explosion[i].x ==x && explosion[i].y ==y)
        {
            return true;
        }
    }
    return false;
}

void readinrw(int n,int *row,int *column)
{
	FILE *fp;
	char filename[6]="0.txt";
	switch(n)
	{
		case 1:
			{
				filename[0]='1';
				break;
			}
		case 2:
			{
				filename[0]='2';
				break;
		 	}
		case 3:
			{
				filename[0]='3';
				break;
			}
	}//Choose different maps according to the players' choices
	fp = fopen(filename,"r");
	char s[100];
	int len;
	if (fp==NULL)
	{
	   printf("Can not open!\n");
       exit(EXIT_FAILURE);	
	}	
	fgets(s,100,fp);
	len = strlen(s);
	*row=*row+1;
	for (int i=0;i<len;i++)
	{
		if(s[i]!=' ')
		{
			*column=*column+1;//as one space corresponds to one number, we can count number in this way
		}
	}
	*column=*column-1;
	while (fgets(s,100,fp)!=NULL)
	{
		*row=*row+1;
	}//if one line points to NULL, it is when the file reaches its end
    fclose(fp);
}

void readinmap(int n,int row,int column,int (*wall)[100],int (*block)[100],struct player *player)
{
	FILE *fp;
	char filename[6]="0.txt";
	switch(n)
	{
		case 1:
			{
				filename[0]='1';
				break;
			}
		case 2:
			{
				filename[0]='2';
				break;
		 	}
		case 3:
			{
				filename[0]='3';
				break;
			}
	}//Choose different maps according to the players' choices
	fp = fopen(filename,"r");
    if (fp==NULL)
	{
	   //printf("Can not open!\n");
        cout<<"Can not open!"<<endl;
       exit(EXIT_FAILURE);	
	}
	
	int item=0;
	
    for (int i=1;i<=row;i++)
    {
    	for (int j=1;j<=column;j++)
    	{
    		fscanf(fp,"%d",&item);
    		//printf("%d",item);
    		if (item==1)
    		{
    			wall[i][j]=1;
    			block[i][j]=0;
			}
			else if (item==2)
			{
				block[i][j]=1;
				wall[i][j]=0;
			}
			else if (item==0)
			{
				wall[i][j]=0;
				block[i][j]=0;
			}
			else if (item==3)
			{
				player[1].x=i;
				player[1].y=j; 
				wall[i][j]=0;
				block[i][j]=0;
			}
			else if (item==4)
			{
				player[2].x=i;
				player[2].y=j;
				wall[i][j]=0;
				block[i][j]=0;
			}
		}
	}
	fclose(fp);
}

int main()
{
    srand((unsigned) time(0));
    //int readItems;
    //int item;
    //char s[100];
    int row=0,column=0;
    int level;
    //scanf("%d",&level);
    cin>>level;
    readinrw(level,&row,&column);
    //initialization: both of two players have 20 points of health, 1 power level, 1 bomb, 2000ms bomb time and corresponding position.
    /*int row=0,column=0;
    char s[100];
    int len;
    
	FILE *fp;
	fp = fopen("map.txt","r");
	if (fp==NULL)
	{
	   printf("Can not open!\n");
       exit(EXIT_FAILURE);	
	}	
	fgets(s,100,fp);
	len = strlen(s);
	row++;
	for (int i=0;i<len;i++)
	{
		if(s[i]!=' ')
		{
			column++;
		}
	}
	column--;
	while (fgets(s,100,fp)!=NULL)
	{
		row++;
	}
    fclose(fp);*/
	int wall[row+1][100];     //In this wall 2-dimension array, 1 represent the unbreakable wall in the map.
	int block[row+1][100];    //In the block 2-dimension array, 1 represent the breakable blocks that can be broken by the bombs.
	
	readinmap(level,row,column,wall,block,player);
	
	//printf("%d\n",column);
	
	//These code blocks are to introduce a map from a txt file where 1 represent the wall, 0 for space,
	//2 for blocks, 3 for where player1 starts and 4 for player 2.
    player[1].health = 20;
    player[1].powerLv = 1;
    player[1].totalNum = 1;
    //player[1].x = 13;
    //player[1].y = 2;
    player[1].bombTime = 2000;
    player[2].health = 20;
    player[2].powerLv = 1;
    player[2].totalNum = 1;
    //player[2].x = 2;
    //player[2].y = 13;
    player[2].bombTime = 2000;
    //keep printing the present map as well as all the information with corresponding string.
    while (player[1].health > 0 && player[2].health > 0)
    {
        for (int i = 1; i <= row; i++)
        {
            for (int j = 1; j <= column; j++)
            {
                if (wall[i][j] == 1)
                {
                    cout<<"#";
                }
                else if (block[i][j] == 1)
                {
                    cout<<"@";
                }
                else if (player[1].x == i && player[1].y == j)
                {
                    cout<<"1";
                }
                else if (player[2].x == i && player[2].y == j)
                {
                    cout<<"2";
                }
                else if (test(i, j))
                {
                    cout<<"*";
                }
                else if (prop[i][j] == 1)
                {
                    cout<<"a";
                }
                else if (prop[i][j] == 2)
                {
                    cout<<"b";
                }
                else if (prop[i][j] == 3)
                {
                    cout<<"c";
                }
                else if (prop[i][j] == 4)
                {
                    cout<<"d";
                }
                else
                    {
                    cout<<" ";
                }
            }
            puts("");
        }
        /*printf("Player1 health: %d  ||",player[1].health);
        printf("Available bombs: %d   ||",player[1].totalNum-player[1].settledNum);
        printf("bomb power level: %d   ||",player[1].powerLv);
        printf("time to explode: %dms\n",player[1].bombTime);
        printf("Player2 health: %d   ||",player[2].health);
        printf("available bombs: %d   ||",player[2].totalNum-player[2].settledNum);
        printf("bomb power level: %d   ||",player[2].powerLv);
        printf("time to explode: %dms\n",player[2].bombTime);*/
        cout<<"Player1 health: "<<player[1].health<<"  ||";
        cout<<"Available bombs: "<<player[1].totalNum-player[1].settledNum<<"  ||";
        cout<<"bomb power level: "<<player[1].powerLv<<"   ||";
        printf("time to explode: %dms\n",player[1].bombTime);
        cout<<"Player2 health: "<<player[2].health<<"  ||";
        cout<<"Available bombs: "<<player[2].totalNum-player[2].settledNum<<"  ||";
        cout<<"bomb power level: "<<player[2].powerLv<<"   ||";
        printf("time to explode: %dms\n",player[2].bombTime);
        //This part is to print all the information of the players in the game.
        if (kbhit())
        {
            char input = getch();
            if (input == -32)
            {
                input = getch();
                if (input == 72 && block[player[1].x - 1][player[1].y] == 0 && wall[player[1].x - 1][player[1].y] == 0
                    && !test(player[1].x - 1, player[1].y))
                {
                    player[1].x--;
                }
                else if (input == 80 && block[player[1].x + 1][player[1].y] == 0
                           && wall[player[1].x + 1][player[1].y] == 0 && !test(player[1].x + 1, player[1].y))
                {
                    player[1].x++;
                }
                else if (input == 75 && block[player[1].x][player[1].y - 1] == 0
                           && wall[player[1].x][player[1].y - 1] == 0 && !test(player[1].x, player[1].y - 1))
                {
                    player[1].y--;
                }
                else if (input == 77 && block[player[1].x][player[1].y + 1] == 0
                           && wall[player[1].x][player[1].y + 1] == 0 && !test(player[1].x, player[1].y + 1))
                {
                    player[1].y++;
                }
            }
            if (input == 'w' && block[player[2].x - 1][player[2].y] == 0 && wall[player[2].x - 1][player[2].y] == 0
                && !test(player[2].x - 1, player[2].y))
            {
                player[2].x--;
            }
            else if (input == 's' && block[player[2].x + 1][player[2].y] == 0 &&
                       wall[player[2].x + 1][player[2].y] == 0
                       && !test(player[2].x, player[2].y + 1))
            {
                player[2].x++;
            }
            else if (input == 'a' && block[player[2].x][player[2].y - 1] == 0 &&
                       wall[player[2].x][player[2].y - 1] == 0
                       && !test(player[2].x, player[2].y - 1))
            {
                player[2].y--;
            }
            else if (input == 'd' && block[player[2].x][player[2].y + 1] == 0 &&
                       wall[player[2].x][player[2].y + 1] == 0
                       && !test(player[2].x, player[2].y + 1))
            {
                player[2].y++;
            }
            //all the codes above are to move the player by identifying the input of the keyboard.
            if (input == 13 && player[1].settledNum < player[1].totalNum)
            {
                bombNum++;
                explosion[bombNum].x = player[1].x;
                explosion[bombNum].y = player[1].y;
                explosion[bombNum].time = clock() + player[1].bombTime;
                explosion[bombNum].maker = 1;
                player[1].settledNum++;
            }
            if (input == ' ' && player[2].settledNum < player[2].totalNum)
            {
                bombNum++;
                explosion[bombNum].x = player[2].x;
                explosion[bombNum].y = player[2].y;
                explosion[bombNum].time = clock() + player[2].bombTime;
                explosion[bombNum].maker = 2;
                player[2].settledNum++;
            }
        }
            //These two code blocks are to settle the bombs by typing the enter and space key.
            for (int i = bombUsed; i <= bombNum; i++)
            {
                if (clock() >= explosion[i].time)
                {
                    bombUsed++;
                    player[explosion[i].maker].settledNum--;
                    for (int j = explosion[i].x; j <= explosion[i].x + player[explosion[i].maker].powerLv; j++)
                    {
                        prop[j][explosion[i].y] = 0;
                        if (wall[j][explosion[i].y] == 1)
                        {                                    //if the explosion meet the block,it will immediately
                            break;                           //stop in this specific direction.
                        }
                        if (block[j][explosion[i].y] == 1)
                        {
                            block[j][explosion[i].y] = 0;
                            prop[j][explosion[i].y] = rand() % 5;  //randomly decide what the properties will be
                            break;                                 // after the block is broken.
                        }
                        if (player[1].x == j && player[1].y == explosion[i].y)
                        {
                            player[1].health -= player[explosion[i].maker].powerLv;
                        }                                                          //if there is a player on the way of
                        if (player[2].x == j && player[2].y == explosion[i].y)    //the explosion, it will lose health.
                        {
                            player[2].health -= player[explosion[i].maker].powerLv;
                        }
                    }
                    for (int j = explosion[i].x; j >= explosion[i].x - player[explosion[i].maker].powerLv; j--)
                    {
                        prop[j][explosion[i].y] = 0;
                        if (wall[j][explosion[i].y] == 1)
                        {
                            break;
                        }
                        if (block[j][explosion[i].y] == 1)
                        {
                            block[j][explosion[i].y] = 0;
                            prop[j][explosion[i].y] = rand() % 5;
                            break;
                        }
                        if (player[1].x == j && player[1].y == explosion[i].y)
                        {
                            player[1].health -= player[explosion[i].maker].powerLv;
                        }
                        if (player[2].x == j && player[2].y == explosion[i].y)
                        {
                            player[2].health -= player[explosion[i].maker].powerLv;
                        }
                    }
                    for (int j = explosion[i].y; j <= explosion[i].y + player[explosion[i].maker].powerLv; j++)
                    {
                        prop[explosion[i].x][j] = 0;
                        if (wall[explosion[i].x][j] == 1)
                        {
                            break;
                        }
                        if (block[explosion[i].x][j] == 1)
                        {
                            block[explosion[i].x][j] = 0;
                            prop[explosion[i].x][j] = rand() % 5;
                            break;
                        }
                        if (player[1].x == explosion[i].x && player[1].y == j)
                        {
                            player[1].health -= player[explosion[i].maker].powerLv;
                        }
                        if (player[2].x == explosion[i].x && player[2].y == j)
                        {
                            player[2].health -= player[explosion[i].maker].powerLv;
                        }
                    }
                    for (int j = explosion[i].y; j >= explosion[i].y - player[explosion[i].maker].powerLv; j--)
                    {
                        prop[explosion[i].x][j] = 0;
                        if (wall[explosion[i].x][j] == 1)
                        {
                            break;
                        }
                        if (block[explosion[i].x][j] == 1)
                        {
                            block[explosion[i].x][j] = 0;
                            prop[explosion[i].x][j] = rand() % 5;
                            break;
                        }
                        if (player[1].x == explosion[i].x && player[1].y == j)
                        {
                            player[1].health -= player[explosion[i].maker].powerLv;
                        }
                        if (player[2].x == explosion[i].x && player[2].y == j)
                        {
                            player[2].health -= player[explosion[i].maker].powerLv;
                        }
                    }
                }
            }
            //These four code block is to settle the effect of the explosion of the bomb in four different directions.
            //If there is a block on the way of the explosion, it will break. If there is a player, it will lose health
            //with corresponding power level of the settled bomb.
            if (prop[player[1].x][player[1].y] == 1)
            {    //if the prop is 1, then the player will have one more bomb.
                prop[player[1].x][player[1].y] = 0;
                player[1].totalNum++;
            }
            if (prop[player[1].x][player[1].y] == 2) //if the prop is 2, then the player will increase the power level
            {                                        //of all its bombs.(which means the bomb can cause 1 more damage.)
                prop[player[1].x][player[1].y] = 0;
                player[1].powerLv++;
            }
            if (prop[player[1].x][player[1].y] ==3) //if the prop is 3, then the player will decrease the time that the
            {                                        //bombs need to explode.
                prop[player[1].x][player[1].y] = 0;
                player[1].bombTime -= player[1].bombTime >= 500 ? 150 : 0;
            }
            if (prop[player[1].x][player[1].y] == 4) //if the prop is 4, then the player will increase its health points
            {                                        //by 5.
                prop[player[1].x][player[1].y] = 0;
                player[1].health += 5;
            }
            if (prop[player[2].x][player[2].y] == 1)
            {
                prop[player[2].x][player[2].y] = 0;
                player[2].totalNum++;
            }
            if (prop[player[2].x][player[2].y] == 2)
            {
                prop[player[2].x][player[2].y] = 0;
                player[2].powerLv++;
            }
            if (prop[player[2].x][player[2].y] == 3)
            {
                prop[player[2].x][player[2].y] = 0;
                player[2].bombTime -= player[2].bombTime >= 500 ? 150 : 0;
            }
            if (prop[player[2].x][player[2].y] == 4)
            {
                prop[player[2].x][player[2].y] = 0;
                player[2].health += 5;
            }
            Sleep(5);
            system("cls");                  //keep refreshing to display latest information
        }
    if((player[1].health || player[2].health ) == 0)
    {
        //printf("You all died!\n");
        cout<<"You all died!"<<endl;
        while(true)
        {
            system("pause");
        }
        return 0;
    }
    if(player[1].health > 0)
    {
        //printf("Player 2 died! Player 1 win!\n");
        cout<<"Player 2 died! Player 1 win!"<<endl;
        while(true)
        {
            system("pause");
        }
        return 0;
    }
    else
        {
        //printf("Player1 died! Player 2 win!\n");
        cout<<"Player 1 died! Player 2 win!"<<endl;
        while (true)
        {
            system("pause");
        }
        return 0;
    }
}
