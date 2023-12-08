#include <iostream>
#include<array>
#include<ncurses.h>
using namespace std;


const int MAP_Y = 10;
const int MAP_X = 10;
array<array<int,10>,10> sapper_map;
array<array<int,10>,10> open_cell;


void bomb_map(int bomb)
{
 int rand_y, rand_x;
 for(int i = 0; i < bomb; i++)
 {
  do
  {
   rand_y = rand() % 10;
   rand_x = rand() % 10;
  }
  while(sapper_map.at(rand_y).at(rand_x) == 1);
  
  
  sapper_map.at(rand_y).at(rand_x) = 1;
 }
}

void fill_map()
{
 for(int i = 0; i < MAP_Y; i++)
 {
  for(int j = 0; j < MAP_X; j++)
  {
   sapper_map.at(i).at(j) = 0;
  }
 }
}

int array_exam(int y, int x)
{
 if ((y < 0 || x < 0) || (y >= MAP_Y || x >= MAP_X))
 return 0;
 else
 return sapper_map.at(y).at(x);
}

int bomb_count(int y, int x)
{
 
return array_exam(y+1,x) +
       array_exam(y+1,x-1) +
       array_exam(y,x-1) +
       array_exam(y-1,x-1) +
       array_exam(y-1,x) +
       array_exam(y-1,x+1) +
       array_exam(y,x+1) +
       array_exam(y+1,x+1);
 
}

void print_map(int bomb)
{ 
 fill_map();
 bomb_map(bomb);
 for(int i = 0; i < MAP_Y; i++)
 {
  for(int j = 0; j < MAP_X; j++)
  {
     cout<<sapper_map.at(i).at(j);
  }
  cout<<endl;
 }
}

void open (int y, int x)
{
 
 
 if((y < 0 || x < 0) || (y >= MAP_Y || x >= MAP_X))
  return;
    
 if(open_cell.at(y).at(x) >= 1)
  return;
  
 open_cell.at(y).at(x) = 1;
 attron(COLOR_PAIR(2));
 
 mvprintw(y, x,"%d", bomb_count(y,x));
 if(bomb_count(y,x) == 0)
  {
    open(y-1,x-1);
   open(y,x-1);
   open(y+1,x+1);
   open(y-1,x);
   open(y+1,x);
   open(y+1,x-1);
   open(y,x+1);
   open(y-1,x+1);
   
  }
  attroff(COLOR_PAIR(2));

}


void nc_print(int bomb)
{
 fill_map();
 bomb_map(bomb);
 
 for(int i = 0; i < MAP_Y; i++)
 {
  for(int j = 0; j < MAP_X; j++)
  {
   mvprintw(i, j, ".");
  }
 }
 
}
 
bool win_lose()
{
 for(int i = 0; i < MAP_Y; i++)
 {
  for(int j = 0; j < MAP_X; j++)
  {
   if(sapper_map.at(i).at(j) == 0 && open_cell.at(i).at(j) == 0)
    return 0;
          
  }
 }
 return 1;
}

void open_bomb()
{
 attron(COLOR_PAIR(1));
     for(int i = 0; i < MAP_Y; i++)
     {
      for(int j = 0; j < MAP_X; j++)
      {
       if(sapper_map.at(i).at(j) == 1)
        mvprintw(i, j, "*");
      }
     }
     
 attroff(COLOR_PAIR(1));     
}

void screen(int y, int x)
{
 start_color();
 init_pair(1, COLOR_RED, COLOR_BLACK);
 init_pair(2, COLOR_BLUE, COLOR_BLACK);

 noecho();
 
 nc_print(3);
 for( ; ;)
 {
 int cursor = getch();
 if (cursor == 'a')
  {
     x--;
     if(x < 0)
      x = 0;
  }     
 else if (cursor == 'w')
  {     
     y--;
     if(y < 0)
      y = 0;
  }     
 else if (cursor == 'd')
  {     
     x++;
     if(x >= MAP_X)
      x = MAP_X-1;
  }     
 else if (cursor == 's')
  {      
     y++;
     if(y >= MAP_Y)
      y = MAP_Y-1;
  }         
 move(y,x);
 if (cursor == ' ')
  {
     mvprintw(y,x, "%d", bomb_count(y,x));
     
     if (sapper_map.at(y).at(x) > 0)
      open_bomb();
     
     else 
      open(y,x); 
  }  
 if(win_lose() == 1)
  mvprintw(10, 10, "You win");     
 }                
}


int main()
{
 initscr();
 //nc_print(10);
 srand(time(NULL));
 //print_map(20);
 
 screen(0,0);
 getch();
 endwin();
 return 0;
}
