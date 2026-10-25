#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define ENABLE_SNAKE

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

#ifdef ENABLE_SNAKE // START NG SNAKE GAME (HANGGANG #IFEND SA DULO NG FILE NA TO) "CURRENTLY 618 LINE NG CODE"

const int FIELD_LEFT   = 6; // Snake field dimensions (baguhin if lalakihan yung phone)
const int FIELD_RIGHT  = 37;
const int FIELD_TOP    = 6;
const int FIELD_BOTTOM = 23;
const int MAX_SNAKE    = 500;   // the field can never hold more than this

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
                    // swap the data of the two nodes
                    Run temp = current->data;
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

    snakeX[0] = 21;  snakeY[0] = 15;    // head
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
    apps.push_back({"Snake",   "S"});
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
            else if (selectedApp->data.name == "Snake") {
                launchSnake(selectedApp->data);
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