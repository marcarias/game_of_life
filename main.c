/*
GAME OF LIFE

To read a small explanation of how the program works, read the code profiling document.

The code has been dividied into three main parts:
- Setup & Auxiliar functions
  + get_int(): Function used to a number from the user input                           CODER 1
  + initall(): Initiallization function. Used to setup ncurses and the first screen.   CODER 1
  + enditall(): Called to end the program.                                             CODER 1
  + init(): Function used to select the mode and generate the first generation         CODER 2
- Computation of next generation
  + update(): Function used to compute the next generation                             CODER 1
- Visualization
  + Doesn't have a function itself, used inside other functions.                       CODER 3

The set CODER was the one responsible for each section, despite that, all coders supervised
and helped in all sections of the code
*/

/* USED LIBRARIES */
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <inttypes.h>

/* FUNCTION DECLARATION */
int initall(void);
int enditall(void);
int ** init(int,int,int);
void display(int **);
void update(int **);
int get_int(void);

/* GLOBAL VARIABLES INITIALIZATION */
int size_row = 10;     // To select dimensions of the game
int size_col = 10;
int approach;          // To select game mode
WINDOW *win;           // To create the window to project the game
char *filename;        // The name of the file that will be read for the game (if necessary)

/* GET_INT(): To get the number inputs with the keyboard */
int get_int(void)
{
  char str[10];                                       // Stores the string the user sets
  str[0] = mvgetch(18,60);                            // Gets the first character and moves the cursor for propper visualization
  for (int i = 1; str[i - 1] != '\n' && i < 9; ++i)   // Get the other characters (up to 10 digit number), until ENTER is pressed
      str[i] = getch();
  str[9] = '\0';

  char *endp;
  int n = strtoimax(str, &endp, 10);                  // Transform str to integer */

  return n;
}

/* INITALL(): Function to setup ncurses and the first game screen */
int initall(void)
{
	initscr();			                               // Start curses mode

  win = newwin(23, 100, 2,2);                    // Generate first window

  start_color();                                 // Initiallizes the use of color in ncurses
  init_color(COLOR_BLACK, 0, 0, 0);              // Sets the RGB Values for the used colors
  init_color(8, 1000, 1000, 1000);
  init_color(9, 1000, 750, 750);
  init_color(10, 1000, 500, 500);
  init_color(11, 1000, 250, 250);
  init_color(12, 1000, 0, 0);
  init_pair(1, COLOR_GREEN, COLOR_BLACK);       // Initiallizes pair of colors for ncurses
  init_pair(6, 8, COLOR_BLACK);
  init_pair(5, 9, COLOR_BLACK);
  init_pair(4, 10, COLOR_BLACK);
  init_pair(3, 11, COLOR_BLACK);
  init_pair(2, 12, COLOR_BLACK);
  refresh();

  box(win, 0, 0);                              // Set a box around the window

  wattron(win,COLOR_PAIR(1));                  //Set up of main screen: Title, authors, and mode selection.
  mvwprintw(win, 2, 2, "   _____              __  __   ______      ____    ______     _        _____   ______   ______ ");
  mvwprintw(win, 3, 2, "  / ____|     /\\     |  \\/  | |  ____|    / __ \\  |  ____|   | |      |_   _| |  ____| |  ____|");
  mvwprintw(win, 4, 2, " | |  __     /  \\    | \\  / | | |__      | |  | | | |__      | |        | |   | |__    | |__  ");
  mvwprintw(win, 5, 2, " | | |_ |   / /\\ \\   | |\\/| | |  __|     | |  | | |  __|     | |        | |   |  __|   |  __| ");
  mvwprintw(win, 6, 2, " | |__| |  / ____ \\  | |  | | | |____    | |__| | | |        | |____   _| |_  | |      | |____ ");
  mvwprintw(win, 7, 2, "  \\_____| /_/    \\_\\ |_|  |_| |______|    \\____/  |_|        |______| |_____| |_|      |______|");
  wattroff(win,COLOR_PAIR(1));
  mvwprintw(win, 9, 48, "by");
  mvwprintw(win, 10, 42, "MARC ZOEL ARIAS");
  mvwprintw(win, 11, 44, "ARNAU BOIX");
  mvwprintw(win, 12, 43, "YERAY NAVARRO");

  mvwprintw(win, 16, 40, "CHOOSE YOUR MODE:");
  mvwprintw(win, 18, 41, "(1) Random");            // RANDOM MODE: A row and column size must be selected and a random initial configuration will be started
  mvwprintw(win, 19, 41, "(2) From txt file");     // TXT FILE MODE: Initialize the first generation as the one given in the txt file (see code profiling)
  mvwprintw(win, 20, 41, "(3) Given structures");  // STRUCTURES MODE: The first generation will be one of the three of the asked one

  wrefresh(win);                               // refreshing the window

  int c = getch();                             // Get character that the user has pressed
  endwin();                                    // Close window
  clear();                                     // Clear screen
	return c;                                    // Return value of the pressed key (use for mode selection)
};

