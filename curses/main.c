#include <curses.h>
#include <stdlib.h>
WINDOW *create_newwin(int height, int width, int starty, int startx,char* label);
void destroy_win(WINDOW *local_win);

char* create_text_buffer(void){
  char* buffer;
  buffer = malloc(100);
  return buffer;
}
char* update_text_buffer(char* ptr,int size){
  char* buffer;

  return buffer;
}

int main(int argc, char *argv[])
{       WINDOW *text_win,*reg_win, *flags_win, *memory_win;

        int ch;
        initscr();                      /* Start curses mode            */
        cbreak();                       /* Line buffering disabled, Pass on
                                         * everty thing to me           */
        keypad(stdscr, TRUE);           /* I need that nifty F1         */
        int text_height = 33;
        int text_width = 40;
				int sides_height = 10;
        int sides_width = 40;

        char *text_buffer;


        refresh();
        text_win = create_newwin(text_height, text_width, 2, 0,"Assembly");//(height, width, starty, startx);
				flags_win = create_newwin(3, sides_width, 2, text_width+1,"Flags");
				reg_win== create_newwin(10, sides_width, 5, text_width+1,"Registers");
				memory_win = create_newwin(20, sides_width, 15, text_width+1,"Memory");
				refresh();
				// int i=100;
        // int curr_y,curr_x;
        // wmove(text_win,1,1);
				// while(i>0){
        //   //wscrl(text_win, 1);
        //   getyx(text_win,curr_y,curr_x);
        //   wmove(text_win,++curr_y,2);
        //   wprintw(text_win,"tex t %d",i);
				// 	wrefresh(text_win);
        //   refresh();
				// 	i--;
				// }
        // while((ch = getch()) != KEY_F(1))
        // {       switch(ch)
        //         {       case KEY_LEFT:
        //                         destroy_win(my_win);
        //                         my_win = create_newwin(height, width, starty,--startx);
        //                         break;
        //                 case KEY_RIGHT:
        //                         destroy_win(my_win);
        //                         my_win = create_newwin(height, width, starty,++startx);
        //                         break;
        //                 case KEY_UP:
        //                         destroy_win(my_win);
        //                         my_win = create_newwin(height, width, --starty,startx);
        //                         break;
        //                 case KEY_DOWN:
        //                         destroy_win(my_win);
        //                         my_win = create_newwin(height, width, ++starty,startx);
        //                         break;
        //         }
        // }
				getch();
        endwin();                       /* End curses mode                */
        return 0;
}
WINDOW *create_newwin(int height, int width, int starty, int startx,char* label)
{       WINDOW *local_win;
        local_win = newwin(height, width, starty, startx);
        box(local_win, 0 , 0);          /* 0, 0 gives default characters
                                         * for the vertical and horizontal
                                         * lines                        */
				mvwprintw(local_win,0,1,label);
        wrefresh(local_win);            /* Show that box                */
        return local_win;
}
void destroy_win(WINDOW *local_win)
{
        /* box(local_win, ' ', ' '); : This won't produce the desired
         * result of erasing the window. It will leave it's four corners
         * and so an ugly remnant of window.
         */
        wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
        /* The parameters taken are
         * 1. win: the window on which to operate
         * 2. ls: character to be used for the left side of the window
         * 3. rs: character to be used for the right side of the window
         * 4. ts: character to be used for the top side of the window
         * 5. bs: character to be used for the bottom side of the window
         * 6. tl: character to be used for the top left corner of the window
         * 7. tr: character to be used for the top right corner of the window
         * 8. bl: character to be used for the bottom left corner of the window
         * 9. br: character to be used for the bottom right corner of the window
         */
        wrefresh(local_win);
        delwin(local_win);
}
