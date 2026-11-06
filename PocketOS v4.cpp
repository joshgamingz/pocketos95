#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cctype>

#ifndef nullptr
#define nullptr NULL  // Error handling for old devc++ versions
#endif

//key controls
const int KEY_UP     = 72;
const int KEY_DOWN   = 80;
const int KEY_LEFT   = 75;
const int KEY_RIGHT  = 77;
const int KEY_DELETE = 83;   // Delete key (comes after the 224 prefix, same as arrows)
const int KEY_ENTER  = 13;
const int KEY_ESC    = 27;

//phone dimensions
const int PHONE_LEFT   = 4;
const int PHONE_RIGHT  = 40;
const int PHONE_TOP    = 3;
const int PHONE_BOTTOM = 27;

void clearRows(int from, int to);   // (defined later in the RPS part, declared here so the home screen can use it)

std::string toLowerStr(const std::string& s) {   // lowercase copy of a string (for A-Z sorting and searching that ignores CAPS)
    std::string result = s;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

struct App {
    std::string name;
    std::string icon;
    std::string developer;
};


struct AppNode {
    App data;
    AppNode* prev;
    AppNode* next;

    AppNode(const App& appData) : data(appData), prev(nullptr), next(nullptr) {}
};


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

    void removeNode(AppNode* node) {   
        if (node == nullptr) {
            return;
        }

        if (node->prev != nullptr) {    
            node->prev->next = node->next;
        } else {
            head = node->next;          
        }

        if (node->next != nullptr) {    
            node->next->prev = node->prev;
        } else {
            tail = node->prev;         
        }

        delete node;
        count--;
    }

    void sortByName(bool ascending) {   
        if (count < 2) {
            return;
        }

        bool swapped = true;
        while (swapped) {
            swapped = false;
            AppNode* current = head;

            while (current->next != nullptr) {
                std::string a = toLowerStr(current->data.name);
                std::string b = toLowerStr(current->next->data.name);

                bool needSwap = false;
                if (ascending == true) {
                    needSwap = (a > b);
                } else {
                    needSwap = (a < b);
                }

                if (needSwap == true) {
                    App temp = current->data;   // swap the data of the two nodes
                    current->data = current->next->data;
                    current->next->data = temp;
                    swapped = true;
                }

                current = current->next;
            }
        }
    }

    AppNode* getHead() const {
        return head;
    }

    AppNode* getTail() const {
        return tail;
    }

    int size() const {
        return count;
    }
};


const int MAX_APPS = 20;    

AppList homeApps;           
AppList storeApps;          
int homeSortMode = 0;      

void applyHomeSort() {      
    if (homeSortMode == 1) {
        homeApps.sortByName(true);
    } else if (homeSortMode == 2) {
        homeApps.sortByName(false);
    }
}


int collectVisible(const AppList& apps, const std::string& filter, AppNode* out[]) {
    std::string key = toLowerStr(filter);
    int n = 0;
    AppNode* current = apps.getHead();
    while (current != nullptr && n < MAX_APPS) {
        if (key.empty() || toLowerStr(current->data.name).find(key) != std::string::npos) {
            out[n] = current;
            n++;
        }
        current = current->next;
    }
    return n;
}

int indexOfName(const AppList& apps, const std::string& name) {   
    int index = 0;
    AppNode* current = apps.getHead();
    while (current != nullptr) {
        if (current->data.name == name) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}

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
        std::cout << "                                    ";
    }
}

// print phone frame
void drawPhoneFrame() {
    gotoxy(0, 0);
    setColor(8); 
    std::cout << "  .-------------------------------------.\n";
    std::cout << "  |               (  O  )               |\n";
    std::cout << "  |-------------------------------------|\n";
    for (int i = 0; i < 25; ++i) {
        std::cout << "  |                                     |\n";
    }
    std::cout << "  |-------------------------------------|\n";
    std::cout << "  |                [ O ]                |\n";
    std::cout << "  '-------------------------------------'\n";
    setColor(7);
}

// home screen display

