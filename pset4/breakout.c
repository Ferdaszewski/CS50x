//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// initial horizontal velocity of ball in pixels
#define VELOCITY 2

// size of paddle in pixels
#define PADW 60
#define PADH 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // sets x and y velocity of the ball
    double velox = 2 * drand48();
    double veloy = VELOCITY;
    
    // wait for click to start
    waitForClick();

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // listen for mouse event
        GEvent mouse = getNextEvent(MOUSE_EVENT);
        
        // if one is heard
        if (mouse != NULL)
        {
            // if the event was a mouse movement
            if (getEventType(mouse) == MOUSE_MOVED)
            {
                // match the paddle's x axis movment to the mouse's
                double x = getX(mouse) - getWidth(paddle) / 2;
                double y = getY(paddle);
                setLocation(paddle, x, y);
            }
        }
        
        // move that ball!
        move(ball, velox, veloy);
        
        // bounce off right or left edge of window
        if ((getX(ball) + getWidth(ball) >= getWidth(window)) || (getX(ball) <= 0))
        {
            velox = -velox;
        }

        // bounce off top edge of window
        else if (getY(ball) <= 0)
        {
            veloy = -veloy;
        }
        
        // check for ball hitting bottom of window
        else if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            // decriment lives by 1
            lives--;
            
            // reset ball
            setLocation(ball,(getWidth(window) / 2) - RADIUS, (getHeight(window) / 2) - RADIUS);
            
            // wait for mouse click to resume game
            waitForClick();
        }

        // linger before moving again
        pause(10);
        
        // check for collision with paddle or bricks
        GObject object = detectCollision(window, ball);
        if ( object !=NULL)
        {
            // check is collision is with a GRect
            if (strcmp(getType(object), "GRect") ==0)
            {   
                // bounce if collision is with paddle
                if (object == paddle)
                {
                    veloy = -veloy;
                }
                else
                {
                    // remove brick and bounce if collision is with brick
                    removeGWindow(window, object);
                    veloy = -veloy;
                    
                    // increment score and update scoreboard
                    points++;
                    updateScoreboard(window, label, points);
                    
                }
            }
        }
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // size of gap between bricks
    int gap = RADIUS / 2;
    
    // size of bricks
    double brickw = (getWidth(window) - gap * (COLS + 1)) / COLS;
    double brickh = brickw / 4;
    
    // create array for all bricks
    GRect brick[ROWS][COLS];
    
    // instantiates bricks
    for (int i = 0, rows = ROWS; i < rows; i++)
    {
        // set x and y of bricks
        double y = 5 * PADH + (brickh + gap) * i;
        double x = gap;
        
        // select color for row
        char* color[8] = {"BLUE", "CYAN", "GREEN", "MAGENTA", "ORANGE", "PINK", "RED", "YELLOW"};
        
        for (int j = 0, cols = COLS; j < cols; j++)
        {
            brick[i][j] = newGRect(x, y, brickw, brickh);
            setFilled(brick[i][j], true);
            setColor(brick[i][j], color[i % 8]);
            add(window, brick[i][j]);
            x = x + brickw + gap;
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // instantiates a solid black ball
    GOval ball = newGOval((getWidth(window) / 2) - RADIUS, (getHeight(window) / 2) - RADIUS, RADIUS * 2, RADIUS * 2);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    
    // add ball to the window
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{    
    // instantiates a solid black paddle
    GRect paddle = newGRect((getWidth(window) - PADW) / 2, getHeight(window) - 5 * PADH, PADW, PADH);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    
    // adds the paddle to the window
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // create scoreboard
    GLabel scoreboard = newGLabel("0");
    
    // center scoreboard in window
    double x = (getWidth(window) - getWidth(scoreboard)) / 2;
    double y = (getHeight(window) - getHeight(scoreboard)) / 2;
    setLocation(scoreboard, x, y);
    
    // style scoreboard
    setFont(scoreboard, "SansSerif-50");
    setColor(scoreboard, "LIGHT_GRAY");
    
    // show scoreboard
    add(window, scoreboard);
    
    return scoreboard;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