/* ENDITALL(): Close curses and end program */
int enditall(void)
{
	endwin();			// End curses mode
	return 0;
};

/* INIT(): Function to setup the first generation (depends on approach used)*/
int ** init(int row, int col, int approach)
{
  /* RANDOM MODE SETUP: A row and column size must be selected for the initialization*/
  if (approach == 49)
  {
    // First screen: Ask for number of columns.
    win = newwin(23, 100, 2,2);                       // Window setup
    refresh();
    box(win, 0, 0);
    wattron(win,COLOR_PAIR(1));
    mvwprintw(win, 2, 2, "   _____              __  __   ______      ____    ______     _        _____   ______   ______ ");
    mvwprintw(win, 3, 2, "  / ____|     /\\     |  \\/  | |  ____|    / __ \\  |  ____|   | |      |_   _| |  ____| |  ____|");
    mvwprintw(win, 4, 2, " | |  __     /  \\    | \\  / | | |__      | |  | | | |__      | |        | |   | |__    | |__  ");
    mvwprintw(win, 5, 2, " | | |_ |   / /\\ \\   | |\\/| | |  __|     | |  | | |  __|     | |        | |   |  __|   |  __| ");
    mvwprintw(win, 6, 2, " | |__| |  / ____ \\  | |  | | | |____    | |__| | | |        | |____   _| |_  | |      | |____ ");
    mvwprintw(win, 7, 2, "  \\_____| /_/    \\_\\ |_|  |_| |______|    \\____/  |_|        |______| |_____| |_|      |______|");
    wattroff(win,COLOR_PAIR(1));
    mvwprintw(win, 16, 40, "Number of columns?");
    mvwprintw(win, 17, 42, "Type and press enter");
    wrefresh(win);

    size_col = get_int();                           // Call get_int() to read the user input and set the column size

    endwin();                                      // Close and clear window
    clear();

    // Second screen: Ask for number of rows.
    win = newwin(23, 100, 2,2);                       // Window setup
    refresh();
    box(win, 0, 0);
    wattron(win,COLOR_PAIR(1));
    mvwprintw(win, 2, 2, "   _____              __  __   ______      ____    ______     _        _____   ______   ______ ");
    mvwprintw(win, 3, 2, "  / ____|     /\\     |  \\/  | |  ____|    / __ \\  |  ____|   | |      |_   _| |  ____| |  ____|");
    mvwprintw(win, 4, 2, " | |  __     /  \\    | \\  / | | |__      | |  | | | |__      | |        | |   | |__    | |__  ");
    mvwprintw(win, 5, 2, " | | |_ |   / /\\ \\   | |\\/| | |  __|     | |  | | |  __|     | |        | |   |  __|   |  __| ");
    mvwprintw(win, 6, 2, " | |__| |  / ____ \\  | |  | | | |____    | |__| | | |        | |____   _| |_  | |      | |____ ");
    mvwprintw(win, 7, 2, "  \\_____| /_/    \\_\\ |_|  |_| |______|    \\____/  |_|        |______| |_____| |_|      |______|");
    wattroff(win,COLOR_PAIR(1));
    mvwprintw(win, 16, 40, "Number of rows?");
    mvwprintw(win, 17, 42, "Type and press enter");
    wrefresh(win);

    size_row = get_int();                           // Call get_int() to read the user input and set the row size

    endwin();                                       // Close and clear window
    clear();

  }
  /* OTHER MODES SETUP:  A txt file must be selected for initialization*/
  else
  {
    if (approach == 50)                                  // If txt mode is selected
    {
      filename = "in_conf.txt";                          // The file to read will be the one configured by the user (a filled one was given as a sample)
    }
    else if (approach == 51){                            // If structures mode is selected
      win = newwin(23, 100, 2,2);                        // Generate a window to select the initial structure (the three given by the problem statement)
      refresh();
      box(win, 0, 0);
      wattron(win,COLOR_PAIR(1));
      mvwprintw(win, 2, 2, "   _____              __  __   ______      ____    ______     _        _____   ______   ______ ");
      mvwprintw(win, 3, 2, "  / ____|     /\\     |  \\/  | |  ____|    / __ \\  |  ____|   | |      |_   _| |  ____| |  ____|");
      mvwprintw(win, 4, 2, " | |  __     /  \\    | \\  / | | |__      | |  | | | |__      | |        | |   | |__    | |__  ");
      mvwprintw(win, 5, 2, " | | |_ |   / /\\ \\   | |\\/| | |  __|     | |  | | |  __|     | |        | |   |  __|   |  __| ");
      mvwprintw(win, 6, 2, " | |__| |  / ____ \\  | |  | | | |____    | |__| | | |        | |____   _| |_  | |      | |____ ");
      mvwprintw(win, 7, 2, "  \\_____| /_/    \\_\\ |_|  |_| |______|    \\____/  |_|        |______| |_____| |_|      |______|");
      wattroff(win,COLOR_PAIR(1));
      mvwprintw(win, 16, 40, "CHOOSE INITIAL STRUCTURE:");
      mvwprintw(win, 18, 41, "(1) Acorn");
      mvwprintw(win, 19, 41, "(2) Diehard");
      mvwprintw(win, 20, 41, "(3) The R-pentomino");
      wrefresh(win);

      int version = getch();                           // Read user input (must be 1,2 or 3)
      endwin();

      if (version == 49)                               // If (1) is selected, use acorn structure
        filename = "acorn.txt";
      else if (version == 50)                          // If (2) is selected, use diehard structure
        filename = "diehard.txt";
      else if (version == 51)                          // If (3) is selected, use rpentomino structure
        filename = "rpentomino.txt";
    }
		FILE *fp = fopen(filename, "r");                   // Read the selected file and count the number of rows and columns
    int columns = 0;
    int lines = 0;
    char i;
    while((i=fgetc(fp))!=EOF)
    {
        if ((i == '0') || (i == '1')) {
            ++columns;
        } else if (i == '\n') {
            size_col = columns;
            lines++;
            columns = 0;
        }
    }
    size_row = lines+1;
  }

  int ** grid = (int **) malloc(size_row * sizeof(int *));    // Initialize an array of pointers representing each cell of size size_col x size_row
  for (int i = 0; i < size_row; ++i)                          // The integer represents the number of generations a cell has been alive for
    grid[i] = (int *) malloc(size_col * sizeof(int));

	if (approach == 50 || approach == 51)                       // If the configuration is being set from mode (2) or (3)
	{
		FILE *fp = fopen(filename, "r");                          // Read the txt file to set the initial configuration of each cell
                                                              // 0 -> initially dead, 1 -> Initially alive
		int ch;

    for (int i = 0; i < size_row; ++i)
    {
      for (int j = 0; j < size_col; ++j)
      {
        ch = fgetc(fp);
        if ((ch != '0') && (ch != '1'))
          ch = fgetc(fp);
        if (ch == '1')
          grid[i][j] = 1;
        else
          grid[i][j] = 0;
      }
    }
	}
	else if (approach == 49)                                    // If the configuration is being set from mode (1)
	{
    for (int i = 1; i < size_row - 1; ++i)
    {
      for (int j = 1; j < size_col - 1; ++j)
        grid[i][j] = rand()%2;                               // Randomly initialize each cell (0 or 1)
    }
	}
return grid;
}

