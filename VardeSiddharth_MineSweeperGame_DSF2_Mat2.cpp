#include<iostream>
using namespace std;

class Tile
{
    int data;
    bool isMine;
    public:
    Tile()
    {
        data = -1;
        isMine = false;
    }

    void SetData(int value)
    {
        data = value;
    }

    int GetData()
    {
        return data;
    }

    void SetIsMine(bool isMine)
    {
        this->isMine = isMine;
    }

    bool GetIsMine()
    {
        return isMine;
    }
};

class Board
{
    int size = 9;
    Tile board[9][9];
    int numberOfMines = 10;
    bool isBoardSet;
    int openedTiles;

    /*void Clear()
    {
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                board[i][j].SetData(-1);
            }
        }
    }*/

    void MakeTileAMine(int x, int y)
    {
        board[x][y].SetIsMine(true);

        /*for(int a = 0, int i = x - 1, int j = y + 1; a < 3; a++, i++, j++)
        {
            if(i < 0 || j < 0)
            {
                continue;
            }
        }*/
    }

    void SetupBoard(int x, int y)
    {
        board[x][y].SetData(0);
        board[x][y].SetIsMine(false);
        srand(time(0));

        int randomX, randomY;
        for(int i = 0; i < numberOfMines; i++)
        {
            randomX = rand() % size;
            randomY = rand() % size;

            if((randomX == x && randomY == y) || (board[randomX][randomY].GetIsMine()))
            {
                i--;
                continue;
            }

            MakeTileAMine(randomX, randomY);
            //cout << "Mine at " << randomX << "," << randomY << endl;
        }
        isBoardSet = true;
    }

    bool IsValidPosition(int x, int y)
    {
        if(x < 0 || y < 0)
        {
            return false;
        }
        else if(x >= size || y >= size)
        {
            return false;
        }

        return true;
    }

    int CountMinesInAdjecentTiles(int x, int y)
    {
        int count = 0;

        //Check in Upper row
        for(int a = 0,i = x - 1,j = y - 1; a < 3; a++, i++)
        {
            if(IsValidPosition(i, j) && board[i][j].GetIsMine())
            {
                count++;
            }
        }

        //Check in Middle row
        for(int a = 0,i = x -1,j = y; a < 3; a++, i++)
        {
            if(IsValidPosition(i, j) && board[i][j].GetIsMine())
            {
                count++;
            }
        }

        //Check in Lower row
        for(int a = 0,i = x - 1,j = y + 1; a < 3; a++, i++)
        {
            if(IsValidPosition(i, j) && board[i][j].GetIsMine())
            {
                count++;
            }
        }

        return count;
    }

    void RevealNumberOfMinesNearby(int x, int y)
    {
        if(!IsValidPosition(x,y) || board[x][y].GetData() != -1 || board[x][y].GetIsMine())
        {
            return;
        }

        int count = CountMinesInAdjecentTiles(x, y);

        board[x][y].SetData(count);
        openedTiles++;
        
        //if there are no mines in Adjecent tiles then 
        //it will reveal the number of mines adjecent to those cell
        if(count == 0)
        {
            for(int a = 0,i = x -1,j = y - 1; a < 3; a++, i++)
            {
                RevealNumberOfMinesNearby(i,j);
            }

            //Check in Middle row
            for(int a = 0,i = x -1,j = y; a < 3; a++, i++)
            {
                if(i != x && j != y)
                {
                    RevealNumberOfMinesNearby(i,j);
                }
            }

            //Check in Lower row
            for(int a = 0,i = x - 1,j = y + 1; a < 3; a++, i++)
            {
                RevealNumberOfMinesNearby(i,j);
            }
        }
    }

    public:
    Board()
    {
        isBoardSet = false;
        openedTiles = 0;
        //Clear();
        Print();
    }

    void Print()
    {
        for(int i = 0; i < size; i++)
        {
            cout << "--------------------\n";
            cout << "|";
            for(int j = 0; j < size; j++)
            {
                int data = board[i][j].GetData();
                if(data != -1)
                {
                    cout << data << "|";
                }
                else
                {
                    cout << " |";
                }
            }
            cout << endl;
        }
        cout << "--------------------\n";
    }

    void PrintFinal()
    {
        for(int i = 0; i < size; i++)
        {
            cout << "--------------------\n";
            cout << "|";
            for(int j = 0; j < size; j++)
            {
                if(!board[i][j].GetIsMine())
                {
                    int data = board[i][j].GetData();
                    if(data != -1)
                    {
                        cout << data << "|";
                    }
                    else
                    {
                        cout << " |";
                    }
                }
                else
                {
                    cout << "*|";
                }
            }
            cout << endl;
        }
        cout << "--------------------\n";
    }

    int GetSize()
    {
        return size;
    }

    bool OpenTile(int x, int y)
    {
        if(!isBoardSet)
        {
            SetupBoard(x,y);
            openedTiles++;
            return false;
        }
        else
        {
            if(board[x][y].GetIsMine())
            {
                //cout << "found Mine\n";
                return true;
            }
            else
            {
                RevealNumberOfMinesNearby(x,y);
                return false;
            }
        }
    }

    bool HasOpenedAllTiles()
    {
        int totalTiles = size * size;
        return ((totalTiles - openedTiles - numberOfMines) == 0);
    }
};

class Input
{
    int maxInput;

    public:
    Input(int maxInput)
    {
        this->maxInput = maxInput;
    }

    int TakeInput()
    {
        int input;
        do{
            cout << "Input value between 0 to " << maxInput - 1 << endl;

            cin >> input;
        }while (input < 0 || input >= maxInput);

        return input;
    }
};

class GameplayManager
{
    Board *board;// = Board();
    Input *userInput;// = Input(board.GetSize());
    bool isGameOver;

    void Introduction()
    {
        cout << "----------------------------------\n";
        cout << "\tWlecome to\t\n";
        cout << "\tMineSwipeer\t\n";
        cout << "----------------------------------\n";
    }

    void GamePlay()
    {
        int x = userInput->TakeInput();
        int y = userInput->TakeInput();
        isGameOver = board->OpenTile(x,y);
        board->Print();
    }

    void GameResult()
    {
        cout << "\n\n--------------------\n";
        if(isGameOver)
        {
            cout << "\tYou Lose\n";
        }
        else if( board->HasOpenedAllTiles())
        {
            cout << "\tYou Win\n";
        }
        cout << "--------------------\n";

        board->PrintFinal();
    }

    public:
    GameplayManager()
    {
        Introduction();
        board = new Board();
        userInput = new Input(board->GetSize());
        isGameOver = false;
    }

    void GameLoop()
    {
        while (!isGameOver && !board->HasOpenedAllTiles())
        {
            GamePlay();
        }

        GameResult();
        cout << "Press any key to Exit" << endl;
        char x;
        cin >> x;
    }

    ~GameplayManager()
    {
        delete board;
        delete userInput;
    }
};

int main()
{    
    GameplayManager gp;
    gp.GameLoop();
    return 0;
}