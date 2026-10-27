#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define ENABLE_SNAKE
#define ENABLE_RPS

//key controls
const int KEY_UP    = 72;
const int KEY_DOWN  = 80;
const int KEY_LEFT  = 75;
const int KEY_RIGHT = 77;
const int KEY_ENTER = 13;
const int KEY_ESC   = 27;

//phone dimensions
const int PHONE_LEFT   = 4;
const int PHONE_RIGHT  = 40;
const int PHONE_TOP    = 3;
const int PHONE_BOTTOM = 27;

struct App {
    string name;
    string icon;
};

// Doubly Linked List Node
struct AppNode {
    App data;
    AppNode* prev;
    AppNode* next;

    AppNode(const App& appData) : data(appData), prev(nullptr), next(nullptr) {}
};

// Doubly Linked List Class
class AppList {
private:
    AppNode* head;
    AppNode* tail;
    int count;

public:
    AppList() : head(nullptr), tail(nullptr), count(0) {}

    ~AppList() {
        AppNode* current = head;
        while (current != nullptr) {
            AppNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const App& app) {
        AppNode* newNode = new AppNode(app);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    AppNode* getHead() const {
        return head;
    }

    int size() const {
        return count;
    }
};

//cursor control op
void gotoxy(short x, short y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Hide console blinking cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
// color settings
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Clear pixels in bezel
void clearScreen() {
    for (int y = PHONE_TOP; y <= PHONE_BOTTOM; ++y) {
        gotoxy(PHONE_LEFT, y);
        cout << "                                    ";
    }
}

// print phone frame
void drawPhoneFrame() {
    gotoxy(0, 0);
    setColor(8); // Dark gray frame
    cout << "  .-------------------------------------.\n";
    cout << "  |               (  O  )               |\n";
    cout << "  |-------------------------------------|\n";
    for (int i = 0; i < 25; ++i) {
        cout << "  |                                     |\n";
    }
    cout << "  |-------------------------------------|\n";
    cout << "  |                [ O ]                |\n";
    cout << "  '-------------------------------------'\n";
    setColor(7);
}

// home screen display
void renderHomeScreen(const AppList& apps, AppNode* selectedApp) {
    // Status bar
    gotoxy(5, 3);
    setColor(11); // Cyan text
    cout << "06:07 PM        1G [|||]";

    // App grid (2 Columns)
    AppNode* current = apps.getHead();
    int index = 0;

    while (current != nullptr) {
        short col = (index % 2 == 0) ? 6 : 18;
        short row = 6 + static_cast<short>(index / 2) * 3;

        gotoxy(col, row);
        if (current == selectedApp) {
            setColor(240); // Highlight selected app (Black text on White)
            cout << "[" << current->data.icon << " " << current->data.name << "]";
        } else {
            setColor(7); // Normal white text
            cout << " " << current->data.icon << " " << current->data.name << " ";
        }

        current = current->next;
        index++;
    }

    // Bottom dock (moved down a bit so a 4th app row fits above it)
    gotoxy(5, 19);
    setColor(8);
    cout << "---------------------";
    gotoxy(5, 20);
    setColor(14); // Yellow text
    cout << "Enter: Open | Esc: Off";
}

// Opened app view loop
void launchApp(const App& app) {
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    cout << "< Back         " << app.icon << " App";

    gotoxy(5, 6);
    setColor(15);
    cout << "Running: " << app.name;

    gotoxy(5, 8);
    setColor(7);
    cout << "Process ID: 4082";

    gotoxy(5, 15);
    setColor(8);
    cout << "---------------------";
    gotoxy(5, 16);
    setColor(14);
    cout << "Press [ESC] to Return";

    // Wait inside app screen until Esc or Backspace
    while (true) {
        int ch = _getch();
        if (ch == KEY_ESC || ch == 8) {
            break;
        }
    }

    clearScreen();
}

void spadeCard(){
    setColor(7);
    gotoxy(20, 11);
    cout <<"     *";
    gotoxy(20, 12);
    cout <<"	***";
    gotoxy(20, 13);
    cout <<"    *****";
    gotoxy(20, 14);
    cout <<"   *******";
    gotoxy(20, 15);
    cout <<"   ** * **";
    gotoxy(20, 16);
    cout <<"     *";
    gotoxy(20, 15);
    cout <<"    ***";		 
}

void heartCard(){
	
}

void launchCard(const App& app) {
    hideCursor();
    clearScreen();
    int selected = 0;
    int score = 0;
    bool running = true;
    
    AppList apps;
    apps.push_back({"Phone",   ">"});
    apps.push_back({"YouWho?", "#"});
    apps.push_back({"Camera",  "O"});
    apps.push_back({"Gallery", "*"});
    apps.push_back({"Music",   "~"});
    apps.push_back({"Settings","@"});
    
    gotoxy(5, 3);
    setColor(11);
    cout << "< Back         " << app.icon << " App";
    
    gotoxy(7,5);
    setColor(7);
    cout << "  _________________________\n";
    
    for(int i = 0; i < 15; i++){
        gotoxy(7,6+i);
        cout << "||                         |\n";
    }
    srand(time(0));
    int randomNum = rand() % 10 + 1;
	
    setColor(7);
    gotoxy(20, 11);
    cout <<"     *";
    gotoxy(20, 12);
    cout <<"	***";
    gotoxy(20, 13);
    cout <<"    *****";
    gotoxy(20, 14);
    cout <<"   *******";
    gotoxy(20, 15);
    cout <<"   ** * **";
    gotoxy(20, 16);
    cout <<"     *";
    gotoxy(20, 17);
    cout <<"    ***";		 
    while (running) {
        gotoxy(10,8);
        setColor(7);
        cout << randomNum << " ";
        int newNum = rand() % 10 + 1;
        gotoxy(29, 4);
        cout << "Score:" << score;
        while(randomNum == newNum){
            newNum = rand() % 10 + 1;
        }

        gotoxy(8, 26);
        if (selected == 0) {
            setColor(240); // Highlight selected app
            cout << "[" << "Higher" << " " << "^"<< "]";
            setColor(7); // Normal white text
            cout << "     " << "Lower" << " " << "v" << " ";
        } else {
            setColor(7); // Highlight selected app 
            cout << " " << "Higher" << " " << "^"<< "     ";
            setColor(240); // Normal white text
            cout << "[" << "Lower" << " " << "v" << "]";
        }

        int ch = _getch();
        if (ch == 224 || ch == 0) { // Arrow key prefix handling
            switch (_getch()) {
                case KEY_LEFT:
                    selected = 0;
                    break;
                case KEY_RIGHT:
                    selected = 1;
                    break;
            }
        } else if (ch == KEY_ENTER) {
            if(selected == 0){
                if(newNum > randomNum){
                    score +=1;
                    randomNum = newNum;
                    gotoxy(20, 22);
                    setColor(2);
                    cout << "/";
                    gotoxy(19, 23);
                    cout << "V";
                    Sleep(1000);
                    gotoxy(20, 22);
                    setColor(7);
                    cout << "/";
                    gotoxy(19, 23);
                    cout << "V";
                }else{
                    string usrname;
                    gotoxy(10,8);
                    cout << newNum;
                    gotoxy(19, 23);
                    cout <<"               ";
                    gotoxy(19, 22);
                    cout <<"GAME OVER [X]";
                    Sleep(1000);
                    gotoxy(19, 22);
                    setColor(4);
                    cout <<"GAME OVER [X]";
                    Sleep(1000);
                    gotoxy(19, 22);
                    cout <<"               ";
                    gotoxy(19, 22);
                    setColor(7);
                    cout <<"Name:";
                    showCursor();
                    cin >> usrname;
                    gotoxy(19, 22);
                    cout <<"               ";
                    hideCursor();
                    score = 0;
                }
            }else{
                if(newNum < randomNum){
                    score +=1;
                    randomNum = newNum;
                    gotoxy(20, 22);
                    setColor(2);
                    cout << "/";
                    gotoxy(19, 23);
                    cout << "V";
                    Sleep(1000);
                    gotoxy(20, 22);
                    setColor(7);
                    cout << "/";
                    gotoxy(19, 23);
                    cout << "V";
                }else{
                    string usrname;
                    setColor(7);
                    gotoxy(10,8);
                    cout << newNum;
                    gotoxy(19, 23);
                    cout <<"               ";
                    gotoxy(19, 22);
                    cout <<"GAME OVER [X]";
                    Sleep(1000);
                    gotoxy(19, 22);
                    setColor(4);
                    cout <<"GAME OVER [X]";
                    Sleep(1000);
                    gotoxy(19, 22);
                    cout <<"               ";
                    gotoxy(19, 22);
                    setColor(7);
                    cout <<"Name:";
                    showCursor();
                    cin >> usrname;
                    gotoxy(19, 22);
                    cout <<"               ";
                    hideCursor();
                    score = 0;
                }
            }
        } else if (ch == KEY_ESC) {
            running = false;
        }
    }

    clearScreen();
}
// =============================================================================================================================
#ifdef ENABLE_SNAKE // START NG SNAKE GAME (HANGGANG #IFEND SA DULO NG FILE NA TO) "CURRENTLY 618 LINE NG CODE"

const int FIELD_LEFT   = 6; // Snake field dimensions (baguhin if lalakihan yung phone)
const int FIELD_RIGHT  = 37;
const int FIELD_TOP    = 6;
const int FIELD_BOTTOM = 23;
const int MAX_SNAKE    = 500; 

struct Run {
    string nickname;
    int score;
    int duration; // set natin as seconds
    string date;
};

struct RunNode { // Doubly Linked List Node (same idea as AppNode) (The whole Snake game is NODESS!!! :>)
    Run data;
    RunNode* prev;
    RunNode* next;

    RunNode(const Run& runData) : data(runData), prev(nullptr), next(nullptr) {}
};

class RunList { // double linked list to save runs 
private:
    RunNode* head;
    RunNode* tail;
    int count;

public:
    RunList() : head(nullptr), tail(nullptr), count(0) {}

    ~RunList() {
        RunNode* current = head;
        while (current != nullptr) {
            RunNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const Run& run) { // add a new run at the end of the list
        RunNode* newNode = new RunNode(run);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void removeNode(RunNode* node) {  // remove a run from the list (the node is already found)
        if (node->prev != nullptr) { // fix the link of the node before it
            node->prev->next = node->next;
        } else {
            head = node->next;      // head is una pointing sa next node
        }

        if (node->next != nullptr) {    // fixed the link of the node after it
            node->next->prev = node->prev;
        } else {
            tail = node->prev;      // it was the last node
        }

        delete node;
        count--;
    }

    RunNode* search(const string& keyword) { // find the first run whose nickname contains the keyword
        RunNode* current = head;
        while (current != nullptr) {
            if (current->data.nickname.find(keyword) != string::npos) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // kung wala nakita
    }

    int getPosition(RunNode* target) const { // which number is this node in the list? 
        int position = 1;
        RunNode* current = head;
        while (current != nullptr) {
            if (current == target) {
                return position;
            }
            current = current->next;
            position++;
        }
        return 0;
    }

    void sortByScore(bool highToLow) { // Bubble sort by score (PARA MASAYA :>) :: highToLow = true  -> biggest score first;  highToLow = false -> smallest score first
        if (count < 2) {
            return;
        }

        bool swapped = true;
        while (swapped) {
            swapped = false;
            RunNode* current = head;

            while (current->next != nullptr) {
                bool needSwap = false;

                if (highToLow == true) {
                    if (current->data.score < current->next->data.score) {
                        needSwap = true;
                    }
                } else {
                    if (current->data.score > current->next->data.score) {
                        needSwap = true;
                    }
                }

                if (needSwap == true) {
                    Run temp = current->data; // swap the data of the two nodes
                    current->data = current->next->data;
                    current->next->data = temp;
                    swapped = true;
                }

                current = current->next;
            }
        }
    }

    RunNode* getHead() const { return head; }
    RunNode* getTail() const { return tail; }
    int size() const { return count; }
};

RunList snakeRuns; // this list lives as long as the program runs

string getTodayDate() { // Get today's date and time as text, example: 2026-10-23 21:45 (REMOVE NA LANG IF NECESSARY PARA SA SNAKE GAME, THIS IS JUST FOR SAVING THE RUNS)
    time_t now = time(0);               // current time in seconds
    tm* timeInfo = localtime(&now);     // break it into year, month, day...
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeInfo);
    return string(buffer);
}

string askText(string label, int row) { // Ask the player to type one word
    gotoxy(5, row);
    setColor(7);
    cout << label;
    showCursor();
    string text;
    cin >> text;
    hideCursor();

    if (text.length() > 10) {
        text = text.substr(0, 10);
    }
    return text;
}

void waitForKey() { // wait until the player presses any key (this is used to pause the screen so the player can read it)
    _getch();
}

void drawSnakeBorder() { // SNAKE MODEL (PALITAN KUNG GUSTO BAGUHIN KULAY)
    setColor(2);    // dark green para retro looks

    gotoxy(FIELD_LEFT, FIELD_TOP);    // WALL NAMAN TO (taas ito sa screen)
    cout << "+";
    for (int x = FIELD_LEFT + 1; x < FIELD_RIGHT; x++) {
        cout << "-";
    }
    cout << "+";

    for (int y = FIELD_TOP + 1; y < FIELD_BOTTOM; y++) { //side walls naman to
        gotoxy(FIELD_LEFT, y);
        cout << "|";
        gotoxy(FIELD_RIGHT, y);
        cout << "|";
    }

    gotoxy(FIELD_LEFT, FIELD_BOTTOM);
    cout << "+";
    for (int x = FIELD_LEFT + 1; x < FIELD_RIGHT; x++) {
        cout << "-";
    }
    cout << "+";
}

void clearSnakeField() {
    for (int y = FIELD_TOP + 1; y < FIELD_BOTTOM; y++) {
        gotoxy(FIELD_LEFT + 1, y);
        for (int x = FIELD_LEFT + 1; x < FIELD_RIGHT; x++) {
            cout << " ";
        }
    }
}

void drawSnakeScore(int score) {
    gotoxy(7, 4);
    setColor(10);
    cout << "SCORE: " << score << "    ";
}

bool isOnSnake(int snakeX[], int snakeY[], int length, int x, int y) { // point (x, y) on any part of the snake?
    for (int i = 0; i < length; i++) {
        if (snakeX[i] == x && snakeY[i] == y) {
            return true;
        }
    }
    return false;
}

void placeFood(int snakeX[], int snakeY[], int length, int& foodX, int& foodY) { // pick a random empty spot for the food (goodspot = not on the snake)
    bool goodSpot = false;
    while (goodSpot == false) {
        foodX = FIELD_LEFT + 1 + rand() % (FIELD_RIGHT - FIELD_LEFT - 1);
        foodY = FIELD_TOP + 1 + rand() % (FIELD_BOTTOM - FIELD_TOP - 1);

        if (isOnSnake(snakeX, snakeY, length, foodX, foodY) == false) {
            goodSpot = true;
        }
    }
}

void playSnake() { // for one round of the game, from start to game over. The score is saved at the end.
    srand(time(0));
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    cout << "< Back         S App";

    drawSnakeBorder();
    clearSnakeField();

    int snakeX[MAX_SNAKE]; // Step 1: create the snake (3 pieces muna, going right)
    int snakeY[MAX_SNAKE];
    int length = 3;

    snakeX[0] = 21;  snakeY[0] = 15;    // head of the snake
    snakeX[1] = 20;  snakeY[1] = 15;
    snakeX[2] = 19;  snakeY[2] = 15;

    int dirX = 1;       // moving right yung starting niya
    int dirY = 0;
    int score = 0;
    int delay = 120;    // ms between moves (smaller = faster) ps: edit na lang if gusto mo ng mas mabilis o mas mabagal
    bool playing = true;
    bool quitEarly = false;

    int foodX = 0;     // Step 2: place the first food
    int foodY = 0;
    placeFood(snakeX, snakeY, length, foodX, foodY);

    for (int i = 0; i < length; i++) {     // Step 3: the snake (@ this is the head po, and o is the body) and the food (*)
        gotoxy(snakeX[i], snakeY[i]);
        if (i == 0) {
            setColor(10);
            cout << "@";
        } else {
            setColor(2);
            cout << "o";
        }
    }
    gotoxy(foodX, foodY);
    setColor(12);
    cout << "*";

    drawSnakeScore(score);
    gotoxy(7, 25);
    setColor(14);
    cout << "Arrows: Move   Esc: Quit";

    time_t startTime = time(0);     // set to know when the game starts :>

    while (playing == true) { // Step 4: game loop (running game na to)

        if (_kbhit()) {       // 4a. check the keyboard (_kbhit = keyboard hit, meaning a key was pressed)
            int ch = _getch();
            if (ch == 224 || ch == 0) {
                int arrow = _getch();
                if (arrow == KEY_UP && dirY != 1) {
                    dirX = 0;
                    dirY = -1;
                } else if (arrow == KEY_DOWN && dirY != -1) {
                    dirX = 0;
                    dirY = 1;
                } else if (arrow == KEY_LEFT && dirX != 1) {
                    dirX = -1;
                    dirY = 0;
                } else if (arrow == KEY_RIGHT && dirX != -1) {
                    dirX = 1;
                    dirY = 0;
                }
            } else if (ch == KEY_ESC) {
                quitEarly = true;
                playing = false;
            }
        }

        if (playing == false) {
            break;
        }

        int newHeadX = snakeX[0] + dirX;    // 4b. head location after the move
        int newHeadY = snakeY[0] + dirY;

        if (newHeadX <= FIELD_LEFT || newHeadX >= FIELD_RIGHT || // 4c. naumpog ba ang ulo sa wall? (or out of bounds)
            newHeadY <= FIELD_TOP  || newHeadY >= FIELD_BOTTOM) {
            playing = false;
            break;
        }

        if (isOnSnake(snakeX, snakeY, length, newHeadX, newHeadY) == true) { // 4d. nabonnggu ba ang ulo sa sarili?
            playing = false;
            break;
        }

        bool ateFood = false; // 4e. did the snake reach the food? 
        if (newHeadX == foodX && newHeadY == foodY) {
            ateFood = true;
        }

        if (ateFood == false) {
            gotoxy(snakeX[length - 1], snakeY[length - 1]);
            cout << " ";
        } else {
            length++;
        }

        for (int i = length - 1; i > 0; i--) { // 4f. move every piece to where the piece in front of it was
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        snakeX[0] = newHeadX;
        snakeY[0] = newHeadY;

        gotoxy(snakeX[1], snakeY[1]); // 4g. draw the old head as body, and the new head
        setColor(2);
        cout << "o";
        gotoxy(snakeX[0], snakeY[0]);
        setColor(10);
        cout << "@";

        if (ateFood == true) {    // 4h. if we ate, add score, go faster, and place new food (TODO: make sure the new food is not on the snake)
            score = score + 10;
            drawSnakeScore(score);

            if (delay > 60) {
                delay = delay - 3;
            }

            placeFood(snakeX, snakeY, length, foodX, foodY);
            gotoxy(foodX, foodY);
            setColor(12);
            cout << "*";
        }

        if (dirY != 0) { // 4i. wait a little (moving up/down looks faster, so wait longer!!!)
            Sleep(delay + 40);
        } else {
            Sleep(delay);
        }
    }

    if (quitEarly == true) { // if the player pressed Esc, leave without saving (THIS IS TO SAVE MEMORY, because the player might not want to save a run that is not finished)
        return;
    }

    time_t endTime = time(0);    // Step 5: game over - find out how long the game lasted
    int duration = static_cast<int>(endTime - startTime);

    Sleep(500);
    while (_kbhit()) {
        _getch();       // throw away keys pressed while crashing
    }

    gotoxy(12, 12);
    setColor(12);
    cout << "*** GAME OVER ***";
    gotoxy(12, 14);
    setColor(10);
    cout << "Score: " << score;
    gotoxy(12, 15);
    cout << "Time : " << duration << " sec";

    string nickname = askText("Nickname: ", 17); // Step 6: ask for a nickname and save the run

    Run newRun;
    newRun.nickname = nickname;
    newRun.score = score;
    newRun.duration = duration;
    newRun.date = getTodayDate();
    snakeRuns.push_back(newRun);

    gotoxy(12, 19);
    setColor(14);
    cout << "Run saved!";
    gotoxy(8, 21);
    setColor(8);
    cout << "Press any key...";
    waitForKey();
}

void drawRunRecord(RunNode* current, bool highToLow, string message) { // history run section (the player can scroll through them, sort, search, rename, delete)
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    cout << "< Back       Run History";

    gotoxy(5, 5);
    setColor(8);
    if (highToLow == true) {
        cout << "Order: Highest -> Lowest";
    } else {
        cout << "Order: Lowest -> Highest";
    }

    gotoxy(5, 7);
    setColor(15);
    cout << "Run " << snakeRuns.getPosition(current) << " of " << snakeRuns.size();

    gotoxy(5, 9);
    setColor(7);
    cout << "Nickname: " << current->data.nickname;
    gotoxy(5, 10);
    cout << "Score   : " << current->data.score;
    gotoxy(5, 11);
    cout << "Duration: " << current->data.duration << " sec";
    gotoxy(5, 12);
    cout << "Date    : " << current->data.date;

    gotoxy(5, 14);
    setColor(8);
    if (current->prev == nullptr) {
        cout << "(first run)";
    }
    if (current->next == nullptr) {
        gotoxy(5, 15);
        cout << "(last run)";
    }

    gotoxy(5, 17);
    setColor(12);
    cout << message;

    gotoxy(5, 21);     // THIS IS THE CONTROL!!!!!!
    setColor(14);
    cout << "<-Prev ->Next H/L:Sort F:Find";
    gotoxy(5, 22);
    cout << "U:Rename  D:Delete  Esc:Back";
}

void showRunHistory() { // HISTORY of all runs (the player can scroll through them, sort, search, rename, delete)
    if (snakeRuns.size() == 0) {
        clearScreen();
        gotoxy(5, 3);
        setColor(11);
        cout << "< Back       Run History";
        gotoxy(8, 10);
        setColor(7);
        cout << "No runs saved yet.";
        gotoxy(8, 12);
        cout << "Play a game first!";
        gotoxy(8, 21);
        setColor(8);
        cout << "Press any key...";
        waitForKey();
        return;
    }

    RunNode* current = snakeRuns.getHead();     // start with the first run
    bool highToLow = true;
    bool viewing = true;
    string message = "";

    while (viewing == true) {
        drawRunRecord(current, highToLow, message);
        message = "";

        int ch = _getch();

        if (ch == 224 || ch == 0) {
            int arrow = _getch();

            if (arrow == KEY_LEFT) { // PREV = go to the previous run
                if (current->prev != nullptr) {
                    current = current->prev;
                } else {
                    message = "Already at the first run.";
                }
            }
            else if (arrow == KEY_RIGHT) { // NEXT = go to the next run
                if (current->next != nullptr) {
                    current = current->next;
                } else {
                    message = "Already at the last run.";
                }
            }
        }
        else if (ch == 'h' || ch == 'H') {
            highToLow = true; // SORT: highest score first
            snakeRuns.sortByScore(true);
            current = snakeRuns.getHead();
            message = "Sorted: highest first.";
        }
        else if (ch == 'l' || ch == 'L') {
            highToLow = false; // SORT: lowest score first
            snakeRuns.sortByScore(false);
            current = snakeRuns.getHead();
            message = "Sorted: lowest first.";
        }
        else if (ch == 'f' || ch == 'F') {
            string keyword = askText("Search name: ", 19); // SEARCH by nickname
            RunNode* found = snakeRuns.search(keyword);
            if (found != nullptr) {
                current = found;
                message = "Found!";
            } else {
                message = "No run with that name.";
            }
        }
        else if (ch == 'u' || ch == 'U') {
            string newName = askText("New nickname: ", 19); // UPDATE the nickname
            current->data.nickname = newName;
            message = "Nickname updated.";
        }
        else if (ch == 'd' || ch == 'D') {
            RunNode* showAfter = current->next; // DELETE the current run and show the next one (or previous if it was the last run)
            if (showAfter == nullptr) {
                showAfter = current->prev;
            }

            snakeRuns.removeNode(current);
            current = showAfter;

            if (current == nullptr) { // if nothing is left, leave the screen
                clearScreen();
                gotoxy(8, 10);
                setColor(7);
                cout << "All runs deleted.";
                gotoxy(8, 21);
                setColor(8);
                cout << "Press any key...";
                waitForKey();
                viewing = false;
            } else {
                message = "Run deleted.";
            }
        }
        else if (ch == KEY_ESC) {
            viewing = false;
        }
    }
}

void launchSnake(const App& app) { // START MENU of the Snake app (Play, Run History, Back)
    hideCursor();

    string options[3] = { "Play", "Run History", "Back" };
    int selected = 0;
    bool inMenu = true;

    while (inMenu == true) {
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        cout << "< Back         " << app.icon << " App";

        gotoxy(14, 7);
        setColor(10);
        cout << "S N A K E";

        for (int i = 0; i < 3; i++) { // draw the 3 options
            gotoxy(13, 11 + i * 2);
            if (i == selected) {
                setColor(240); // highlighted
                cout << "[ " << options[i] << " ]";
            } else {
                setColor(7);
                cout << "  " << options[i] << "  ";
            }
        }

        gotoxy(5, 25);
        setColor(14);
        cout << "Up/Down: Move   Enter: Pick";

        int ch = _getch();
        if (ch == 224 || ch == 0) {
            int arrow = _getch();
            if (arrow == KEY_UP && selected > 0) {
                selected--;
            } else if (arrow == KEY_DOWN && selected < 2) {
                selected++;
            }
        }
        else if (ch == KEY_ENTER) {
            if (selected == 0) {
                playSnake();
            } else if (selected == 1) {
                showRunHistory();
            } else {
                inMenu = false;
            }
        }
        else if (ch == KEY_ESC) {
            inMenu = false;
        }
    }

    setColor(7);
    clearScreen();
}
#endif // GOING BACK TO THE MAIN APP AND *END OF THE SNAKE GAME CODE* 
// =============================================================================================================================
#ifdef ENABLE_RPS // START NG RPS GAME (HANGGANG #endif SA DULO NG BLOCK NA TO)

const int RPS_TARGET = 2;   // points needed to win a match (NAKA SET SA 2 PTS, CHANGE NA LANG IF GUSTO MO)
const int QUIT_PICK  = 3;   // special value: the player pressed Esc
const string CHOICE_NAMES[3] = { "Rock", "Paper", "Scissors" };   // 0 = Rock, 1 = Paper, 2 = Scissors

const string RPS_ART[3][6] = { // 6 lines lang para kasya sa mismong screen ng phone, 3 choices lang (Rock, Paper, Scissors)
    {   // ROCK
        "    _______   ",
        "---'   ____)  ",
        "      (_____) ",
        "      (_____) ",
        "      (____)  ",
        "---.__(___)   "
    },
    {   // PAPER
        "    _______       ",
        "---'   ____)____  ",
        "          ______) ",
        "          _______)",
        "         _______) ",
        "---.__________)   "
    },
    {   // SCISSORS
        "    _______       ",
        "---'   ____)____  ",
        "          ______) ",
        "       __________)",
        "      (____)      ",
        "---.__(___)       "
    }
};

struct Rival { // Rival Registry: one rival + their stats (DATA PART)
    string name;
    int wins;
    int losses;
    int matches;
};

struct Battle { // Battle Archive: one finished match
    string player1;
    string player2;
    string winner;
    string choices; // every round, example: "R-S P-P S-R "
    string date;
    int score1;
    int score2;
    int rounds;
};

int winRate(const Rival& r) { // win rate in percent
    if (r.matches == 0) {
        return 0;
    }
    return r.wins * 100 / r.matches;
}

int getValue(const Rival& r, char mode) { // the number we sort by
    if (mode == 'W') {
        return r.wins;
    } else if (mode == 'R') {
        return winRate(r);
    }
    return r.matches;
}

struct RivalNode { // Doubly Linked List Node (same idea sa Snake) (The whole RPS game is NODESS!!! :>)
    Rival data;
    RivalNode* prev;
    RivalNode* next;

    RivalNode(const Rival& rivalData) : data(rivalData), prev(nullptr), next(nullptr) {}
};

class RivalList {
private:
    RivalNode* head;
    RivalNode* tail;
    int count;

public:
    RivalList() : head(nullptr), tail(nullptr), count(0) {}

    ~RivalList() {
        RivalNode* current = head;
        while (current != nullptr) {
            RivalNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const Rival& rival) {    // add a new rival at the end
        RivalNode* newNode = new RivalNode(rival);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    RivalNode* search(const string& name) {     // find the rival with this exact name
        RivalNode* current = head;
        while (current != nullptr) {
            if (current->data.name == name) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void sortBy(char mode) {    // Bubble sort, biggest first. mode: W = wins, R = win rate, M = matches
        if (count < 2) {
            return;
        }
        bool swapped = true;
        while (swapped) {
            swapped = false;
            RivalNode* current = head;
            while (current->next != nullptr) {
                if (getValue(current->data, mode) < getValue(current->next->data, mode)) {
                    Rival temp = current->data;     // swap the data of the two nodes
                    current->data = current->next->data;
                    current->next->data = temp;
                    swapped = true;
                }
                current = current->next;
            }
        }
    }

    RivalNode* getHead() const { return head; }
    RivalNode* getTail() const { return tail; }
    int size() const { return count; }
};

struct BattleNode { // Doubly Linked List Node (same idea sa Snake) (The whole RPS game is NODESS!!! :>)
    Battle data;
    BattleNode* prev;
    BattleNode* next;

    BattleNode(const Battle& battleData) : data(battleData), prev(nullptr), next(nullptr) {}
};

class BattleList {
private:
    BattleNode* head;
    BattleNode* tail;
    int count;

public:
    BattleList() : head(nullptr), tail(nullptr), count(0) {}

    ~BattleList() {
        BattleNode* current = head;
        while (current != nullptr) {
            BattleNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const Battle& battle) {  // add a new battle at the end
        BattleNode* newNode = new BattleNode(battle);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    int getPosition(BattleNode* target) const { // which number is this battle in the list?
        int position = 1;
        BattleNode* current = head;
        while (current != nullptr) {
            if (current == target) {
                return position;
            }
            current = current->next;
            position++;
        }
        return 0;
    }

    BattleNode* getHead() const { return head; }
    int size() const { return count; }
};

RivalList rpsRivals;    // these two lists live as long as the program runs
BattleList rpsBattles;

string rpsToday() {     // today's date and time as text (Sae as Snake's getTodayDate, remove if not needed)
    time_t now = time(0);
    tm* timeInfo = localtime(&now);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeInfo);
    return string(buffer);
}

string rpsAsk(string label, int row) {  // ask the player to type one word (max 10 letters)
    gotoxy(5, row);
    setColor(7);
    cout << label;
    showCursor();
    string text;
    cin >> text;
    hideCursor();
    if (text.length() > 10) {
        text = text.substr(0, 10);
    }
    return text;
}

void rpsWait() { //para hindi agad nagra-run 
    _getch();
}

void drawRpsHeader(string title) { // clear the screen and print the top bar
    clearScreen();
    gotoxy(5, 3);
    setColor(11);
    cout << "< Back   " << title;
}

void clearRows(int from, int to) {      // erase only some rows of the phone screen (para walang flicker)
    for (int y = from; y <= to; y++) {
        gotoxy(4, y);
        cout << string(36, ' ');
    }
}

void showEmpty(string title) {          // message when nothing is saved yet
    drawRpsHeader(title);
    gotoxy(8, 10);
    setColor(7);
    cout << "Nothing saved yet.";
    gotoxy(8, 12);
    cout << "Play a match first!";
    gotoxy(8, 21);
    setColor(8);
    cout << "Press any key...";
    rpsWait();
}

int roundResult(int a, int b) {     // 0 = draw, 1 = first player wins, 2 = second player wins
    if (a == b) {
        return 0;
    }
    if ((a + 1) % 3 == b) {         // the next number beats the one before it (paper beats rock...)
        return 2;
    }
    return 1;
}

void drawHand(int choice, int x, int y) {   // print one hand (6 lines of ASCII art)
    for (int i = 0; i < 6; i++) {
        gotoxy(x, y + i);
        cout << RPS_ART[choice][i];
    }
}

void drawFists(int y, int color, string word) {     // both fists (rock) at row y, with a word under them
    clearRows(8, 17);
    setColor(color);
    drawHand(0, 4, y);      // left hand = player 1
    drawHand(0, 22, y);     // right hand = player 2
    gotoxy(5, 17);
    cout << word;
}

void shakeAnimation() {     // fists pump up and down while counting down
    string words[3] = { "ROCK...", "PAPER...", "SCISSORS..." };
    for (int beat = 0; beat < 3; beat++) {
        for (int up = 1; up >= 0; up--) {
            int y = 8;      // fists up
            if (up == 0) {
                y = 10;     // fists down (PARA MAY ALOG)
            }
            drawFists(y, 10, words[beat]);
            Sleep(220);
        }
    }
    drawFists(8, 14, "SHOOT!");
    Sleep(400);
}

void blinkResult(string text, int color) {  // blink the result text (AESTHETIC WISE LANG, REMOVE IF GUSTO MO)
    for (int i = 0; i < 5; i++) {
        gotoxy(5, 17);
        if (i % 2 == 0) {
            setColor(color);
        } else {
            setColor(8);
        }
        cout << ">> " << text << " <<";
        Sleep(250);
    }
}

int askChoice(string name) {    // the player picks a hand (keys are hidden so player 2 can't peek)
    setColor(14);
    gotoxy(5, 9);
    cout << name << ", pick your hand:";
    gotoxy(5, 11);
    cout << "R:Rock  P:Paper  S:Scissors";

    int pick = -1;
    while (pick == -1) {
        int ch = _getch();
        if (ch == 'r' || ch == 'R') {
            pick = 0;
        } else if (ch == 'p' || ch == 'P') {
            pick = 1;
        } else if (ch == 's' || ch == 'S') {
            pick = 2;
        } else if (ch == KEY_ESC) {
            pick = QUIT_PICK;
        }
    }

    clearRows(9, 11);   // erase the prompt
    return pick;
}

RivalNode* getRival(string name) {  // find the rival, or register a new one
    RivalNode* found = rpsRivals.search(name);
    if (found != nullptr) {
        return found;
    }
    Rival newRival = { name, 0, 0, 0 };
    rpsRivals.push_back(newRival);
    return rpsRivals.getTail();
}

void recordMatch(const Battle& battle) {    // save the match in the archive, and auto update na rin sa stats ng players
    RivalNode* rival1 = getRival(battle.player1);
    RivalNode* rival2 = getRival(battle.player2);

    rival1->data.matches++;
    rival2->data.matches++;

    if (battle.winner == battle.player1) {
        rival1->data.wins++;
        rival2->data.losses++;
    } else {
        rival2->data.wins++;
        rival1->data.losses++;
    }
    rpsBattles.push_back(battle);
}

void playMatch(string p1, string p2, bool vsComputer) { // one match, from start to the end. Saved at the end. (THE ACTUAL MATCH)
    int score1 = 0;
    int score2 = 0;
    int round = 0;
    string choices = "";
    bool quit = false;

    while (score1 < RPS_TARGET && score2 < RPS_TARGET && quit == false) {
        round++;

        drawRpsHeader("Round " + to_string(round));     // Step 1: show the scoreboard
        gotoxy(5, 5);
        setColor(15);
        cout << p1 << " " << score1 << " - " << score2 << " " << p2;

        int pick1 = askChoice(p1);      // Step 2: both players choose
        int pick2 = QUIT_PICK;
        if (pick1 != QUIT_PICK) {
            if (vsComputer == true) {
                pick2 = rand() % 3;     // the computer picks randomly
            } else {
                pick2 = askChoice(p2);
            }
        }

        if (pick1 == QUIT_PICK || pick2 == QUIT_PICK) {
            quit = true;
            break;
        }

        gotoxy(4, 7);                   // Step 3: names above the hands, then the countdown animation
        setColor(15);
        cout << p1;
        gotoxy(22, 7);
        cout << p2;
        shakeAnimation();

        int result = roundResult(pick1, pick2);     // Step 4: who won this round? (also pick the color + message)
        int color = 14;                             // draw = yellow
        string message = "IT'S A DRAW!";
        if (result == 1) {
            score1++;
            color = 10;                             // player 1 wins = green
            message = p1 + " WINS!";
        } else if (result == 2) {
            score2++;
            color = 12;                             // player 2 wins = red
            message = p2 + " WINS!";
        }

        clearRows(8, 17);               // Step 5: reveal both hands in the result color
        setColor(color);
        drawHand(pick1, 4, 8);
        drawHand(pick2, 22, 8);
        gotoxy(4, 15);
        cout << CHOICE_NAMES[pick1];
        gotoxy(22, 15);
        cout << CHOICE_NAMES[pick2];
        blinkResult(message, color);

        choices = choices + CHOICE_NAMES[pick1][0] + "-" + CHOICE_NAMES[pick2][0] + " ";  // remember the round (first letters, ex: R-S)

        gotoxy(5, 22);
        setColor(8);
        cout << "Press any key...";
        rpsWait();
    }

    if (quit == true) {     // Esc = leave without saving
        return;
    }

    string winner = p2;     // Step 6: match over, save the battle
    if (score1 > score2) {
        winner = p1;
    }

    Battle battle = { p1, p2, winner, choices, rpsToday(), score1, score2, round };
    recordMatch(battle);

    drawRpsHeader("Match Over");
    gotoxy(11, 9);
    setColor(12);
    cout << "*** MATCH OVER ***";
    gotoxy(11, 11);
    setColor(10);
    cout << winner << " WINS!";
    gotoxy(11, 13);
    cout << "Score: " << score1 << " - " << score2;
    gotoxy(11, 15);
    setColor(14);
    cout << "Battle saved!";
    gotoxy(8, 21);
    setColor(8);
    cout << "Press any key...";
    rpsWait();
}

void startMatch(bool vsComputer) {      // ask for the names, then play
    drawRpsHeader("New Match");
    string p1 = rpsAsk("Player 1 name: ", 8);
    string p2 = "Computer";
    if (vsComputer == false) {
        p2 = rpsAsk("Player 2 name: ", 10);
    }
    if (p1 == p2) {         // two different rivals can't share a name
        p2 = p2 + "2";
    }
    playMatch(p1, p2, vsComputer);
}

void drawBattle(BattleNode* current, string message) {   // show one saved battle (THE BATTLE ARCHIVED IN THE LIST)
    drawRpsHeader("Battle Archive");

    gotoxy(5, 5);
    setColor(15);
    cout << "Battle " << rpsBattles.getPosition(current) << " of " << rpsBattles.size();

    gotoxy(5, 7);
    setColor(7);
    cout << "Date   : " << current->data.date;
    gotoxy(5, 8);
    cout << "Fighter: " << current->data.player1 << " vs " << current->data.player2;
    gotoxy(5, 9);
    cout << "Score  : " << current->data.score1 << " - " << current->data.score2;
    gotoxy(5, 10);
    cout << "Winner : " << current->data.winner;
    gotoxy(5, 11);
    cout << "Rounds : " << current->data.rounds;
    gotoxy(5, 12);
    cout << "Picks (P1-P2):";
    gotoxy(5, 13);
    cout << current->data.choices.substr(0, 30);

    gotoxy(5, 15);
    setColor(12);
    cout << message;

    gotoxy(5, 21);
    setColor(14);
    cout << "<-Prev  ->Next  Esc:Back";
}

void showArchive() {    // scroll through the battles with the arrow keys
    if (rpsBattles.size() == 0) {
        showEmpty("Battle Archive");
        return;
    }

    BattleNode* current = rpsBattles.getHead();
    bool viewing = true;
    string message = "";

    while (viewing == true) {
        drawBattle(current, message);
        message = "";

        int ch = _getch();
        if (ch == 224 || ch == 0) {
            int arrow = _getch();
            if (arrow == KEY_LEFT) {
                if (current->prev != nullptr) {
                    current = current->prev;
                } else {
                    message = "This is the first battle.";
                }
            } else if (arrow == KEY_RIGHT) {
                if (current->next != nullptr) {
                    current = current->next;
                } else {
                    message = "This is the last battle.";
                }
            }
        } else if (ch == KEY_ESC) {
            viewing = false;
        }
    }
}

void showRankings() {   // list the rivals, sorted by wins / win rate / matches (RANKING NAMAN ITO)
    if (rpsRivals.size() == 0) {
        showEmpty("Rival Rankings");
        return;
    }

    char mode = 'W';
    bool viewing = true;

    while (viewing == true) {
        rpsRivals.sortBy(mode);
        drawRpsHeader("Rival Rankings");

        gotoxy(5, 5);
        setColor(8);
        if (mode == 'W') {
            cout << "Sorted by: Wins";
        } else if (mode == 'R') {
            cout << "Sorted by: Win rate";
        } else {
            cout << "Sorted by: Matches";
        }

        gotoxy(5, 7);   // column titles
        setColor(15);
        cout << "#  Name        W   L   M   %";

        RivalNode* current = rpsRivals.getHead();   // one row per rival (max 12 rows fit)
        int rank = 1;
        setColor(7);
        while (current != nullptr && rank <= 12) {
            int row = 7 + rank;
            gotoxy(5, row);
            cout << rank;
            gotoxy(8, row);
            cout << current->data.name;
            gotoxy(20, row);
            cout << current->data.wins;
            gotoxy(24, row);
            cout << current->data.losses;
            gotoxy(28, row);
            cout << current->data.matches;
            gotoxy(32, row);
            cout << winRate(current->data) << "%";
            current = current->next;
            rank++;
        }

        gotoxy(5, 22);
        setColor(14);
        cout << "W:Wins  R:Rate  M:Matches";
        gotoxy(5, 23);
        cout << "Esc:Back";

        int ch = _getch();
        if (ch == 'w' || ch == 'W') {
            mode = 'W';
        } else if (ch == 'r' || ch == 'R') {
            mode = 'R';
        } else if (ch == 'm' || ch == 'M') {
            mode = 'M';
        } else if (ch == KEY_ESC) {
            viewing = false;
        }
    }
}

void launchRps(const App& app) { //LOGIN MENU of the RPS app (Vs Computer, 2 Players, Rankings, Battle Archive, Back)
    hideCursor();
    srand(time(0));

    string options[5] = { "Vs Computer", "2 Players", "Rankings", "Battle Archive", "Back" };
    int selected = 0;
    bool inMenu = true;

    while (inMenu == true) {
        drawRpsHeader("      " + app.icon + " App");

        gotoxy(10, 5);
        setColor(14);
        cout << "ROCK PAPER SCISSORS";

        for (int i = 0; i < 5; i++) {   // draw the 5 options
            gotoxy(11, 9 + i * 2);
            if (i == selected) {
                setColor(240);
                cout << "[ " << options[i] << " ]";
            } else {
                setColor(7);
                cout << "  " << options[i] << "  ";
            }
        }

        gotoxy(5, 25);
        setColor(14);
        cout << "Up/Down: Move   Enter: Pick";

        int ch = _getch();
        if (ch == 224 || ch == 0) {
            int arrow = _getch();
            if (arrow == KEY_UP && selected > 0) {
                selected--;
            } else if (arrow == KEY_DOWN && selected < 4) {
                selected++;
            }
        } else if (ch == KEY_ENTER) {
            if (selected == 0) {
                startMatch(true);
            } else if (selected == 1) {
                startMatch(false);
            } else if (selected == 2) {
                showRankings();
            } else if (selected == 3) {
                showArchive();
            } else {
                inMenu = false;
            }
        } else if (ch == KEY_ESC) {
            inMenu = false;
        }
    }

    setColor(7);
    clearScreen();
}
#endif // END OF THE RPS GAME CODE
//==================================================================================================================================

int main() {
    hideCursor();
    system("cls");

    AppList apps;
    apps.push_back({"Phone",   ">"});
    apps.push_back({"YouWho?", "#"});
    apps.push_back({"Camera",  "O"});
    apps.push_back({"Gallery", "*"});
    apps.push_back({"Music",   "~"});
    apps.push_back({"Settings","@"});

#ifdef ENABLE_SNAKE
    apps.push_back({"BitBite", "S"});
#endif

#ifdef ENABLE_RPS
    apps.push_back({"Rock 'n' Clash", "R"});
#endif

    AppNode* selectedApp = apps.getHead();
    bool running = true;

    drawPhoneFrame();

    while (running && selectedApp != nullptr) {
        renderHomeScreen(apps, selectedApp);

        int ch = _getch();
        if (ch == 224 || ch == 0) { // Arrow key prefix handling
            switch (_getch()) {
                case KEY_UP:
                    // Move 2 nodes back for row navigation
                    if (selectedApp->prev && selectedApp->prev->prev) {
                        selectedApp = selectedApp->prev->prev;
                    }
                    break;

                case KEY_DOWN:
                    // Move 2 nodes forward for row navigation
                    if (selectedApp->next && selectedApp->next->next) {
                        selectedApp = selectedApp->next->next;
                    }
                    break;

                case KEY_LEFT:
                    // Move 1 node back
                    if (selectedApp->prev) {
                        selectedApp = selectedApp->prev;
                    }
                    break;

                case KEY_RIGHT:
                    // Move 1 node forward
                    if (selectedApp->next) {
                        selectedApp = selectedApp->next;
                    }
                    break;
            }
        } else if (ch == KEY_ENTER) {
            if (selectedApp->data.name == "YouWho?") {
                launchCard(selectedApp->data);
            }
#ifdef ENABLE_SNAKE
            else if (selectedApp->data.name == "BitBite") {
                launchSnake(selectedApp->data);
            }
#endif
#ifdef ENABLE_RPS
            else if (selectedApp->data.name == "Rock 'n' Clash") {
                launchRps(selectedApp->data);
            }
#endif
        } else if (ch == KEY_ESC) {
            running = false;
        }
    }

    clearScreen();
    gotoxy(10, 10);
    setColor(8);
    cout << "Device Turned Off";
    gotoxy(0, 20);
    setColor(7);
    return 0;
}