/* DISPLAY():  Display the current generation on screen*/
void display(int ** grid)
{
  for (int i = 1; i < size_row -1; ++i)            //For each cell
  {
    for (int j = 1; j < size_col -1; ++j)
    {
      if(grid[i][j] >= 1)                          //If the cell is alive
      {
        if(grid[i][j] <= 10)                       //Decide the color depending on the generations it has lived for
        {                                          //New living cell -> Red color, Old cell -> White color
          wattron(win,COLOR_PAIR(2));
          mvwaddch(win,i,j,' ' | A_REVERSE);
          wattroff(win,COLOR_PAIR(2));
        }
        else if (grid[i][j] <= 20)
        {
          wattron(win,COLOR_PAIR(3));
          mvwaddch(win,i,j,' ' | A_REVERSE);
          wattroff(win,COLOR_PAIR(3));
        }
        else if (grid[i][j] <= 30)
        {
          wattron(win,COLOR_PAIR(4));
          mvwaddch(win,i,j,' ' | A_REVERSE);
          wattroff(win,COLOR_PAIR(4));
        }
        else if (grid[i][j] <= 40)
        {
          wattron(win,COLOR_PAIR(5));
          mvwaddch(win,i,j,' ' | A_REVERSE);
          wattroff(win,COLOR_PAIR(5));
        }
        else
        {
          wattron(win,COLOR_PAIR(6));
          mvwaddch(win,i,j,' ' | A_REVERSE);
          wattroff(win,COLOR_PAIR(6));
        }

      }
      else if(grid[i][j] == 0)                 // If the cell is dead, print an space (nothing)
        mvwaddch(win,i,j,' ');
    }
  }
  move(0, 0);                                  // Set the cursor to (0,0) so it doesn't bother the user
  wrefresh(win);
}