void renderHomeScreen(AppNode* visible[], int visCount, int selected,
                      const std::string& filter, bool searching, const std::string& message) {
    // Status bar
    gotoxy(5, 3);
    setColor(11); // Cyan text
    std::cout << "06:07 PM                   1G [|||]";

    clearRows(4, 23);   
    // Search bar
    gotoxy(5, 4);
    if (searching == true) {
        setColor(15);
        std::cout << "Search: " << filter << "_";
    } else {
        setColor(8);
        std::cout << "Search: (press F)";
    }

    // Sort label
    gotoxy(5, 5);
    setColor(8);
    if (homeSortMode == 1) {
        std::cout << "Sort: A-Z";
    } else if (homeSortMode == 2) {
        std::cout << "Sort: Z-A";
    } else {
        std::cout << "Sort: None";
    }

    // App grid (2 Columns)
    if (visCount == 0) {
        gotoxy(10, 10);
        setColor(7);
        std::cout << "No apps found.";
    }

    for (int index = 0; index < visCount; index++) {
        short col = (index % 2 == 0) ? 4 : 22;      // columns are 18 wide so even the longest name fits
        short row = 6 + static_cast<short>(index / 2) * 3;

        gotoxy(col, row);
        if (index == selected) {
            setColor(240); // Highlight selected app (Black text on White)
            std::cout << "[" << visible[index]->data.icon << " " << visible[index]->data.name << "]";
        } else {
            setColor(7); // Normal white text
            std::cout << " " << visible[index]->data.icon << " " << visible[index]->data.name << " ";
        }
    }

    // Bottom dock
    gotoxy(5, 19);
    setColor(8);
    std::cout << "---------------------";
    gotoxy(5, 20);
    setColor(14); 
    if (searching == true) {
        std::cout << "Type name  Enter:Open  Esc:Cancel";
    } else {
        std::cout << "Enter: Open | Esc: Off";
    }
    gotoxy(5, 21);
    if (searching == true) {
        std::cout << "Arrows:Select  Backspace:Erase";
    } else {
        std::cout << "S:Sort A-Z/Z-A F:Find Del:Remove";
    }

    // Message line
    gotoxy(5, 22);
    setColor(12);
    std::cout << message;
}

// Opened app view loop
void launchApp(const App& app) {
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back         " << app.icon << " App";

    gotoxy(5, 6);
    setColor(15);
    std::cout << "Running: " << app.name;

    gotoxy(5, 8);
    setColor(7);
    std::cout << "Process ID: 4082";

    gotoxy(5, 15);
    setColor(8);
    std::cout << "---------------------";
    gotoxy(5, 16);
    setColor(14);
    std::cout << "Press [ESC] to Return";

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
    std::cout <<"     *";
    gotoxy(20, 12);
    std::cout <<"	***";
    gotoxy(20, 13);
    std::cout <<"   *****";
    gotoxy(20, 14);
    std::cout <<"  *******";
    gotoxy(20, 15);
    std::cout <<"  ** * **";
    gotoxy(20, 16);
    std::cout <<"     *";
    gotoxy(20, 17);
    std::cout <<"    ***";
}

void choiceIncorrect(int newNum, int &score){
	std::string usrname;
    gotoxy(10,8);
    std::cout << newNum;
    gotoxy(19, 23);
    std::cout <<"               ";
    gotoxy(19, 22);
    std::cout <<"GAME OVER [X]";
    Sleep(1000);
    gotoxy(19, 22);
    setColor(4);
    std::cout <<"GAME OVER [X]";
    Sleep(1000);
    gotoxy(19, 22);
    std::cout <<"               ";
    gotoxy(19, 22);
    setColor(7);
    std::cout <<"Name:";
    showCursor();
    std::cin >> usrname;
    gotoxy(19, 22);
    std::cout <<"               ";
    hideCursor();
    score = 0;
}

void launchCard(const App& app) {
    hideCursor();
    clearScreen();
    int selected = 0;
    int score = 0;
    bool running = true;

    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back         " << app.icon << " App";

    gotoxy(7,5);
    setColor(7);
    std::cout << "  _________________________\n";

    for(int i = 0; i < 15; i++){
        gotoxy(7,6+i);
        std::cout << "||                         |\n";
    }
    srand(time(0));
    int randomNum = rand() % 10 + 1;

    spadeCard();
    while (running) {
        gotoxy(10,8);
        setColor(7);
        std::cout << randomNum << " ";
        int newNum = rand() % 10 + 1;
        gotoxy(29, 4);
        std::cout << "Score:" << score;
        while(randomNum == newNum){
            newNum = rand() % 10 + 1;
        }

        gotoxy(8, 26);
        if (selected == 0) {
            setColor(240); // Highlight selected app
            std::cout << "[" << "Higher" << " " << "^"<< "]";
            setColor(7); // Normal white text
            std::cout << "     " << "Lower" << " " << "v" << " ";
        } else {
            setColor(7); // Highlight selected app
            std::cout << " " << "Higher" << " " << "^"<< "     ";
            setColor(240); // Normal white text
            std::cout << "[" << "Lower" << " " << "v" << "]";
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
                    std::cout << "/";
                    gotoxy(19, 23);
                    std::cout << "V";
                    Sleep(1000);
                    gotoxy(20, 22);
                    setColor(7);
                    std::cout << "/";
                    gotoxy(19, 23);
                    std::cout << "V";
                }else{
                    choiceIncorrect(newNum, score);
                }
            }else{
                if(newNum < randomNum){
                    score +=1;
                    randomNum = newNum;
                    gotoxy(20, 22);
                    setColor(2);
                    std::cout << "/";
                    gotoxy(19, 23);
                    std::cout << "V";
                    Sleep(1000);
                    gotoxy(20, 22);
                    setColor(7);
                    std::cout << "/";
                    gotoxy(19, 23);
                    std::cout << "V";
                }else{
                    setColor(7);
                    choiceIncorrect(newNum, score);
                }
            }
        } else if (ch == KEY_ESC) {
            running = false;
        }
    }

    clearScreen();
}
//snake game

