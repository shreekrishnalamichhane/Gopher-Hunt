#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define SIZE 10

// main board 3-D array
// First Index = Size of rows
// Second Index = Size of columns
// Third Index = Various Layers
// Layer 0. current playing board
// Layer 1. position of gophers
// Layer 2. explored & unexplored cells
string board[SIZE][SIZE][3];

// No of possible explorable cell without gophers is
// (size x size) - no. of Gophers
int unexplored = SIZE * SIZE - SIZE;

// Prototypes.
// One time initialization function
void setup();

// Looping function
void loop();

// Function to clear whole board with provided character.
void clearBoard(int, char);

// Function to render the current board provided layer.
void renderBoard(int);

// Function to place N gophers.
void placeNGophers(int, int);

// Function to place a gopher at random location
void placeGopher();

// Function to generate random integer in between
// two provided integers
int randomIdxBetween(int, int);

// get number of gophers in all eight possible
// neighbour place if place exists.
// neglects neighbours of corner cells which have
// less neighbour possible cells.
int getNeighbourGopherCount(int, int);

// explore neighbours if gopher count at current cell is 0
//  it will be safe to explore.
// returns how many neighbours were explored.
int exploreSafeNeighbours(int, int);

// Actual Game playing logic
int playGame(int, int);

// Function to clear the screen.
void clearScreen();

// Main function
int main()
{
    // calling a setup function
    setup();
}

void setup()
{
    // clear the current playing board layer 0
    clearBoard(0, '+');
    // clear the gopher position board layer 1
    clearBoard(1, '+');

    // place N gophers at board layer 1
    placeNGophers(1, SIZE);

    // render the current playing board layer 1
    renderBoard(0);

    // 'U' for unexplored, 'E' for explored cell.
    // Initially initializing with 'U'.
    clearBoard(2, 'U');

    // Calling the loop function
    loop();
}

void loop()
{
    // while there reamins the inexplored cells in the board
    // Keep playing the game
    while (unexplored > 0)
    {
        int posX, posY;
        cout << endl
             << "Enter X and Y :";
        cin >> posX >> posY;
        playGame(posX, posY);
    }
    // if player loose, then the program will exit inside
    // upper while loop and never reach here.
    cout << "Congratulations! You Win." << endl;
}

void clearBoard(int layer, char clearCharacter)
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j][layer] = clearCharacter;
        }
}

void renderBoard(int layer = 0)
{
    // There is a lot of extra code here, just to make the output
    // on the console looks pretty.
    cout << "    ";
    for (int i = 0; i < SIZE; i++)
        cout << i << " | ";
    cout << endl;
    cout << "   ---------------------------------------" << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << i << " | ";
        for (int j = 0; j < SIZE; j++)
        {
            cout << board[i][j][layer];
            if (j != SIZE - 1)
                cout << "   ";
        }
        cout << " | " << endl;
        if (i != SIZE - 1)
            cout << endl;
    }
    cout << "   ---------------------------------------" << endl;
}

int randomIdxBetween(int low, int high)
{
    return (low + (rand() % (high - low)));
}

bool placeGopher(int layer, int currentGopherIdx)
{
    // generate a random seed.
    int seed = time(0) + currentGopherIdx;
    // seed the rand() function
    srand(seed);
    // get two random values for x, y cordinates.
    int x = randomIdxBetween(0, SIZE);
    int y = randomIdxBetween(0, SIZE);

    // if randomly generated cell already has gopher, then return false
    // notifying , gopher was not placed.
    if (board[x][y][layer] == "G")
        return false;
    // else place the gopher at the cell.
    //  and return true, notifying new gopher was placed.
    else
    {
        board[x][y][layer] = "G";
        return true;
    }
}

void placeNGophers(int layer, int noOfGophers = SIZE)
{
    // Before placing any gophers, we clear the board with '+'.
    clearBoard(layer, '+');

    // then we place N = noOfGophers gophers to the board.
    for (int i = 0; i < noOfGophers; i++)
    {
        // if gopher placement was not succeed
        if (!placeGopher(1, i))
            // then we try to place that gopher again in another random cell.
            placeGopher(1, i);
    }
}