/* UPDATE():  Update cells to compute next generation*/
void update(int ** grid)
{
  int prev_grid[size_row][size_col];                   // Generate and copy the current cell state
  for (int i = 0; i < size_row; ++i)
  {
    for (int j = 0; j < size_col; ++j)
        prev_grid[i][j] = grid[i][j];
  }
  for (int i = 1; i < size_row - 1; ++i)              // Count the number of living neighbours
  {
    for (int j = 1; j < size_col - 1; ++j)
    {
      int neigh = (prev_grid[i-1][j+1] != 0) +
                  (prev_grid[i-1][j] != 0) +
                  (prev_grid[i-1][j-1] != 0) +
                  (prev_grid[i][j+1] != 0) +
                  (prev_grid[i][j-1] != 0) +
                  (prev_grid[i+1][j-1] != 0) +
                  (prev_grid[i+1][j] != 0) +
                  (prev_grid[i+1][j+1] != 0);
      //Computation of next state
      if ((prev_grid[i][j] >= 1) && (neigh < 2))                       // If a living cell has less than 2 neighbours
          grid[i][j] = 0;                                              // it dies
      else if ((prev_grid[i][j] >= 1) && (neigh <= 3) && (neigh >= 2)) // If a living cell has 2 or 3 neighbours
          grid[i][j] += 1;                                             // it lives for another generation
      else if ((prev_grid[i][j] >= 1) && (neigh > 3))                  // If a living cell has more than 3 neighbours
          grid[i][j] = 0;                                              // it dies
      else if ((prev_grid[i][j] == 0) && (neigh == 3))                 // If a dead cell has 3 neighbours
          grid[i][j] = 1;                                              // it borns (generation count set to 1)
    }
  }
}

/*MAIN(): Main function */
int main()
{
  approach = initall();                                               // Initialize and read user input to select MODE

  int ** grid = init(size_row,size_col,approach);                     // Generate a grid with the used cells using the init function

  clear();
  win = newwin(size_row, size_col, 2,2);                              // Generate a window for the game to be played in
  refresh();
  box(win, 0, 0);
  wrefresh(win);

  display(grid);                                                      // Display the initial state
  getch();                                                            // Wait for user input
  int ex;
  while (ex != 52)                                                    // As long as the user doesn't press key [4]
  {
    update(grid);                                                     // Compute next generation
    display(grid);                                                    // Display next generation
    ex = getch();                                                     // Wait for user input
  }
  for (int i = 0; i < size_row; ++i)
    free(grid[i]);
  free(grid);                                                         // Free space of pointers
  enditall();                                                         // End program
}