const int FIELD_LEFT   = 6; // snake field
const int FIELD_RIGHT  = 37;
const int FIELD_TOP    = 6;
const int FIELD_BOTTOM = 23;
const int MAX_SNAKE    = 500;

struct Run {
    std::string nickname;
    int score;
    int duration; // set natin as seconds
    std::string date;
};

struct RunNode { // Doubly Linked List Node (same idea as AppNode) 
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

    void push_back(const Run& run) { // add a new run at the end 
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

    void removeNode(RunNode* node) {  
        if (node->prev != nullptr) { 
            node->prev->next = node->next;
        } else {
            head = node->next;      
        }

        if (node->next != nullptr) {    
            node->next->prev = node->prev;
        } else {
            tail = node->prev;      
        }

        delete node;
        count--;
    }

    RunNode* search(const std::string& keyword) { 
        RunNode* current = head;
        while (current != nullptr) {
            if (current->data.nickname.find(keyword) != std::string::npos) {
                return current;
            }
            current = current->next;
        }
        return nullptr; 
    }

    int getPosition(RunNode* target) const { // counter
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

    void sortByScore(bool highToLow) { // Bubble sort by score 
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

RunList snakeRuns; 

std::string getTodayDate() { //
    time_t now = time(0);               // current time in seconds
    tm* timeInfo = localtime(&now);    
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeInfo);
    return std::string(buffer);
}

std::string askText(std::string label, int row) { // Ask the player to type one word
    gotoxy(5, row);
    setColor(7);
    std::cout << label;
    showCursor();
    std::string text;
    std::cin >> text;
    hideCursor();

    if (text.length() > 10) {
        text = text.substr(0, 10);
    }
    return text;
}

void waitForKey() { 
    _getch();
}

void drawSnakeBorder() { // SNAKE MODEL 
    setColor(2);    // dark green para retro looks

    gotoxy(FIELD_LEFT, FIELD_TOP);    // WALL NAMAN TO (taas ito sa screen)
    std::cout << "+";
    for (int x = FIELD_LEFT + 1; x < FIELD_RIGHT; x++) {
        std::cout << "-";
    }
    std::cout << "+";

    for (int y = FIELD_TOP + 1; y < FIELD_BOTTOM; y++) { //side walls naman to
        gotoxy(FIELD_LEFT, y);
        std::cout << "|";
        gotoxy(FIELD_RIGHT, y);
        std::cout << "|";
    }

    gotoxy(FIELD_LEFT, FIELD_BOTTOM);
    std::cout << "+";
    for (int x = FIELD_LEFT + 1; x < FIELD_RIGHT; x++) {
        std::cout << "-";
    }
    std::cout << "+";
}

void clearSnakeField() {
    for (int y = FIELD_TOP + 1; y < FIELD_BOTTOM; y++) {
        gotoxy(FIELD_LEFT + 1, y);
        for (int x = FIELD_LEFT + 1; x < FIELD_RIGHT; x++) {
            std::cout << " ";
        }
    }
}

void drawSnakeScore(int score) {
    gotoxy(7, 4);
    setColor(10);
    std::cout << "SCORE: " << score << "    ";
}

bool isOnSnake(int snakeX[], int snakeY[], int length, int x, int y) { // point (x, y) on any part of the snake?
    for (int i = 0; i < length; i++) {
        if (snakeX[i] == x && snakeY[i] == y) {
            return true;
        }
    }
    return false;
}

void placeFood(int snakeX[], int snakeY[], int length, int& foodX, int& foodY) { // random cords to place
    bool goodSpot = false;
    while (goodSpot == false) {
        foodX = FIELD_LEFT + 1 + rand() % (FIELD_RIGHT - FIELD_LEFT - 1);
        foodY = FIELD_TOP + 1 + rand() % (FIELD_BOTTOM - FIELD_TOP - 1);

        if (isOnSnake(snakeX, snakeY, length, foodX, foodY) == false) {
            goodSpot = true;
        }
    }
}

void playSnake() {  // triggers game
    srand(time(0));
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back         S App";

    drawSnakeBorder();
    clearSnakeField();

    int snakeX[MAX_SNAKE]; 
    int snakeY[MAX_SNAKE];
    int length = 3;

    snakeX[0] = 21;  snakeY[0] = 15;    // head of the snake
    snakeX[1] = 20;  snakeY[1] = 15;
    snakeX[2] = 19;  snakeY[2] = 15;

    int dirX = 1;       // moving right yung starting niya
    int dirY = 0;
    int score = 0;
    int delay = 120;    
    bool playing = true;
    bool quitEarly = false;

    int foodX = 0;     
    int foodY = 0;
    placeFood(snakeX, snakeY, length, foodX, foodY);

    for (int i = 0; i < length; i++) {     // printing head and body
        gotoxy(snakeX[i], snakeY[i]);
        if (i == 0) {
            setColor(10);
            std::cout << "@";
        } else {
            setColor(2);
            std::cout << "o";
        }
    }
    gotoxy(foodX, foodY);
    setColor(12);
    std::cout << "*";

    drawSnakeScore(score);
    gotoxy(7, 25);
    setColor(14);
    std::cout << "Arrows: Move   Esc: Quit";

    time_t startTime = time(0);     

    while (playing == true) { //loop while not game over

        if (_kbhit()) {       
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

        int newHeadX = snakeX[0] + dirX;    // head location after the move
        int newHeadY = snakeY[0] + dirY;

        if (newHeadX <= FIELD_LEFT || newHeadX >= FIELD_RIGHT || // check if snake is out of field
            newHeadY <= FIELD_TOP  || newHeadY >= FIELD_BOTTOM) {
            playing = false;
            break;
        }

        if (isOnSnake(snakeX, snakeY, length, newHeadX, newHeadY) == true) { // check if snake ate itself
            playing = false;
            break;
        }

        bool ateFood = false; // 
        if (newHeadX == foodX && newHeadY == foodY) {
            ateFood = true;
        }

        if (ateFood == false) {
            gotoxy(snakeX[length - 1], snakeY[length - 1]);
            std::cout << " ";
        } else {
            length++;
        }

        for (int i = length - 1; i > 0; i--) { // move every piece to where the piece in front of it was
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        snakeX[0] = newHeadX;
        snakeY[0] = newHeadY;

        gotoxy(snakeX[1], snakeY[1]); // draw the old head as body, and the new head
        setColor(2);
        std::cout << "o";
        gotoxy(snakeX[0], snakeY[0]);
        setColor(10);
        std::cout << "@";

        if (ateFood == true) {    // 4h. if we ate, add score, go faster, and place new food (TODO: make sure the new food is not on the snake)
            score = score + 10;
            drawSnakeScore(score);

            if (delay > 60) {
                delay = delay - 3;
            }

            placeFood(snakeX, snakeY, length, foodX, foodY);
            gotoxy(foodX, foodY);
            setColor(12);
            std::cout << "*";
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
    std::cout << "*** GAME OVER ***";
    gotoxy(12, 14);
    setColor(10);
    std::cout << "Score: " << score;
    gotoxy(12, 15);
    std::cout << "Time : " << duration << " sec";

    std::string nickname = askText("Nickname: ", 17); // Step 6: ask for a nickname and save the run

    Run newRun;
    newRun.nickname = nickname;
    newRun.score = score;
    newRun.duration = duration;
    newRun.date = getTodayDate();
    snakeRuns.push_back(newRun);

    gotoxy(12, 19);
    setColor(14);
    std::cout << "Run saved!";
    gotoxy(8, 21);
    setColor(8);
    std::cout << "Press any key...";
    waitForKey();
}

void drawRunRecord(RunNode* current, bool highToLow, std::string message) { // history run section (the player can scroll through them, sort, search, rename, delete)
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back       Run History";

    gotoxy(5, 5);
    setColor(8);
    if (highToLow == true) {
        std::cout << "Order: Highest -> Lowest";
    } else {
        std::cout << "Order: Lowest -> Highest";
    }

    gotoxy(5, 7);
    setColor(15);
    std::cout << "Run " << snakeRuns.getPosition(current) << " of " << snakeRuns.size();

    gotoxy(5, 9);
    setColor(7);
    std::cout << "Nickname: " << current->data.nickname;
    gotoxy(5, 10);
    std::cout << "Score   : " << current->data.score;
    gotoxy(5, 11);
    std::cout << "Duration: " << current->data.duration << " sec";
    gotoxy(5, 12);
    std::cout << "Date    : " << current->data.date;

    gotoxy(5, 14);
    setColor(8);
    if (current->prev == nullptr) {
        std::cout << "(first run)";
    }
    if (current->next == nullptr) {
        gotoxy(5, 15);
        std::cout << "(last run)";
    }

    gotoxy(5, 17);
    setColor(12);
    std::cout << message;

    gotoxy(5, 21);     // THIS IS THE CONTROL!!!!!!
    setColor(14);
    std::cout << "<-Prev ->Next H/L:Sort F:Find";
    gotoxy(5, 22);
    std::cout << "U:Rename  D:Delete  Esc:Back";
}

void showRunHistory() { // HISTORY of all runs (the player can scroll through them, sort, search, rename, delete)
    if (snakeRuns.size() == 0) {
        clearScreen();
        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back       Run History";
        gotoxy(8, 10);
        setColor(7);
        std::cout << "No runs saved yet.";
        gotoxy(8, 12);
        std::cout << "Play a game first!";
        gotoxy(8, 21);
        setColor(8);
        std::cout << "Press any key...";
        waitForKey();
        return;
    }

    RunNode* current = snakeRuns.getHead();     // start with the first run
    bool highToLow = true;
    bool viewing = true;
    std::string message = "";

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
            std::string keyword = askText("Search name: ", 19); // SEARCH by nickname
            RunNode* found = snakeRuns.search(keyword);
            if (found != nullptr) {
                current = found;
                message = "Found!";
            } else {
                message = "No run with that name.";
            }
        }
        else if (ch == 'u' || ch == 'U') {
            std::string newName = askText("New nickname: ", 19); // UPDATE the nickname
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
                std::cout << "All runs deleted.";
                gotoxy(8, 21);
                setColor(8);
                std::cout << "Press any key...";
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

    std::string options[3] = { "Play", "Run History", "Back" };
    int selected = 0;
    bool inMenu = true;

    while (inMenu == true) {
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back         " << app.icon << " App";

        gotoxy(14, 7);
        setColor(10);
        std::cout << "S N A K E";

        for (int i = 0; i < 3; i++) { // draw the 3 options
            gotoxy(13, 11 + i * 2);
            if (i == selected) {
                setColor(240); // highlighted
                std::cout << "[ " << options[i] << " ]";
            } else {
                setColor(7);
                std::cout << "  " << options[i] << "  ";
            }
        }

        gotoxy(5, 25);
        setColor(14);
        std::cout << "Up/Down: Move   Enter: Pick";

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
// end of snake game code

// start of rps game

const int WIN_SCORE = 2;   
const int QUIT_PICK  = 3;   // special value: the player pressed Esc
const std::string CHOICE_NAMES[3] = { "Rock", "Paper", "Scissors" };   // 0 = Rock, 1 = Paper, 2 = Scissors

const std::string RPS_ART[3][6] = { // 6 lines onlly para kasya sa screen
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
    std::string name;
    int wins;
    int losses;
    int matches;
};

struct Battle { // Battle Archive: one finished match
    std::string player1;
    std::string player2;
    std::string winner;
    std::string choices; // every round, example: "R-S P-P S-R "
    std::string date;
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

struct RivalNode { 
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

    RivalNode* search(const std::string& name) {     // find the rival with this exact name
        RivalNode* current = head;
        while (current != nullptr) {
            if (current->data.name == name) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void sortBy(char mode) {    // Bubble sort
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

struct BattleNode { 
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

    int getPosition(BattleNode* target) const { // battle number counter
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

std::string rpsToday() {     // today's date and time as text 
    time_t now = time(0);
    tm* timeInfo = localtime(&now);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeInfo);
    return std::string(buffer);
}

std::string rpsAsk(std::string label, int row) {  // ask the player to type one word (max 10 letters)
    gotoxy(5, row);
    setColor(7);
    std::cout << label;
    showCursor();
    std::string text;
    std::cin >> text;
    hideCursor();
    if (text.length() > 10) {
        text = text.substr(0, 10);
    }
    return text;
}

void rpsWait() { //para hindi agad nagra-run
    _getch();
}

void drawRpsHeader(std::string title) { // print the top bar
    clearScreen();
    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back   " << title;
}

void clearRows(int from, int to) {      // erase only some rows of the phone screen 
    for (int y = from; y <= to; y++) {
        gotoxy(4, y);
        std::cout << std::string(36, ' ');
    }
}

void showEmpty(std::string title) {          // message when nothing is saved yet
    drawRpsHeader(title);
    gotoxy(8, 10);
    setColor(7);
    std::cout << "Nothing saved yet.";
    gotoxy(8, 12);
    std::cout << "Play a match first!";
    gotoxy(8, 21);
    setColor(8);
    std::cout << "Press any key...";
    rpsWait();
}

int roundResult(int a, int b) {     // 0 = draw, 1 = first player wins, 2 = second player wins
    if (a == b) {
        return 0;
    }
    if ((a + 1) % 3 == b) {         // the next number beats the one before it 
        return 2;
    }
    return 1;
}

void drawHand(int choice, int x, int y) {   // print hand
    for (int i = 0; i < 6; i++) {
        gotoxy(x, y + i);
        std::cout << RPS_ART[choice][i];
    }
}

void drawFists(int y, int color, std::string word) {     // both fists (rock) at row y, with a word under them
    clearRows(8, 17);
    setColor(color);
    drawHand(0, 4, y);      
    drawHand(0, 22, y);     
    gotoxy(5, 17);
    std::cout << word;
}

void shakeAnimation() {     
    std::string words[3] = { "ROCK...", "PAPER...", "SCISSORS..." };
    for (int beat = 0; beat < 3; beat++) {
        for (int up = 1; up >= 0; up--) {
            int y = 8;      // fists up
            if (up == 0) {
                y = 10;     // fists down (para umalog)
            }
            drawFists(y, 10, words[beat]);
            Sleep(220);
        }
    }
    drawFists(8, 14, "SHOOT!");
    Sleep(400);
}

void blinkResult(std::string text, int color) {  // blink the result text 
    for (int i = 0; i < 5; i++) {
        gotoxy(5, 17);
        if (i % 2 == 0) {
            setColor(color);
        } else {
            setColor(8);
        }
        std::cout << ">> " << text << " <<";
        Sleep(250);
    }
}

int askChoice(std::string name) {    // ask user
    setColor(14);
    gotoxy(5, 9);
    std::cout << name << ", pick your hand:";
    gotoxy(5, 11);
    std::cout << "1:Rock  2:Paper  3:Scissors";

    int pick = -1;
    while (pick == -1) {
        int ch = _getch();
        if (ch == '1') {
            pick = 0;
        } else if (ch == '2') {
            pick = 1;
        } else if (ch == '3') {
            pick = 2;
        } else if (ch == KEY_ESC) {
            pick = QUIT_PICK;
        }
    }

    clearRows(9, 11);   // erase the prompt
    return pick;
}

RivalNode* getRival(std::string name) {  // find the rival, or register a new one
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

void playMatch(std::string p1, std::string p2, bool vsComputer) { 
    int score1 = 0;
    int score2 = 0;
    int round = 0;
    std::string choices = "";
    bool quit = false;

    while (score1 < WIN_SCORE && score2 < WIN_SCORE && quit == false) {
        round++;

        drawRpsHeader("Round " + std::to_string(round));     // scoreboard
        gotoxy(5, 5);
        setColor(15);
        std::cout << p1 << " " << score1 << " - " << score2 << " " << p2;

        int pick1 = askChoice(p1);      // ask user
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

        gotoxy(4, 7);                   //names above the hands, then the countdown animation
        setColor(15);
        std::cout << p1;
        gotoxy(22, 7);
        std::cout << p2;
        shakeAnimation();

        int result = roundResult(pick1, pick2);     // winner of round
        int color = 14;                             // draw = yellow
        std::string message = "IT'S A DRAW!";
        if (result == 1) {
            score1++;
            color = 10;                             // player 1 wins = green
            message = p1 + " WINS!";
        } else if (result == 2) {
            score2++;
            color = 12;                             // player 2 wins = red
            message = p2 + " WINS!";
        }

        clearRows(8, 17);               // reveal both hands in the result color
        setColor(color);
        drawHand(pick1, 4, 8);
        drawHand(pick2, 22, 8);
        gotoxy(4, 15);
        std::cout << CHOICE_NAMES[pick1];
        gotoxy(22, 15);
        std::cout << CHOICE_NAMES[pick2];
        blinkResult(message, color);

        choices = choices + CHOICE_NAMES[pick1][0] + "-" + CHOICE_NAMES[pick2][0] + " ";  // remember the round (first letters, ex: R-S)

        gotoxy(5, 22);
        setColor(8);
        std::cout << "Press any key...";
        rpsWait();
    }

    if (quit == true) {     // Esc = leave without saving
        return;
    }

    std::string winner = p2;     // Step 6: match over, save the battle
    if (score1 > score2) {
        winner = p1;
    }

    Battle battle = { p1, p2, winner, choices, rpsToday(), score1, score2, round };
    recordMatch(battle);

    drawRpsHeader("Match Over");
    gotoxy(11, 9);
    setColor(12);
    std::cout << "*** MATCH OVER ***";
    gotoxy(11, 11);
    setColor(10);
    std::cout << winner << " WINS!";
    gotoxy(11, 13);
    std::cout << "Score: " << score1 << " - " << score2;
    gotoxy(11, 15);
    setColor(14);
    std::cout << "Battle saved!";
    gotoxy(8, 21);
    setColor(8);
    std::cout << "Press any key...";
    rpsWait();
}

void startMatch(bool vsComputer) {      // ask for the names, then play
    drawRpsHeader("New Match");
    std::string p1 = rpsAsk("Player 1 name: ", 8);
    std::string p2 = "Computer";
    if (vsComputer == false) {
        p2 = rpsAsk("Player 2 name: ", 10);
    }
    if (p1 == p2) {         // two different rivals can't share a name
        p2 = p2 + "2";
    }
    playMatch(p1, p2, vsComputer);
}

void drawBattle(BattleNode* current, std::string message) {   // show one saved battle (THE BATTLE ARCHIVED IN THE LIST)
    drawRpsHeader("Battle History");

    gotoxy(5, 5);
    setColor(15);
    std::cout << "Battle " << rpsBattles.getPosition(current) << " of " << rpsBattles.size();

    gotoxy(5, 7);
    setColor(7);
    std::cout << "Date   : " << current->data.date;
    gotoxy(5, 8);
    std::cout << "Fighter: " << current->data.player1 << " vs " << current->data.player2;
    gotoxy(5, 9);
    std::cout << "Score  : " << current->data.score1 << " - " << current->data.score2;
    gotoxy(5, 10);
    std::cout << "Winner : " << current->data.winner;
    gotoxy(5, 11);
    std::cout << "Rounds : " << current->data.rounds;
    gotoxy(5, 12);
    std::cout << "Picks (P1-P2):";
    gotoxy(5, 13);
    std::cout << current->data.choices.substr(0, 30);

    gotoxy(5, 15);
    setColor(12);
    std::cout << message;

    gotoxy(5, 21);
    setColor(14);
    std::cout << "<-Prev  ->Next  Esc:Back";
}

void showHistory() {    // scroll through the battles with the arrow keys
    if (rpsBattles.size() == 0) {
        showEmpty("Battle History");
        return;
    }

    BattleNode* current = rpsBattles.getHead();
    bool viewing = true;
    std::string message = "";

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

void showRankings() {   
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
            std::cout << "Sorted by: Wins";
        } else if (mode == 'R') {
            std::cout << "Sorted by: Win rate";
        } else {
            std::cout << "Sorted by: Matches";
        }

        gotoxy(5, 7);   // column titles
        setColor(15);
        std::cout << "#  Name        W   L   M   %";

        RivalNode* current = rpsRivals.getHead();   // one row per rival (max 12 rows fit)
        int rank = 1;
        setColor(7);
        while (current != nullptr && rank <= 12) {
            int row = 7 + rank;
            gotoxy(5, row);
            std::cout << rank;
            gotoxy(8, row);
            std::cout << current->data.name;
            gotoxy(20, row);
            std::cout << current->data.wins;
            gotoxy(24, row);
            std::cout << current->data.losses;
            gotoxy(28, row);
            std::cout << current->data.matches;
            gotoxy(32, row);
            std::cout << winRate(current->data) << "%";
            current = current->next;
            rank++;
        }

        gotoxy(5, 22);
        setColor(14);
        std::cout << "W:Wins  R:Rate  M:Matches";
        gotoxy(5, 23);
        std::cout << "Esc:Back";

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

void launchRps(const App& app) { //home menu display
    hideCursor();
    srand(time(0));

    std::string options[5] = { "Vs Computer", "2 Players", "Rankings", "Battle History", "Back" };
    int selected = 0;
    bool inMenu = true;

    while (inMenu == true) {
        drawRpsHeader("      " + app.icon + " App");

        gotoxy(10, 5);
        setColor(14);
        std::cout << "ROCK PAPER SCISSORS";

        for (int i = 0; i < 5; i++) {   // draw the 5 options
            gotoxy(11, 9 + i * 2);
            if (i == selected) {
                setColor(240);
                std::cout << "[ " << options[i] << " ]";
            } else {
                setColor(7);
                std::cout << "  " << options[i] << "  ";
            }
        }

        gotoxy(5, 25);
        setColor(14);
        std::cout << "Up/Down: Move   Enter: Pick";

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
                showHistory();
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
// rps code end


//appmart  codes

void launchStore(const App& app) {
    hideCursor();
    int selected = 0;
    std::string message = "";
    bool running = true;

    while (running == true) {
        AppNode* items[MAX_APPS];
        int itemCount = collectVisible(storeApps, "", items);   

        if (selected >= itemCount) {  
            selected = itemCount - 1;
        }
        if (selected < 0) {
            selected = 0;
        }

        clearRows(3, 26);   // erase the old screen

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back         " << app.icon << " Store";

        gotoxy(5, 5);
        setColor(14);
        std::cout << "APP MART - Download apps";
        gotoxy(5, 6);
        setColor(8);
        std::cout << itemCount << " app(s) available";

        if (itemCount == 0) {
            gotoxy(8, 11);
            setColor(7);
            std::cout << "Everything is installed!";
            gotoxy(8, 13);
            std::cout << "Delete an app on the home";
            gotoxy(8, 14);
            std::cout << "screen to find it here.";
        }

        for (int i = 0; i < itemCount; i++) {   // one row per app 
            std::string name = items[i]->data.name;
            while (name.length() < 14) {        // name  padding
                name = name + " ";
            }

            gotoxy(6, 8 + i * 2);
            if (i == selected) {
                setColor(240);  // highlighted
                std::cout << "[ " << items[i]->data.icon << " " << name << "  GET ]";
            } else {
                setColor(7);
                std::cout << "  " << items[i]->data.icon << " " << name << "  GET  ";
            }
        }

        gotoxy(5, 24);
        setColor(10);
        std::cout << message;

        gotoxy(5, 25);
        setColor(14);
        std::cout << "Up/Down: Move   Enter: Download";
        gotoxy(5, 26);
        std::cout << "Esc: Back";

        message = "";

        int ch = _getch();
        if (ch == 224 || ch == 0) {
            int arrow = _getch();
            if (arrow == KEY_UP && selected > 0) {
                selected--;
            } else if (arrow == KEY_DOWN && selected < itemCount - 1) {
                selected++;
            }
        } else if (ch == KEY_ENTER) {
            if (itemCount > 0) {
                AppNode* node = items[selected];
                App chosen = node->data;        
                storeApps.removeNode(node);
                homeApps.push_back(chosen);     
                applyHomeSort();                
                message = chosen.name + " added to home!";
            } else {
                message = "Nothing to download.";
            }
        } else if (ch == KEY_ESC) {
            running = false;
        }
    }

    setColor(7);
    clearScreen();
}
// end of appmart code

// open an app from the home screen
void openApp(const App& app) {
    if (app.name == "YouWho?") {
        launchCard(app);
    }
    else if (app.name == "BitBite") {
        launchSnake(app);
    }
    else if (app.name == "Rock 'n' Clash") {
        launchRps(app);
    }
    else if (app.name == "AppMart") {
        launchStore(app);
    }
}

// move highlight
void moveSelection(int& selected, int count, int arrow) {
    if (count == 0) {
        return;
    }

    if (arrow == KEY_LEFT) {
        if (selected > 0) {
            selected--;
        }
    } else if (arrow == KEY_RIGHT) {
        if (selected < count - 1) {
            selected++;
        }
    } else if (arrow == KEY_UP) {
        if (selected >= 2) {
            selected -= 2;
        }
    } else if (arrow == KEY_DOWN) {
        if (selected + 2 < count) {
            selected += 2;
        } else if (selected / 2 < (count - 1) / 2) {    
            selected = count - 1;
        }
    }
}

// delete an app from the home screen 
std::string removeHomeApp(AppNode* node) {
    if (node->data.name == "AppMart") {     
        return "AppMart can't be removed.";
    }

    App removed = node->data;      
    homeApps.removeNode(node);
    storeApps.push_back(removed);
    return removed.name + " is back in AppMart.";
}

int main() {
    hideCursor();
    system("cls");

    // apps at home screen initially
    homeApps.push_back({"Phone",   ">"});
    homeApps.push_back({"YouWho?", "#"});
    homeApps.push_back({"BitBite", "S"});
    homeApps.push_back({"Rock 'n' Clash", "R"});
    homeApps.push_back({"AppMart", "6"});

    // apps on the appmart
    storeApps.push_back({"Camera",   "O"});
    storeApps.push_back({"Gallery",  "*"});
    storeApps.push_back({"Music",    "~"});
    storeApps.push_back({"Settings", "@"});

    int selected = 0;               // highlighted app 
    bool searching = false;         // true while the search bar is being typed in
    std::string filter = "";     
    std::string message = "";
    bool running = true;

    drawPhoneFrame();

    while (running) {
        AppNode* visible[MAX_APPS];
        int visCount = collectVisible(homeApps, filter, visible);   // empty filter = all home apps

        if (selected >= visCount) {     // keep the highlight inside the list
            selected = visCount - 1;
        }
        if (selected < 0) {
            selected = 0;
        }

        renderHomeScreen(visible, visCount, selected, filter, searching, message);
        message = "";

        int ch = _getch();

        if (ch == 224 || ch == 0) { // Arrow key prefix handling
            int arrow = _getch();
            if (arrow == KEY_UP || arrow == KEY_DOWN || arrow == KEY_LEFT || arrow == KEY_RIGHT) {
                moveSelection(selected, visCount, arrow);
            } else if (arrow == KEY_DELETE && searching == false) {
                if (visCount > 0) {
                    message = removeHomeApp(visible[selected]);
                }
            }
        }
        else if (searching == true) {
            // search bar mode
            if (ch == KEY_ENTER) {
                if (visCount > 0) {
                    App toOpen = visible[selected]->data;   // copy, the list may change inside the app
                    searching = false;
                    filter = "";
                    openApp(toOpen);
                    selected = indexOfName(homeApps, toOpen.name);  // keep it highlighted on the normal home screen
                }
            } else if (ch == KEY_ESC) {
                searching = false;
                filter = "";
                selected = 0;
            } else if (ch == 8) {   // Backspace
                if (!filter.empty()) {
                    filter.erase(filter.length() - 1);
                    selected = 0;
                }
            } else if (ch >= 32 && ch <= 126) {     // normal typed character
                if (filter.length() < 20) {
                    filter += static_cast<char>(ch);
                    selected = 0;
                }
            }
        }
        else {
            // ---- NORMAL HOME MODE ----
            if (ch == KEY_ENTER) {
                if (visCount > 0) {
                    App toOpen = visible[selected]->data;
                    openApp(toOpen);
                    selected = indexOfName(homeApps, toOpen.name);
                }
            } else if (ch == KEY_ESC) {
                running = false;
            } else if (ch == 's' || ch == 'S') {
                // sort
                std::string keepName = "";
                if (visCount > 0) {
                    keepName = visible[selected]->data.name;    // keeps highlight
                }

                if (homeSortMode == 1) {
                    homeSortMode = 2;
                    message = "Sorted: Z-A.";
                } else {
                    homeSortMode = 1;
                    message = "Sorted: A-Z.";
                }
                applyHomeSort();
                selected = indexOfName(homeApps, keepName);
            } else if (ch == 'f' || ch == 'F' || ch == '/') {
                searching = true;       // open the search bar
                filter = "";
                selected = 0;
            } else if (ch == 'd' || ch == 'D') {
                if (visCount > 0) {
                    message = removeHomeApp(visible[selected]);
                }
            }
        }
    }

    clearScreen();
    gotoxy(10, 10);
    setColor(8);
    std::cout << "Device Turned Off";
    gotoxy(0, 20);
    setColor(7);
    return 0;
}