int getNeighbourGopherCount(int posX, int posY)
{
    // here we are counting the number of gophers in the neighbour cells.
    int count = 0;
    int i = posX, j = posY; // just for convinience

    // top left
    if (i > 0 && j > 0 && board[i - 1][j - 1][1] == "G")
        count++;

    // hard top
    if (i > 0 && board[i - 1][j][1] == "G")
        count++;

    // top right
    if (i > 0 && j < SIZE - 1 && board[i - 1][j + 1][1] == "G")
        count++;

    // hard left
    if (j > 0 && board[i][j - 1][1] == "G")
        count++;

    // hard right
    if (j < SIZE - 1 && board[i][j + 1][1] == "G")
        count++;

    // bottom left
    if (i < SIZE - 1 && j > 0 && board[i + 1][j - 1][1] == "G")
        count++;

    // hard bottom
    if (i < SIZE - 1 && board[i + 1][j][1] == "G")
        count++;

    // bottom right
    if (i < SIZE - 1 && j < SIZE - 1 && board[i + 1][j + 1][1] == "G")
        count++;

    // return the number of gophers found.
    return count;
}

int exploreSafeNeighbours(int posX, int posY)
{
    int count = 0;
    int i = posX, j = posY; // just for convinience

    // top left
    if (i > 0 && j > 0 && board[i - 1][j - 1][2] != "E")
    {
        count++;
        playGame(i - 1, j - 1);
    }
    // hard top
    if (i > 0 && board[i - 1][j][2] != "E")
    {
        count++;
        playGame(i - 1, j);
    }
    // top right
    if (i > 0 && j < SIZE - 1 && board[i - 1][j + 1][2] != "E")
    {
        count++;
        playGame(i - 1, j + 1);
    }
    // hard left
    if (j > 0 && board[i][j - 1][2] != "E")
    {
        count++;
        playGame(i, j - 1);
    }
    // hard right
    if (j < SIZE - 1 && board[i][j + 1][2] != "E")
    {
        count++;
        playGame(i, j + 1);
    }
    // bottom left
    if (i < SIZE - 1 && j > 0 && board[i + 1][j - 1][2] != "E")
    {
        count++;
        playGame(i + 1, j - 1);
    }
    // hard bottom
    if (i < SIZE - 1 && board[i + 1][j][2] != "E")
    {
        count++;
        playGame(i + 1, j);
    }
    // bottom right
    if (i < SIZE - 1 && j < SIZE - 1 && board[i + 1][j + 1][2] != "E")
    {
        count++;
        playGame(i + 1, j + 1);
    }
    return count;
}

// Actual Game playing logic
int playGame(int posX, int posY)
{
    // Checking for input bound error
    if (posX < 0 || posX >= SIZE || posY < 0 || posY >= SIZE)
    {
        cout << endl
             << "Wrong Cordinate Provided - Bound Error. Try Again." << endl;
    }
    // if no input bound error, then
    else
    {
        // if current cell is already explored
        if (board[posX][posY][2] == "E")
        {
            cout << endl
                 << "This cell is already explored." << endl;
        }
        // if current cell contains gopher, user loses.
        else if (board[posX][posY][1] == "G")
        {
            cout << endl
                 << "Landed on gopher." << endl;
            cout << "You loose !" << endl;
            exit(0);
        }
        // if current cell is not explored,  and not contains gophers then,
        else
        {
            // explore it

            // marking the current cell as explored cell.
            board[posX][posY][2] = "E";

            // get neighbour gopher count
            int c = getNeighbourGopherCount(posX, posY);

            // add it into the current cell
            board[posX][posY][0] = to_string(c);

            // decrease the number of unexplored cell by one.
            unexplored--;
            // clear the screen
            clearScreen();
            // render the current playing board layer 0
            renderBoard(0);

            // if number of gophers in neighbour is 0 then
            // all neighbour are safe to explore
            if (c == 0)
                // explore all neighbour cells.
                exploreSafeNeighbours(posX, posY);
        }
    }
}

void clearScreen()
{
    system("cls");
}