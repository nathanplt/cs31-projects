// bees.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Room::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the room
const int MAXCOLS = 25;             // max number of columns in the room
const int MAXBEES = 150;            // max number of bees allowed
const int INITIAL_BEE_HEALTH = 2;

const int UP      = 0;
const int DOWN    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Room;  // This is needed to let the compiler know that Room is a
             // type name, since it's mentioned in the Bee declaration.

class Bee
{
  public:
        // Constructor
    Bee(Room* rp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();
    bool getSwatted(int dir);

  private:
    Room* m_room;
    int   m_row;
    int   m_col;
    bool m_swatted;
};

class Player
{
  public:
        // Constructor
    Player(Room *rp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

        // Mutators
    void   stand();
    void   moveOrSwat(int dir);
    void   setDead();

  private:
    Room* m_room;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

class Room
{
  public:
        // Constructor/destructor
    Room(int nRows, int nCols);
    ~Room();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     beeCount() const;
    int     numBeesAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;

        // Mutators
    bool   addBee(int r, int c);
    bool   addPlayer(int r, int c);
    bool   swatBeeAt(int r, int c, int dir);
    bool   moveBees();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Bee*    m_bees[MAXBEES];
    int     m_nBees;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nBees);
    ~Game();

        // Mutators
    void play();

  private:
    Room* m_room;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Bee implementation
///////////////////////////////////////////////////////////////////////////

Bee::Bee(Room* rp, int r, int c)
{
    if (rp == nullptr)
    {
        cout << "***** A bee must be created in some Room!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > rp->rows()  ||  c < 1  ||  c > rp->cols())
    {
        cout << "***** Bee created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_room = rp;
    m_row = r;
    m_col = c;
    m_swatted = false;
}

int Bee::row() const
{
    return m_row;
}

int Bee::col() const
{
    return m_col;
}

void Bee::move()
{
      // Attempt to move in a random direction; if bee can't move, don't move
    int dir = randInt(0, NUMDIRS-1);  // dir is now UP, DOWN, LEFT, or RIGHT

    m_room->determineNewPosition(m_row, m_col, dir);
}

bool Bee::getSwatted(int dir)  // return true if dies
{
    // If the bee has been swatted once before, return true
    // (since a second swat kills a bee).  Otherwise, if possible,
    // move the bee one position in direction dir and return false
    // (since it survived the swat).  Otherwise, do not move, but return
    // true (since stepping back causes the bee to die by smashing into a
    // wall of the room).
    
    if (m_swatted) {
        return true;
    }
    
    m_swatted = true;
    
    if (!m_room->determineNewPosition(m_row, m_col, dir)) {
        return true;
    }
    
    return false; 
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Room* rp, int r, int c)
{
    if (rp == nullptr)
    {
        cout << "***** The player must be created in some Room!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > rp->rows()  ||  c < 1  ||  c > rp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_room = rp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::moveOrSwat(int dir)
{
    m_age++;
    
    // If there is a bee adjacent to the player in the direction
    // dir, swat it.  Otherwise, move the player to that position if
    // possible (i.e., if the move would not be out of the room).
    
    int n_row = m_row;
    int n_col = m_col;
    
    if (!m_room->determineNewPosition(n_row, n_col, dir)) {
        return;
    }
    
    if (m_room->numBeesAt(n_row, n_col)) {
        m_room->swatBeeAt(n_row, n_col, dir);
        return;
    }
    
    m_row = n_row;
    m_col = n_col;
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Room implementations
///////////////////////////////////////////////////////////////////////////

Room::Room(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Room created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nBees = 0;
}

Room::~Room()
{
    delete m_player;
    
    for (int i=0; i<m_nBees; i++) {
        delete m_bees[i];
    }
}

int Room::rows() const
{
    return m_rows;
}

int Room::cols() const
{
    return m_cols;
}

Player* Room::player() const
{
    return m_player;
}

int Room::beeCount() const
{
    return m_nBees;
}

int Room::numBeesAt(int r, int c) const
{
    // Return the number of bees at row r, column c.
    
    int count = 0;
    for (int i=0; i<m_nBees; i++) {
        if (m_bees[i]->row() == r && m_bees[i]->col() == c) {
            count++;
        }
    }
    return count;
}

bool Room::determineNewPosition(int& r, int& c, int dir) const
{
    // If a move from row r, column c, one step in direction dir
    // would go out of the room, leave r and c unchanged and return false.
    // Otherwise, set r or c so that row r, column c, is now the new
    // position resulting from the proposed move, and return true.
    
    switch (dir)
    {
      case UP:
        if (r - 1 < 1) {
            return false;
        }
        r--;
        break;
      case DOWN:
        if (r + 1 > m_rows) {
            return false;
        }
        r++;
        break;
      case LEFT:
        if (c - 1 < 1) {
            return false;
        }
        c--;
        break;
      case RIGHT:
        if (c + 1 > m_cols) {
            return false;
        }
        c++;
        break;
      default:
        return false;
    }
    return true;
}

void Room::display() const
{
      // Position (row,col) in the room coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

      // Indicate each bee's position
    for (int i=0; i<m_nBees; i++) {
        r = m_bees[i]->row() - 1;
        c = m_bees[i]->col() - 1;
        
        if (grid[r][c] == '.') {
            grid[r][c] = 'B';
        } else if (grid[r][c] == 'B') {
            grid[r][c] = '2';
        } else if (grid[r][c] < '9') {
            grid[r][c]++;
        }
    }

      // Indicate player's position
    if (m_player != nullptr)
    {
          // Set the char to '@', unless there's also a bee there,
          // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, bee, and player info
    cout << endl;
    cout << "There are " << beeCount() << " bees remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Room::addBee(int r, int c)
{
      // If there are MAXBEES bees, return false.  Otherwise,
      // dynamically allocate a new Bee at coordinates (r,c).  Save the
      // pointer to the newly allocated Bee and return true.

      // Your function must work as specified in the preceding paragraph even
      // in this scenario (which won't occur in this game):  MAXBEES
      // are added, then some are destroyed, then more are added.
    
    if (m_nBees == MAXBEES) {
        return false;
    }
    
    m_bees[m_nBees] = new Bee(this, r, c);
    m_nBees++;
    return true;
}

bool Room::addPlayer(int r, int c)
{
      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Dynamically allocate a new Player and add it to the room
    m_player = new Player(this, r, c);
    return true;
}

bool Room::swatBeeAt(int r, int c, int dir)
{
    for (int i=0; i<m_nBees; i++) {
        Bee* curr = m_bees[i];
        
        if (curr->row() == r && curr->col() == c) {
            if (curr->getSwatted(dir)) {
                delete curr;
                m_bees[i] = m_bees[m_nBees-1];
                m_bees[m_nBees-1] = nullptr;
                m_nBees--;
                return true;
            }
            break;
        }
    }
    return false;
}

bool Room::moveBees()
{
    for (int k = 0; k < m_nBees; k++)
    {
      //        Have the k-th bee in the room make one move.
      //        If that move results in that bee being in the same
      //        position as the player, the player dies.
        
        m_bees[k]->move();
        if (m_bees[k]->row() == m_player->row() && m_bees[k]->col() == m_player->col()) {
            m_player->setDead();
        }
    }

      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nBees)
{
    if (nBees < 0)
    {
        cout << "***** Cannot create Game with negative number of bees!" << endl;
        exit(1);
    }
    if (nBees > MAXBEES)
    {
        cout << "***** Trying to create Game with " << nBees
             << " bees; only " << MAXBEES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nBees > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the bees!" << endl;
        exit(1);
    }

        // Create room
    m_room = new Room(rows, cols);

        // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_room->addPlayer(rPlayer, cPlayer);

      // Populate with bees
    while (nBees > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a bee where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_room->addBee(r, c);
        nBees--;
    }
}

Game::~Game()
{
    delete m_room;
}

void Game::play()
{
    m_room->display();
    Player* player = m_room->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_room->beeCount() > 0)
    {
        cout << endl;
        cout << "Move (u/d/l/r/q or nothing): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)  // player stands
            player->stand();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                player->moveOrSwat(decodeDirection(action[0]));
                break;
            }
        }
        m_room->moveBees();
        m_room->display();
    }
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rnd;
    static default_random_engine generator(rnd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}



///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

