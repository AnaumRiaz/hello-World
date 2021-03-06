//
// breakout.c
//
// Anaum Riaz
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600 
#define WIDTH 400 

// height and width of paddle in pixels
#define PAD_HGT 12
#define PAD_WDH 62

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points, int check);
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
   
    // Declaring velocity
    double velocityx = drand48();
    double velocityy = 2.0;
   
   
    waitForClick();
   
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // moving paddle with mouse
        GEvent event = getNextEvent(MOUSE_EVENT);

        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // paddle follows top cursor
                
                double x = getX(event) - getWidth(paddle) / 2;
                if (x <= (WIDTH - PAD_WDH ) && x >= 0)
                { 
                    setLocation(paddle, x, HEIGHT / 1.25);
                }
                
            }
        }
        
 // ****************** Managing ball movement************       
        
        move(ball, velocityx, velocityy);

        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityx = -velocityx;
        }

        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            velocityx = -velocityx;
        }

        // ball hits bottom
        else if (getY(ball) + getHeight(ball) >= getHeight(window))
        {   
            lives = lives - 1;
            waitForClick();
            setLocation(ball, (WIDTH / 2 - RADIUS),HEIGHT / 2);
            setLocation(paddle,(WIDTH - PAD_WDH) / 2, HEIGHT / 1.25);
        } 
        
        // ball hits top 
        else if (getY(ball) <= 0)
        {   
            velocityy = -velocityy;   
        } 
        
        // *************** Checking for collision ************ 
            
        GObject object = detectCollision (window, ball);

        if (object == paddle)
        {
            double y = getY(ball) + 2 * RADIUS;
            // checking if the bottow of the ball is touching paddle
            if (y <= HEIGHT / 1.25)
            {
                velocityy = -velocityy;
            }
        }

        // if ball hits brick
        if (object != NULL && object != paddle)
        {
            
            if (strcmp(getType(object), "GRect") == 0 )
            {
                removeGWindow(window, object);
                velocityy = -velocityy;
                points = points + 1;
                updateScoreboard (window, label, points,2);
                bricks = bricks - 1;
                if (bricks == 0)
                {
                    updateScoreboard (window, label, points,0);
                }
            }
        }
        // linger before moving again
        pause(10); 
          
    }
    if (lives == 0)
    {
        updateScoreboard (window, label, points,1);
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
    
    int y = 20;
   
    for(int i = 0; i < ROWS; i++)
    {
        int x = 2;
        y = y + 14;
        string color = "MAGENTA";
        switch (i)
        {
            case(0):
                break;
            case(1):
                color = "BLUE";
                break;
            case(2):
                color = "RED";
                break;
            case(3):
                color = "GREEN";
                break;
            case(4):
                color = "ORANGE";
                break;        
        }
               
        for(int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x, y, 36, 10);
            add (window, brick);
            setColor(brick, color);
            setFilled(brick, true); 
            // 40 because 4 is space ans 36 brink length  
            x = x + 40;   
        }
        
    }
    
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval((WIDTH / 2 - RADIUS),HEIGHT / 2,2 * RADIUS,2 * RADIUS);
    setFilled(ball, true);
    setColor(ball,"BLACK");
    
    // setLocation(ball, WIDTH/2,HEIGHT/2);
    add(window,ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect ((WIDTH - PAD_WDH) / 2, HEIGHT / 1.25, PAD_WDH, PAD_HGT);
 
    // colored and filled
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    
    // add paddle to window
    add(window, paddle);

    return paddle;

}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel score = newGLabel("");
    setFont(score, "SansSerif-36");
    
    
     // location of scoreboard
    double x = (getWidth(window) - getWidth(score)) / 2;
    double y = (getHeight(window) - getHeight(score)) / 2;
    setLocation(score, x, y);

    // add scoreboard to window
    add (window, score);

    return score;
}


/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points, int check)
{
    // update score
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);
    
    // Added code: print the status of the game
    if (check == 0)
    {
        setLabel(label, "YOU WIN");
    }
    else if (check == 1)
    {
        setLabel(label, "YOU LOSE");
    }
    
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
