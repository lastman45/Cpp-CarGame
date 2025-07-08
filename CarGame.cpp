#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 80  // Width of console screen
#define SCREEN_HEIGHT 25 // Height of console screen
#define WIN_WIDTH 70     // Width of the game window

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // Gets the handle for the console output
COORD CursorPosition;                             // Structure to hold cursor position

int enemyY[3];     // Array to hold the Y positions of enemies
int enemyX[3];     // Array to hold the X positions of enemies
int enemyFlag[3];  // Array to indicate if an enemy is active (1) or not (0)
char car[4][4] = { // Car shape
    {' ', '*', '*', ' '},
    {'*', '*', '*', '*'},
    {' ', '*', '*', ' '},
    {'*', '*', '*', '*'}};
int carPos = WIN_WIDTH / 2; // Initial position of the car in the center of the window
int score = 0;              // Player's score

void gotoxy(int x, int y)
{
    CursorPosition.X = x;                              // Set the X coordinate
    CursorPosition.Y = y;                              // Set the Y coordinate
    SetConsoleCursorPosition(console, CursorPosition); // Move the cursor to the specified position
}

void setcursor(bool visible, DWORD size)
{
    if (size == 0) // If size is not specified, set a default size
        size = 20;
    CONSOLE_CURSOR_INFO lpCursor;             // Structure to hold cursor information
    lpCursor.bVisible = visible;              // Set visibility
    lpCursor.dwSize = size;                   // Set size
    SetConsoleCursorInfo(console, &lpCursor); // Apply the cursor settings
}

void drawBorder()
{
    for (int i = 0; i < SCREEN_HEIGHT; i++) // Loop through each row
    {
        gotoxy(0, i);         // Move cursor to the left border
        cout << "|";          // Left border
        gotoxy(WIN_WIDTH, i); // Move cursor to the right border
        cout << "|";          // Right border
    }
    for (int j = 0; j < WIN_WIDTH; j++) // Loop through each column
    {
        gotoxy(j, 0);                 // Move cursor to the top border
        cout << "_";                  // Top border
        gotoxy(j, SCREEN_HEIGHT - 1); // Move cursor to the bottom border
        cout << ".";                  // Bottom border
    }
}

// Function to randomly set the X position of an enemy
void GetEnemy(int index)
{
    enemyX[index] = 17 + rand() % (33);
}

// Function to draw an enemy on the screen
void DrawEnemy(int index)
{
    if (enemyFlag[index] == 1) // Check if the enemy is active
    {
        gotoxy(enemyX[index], enemyY[index]); // Move cursor to enemy's position
        cout << "####";                       // Enemy shape
        gotoxy(enemyX[index], enemyY[index] + 1);
        cout << " ## ";
        gotoxy(enemyX[index], enemyY[index] + 2);
        cout << "####";
        gotoxy(enemyX[index], enemyY[index] + 3);
        cout << " ## ";
    }
}

// Function to clear an enemy from the screen
void ClearEnemy(int index)
{
    if (enemyFlag[index] == 1) // Check if the enemy is active
    {
        gotoxy(enemyX[index], enemyY[index]); // Move cursor to enemy's position
        cout << "    ";                       // Clear enemy shape
        gotoxy(enemyX[index], enemyY[index] + 1);
        cout << "    ";
        gotoxy(enemyX[index], enemyY[index] + 2);
        cout << "    ";
        gotoxy(enemyX[index], enemyY[index] + 3);
        cout << "    ";
    }
}

// Function to reset an enemy's position and state
void ResetEnemy(int index)
{
    ClearEnemy(index); // Clear the enemy from the screen
    enemyY[index] = 1; // Reset Y position to the top
    GetEnemy(index);   // Get a new random X position for the enemy
}

// Function to draw the player's car on the screen
void DrawCar()
{
    for (int i = 0; i < 4; i++) // Loop through each row of the car
    {
        for (int j = 0; j < 4; j++) // Loop through each column of the car
        {
            gotoxy(carPos + j, i + 20); // Move cursor to the car's position
            cout << car[i][j];          // Draw the car shape
        }
    }
}

// Function to clear the player's car from the screen
void ClearCar()
{
    for (int i = 0; i < 4; i++) // Loop through each row of the car
    {
        for (int j = 0; j < 4; j++) // Loop through each column of the car
        {
            gotoxy(carPos + j, i + 20); // Move cursor to the car's position
            cout << " ";                // Clear the car shape
        }
    }
}

// Function to check for collisions between the car and enemies
int Collision()
{
    for (int i = 0; i < 3; i++) // Loop through all enemies
    {
        if (enemyFlag[i] == 1 && enemyY[i] + 4 >= 20) // Check if enemy is active and near the car
        {
            if (enemyX[i] + 4 > carPos && enemyX[i] < carPos + 4) // Check for horizontal overlap
            {
                return 1; // Collision detected
            }
        }
    }
    return 0; // No collision
}

