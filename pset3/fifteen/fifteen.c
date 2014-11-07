/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// blank tile possition
int iblank, jblank;

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(1000000);
        }

        // sleep for animation's sake
        usleep(50000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j. Tile 0 is the empty space
 */
void init(void)
{
    // the current tile number, starting at largest
    int tilenum = d * d - 1;
    
    // assign numbers to each tile row by row, starting upper left
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = tilenum;
            tilenum--;
        }
    }
    
    // swap last two tiles if the number of tiles is odd
    if ((d * d - 1) % 2 == 1)
    {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // print top border
    printf("\n");
    printf("    ");
    for (int k = 0; k < d; k++)
    {
        printf("__   ");
    }
    printf("\n\n");
    
    // print board tiles
    for (int i = 0; i < d; i++)
    {        
        // print each row with padding and tile edge
        printf("  | ");
        for (int j = 0; j < d; j++)
        {            
            // for the blank tile print nothing
            if (board[i][j] == 0)
            {
                printf("   | ");
            }
            else
            {
                printf("%2d | ", board[i][j]);
            }
        }
    
        // print row divider
        printf("\n");
        printf("    ");
        for (int k = 0; k < d; k++)
        {
            printf("__   ");
        }
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // set blank tile location on first run
    if (board[d - 1][d - 1] == 0)
    {
        iblank = d - 1;
        jblank = d - 1;
    }
    
    // locate the tile
    int found, itile, jtile;
    found = 0;
    for (int i = 0; i < d && found == 0; i++)
    {
        for (int j = 0; j < d && found == 0; j++)
        {
            if (board[i][j] == tile)
            {
                itile = i;
                jtile = j;
                found = 1;
            }
        }
    }
        
    // returns false if it is a illegal move
    if ((itile == iblank && abs(jtile - jblank) == 1) || (jtile == jblank && abs(itile - iblank) == 1))
    {
        // move tile
        board[iblank][jblank] = tile;
        board[itile][jtile] = 0;
        iblank = itile;
        jblank = jtile;
        return true;
    }
    else
    {
        return false;
    }

}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // check each tile for correct position
    int sequence = 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++, sequence++)
        {
            if (sequence == d * d)
            {
                return true;
            }
            else if (board[i][j] != sequence)
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