// Function to handle game over scenario
void GameOver()
{
    system("cls");
    gotoxy(30, 10); // Move cursor to a specific position
    cout << "Game Over!";
    gotoxy(30, 11);
    cout << "Your Score: " << score; // Display the player's score
    gotoxy(30, 12);
    cout << "Press any key to exit...";
    _getch();
    exit(0);
}

// Function to update and display the score
void UpdateScore()
{
    gotoxy(WIN_WIDTH + 7, 5);   // Move cursor to score position
    cout << "Score: " << score; // Display the score
}

// Function to display game instructions
void Instructions()
{
    system("cls");
    cout << "Game Instructions:" << endl;
    cout << "\nAvoid Cars by moving your car left and right." << endl;
    cout << "Press 'a' to move left, 'd' to move right, and ESC to quit." << endl;
    cout << "Press any key to start the game..." << endl;
    _getch();
}

void Play()
{
    carPos = WIN_WIDTH / 2;  // Reset car position to center
    score = 0;               // Reset score
    bool keyPressed = false; // Debounce variable to prevent multiple key presses

    enemyFlag[0] = 1; // Activate the first enemy
    enemyFlag[1] = 0; // Deactivate the second enemy
    enemyY[0] = 1;    // Set initial Y position for the first enemy
    enemyY[1] = 1;    // Set initial Y position for the second enemy

    system("cls");
    drawBorder();
    UpdateScore();
    GetEnemy(0); // Get initial position for the first enemy
    GetEnemy(1); // Get initial position for the second enemy

    // Display game title and controls
    gotoxy(WIN_WIDTH + 7, 2);
    cout << "CAR GAME";
    gotoxy(WIN_WIDTH + 6, 4);
    cout << "------------";
    gotoxy(WIN_WIDTH + 6, 6);
    cout << "------------";
    gotoxy(WIN_WIDTH + 7, 12);
    cout << "CONTROLS";
    gotoxy(WIN_WIDTH + 7, 13);
    cout << "--------";
    gotoxy(WIN_WIDTH + 7, 14);
    cout << "A Key - Left";
    gotoxy(WIN_WIDTH + 7, 15);
    cout << "D Key - Right";

    gotoxy(18, 5);
    cout << "Press any key to start...";
    _getch();
    gotoxy(18, 5);
    cout << "                         ";

    while (1)
    {
        if (_kbhit()) // Check if a key has been pressed
        {
            char ch = _getch(); // Get the pressed key
            if (!keyPressed)    // Check if the key was not already pressed
            {
                if (ch == 'a' || ch == 'A')
                {
                    if (carPos > 18) // Check bounds
                        carPos -= 4; // Move car left
                }
                if (ch == 'd' || ch == 'D')
                {
                    if (carPos < 50) // Check bounds
                        carPos += 4; // Move car right
                }
                if (ch == 27) // ESC key to exit
                {
                    break; // Exit the loop
                }
                keyPressed = true; // Set key as pressed
            }
        }
        else
        {
            keyPressed = false; // Reset when no key is pressed
        }

        DrawCar();
        DrawEnemy(0);
        DrawEnemy(1);

        if (Collision() == 1)
        {
            GameOver();
            return;
        }

        Sleep(50 - (score / 5)); // Control game speed based on score
        ClearCar();              // Clear the car from the screen
        ClearEnemy(0);           // Clear the first enemy
        ClearEnemy(1);           // Clear the second enemy

        if (enemyY[0] == 10 && enemyFlag[1] == 0) // Activate second enemy after first reaches a certain point
        {
            enemyFlag[1] = 1; // Activate second enemy
        }

        if (enemyFlag[0] == 1) // Move first enemy down if active
        {
            enemyY[0]++;
        }
        if (enemyFlag[1] == 1) // Move second enemy down if active
        {
            enemyY[1]++;
        }

        if (enemyY[0] > SCREEN_HEIGHT - 4) // Check if first enemy has moved off screen
        {
            ResetEnemy(0); // Reset first enemy
            score++;       // Increment score
            UpdateScore(); // Update displayed score
        }
        if (enemyY[1] > SCREEN_HEIGHT - 4) // Check if second enemy has moved off screen
        {
            ResetEnemy(1); // Reset second enemy
            score++;       // Increment score
            UpdateScore(); // Update displayed score
        }
    }
}

int main()
{
    setcursor(0, 0);             // Hide the cursor
    srand((unsigned)time(NULL)); // Seed the random number generator

    do
    {
        system("cls");
        gotoxy(10, 5);
        cout << "__________"; // Menu border
        gotoxy(10, 6);
        cout << "CAR GAME";
        gotoxy(10, 7);
        cout << "__________";
        gotoxy(10, 9);
        cout << "1. Start New Game";
        gotoxy(10, 10);
        cout << "2. Instructions";
        gotoxy(10, 11);
        cout << "3. Quit";
        gotoxy(10, 13);
        cout << "Option?: ";

        char op = _getch();

        if (op == '1')
            Play();
        else if (op == '2')
            Instructions();
        else if (op == '3')
            exit(0);
    } while (1); // Repeat the menu until the user chooses to quit

    return 0;
}
