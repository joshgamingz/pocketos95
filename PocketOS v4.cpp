

#include <iostream>
#include <string>
#include <limits>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cctype>

#ifndef nullptr
#define nullptr NULL  // Error handling for old devc++ versions
#endif

// ---- SHARED CONSTANTS ----

// key codes given back by readKey(). 
const int KEY_NONE      = -1;    
const int KEY_UP        = 1000;
const int KEY_DOWN      = 1001;
const int KEY_LEFT      = 1002;
const int KEY_RIGHT     = 1003;
const int KEY_DELETE    = 1004; 
const int KEY_ENTER     = 13;    
const int KEY_ESC       = 27;   
const int KEY_BACKSPACE = 8;

//phone dimensions
const int PHONE_LEFT   = 4;
const int PHONE_RIGHT  = 40;
const int PHONE_TOP    = 3;
const int PHONE_BOTTOM = 27;

// Function Prototypes

void clearRows(int from, int to);   // defined later in the RPS part
bool readField(short x, short y, int width, int maxLen, std::string& text);   // defined after the keyboard part

// ---- SHARED HELPERS ----

std::string toLowerStr(const std::string& s) {   // lowercase copy of a string
    std::string result = s;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

bool sameName(const std::string& a, const std::string& b) {     
    return toLowerStr(a) == toLowerStr(b);
}

// Console Functions

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

// ---- KEYBOARD ----

// Waits for one key and gives back a simple code.
// letterKeys = true : W A S D become the arrows, E and Space become Enter, Q becomes Esc,
//                     every other letter comes back in lowercase (example: 'f')
// letterKeys = false: used when the player is typing, so letters stay letters
int readKey(bool letterKeys) {
    int ch = _getch();

    if (ch == 224 || ch == 0) {     // arrow keys and Delete send two codes
        int code = _getch();
        if (code == 72) {
            return KEY_UP;
        } else if (code == 80) {
            return KEY_DOWN;
        } else if (code == 75) {
            return KEY_LEFT;
        } else if (code == 77) {
            return KEY_RIGHT;
        } else if (code == 83) {
            return KEY_DELETE;
        }
        return KEY_NONE;
    }

    if (letterKeys == true) {
        if (ch == ' ') {
            return KEY_ENTER;
        }
        if (std::isalpha(ch)) {
            int low = std::tolower(ch);
            if (low == 'w') {
                return KEY_UP;
            } else if (low == 's') {
                return KEY_DOWN;
            } else if (low == 'a') {
                return KEY_LEFT;
            } else if (low == 'd') {
                return KEY_RIGHT;
            } else if (low == 'e') {
                return KEY_ENTER;
            } else if (low == 'q') {
                return KEY_ESC;
            }
            return low;
        }
    }
    return ch;
}

// ---- INPUT AND DATE HELPERS ----

std::string readWord() {   // (not used anymore, askText uses readField) read one word, then throw away the rest of the line
    std::string text;
    if (!(std::cin >> text)) {
        std::cin.clear();   // input failed, reset the stream so the program keeps working
        text = "";
    }
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    return text;
}

std::string getTodayDate() { //
    time_t now = time(0);               // current time in seconds
    tm* timeInfo = localtime(&now);    
    if (timeInfo == nullptr) {          // localtime can fail, don't crash
        return "Unknown date";
    }
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeInfo);
    return std::string(buffer);
}

std::string askText(std::string label, int row) { // Ask the player to type a short name (max 10 letters)
    gotoxy(5, row);
    setColor(7);
    std::cout << label;

    std::string text = "";
    short startX = static_cast<short>(5 + label.length());
    readField(startX, static_cast<short>(row), 10, 10, text);   // typing stops at 10 letters, Esc keeps what is typed

    if (text.empty()) {         // nothing was typed, use a default name
        text = "Player";
    }
    return text;
}

void waitForKey() { 
    readKey(false);
}

// Type a text (spaces allowed) inside a box so it never goes outside the phone screen.
// x, y = top left of the box, width = letters per row, maxLen = most letters allowed.
// Enter = done (true), Esc = cancel (false)
bool readField(short x, short y, int width, int maxLen, std::string& text) {
    int rows = (maxLen + width - 1) / width;
    text = "";
    bool done = false;
    bool accepted = true;
    showCursor();

    while (done == false) {
        setColor(15);
        for (int r = 0; r < rows; r++) {        // draw the text, cut into rows
            gotoxy(x, y + r);
            std::string part = "";
            int start = r * width;
            if (start < static_cast<int>(text.length())) {
                part = text.substr(start, width);
            }
            while (static_cast<int>(part.length()) < width) {
                part += " ";
            }
            std::cout << part;
        }

        int cursorX = x + static_cast<int>(text.length()) % width;
        int cursorY = y + static_cast<int>(text.length()) / width;
        if (cursorY >= y + rows) {              // the box is full, keep the cursor on the last place
            cursorY = y + rows - 1;
            cursorX = x + width - 1;
        }
        gotoxy(static_cast<short>(cursorX), static_cast<short>(cursorY));

        int key = readKey(false);
        if (key == KEY_ENTER) {
            done = true;
        } else if (key == KEY_ESC) {
            accepted = false;
            done = true;
        } else if (key == KEY_BACKSPACE) {
            if (!text.empty()) {
                text.erase(text.length() - 1);
            }
        } else if (key >= 32 && key <= 126) {
            if (static_cast<int>(text.length()) < maxLen) {
                text += static_cast<char>(key);
            }
        }
    }

    hideCursor();
    return accepted;
}

void showEmptyScreen(const std::string& title, const std::string& hint) {   // message when nothing is saved yet
    clearScreen();
    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back   " << title;
    gotoxy(8, 10);
    setColor(7);
    std::cout << "Nothing saved yet.";
    gotoxy(8, 12);
    std::cout << hint;
    gotoxy(8, 21);
    setColor(8);
    std::cout << "Press any key...";
    waitForKey();
}

// ---- MODULE 1: HOME SCREEN AND APP LIST ----

const int MAX_APPS = 14;    // most apps the home screen or AppMart can show

// ---- LINKED LIST: APPS ----

struct App {
    std::string name;   // the name on the screen, the player can rename it
    std::string icon;
    std::string id;     // never changes, the program opens apps with this
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

int indexOfId(const AppList& apps, const std::string& id) {   // position of the app with this id, -1 if it is not there
    int index = 0;
    AppNode* current = apps.getHead();
    while (current != nullptr) {
        if (current->data.id == id) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
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
        short row = 6 + static_cast<short>(index / 2) * 2;

        gotoxy(col, row);
        if (index == selected) {
            setColor(240); // Highlight selected app
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
        std::cout << "Enter/E: Open   Q: Power off";
    }
    gotoxy(5, 21);
    if (searching == true) {
        std::cout << "Arrows:Select  Backspace:Erase";
    } else {
        std::cout << "F:Find Z:Sort R:Rename X:Remove";
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
    std::cout << "Press [Q] to Return";

    // Wait inside app screen until Esc, Q or Backspace
    while (true) {
        int key = readKey(true);
        if (key == KEY_ESC || key == KEY_BACKSPACE) {
            break;
        }
    }

    clearScreen();
}

// ---- MODULE 2: CARD GAME (HIGHER OR LOWER) ----

// ---- LINKED LIST: CARD GAME PLAYERS (LEADERBOARD) ----

struct CardPlayer {     // one user and the best score they ever got
    std::string name;
    int best;
    int plays;
};

int cardValue(const CardPlayer& p, char mode) {     // the number we sort by
    if (mode == 'B') {
        return p.best;
    }
    return p.plays;
}

struct CardPlayerNode {
    CardPlayer data;
    CardPlayerNode* prev;
    CardPlayerNode* next;

    CardPlayerNode(const CardPlayer& playerData) : data(playerData), prev(nullptr), next(nullptr) {}
};

class CardPlayerList {
private:
    CardPlayerNode* head;
    CardPlayerNode* tail;
    int count;

public:
    CardPlayerList() : head(nullptr), tail(nullptr), count(0) {}

    ~CardPlayerList() {
        CardPlayerNode* current = head;
        while (current != nullptr) {
            CardPlayerNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const CardPlayer& player) {      // add a new user at the end
        CardPlayerNode* newNode = new CardPlayerNode(player);
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

    CardPlayerNode* search(const std::string& name) {   // find the user with this name, ignores CAPS
        CardPlayerNode* current = head;
        while (current != nullptr) {
            if (sameName(current->data.name, name)) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void sortBy(char mode) {    // Bubble sort, biggest first. mode: B = best score, P = plays
        if (count < 2) {
            return;
        }
        bool swapped = true;
        while (swapped) {
            swapped = false;
            CardPlayerNode* current = head;
            while (current->next != nullptr) {
                if (cardValue(current->data, mode) < cardValue(current->next->data, mode)) {
                    CardPlayer temp = current->data;    // swap the data of the two nodes
                    current->data = current->next->data;
                    current->next->data = temp;
                    swapped = true;
                }
                current = current->next;
            }
        }
    }

    CardPlayerNode* getHead() const { return head; }
    CardPlayerNode* getTail() const { return tail; }
    int size() const { return count; }
};

CardPlayerList cardPlayers;

bool updateCardPlayer(const std::string& name, int score) {     // true if this is a new best score of the user
    CardPlayerNode* player = cardPlayers.search(name);
    if (player == nullptr) {
        CardPlayer newPlayer = { name, score, 1 };
        cardPlayers.push_back(newPlayer);
        return score > 0;
    }

    player->data.plays++;
    if (score > player->data.best) {
        player->data.best = score;
        return true;
    }
    return false;
}

void spadeCard(){
    setColor(7);
    gotoxy(20, 11);
    std::cout <<"     *";
    gotoxy(20, 12);
    std::cout <<"    ***";
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

int pickNextCard(int current) {     // a random card from 1 to 10 that is not the same as the current one
    int number = rand() % 10 + 1;
    while (number == current) {
        number = rand() % 10 + 1;
    }
    return number;
}

void drawCardTable() {
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back      Card Game";

    gotoxy(7, 5);
    setColor(7);
    std::cout << "  _________________________";

    for (int i = 0; i < 15; i++) {
        gotoxy(7, 6 + i);
        std::cout << "||                         |";
    }

    spadeCard();

    gotoxy(5, 27);
    setColor(14);
    std::cout << "A/D: Pick   Enter/E: OK  Q: Back";
}

void drawCardScore(int score) {
    gotoxy(29, 4);
    setColor(7);
    std::cout << "Score:" << score << "   ";
}

void drawCardButtons(int selected) {
    gotoxy(8, 26);
    if (selected == 0) {
        setColor(240);      // highlight the chosen button
        std::cout << "[Higher ^]";
        setColor(7);
        std::cout << "     Lower v ";
    } else {
        setColor(7);
        std::cout << " Higher ^     ";
        setColor(240);
        std::cout << "[Lower v]";
    }
}

void cardGameOver(int revealed, int score) {    // show the card, ask the name, save the score
    gotoxy(10, 8);
    setColor(12);
    std::cout << revealed << " ";

    for (int i = 0; i < 4; i++) {       // blinking GAME OVER
        gotoxy(13, 22);
        if (i % 2 == 0) {
            setColor(12);
        } else {
            setColor(4);
        }
        std::cout << "GAME OVER [X]";
        Sleep(300);
    }

    std::string name = askText("Name: ", 23);
    bool newBest = updateCardPlayer(name, score);
    CardPlayerNode* player = cardPlayers.search(name);

    gotoxy(5, 24);
    setColor(15);
    std::cout << "Score: " << score;
    if (player != nullptr) {
        std::cout << "   Best: " << player->data.best;
    }

    gotoxy(5, 25);
    if (newBest == true) {
        setColor(10);
        std::cout << "NEW BEST!";
    }
    gotoxy(19, 25);
    setColor(8);
    std::cout << "Press any key...";
    waitForKey();

    setColor(7);
    clearRows(22, 25);
}

void playCard() {
    hideCursor();
    drawCardTable();

    int selected = 0;
    int score = 0;
    int cardNum = rand() % 10 + 1;
    int nextNum = pickNextCard(cardNum);
    bool playing = true;

    while (playing == true) {
        gotoxy(10, 8);
        setColor(7);
        std::cout << cardNum << " ";
        drawCardScore(score);
        drawCardButtons(selected);

        int key = readKey(true);
        if (key == KEY_LEFT || key == KEY_UP) {
            selected = 0;
        } else if (key == KEY_RIGHT || key == KEY_DOWN) {
            selected = 1;
        } else if (key == KEY_ENTER) {
            bool correct = false;
            if (selected == 0) {
                correct = (nextNum > cardNum);
            } else {
                correct = (nextNum < cardNum);
            }

            if (correct == true) {
                score++;
                gotoxy(10, 8);          // show the new card with a check mark
                setColor(10);
                std::cout << nextNum << " ";
                gotoxy(20, 22);
                setColor(2);
                std::cout << "/";
                gotoxy(19, 23);
                std::cout << "V";
                Sleep(600);
                gotoxy(20, 22);
                std::cout << " ";
                gotoxy(19, 23);
                std::cout << " ";

                cardNum = nextNum;
                nextNum = pickNextCard(cardNum);
            } else {
                cardGameOver(nextNum, score);
                score = 0;
                cardNum = rand() % 10 + 1;      // a fresh round
                nextNum = pickNextCard(cardNum);
            }
        } else if (key == KEY_ESC) {
            playing = false;
        }
    }

    setColor(7);
}

void showCardLeaderboard() {    // best score of every user
    if (cardPlayers.size() == 0) {
        showEmptyScreen("Leaderboard", "Play a game first!");
        return;
    }

    char mode = 'B';
    bool viewing = true;

    while (viewing == true) {
        cardPlayers.sortBy(mode);
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back      Leaderboard";

        gotoxy(5, 5);
        setColor(8);
        if (mode == 'B') {
            std::cout << "Sorted by: Best score";
        } else {
            std::cout << "Sorted by: Games played";
        }

        gotoxy(5, 7);   // column titles
        setColor(15);
        std::cout << "#  Name        Best  Plays";

        CardPlayerNode* current = cardPlayers.getHead();    // one row per user, max 12 rows fit
        int rank = 1;
        while (current != nullptr && rank <= 12) {
            int row = 7 + rank;
            if (rank == 1) {
                setColor(14);       // first place is yellow
            } else {
                setColor(7);
            }
            gotoxy(5, row);
            std::cout << rank;
            gotoxy(8, row);
            std::cout << current->data.name;
            gotoxy(20, row);
            std::cout << current->data.best;
            gotoxy(26, row);
            std::cout << current->data.plays;
            current = current->next;
            rank++;
        }

        gotoxy(5, 22);
        setColor(14);
        std::cout << "1:Best score  2:Plays";
        gotoxy(5, 23);
        std::cout << "A/D:Switch sort  Q:Back";

        int key = readKey(true);
        if (key == '1') {
            mode = 'B';
        } else if (key == '2') {
            mode = 'P';
        } else if (key == KEY_LEFT || key == KEY_RIGHT) {
            if (mode == 'B') {
                mode = 'P';
            } else {
                mode = 'B';
            }
        } else if (key == KEY_ESC) {
            viewing = false;
        }
    }
}

void launchCard(const App& app) {   // START MENU of the Card Game app
    hideCursor();
    srand(time(0));

    std::string options[3] = { "Play", "Leaderboard", "Back" };
    int selected = 0;
    bool inMenu = true;

    while (inMenu == true) {
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back         " << app.icon << " App";

        gotoxy(13, 7);
        setColor(14);
        std::cout << "CARD GAME";
        gotoxy(9, 8);
        setColor(8);
        std::cout << "Higher or Lower?";

        for (int i = 0; i < 3; i++) {   // draw the 3 options
            gotoxy(12, 11 + i * 2);
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
        std::cout << "W/S: Move   Enter/E: Pick";

        int key = readKey(true);
        if (key == KEY_UP && selected > 0) {
            selected--;
        } else if (key == KEY_DOWN && selected < 2) {
            selected++;
        } else if (key == KEY_ENTER) {
            if (selected == 0) {
                playCard();
            } else if (selected == 1) {
                showCardLeaderboard();
            } else {
                inMenu = false;
            }
        } else if (key == KEY_ESC) {
            inMenu = false;
        }
    }

    setColor(7);
    clearScreen();
}
// end of card game code

// ---- MODULE 3: SNAKE (BITBITE) ----

//snake game

const int FIELD_LEFT   = 6; // snake field
const int FIELD_RIGHT  = 37;
const int FIELD_TOP    = 6;
const int FIELD_BOTTOM = 23;
const int FIELD_CELLS  = (FIELD_RIGHT - FIELD_LEFT - 1) * (FIELD_BOTTOM - FIELD_TOP - 1);   // how many free boxes inside the walls
const int MAX_SNAKE    = 500;

// ---- LINKED LIST: SNAKE RUNS ----

struct Run {
    std::string nickname;
    int score;
    int duration; // set natin as seconds
    std::string date;
};

struct RunNode { // Doubly Linked List Node
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
        if (node == nullptr) {      // nothing to remove
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

void drawSnakeBorder() { // SNAKE MODEL 
    setColor(2);    // dark green para retro looks

    gotoxy(FIELD_LEFT +1, FIELD_TOP);    // WALL NAMAN TO
    std::cout << "_";
    for (int x = FIELD_LEFT + 2; x < FIELD_RIGHT; x++) {
        std::cout << "_";
    }
    

    for (int y = FIELD_TOP + 1; y < FIELD_BOTTOM; y++) { //side walls naman to
        gotoxy(FIELD_LEFT, y);
        std::cout << "|";
        gotoxy(FIELD_RIGHT, y);
        std::cout << "|";
    }

    gotoxy(FIELD_LEFT, FIELD_BOTTOM);
    std::cout << "|";
    for (int x = FIELD_LEFT + 1; x < FIELD_RIGHT; x++) {
        std::cout << "-";
    }
    std::cout << "|";
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
    std::cout << "Arrows/WASD: Move   Q: Quit";

    time_t startTime = time(0);     

    while (playing == true) { //loop while not game over

        if (_kbhit()) {       
            int key = readKey(true);    // arrow keys and W A S D work the same
            if (key == KEY_UP && dirY != 1) {
                dirX = 0;
                dirY = -1;
            } else if (key == KEY_DOWN && dirY != -1) {
                dirX = 0;
                dirY = 1;
            } else if (key == KEY_LEFT && dirX != 1) {
                dirX = -1;
                dirY = 0;
            } else if (key == KEY_RIGHT && dirX != -1) {
                dirX = 1;
                dirY = 0;
            } else if (key == KEY_ESC) {
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

        if (ateFood == true) {
            score = score + 10;
            drawSnakeScore(score);

            if (length >= FIELD_CELLS) {    // the whole field is full, no more space for food
                playing = false;
                break;
            }

            if (delay > 60) {
                delay = delay - 3;
            }

            placeFood(snakeX, snakeY, length, foodX, foodY);
            gotoxy(foodX, foodY);
            setColor(12);
            std::cout << "*";
        }

        if (dirY != 0) {
            Sleep(delay + 40);
        } else {
            Sleep(delay);
        }
    }

    if (quitEarly == true) { // if the player pressed Esc, leave without saving
        return;
    }

    time_t endTime = time(0);
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

    std::string nickname = askText("Nickname: ", 17);

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

void drawRunRecord(RunNode* current, bool highToLow, std::string message) { // history run section
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

    gotoxy(5, 20);     // THIS IS THE CONTROL!!!!!!
    setColor(14);
    std::cout << "A/D: Prev/Next   F: Find";
    gotoxy(5, 21);
    std::cout << "W: Highest   S: Lowest";
    gotoxy(5, 22);
    std::cout << "R: Rename   X: Delete   Q: Back";
}

void showRunHistory() { // HISTORY of all runs
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

        int key = readKey(true);

        if (key == KEY_LEFT) { // PREV = go to the previous run
            if (current->prev != nullptr) {
                current = current->prev;
            } else {
                message = "Already at the first run.";
            }
        }
        else if (key == KEY_RIGHT) { // NEXT = go to the next run
            if (current->next != nullptr) {
                current = current->next;
            } else {
                message = "Already at the last run.";
            }
        }
        else if (key == KEY_UP) {
            highToLow = true; // SORT: highest score first
            snakeRuns.sortByScore(true);
            current = snakeRuns.getHead();
            message = "Sorted: highest first.";
        }
        else if (key == KEY_DOWN) {
            highToLow = false; // SORT: lowest score first
            snakeRuns.sortByScore(false);
            current = snakeRuns.getHead();
            message = "Sorted: lowest first.";
        }
        else if (key == 'f') {
            std::string keyword = askText("Search name: ", 19); // SEARCH by nickname
            RunNode* found = snakeRuns.search(keyword);
            if (found != nullptr) {
                current = found;
                message = "Found!";
            } else {
                message = "No run with that name.";
            }
        }
        else if (key == 'r') {
            std::string newName = askText("New nickname: ", 19); // UPDATE the nickname
            current->data.nickname = newName;
            message = "Nickname updated.";
        }
        else if (key == 'x' || key == KEY_DELETE) {
            RunNode* showAfter = current->next; // DELETE the current run and show the next one
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
        else if (key == KEY_ESC) {
            viewing = false;
        }
    }
}

void launchSnake(const App& app) { // START MENU of the Snake app
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
        std::cout << "W/S: Move   Enter/E: Pick";

        int key = readKey(true);
        if (key == KEY_UP && selected > 0) {
            selected--;
        } else if (key == KEY_DOWN && selected < 2) {
            selected++;
        } else if (key == KEY_ENTER) {
            if (selected == 0) {
                playSnake();
            } else if (selected == 1) {
                showRunHistory();
            } else {
                inMenu = false;
            }
        } else if (key == KEY_ESC) {
            inMenu = false;
        }
    }

    setColor(7);
    clearScreen();
}
// end of snake game code

// ---- MODULE 4: ROCK PAPER SCISSORS ----

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

// ---- LINKED LISTS: RIVALS AND BATTLES ----

struct Rival { // Rival Registry: one rival + their stats
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
    return getTodayDate();
}

std::string rpsAsk(std::string label, int row) {  // ask the player to type a name, same limit as askText (10 letters)
    return askText(label, row);
}

void rpsWait() { //para hindi agad nagra-run
    waitForKey();
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

void drawFists(int y, int color, std::string word) {     // both fists at row y, with a word under them
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
                y = 10;     // fists down
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
    gotoxy(5, 13);
    std::cout << "Q: Quit";

    int pick = -1;
    while (pick == -1) {
        int key = readKey(true);
        if (key == '1') {
            pick = 0;
        } else if (key == '2') {
            pick = 1;
        } else if (key == '3') {
            pick = 2;
        } else if (key == KEY_ESC) {
            pick = QUIT_PICK;
        }
    }

    clearRows(9, 13);   // erase the prompt
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

        choices = choices + CHOICE_NAMES[pick1][0] + "-" + CHOICE_NAMES[pick2][0] + " ";  // remember the round

        gotoxy(5, 22);
        setColor(8);
        std::cout << "Press any key...";
        rpsWait();
    }

    if (quit == true) {     // Esc = leave without saving
        return;
    }

    std::string winner = p2;
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

void drawBattle(BattleNode* current, std::string message) {   // show one saved battle
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
    std::cout << "A/D: Prev/Next   Q: Back";
}

void showHistory() {    // scroll through the battles with the arrow keys or A/D
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

        int key = readKey(true);
        if (key == KEY_LEFT) {
            if (current->prev != nullptr) {
                current = current->prev;
            } else {
                message = "This is the first battle.";
            }
        } else if (key == KEY_RIGHT) {
            if (current->next != nullptr) {
                current = current->next;
            } else {
                message = "This is the last battle.";
            }
        } else if (key == KEY_ESC) {
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

        RivalNode* current = rpsRivals.getHead();   // one row per rival
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
        std::cout << "1:Wins  2:Rate  3:Matches";
        gotoxy(5, 23);
        std::cout << "A/D:Switch sort  Q:Back";

        int key = readKey(true);
        if (key == '1') {
            mode = 'W';
        } else if (key == '2') {
            mode = 'R';
        } else if (key == '3') {
            mode = 'M';
        } else if (key == KEY_RIGHT) {
            if (mode == 'W') {
                mode = 'R';
            } else if (mode == 'R') {
                mode = 'M';
            } else {
                mode = 'W';
            }
        } else if (key == KEY_LEFT) {
            if (mode == 'W') {
                mode = 'M';
            } else if (mode == 'R') {
                mode = 'W';
            } else {
                mode = 'R';
            }
        } else if (key == KEY_ESC) {
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
        std::cout << "W/S: Move   Enter/E: Pick";

        int key = readKey(true);
        if (key == KEY_UP && selected > 0) {
            selected--;
        } else if (key == KEY_DOWN && selected < 4) {
            selected++;
        } else if (key == KEY_ENTER) {
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
        } else if (key == KEY_ESC) {
            inMenu = false;
        }
    }

    setColor(7);
    clearScreen();
}
// rps code end

// ---- MODULE 5: THE ESCAPE PROTOCOL (THE ABYSSAL PROTOCOL STORY) ----

// the story is made of scene nodes, every choice is a pointer to the next scene
// 5 rounds, 2 options each. Only the path A-A-B-B-A survives.

enum SceneId {
    SCN_JUNCTION, SCN_ARM, SCN_NURSERY, SCN_DECK, SCN_LIFT,
    END_PIT, END_SPORES, END_SPECIMEN, END_ABYSS, END_FROZEN, END_SURVIVED,
    SCENE_COUNT
};

const int ESC_TEXT_WIDTH = 33;   // letters per line inside the phone screen
const int MAX_WRAP_LINES = 20;
const int ESC_QUIT = -1;         // special value: the player pressed Esc

const std::string ESC_PROLOGUE = "You wake face-down on cold, slick concrete, choking on the stench of copper and burnt hair. Red strobe lights pulse down an endless corridor. The blast door behind you seals with a hydraulic thud. Wet, ragged breathing echoes inside the walls.";

// ---- LINKED NODES: STORY SCENES ----

struct StoryNode {
    std::string title;
    std::string text;
    std::string options[3];
    StoryNode* next[3];     // one link for every choice: A, B, C
    int optionCount;
    int points;             // points the player gets for reaching this scene
    bool isEnding;
    bool escaped;           // true = the player got out alive
};

// ---- LINKED LIST: PATH OF THE CURRENT RUN ----

struct PathNode {
    char choice;
    PathNode* prev;
    PathNode* next;

    PathNode(char c) : choice(c), prev(nullptr), next(nullptr) {}
};

class PathList {
private:
    PathNode* head;
    PathNode* tail;
    int count;

public:
    PathList() : head(nullptr), tail(nullptr), count(0) {}

    ~PathList() {
        PathNode* current = head;
        while (current != nullptr) {
            PathNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(char choice) {   // add the choice at the end of the trail
        PathNode* newNode = new PathNode(choice);
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

    std::string toString() const {  // example: A-B-C
        std::string result = "";
        PathNode* current = head;
        while (current != nullptr) {
            if (!result.empty()) {
                result += "-";
            }
            result += current->choice;
            current = current->next;
        }
        return result;
    }

    int size() const { return count; }
};

// ---- LINKED LIST: ESCAPE RUNS ----

struct EscRun {
    std::string name;
    int percent;
    int points;
    std::string ending;
    std::string path;
    std::string date;
};

struct EscRunNode {
    EscRun data;
    EscRunNode* prev;
    EscRunNode* next;

    EscRunNode(const EscRun& runData) : data(runData), prev(nullptr), next(nullptr) {}
};

class EscRunList {
private:
    EscRunNode* head;
    EscRunNode* tail;
    int count;

public:
    EscRunList() : head(nullptr), tail(nullptr), count(0) {}

    ~EscRunList() {
        EscRunNode* current = head;
        while (current != nullptr) {
            EscRunNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const EscRun& run) {     // add a new run at the end
        EscRunNode* newNode = new EscRunNode(run);
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

    void removeNode(EscRunNode* node) {
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

    EscRunNode* search(const std::string& keyword) {    // first run whose name has the keyword, ignores CAPS
        std::string key = toLowerStr(keyword);
        EscRunNode* current = head;
        while (current != nullptr) {
            if (toLowerStr(current->data.name).find(key) != std::string::npos) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    int getPosition(EscRunNode* target) const {     // run number counter
        int position = 1;
        EscRunNode* current = head;
        while (current != nullptr) {
            if (current == target) {
                return position;
            }
            current = current->next;
            position++;
        }
        return 0;
    }

    void sortByPercent(bool highToLow) {    // Bubble sort by percent
        if (count < 2) {
            return;
        }

        bool swapped = true;
        while (swapped) {
            swapped = false;
            EscRunNode* current = head;

            while (current->next != nullptr) {
                bool needSwap = false;

                if (highToLow == true) {
                    needSwap = (current->data.percent < current->next->data.percent);
                } else {
                    needSwap = (current->data.percent > current->next->data.percent);
                }

                if (needSwap == true) {
                    EscRun temp = current->data;    // swap the data of the two nodes
                    current->data = current->next->data;
                    current->next->data = temp;
                    swapped = true;
                }

                current = current->next;
            }
        }
    }

    EscRunNode* getHead() const { return head; }
    EscRunNode* getTail() const { return tail; }
    int size() const { return count; }
};

// ---- LINKED LIST: ESCAPE PLAYERS ----

struct EscPlayer {      // one user and the best percent they ever got
    std::string name;
    int best;
    int plays;
};

int escValue(const EscPlayer& p, char mode) {   // the number we sort by
    if (mode == 'B') {
        return p.best;
    }
    return p.plays;
}

struct EscPlayerNode {
    EscPlayer data;
    EscPlayerNode* prev;
    EscPlayerNode* next;

    EscPlayerNode(const EscPlayer& playerData) : data(playerData), prev(nullptr), next(nullptr) {}
};

class EscPlayerList {
private:
    EscPlayerNode* head;
    EscPlayerNode* tail;
    int count;

public:
    EscPlayerList() : head(nullptr), tail(nullptr), count(0) {}

    ~EscPlayerList() {
        EscPlayerNode* current = head;
        while (current != nullptr) {
            EscPlayerNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const EscPlayer& player) {   // add a new user at the end
        EscPlayerNode* newNode = new EscPlayerNode(player);
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

    void removeNode(EscPlayerNode* node) {
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

    EscPlayerNode* search(const std::string& name) {    // find the user with this name
        EscPlayerNode* current = head;
        while (current != nullptr) {
            if (sameName(current->data.name, name)) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void sortBy(char mode) {    // Bubble sort, biggest first. mode: B = best percent, P = plays
        if (count < 2) {
            return;
        }
        bool swapped = true;
        while (swapped) {
            swapped = false;
            EscPlayerNode* current = head;
            while (current->next != nullptr) {
                if (escValue(current->data, mode) < escValue(current->next->data, mode)) {
                    EscPlayer temp = current->data;     // swap the data of the two nodes
                    current->data = current->next->data;
                    current->next->data = temp;
                    swapped = true;
                }
                current = current->next;
            }
        }
    }

    EscPlayerNode* getHead() const { return head; }
    EscPlayerNode* getTail() const { return tail; }
    int size() const { return count; }
};

StoryNode escScenes[SCENE_COUNT];
EscRunList escRuns;
EscPlayerList escPlayers;
int escMaxPoints = 1;       // best score the story allows, so 100% is possible

// ---- STORY SETUP ----

void setScene(StoryNode& scene, const std::string& title, const std::string& text, int points, bool isEnding, bool escaped) {
    scene.title = title;
    scene.text = text;
    scene.points = points;
    scene.isEnding = isEnding;
    scene.escaped = escaped;
    scene.optionCount = 0;
    for (int i = 0; i < 3; i++) {
        scene.options[i] = "";
        scene.next[i] = nullptr;
    }
}

void addOption(StoryNode& scene, const std::string& text, StoryNode* target) {   // link a choice to the next scene
    if (scene.optionCount >= 3 || target == nullptr) {
        return;
    }
    scene.options[scene.optionCount] = text;
    scene.next[scene.optionCount] = target;
    scene.optionCount++;
}

int bestFrom(const StoryNode* scene) {      // recursion: most points the player can still collect from this scene
    if (scene == nullptr) {
        return 0;
    }
    int bestNext = 0;
    for (int i = 0; i < scene->optionCount; i++) {
        int total = bestFrom(scene->next[i]);
        if (total > bestNext) {
            bestNext = total;
        }
    }
    return scene->points + bestNext;
}

void buildStory() {
    // rounds: every round cleared is worth 20 points
    setScene(escScenes[SCN_JUNCTION], "Round 1: The Junction",
        "A dead end splits into two narrow passages. Left: thick yellow fog that smells of vinegar and rotting flesh, with live cables spitting blue sparks. Right: a steep slope into blackness, tar-like fluid on the floor, and wet bare feet slapping toward you.",
        0, false, false);
    setScene(escScenes[SCN_ARM], "Round 2: The Severed Arm",
        "A rusted security door ends the tunnel. A dead worker lies against the wall, his severed arm pinned under a steel beam, still clutching a blood-soaked Blue Keycard. Beside the door, an emergency lever is caked in pulsing black spores.",
        20, false, false);
    setScene(escScenes[SCN_NURSERY], "Round 3: The Nursery Room",
        "You wade into a flooded hub that smells of formaldehyde. A glass cylinder of green fluid holds a translucent humanoid creature, its fingers twitching. The exit door flashes: SPECIMEN OVERFLOW DETECTED.",
        20, false, false);
    setScene(escScenes[SCN_DECK], "Round 4: The Observation Deck",
        "A monitoring deck overlooks a vast pit. An old elevator cage hangs from chains, your only way out. Behind you, something massive tears through the vents and the walls buckle.",
        20, false, false);
    setScene(escScenes[SCN_LIFT], "Final Round: The Escape Lift",
        "The lift grinds upward. Then something slams the roof and claws peel the steel open like a tin can. A black maw drips corrosive bile above you. Two emergency systems are in reach.",
        20, false, false);

    // endings
    setScene(escScenes[END_PIT], "DEAD END: THE PIT",
        "The sludge is a bottomless pit of grease. You slide into the dark and your knees shatter on landing. Cold hands drag you under the black pool.",
        0, true, false);
    setScene(escScenes[END_SPORES], "DEAD END: SPORES",
        "The crust shatters and spores burst into your face. Your lungs burn and black roots sprout in your throat, choking you from the inside.",
        0, true, false);
    setScene(escScenes[END_SPECIMEN], "DEAD END: THE SPECIMEN",
        "The glass retracts and the creature drops into the water. It crosses the room in a blink and drags you under, fingers deep in your throat.",
        0, true, false);
    setScene(escScenes[END_ABYSS], "DEAD END: THE ABYSS",
        "Pale claws burst through the floor mid-sprint. The gantry splits in two and you fall screaming into the bottomless dark.",
        0, true, false);
    setScene(escScenes[END_FROZEN], "DEAD END: FROZEN",
        "The vents spray liquid nitrogen into the cage. The beast barely flinches, but the freezing mist locks the lift mid-shaft. Its claws find you in the cold.",
        0, true, false);
    setScene(escScenes[END_SURVIVED], "SURVIVED",
        "The cut cable drags the beast into the abyss. The main hoist holds, and the lift bursts through the surface doors. The facility locks forever beneath the earth. You breathe the night air.",
        20, true, true);

    // choices: every choice links to the next scene
    addOption(escScenes[SCN_JUNCTION], "The Fogged Corridor: push through the yellow mist and sparking cables.", &escScenes[SCN_ARM]);
    addOption(escScenes[SCN_JUNCTION], "The Black Sludge Hallway: descend into the dark, stepping into the fluid.", &escScenes[END_PIT]);

    addOption(escScenes[SCN_ARM], "Reach under the beam and pry the keycard from his fingers.", &escScenes[SCN_NURSERY]);
    addOption(escScenes[SCN_ARM], "Yank the spore-covered emergency lever down.", &escScenes[END_SPORES]);

    addOption(escScenes[SCN_NURSERY], "Press the emergency flush button to drain the cylinder.", &escScenes[END_SPECIMEN]);
    addOption(escScenes[SCN_NURSERY], "Jam the override conduit into the power junction to shock the lock open.", &escScenes[SCN_DECK]);

    addOption(escScenes[SCN_DECK], "Sprint across the narrow steel gantry bridge.", &escScenes[END_ABYSS]);
    addOption(escScenes[SCN_DECK], "Clip a cargo pulley to the overhead cable and zip to the elevator roof.", &escScenes[SCN_LIFT]);

    addOption(escScenes[SCN_LIFT], "Cut the auxiliary support cable with the emergency shears.", &escScenes[END_SURVIVED]);
    addOption(escScenes[SCN_LIFT], "Slam the red FIRE SUPPRESSION button to blast the roof vents.", &escScenes[END_FROZEN]);

    escMaxPoints = bestFrom(&escScenes[SCN_JUNCTION]);
    if (escMaxPoints < 1) {     // never divide by zero
        escMaxPoints = 1;
    }
}

// ---- SCREEN HELPERS ----

int wrapText(const std::string& text, int width, std::string lines[], int maxLines) {   // cut a long text into lines that fit the screen
    int count = 0;
    std::string line = "";
    std::string word = "";

    for (size_t i = 0; i <= text.length(); i++) {
        char c = ' ';
        if (i < text.length()) {
            c = text[i];
        }

        if (c != ' ') {
            word += c;
        } else if (!word.empty()) {
            if (line.empty()) {
                line = word;
            } else if (static_cast<int>(line.length() + 1 + word.length()) <= width) {
                line = line + " " + word;
            } else {
                if (count < maxLines) {
                    lines[count] = line;
                    count++;
                }
                line = word;
            }
            word = "";
        }
    }

    if (!line.empty() && count < maxLines) {
        lines[count] = line;
        count++;
    }
    return count;
}

int printWrapped(const std::string& text, short x, short y, int width) {   // returns how many lines were printed
    std::string lines[MAX_WRAP_LINES];
    int n = wrapText(text, width, lines, MAX_WRAP_LINES);
    for (int i = 0; i < n; i++) {
        gotoxy(x, y + i);
        std::cout << lines[i];
    }
    return n;
}

int printOption(char letter, const std::string& text, short x, short y) {
    std::string lines[MAX_WRAP_LINES];
    int n = wrapText(text, ESC_TEXT_WIDTH - 3, lines, MAX_WRAP_LINES);
    for (int i = 0; i < n; i++) {
        gotoxy(x, y + i);
        if (i == 0) {
            setColor(14);
            std::cout << letter << ") ";
        } else {
            std::cout << "   ";
        }
        setColor(15);
        std::cout << lines[i];
    }
    return n;
}

void drawEscHeader() {
    clearScreen();
    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back    The Escape Protocol";
}

void strobeTitle(const std::string& title, short row) {     // emergency light blink
    for (int i = 0; i < 6; i++) {
        gotoxy(5, row);
        if (i % 2 == 0) {
            setColor(12);
        } else {
            setColor(4);
        }
        std::cout << title;
        Sleep(150);
    }
}

void showPrologue() {
    drawEscHeader();
    strobeTitle("!! EMERGENCY !!", 5);

    setColor(7);
    printWrapped(ESC_PROLOGUE, 5, 7, ESC_TEXT_WIDTH);

    gotoxy(8, 22);
    setColor(8);
    std::cout << "Press any key...";
    waitForKey();
}

void showScene(const StoryNode* scene) {
    drawEscHeader();

    gotoxy(5, 5);
    setColor(14);
    std::cout << scene->title;

    setColor(7);
    int row = 7;
    row += printWrapped(scene->text, 5, row, ESC_TEXT_WIDTH);
    row++;      // empty line before the choices

    for (int i = 0; i < scene->optionCount; i++) {
        row += printOption(static_cast<char>('A' + i), scene->options[i], 5, row);
    }

    gotoxy(5, 27);
    setColor(14);
    if (scene->optionCount == 2) {
        std::cout << "Choose: A or B      Q: Quit";
    } else {
        std::cout << "Choose: A, B or C   Q: Quit";
    }
}

int askOption(int optionCount) {    // wait for A, B or C (1, 2, 3 also work). Returns 0, 1, 2 or ESC_QUIT
    while (true) {
        int key = readKey(false);   // here the letters are the answers, so W A S D are not moves
        if (key == KEY_ESC || key == 'q' || key == 'Q') {
            return ESC_QUIT;
        }

        int pick = -1;
        if (key >= 'a' && key <= 'z') {
            pick = key - 'a';
        } else if (key >= 'A' && key <= 'Z') {
            pick = key - 'A';
        } else if (key >= '1' && key <= '9') {
            pick = key - '1';
        }

        if (pick >= 0 && pick < optionCount) {
            return pick;
        }
    }
}

void showEscEmpty(const std::string& title) {   // message when nothing is saved yet
    clearScreen();
    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back   " << title;
    gotoxy(8, 10);
    setColor(7);
    std::cout << "Nothing saved yet.";
    gotoxy(8, 12);
    std::cout << "Finish a run first!";
    gotoxy(8, 21);
    setColor(8);
    std::cout << "Press any key...";
    waitForKey();
}

// ---- PLAYER RECORDS ----

bool updatePlayer(const std::string& name, int percent) {   // true if this is the best percent of the user
    EscPlayerNode* player = escPlayers.search(name);
    if (player == nullptr) {
        EscPlayer newPlayer = { name, percent, 1 };
        escPlayers.push_back(newPlayer);
        return true;
    }

    player->data.plays++;
    if (percent > player->data.best) {
        player->data.best = percent;
        return true;
    }
    return false;
}

void refreshPlayer(const std::string& name) {   // after a run is deleted, count the runs of that user again
    EscPlayerNode* player = escPlayers.search(name);
    if (player == nullptr) {
        return;
    }

    int best = 0;
    int plays = 0;
    EscRunNode* current = escRuns.getHead();
    while (current != nullptr) {
        if (sameName(current->data.name, name)) {
            plays++;
            if (current->data.percent > best) {
                best = current->data.percent;
            }
        }
        current = current->next;
    }

    if (plays == 0) {
        escPlayers.removeNode(player);
    } else {
        player->data.best = best;
        player->data.plays = plays;
    }
}

// ---- PLAYING ----

bool playEscape(const std::string& name, bool showIntro) {   // one full run. Returns true if the player wants to retry
    PathList path;
    StoryNode* current = &escScenes[SCN_JUNCTION];
    int points = 0;

    if (showIntro == true) {
        showPrologue();
    }

    while (current != nullptr && current->isEnding == false) {
        if (current->optionCount == 0) {    // a scene with no choices and no ending, should never happen
            current = nullptr;
            break;
        }

        points += current->points;
        showScene(current);

        int pick = askOption(current->optionCount);
        if (pick == ESC_QUIT) {
            return false;       // Esc = leave without saving
        }

        path.push_back(static_cast<char>('A' + pick));
        current = current->next[pick];
    }

    if (current == nullptr) {
        drawEscHeader();
        gotoxy(8, 10);
        setColor(12);
        std::cout << "The story is broken here.";
        gotoxy(8, 21);
        setColor(8);
        std::cout << "Press any key...";
        waitForKey();
        return false;
    }

    points += current->points;
    int percent = points * 100 / escMaxPoints;
    if (percent > 100) {
        percent = 100;
    }

    EscRun run;
    run.name = name;
    run.percent = percent;
    run.points = points;
    run.ending = current->title;
    run.path = path.toString();
    run.date = getTodayDate();
    escRuns.push_back(run);

    bool newBest = updatePlayer(name, percent);
    EscPlayerNode* player = escPlayers.search(name);

    drawEscHeader();
    gotoxy(5, 5);
    if (current->escaped == true) {
        setColor(10);
    } else {
        setColor(12);
    }
    std::cout << "ENDING: " << current->title;

    setColor(7);
    int row = 7;
    row += printWrapped(current->text, 5, row, ESC_TEXT_WIDTH);
    row++;

    gotoxy(5, row);
    setColor(15);
    std::cout << "Score: " << points << " pts = " << percent << "%";
    gotoxy(5, row + 1);
    std::cout << "Path : " << run.path;
    if (player != nullptr) {
        gotoxy(5, row + 2);
        std::cout << "Best : " << player->data.best << "%";
    }
    if (newBest == true) {
        gotoxy(5, row + 4);
        setColor(10);
        std::cout << "NEW BEST!";
    }

    gotoxy(5, 25);
    setColor(14);
    std::cout << "R: Retry   Q: Menu";

    while (true) {
        int key = readKey(true);
        if (key == 'r') {
            return true;
        } else if (key == KEY_ESC) {
            return false;
        }
    }
}

void startEscape() {    // ask for the name, then play until the player stops
    drawEscHeader();
    gotoxy(5, 6);
    setColor(8);
    std::cout << "Your best % is saved";
    gotoxy(5, 7);
    std::cout << "under your name.";

    std::string name = askText("Your name: ", 10);

    bool again = true;
    bool firstRun = true;
    while (again == true) {
        again = playEscape(name, firstRun);
        firstRun = false;
    }
}






// ---- BEST RANKINGS ----

void showEscRankings() {    // best percent of every user
    if (escPlayers.size() == 0) {
        showEscEmpty("Best Rankings");
        return;
    }

    char mode = 'B';
    bool viewing = true;

    while (viewing == true) {
        escPlayers.sortBy(mode);
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back      Best Rankings";

        gotoxy(5, 5);
        setColor(8);
        if (mode == 'B') {
            std::cout << "Sorted by: Best %";
        } else {
            std::cout << "Sorted by: Plays";
        }

        gotoxy(5, 7);   // column titles
        setColor(15);
        std::cout << "#  Name        Best  Plays";

        EscPlayerNode* current = escPlayers.getHead();   // one row per user, max 12 rows fit
        int rank = 1;
        setColor(7);
        while (current != nullptr && rank <= 12) {
            int row = 7 + rank;
            gotoxy(5, row);
            std::cout << rank;
            gotoxy(8, row);
            std::cout << current->data.name;
            gotoxy(20, row);
            std::cout << current->data.best << "%";
            gotoxy(26, row);
            std::cout << current->data.plays;
            current = current->next;
            rank++;
        }

        gotoxy(5, 22);
        setColor(14);
        std::cout << "1:Best %  2:Plays";
        gotoxy(5, 23);
        std::cout << "A/D:Switch sort  Q:Back";

        int key = readKey(true);
        if (key == '1') {
            mode = 'B';
        } else if (key == '2') {
            mode = 'P';
        } else if (key == KEY_LEFT || key == KEY_RIGHT) {
            if (mode == 'B') {
                mode = 'P';
            } else {
                mode = 'B';
            }
        } else if (key == KEY_ESC) {
            viewing = false;
        }
    }
}

// ---- START MENU ----

void launchEscape(const App& app) {     // START MENU of the Escape Protocol app
    hideCursor();
    buildStory();

    std::string options[3] = { "Play", "Best Rankings", "Back" };
    int selected = 0;
    bool inMenu = true;

    while (inMenu == true) {
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back         " << app.icon << " App";

        gotoxy(12, 6);
        setColor(12);
        std::cout << "THE ESCAPE";
        gotoxy(12, 7);
        std::cout << "PROTOCOL";

        for (int i = 0; i < 3; i++) {   // draw the 3 options
            gotoxy(11, 11 + i * 2);
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
        std::cout << "W/S: Move   Enter/E: Pick";

        int key = readKey(true);
        if (key == KEY_UP && selected > 0) {
            selected--;
        } else if (key == KEY_DOWN && selected < 2) {
            selected++;
        } else if (key == KEY_ENTER) {
            if (selected == 0) {
                startEscape();
            } else if (selected == 1) {
                showEscRankings();
            } else {
                inMenu = false;
            }
        } else if (key == KEY_ESC) {
            inMenu = false;
        }
    }

    setColor(7);
    clearScreen();
}
// end of escape protocol code


// ---- MODULE 6: YOUWHO MAIL ----

// a simple email app. Every sent email is one node of a doubly linked list

const std::string MY_ADDRESS = "me@youwho.com";

// ---- LINKED LIST: SENT EMAILS ----

struct Email {
    std::string to;
    std::string subject;
    std::string body;
    std::string date;
};

struct EmailNode {
    Email data;
    EmailNode* prev;
    EmailNode* next;

    EmailNode(const Email& emailData) : data(emailData), prev(nullptr), next(nullptr) {}
};

class EmailList {
private:
    EmailNode* head;
    EmailNode* tail;
    int count;

public:
    EmailList() : head(nullptr), tail(nullptr), count(0) {}

    ~EmailList() {
        EmailNode* current = head;
        while (current != nullptr) {
            EmailNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const Email& email) {    // a new sent email goes at the end
        EmailNode* newNode = new EmailNode(email);
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

    void removeNode(EmailNode* node) {
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

    EmailNode* search(const std::string& keyword) {     // first email with the keyword in the address or subject, ignores CAPS
        std::string key = toLowerStr(keyword);
        EmailNode* current = head;
        while (current != nullptr) {
            if (toLowerStr(current->data.to).find(key) != std::string::npos ||
                toLowerStr(current->data.subject).find(key) != std::string::npos) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    int getPosition(EmailNode* target) const {  // email number counter
        int position = 1;
        EmailNode* current = head;
        while (current != nullptr) {
            if (current == target) {
                return position;
            }
            current = current->next;
            position++;
        }
        return 0;
    }

    EmailNode* getHead() const { return head; }
    EmailNode* getTail() const { return tail; }
    int size() const { return count; }
};

EmailList sentEmails;

bool isValidAddress(const std::string& address) {   // simple check: something@something, one @, no spaces
    size_t at = address.find('@');
    if (at == std::string::npos || at == 0 || at == address.length() - 1) {
        return false;
    }
    if (address.find(' ') != std::string::npos) {
        return false;
    }
    if (address.find('@', at + 1) != std::string::npos) {
        return false;
    }
    return true;
}

void drawMailHeader(const std::string& title) {
    clearScreen();
    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back   " << title;
}

// ---- WRITE AN EMAIL ----

void composeEmail() {
    drawMailHeader("YouWho: New Email");

    gotoxy(5, 5);
    setColor(15);
    std::cout << "To     :";
    gotoxy(5, 7);
    std::cout << "Subject:";
    gotoxy(5, 9);
    std::cout << "Message:";
    gotoxy(5, 25);
    setColor(14);
    std::cout << "Enter: Next / Send   Esc: Cancel";

    Email mail;

    bool valid = false;
    while (valid == false) {        // ask the address until it looks right
        if (readField(14, 5, 25, 25, mail.to) == false) {
            return;                 // Esc = cancel, nothing is sent
        }

        if (isValidAddress(mail.to) == true) {
            valid = true;
            clearRows(14, 14);
        } else {
            gotoxy(14, 5);          // wipe the wrong address
            std::cout << std::string(25, ' ');
            gotoxy(5, 14);
            setColor(12);
            std::cout << "Use an address like a@b.com";
        }
    }

    if (readField(14, 7, 25, 25, mail.subject) == false) {
        return;
    }
    if (mail.subject.empty()) {
        mail.subject = "(no subject)";
    }

    if (readField(5, 10, 33, 99, mail.body) == false) {
        return;
    }
    if (mail.body.empty()) {
        mail.body = "(no message)";
    }

    mail.date = getTodayDate();
    sentEmails.push_back(mail);

    drawMailHeader("YouWho: New Email");
    gotoxy(12, 9);
    setColor(10);
    std::cout << "Email sent!";
    gotoxy(5, 12);
    setColor(7);
    std::cout << "To     : " << mail.to;
    gotoxy(5, 13);
    std::cout << "Subject: " << mail.subject;
    gotoxy(8, 21);
    setColor(8);
    std::cout << "Press any key...";
    waitForKey();
}

// ---- SENT MAIL ----

void drawMailRecord(EmailNode* current, const std::string& message) {
    drawMailHeader("Sent Mail");

    gotoxy(5, 5);
    setColor(15);
    std::cout << "Email " << sentEmails.getPosition(current) << " of " << sentEmails.size();
    setColor(8);
    if (current->next == nullptr) {
        std::cout << "  (newest)";
    } else if (current->prev == nullptr) {
        std::cout << "  (oldest)";
    }

    gotoxy(5, 7);
    setColor(7);
    std::cout << "From   : " << MY_ADDRESS;
    gotoxy(5, 8);
    std::cout << "To     : " << current->data.to;
    gotoxy(5, 9);
    std::cout << "Subject: " << current->data.subject;
    gotoxy(5, 10);
    std::cout << "Date   : " << current->data.date;

    gotoxy(5, 12);
    setColor(15);
    std::cout << "Message:";
    setColor(7);
    printWrapped(current->data.body, 5, 13, ESC_TEXT_WIDTH);

    gotoxy(5, 18);
    setColor(12);
    std::cout << message;

    gotoxy(5, 21);
    setColor(14);
    std::cout << "A/D: Older/Newer   F: Find";
    gotoxy(5, 22);
    std::cout << "X: Delete   Q: Back";
}

void showSentMail() {   // scroll through the sent emails, find, delete
    if (sentEmails.size() == 0) {
        showEmptyScreen("Sent Mail", "Send an email first!");
        return;
    }

    EmailNode* current = sentEmails.getTail();      // start with the newest email
    bool viewing = true;
    std::string message = "";

    while (viewing == true) {
        drawMailRecord(current, message);
        message = "";

        int key = readKey(true);

        if (key == KEY_LEFT) {
            if (current->prev != nullptr) {
                current = current->prev;
            } else {
                message = "This is the oldest email.";
            }
        }
        else if (key == KEY_RIGHT) {
            if (current->next != nullptr) {
                current = current->next;
            } else {
                message = "This is the newest email.";
            }
        }
        else if (key == 'f') {
            gotoxy(5, 19);
            setColor(7);
            std::cout << "Find: ";
            std::string keyword = "";
            if (readField(11, 19, 20, 20, keyword) == true && !keyword.empty()) {
                EmailNode* found = sentEmails.search(keyword);
                if (found != nullptr) {
                    current = found;
                    message = "Found!";
                } else {
                    message = "No email with that.";
                }
            }
        }
        else if (key == 'x' || key == KEY_DELETE) {
            EmailNode* showAfter = current->next;
            if (showAfter == nullptr) {
                showAfter = current->prev;
            }

            sentEmails.removeNode(current);
            current = showAfter;

            if (current == nullptr) {
                clearScreen();
                gotoxy(8, 10);
                setColor(7);
                std::cout << "All emails deleted.";
                gotoxy(8, 21);
                setColor(8);
                std::cout << "Press any key...";
                waitForKey();
                viewing = false;
            } else {
                message = "Email deleted.";
            }
        }
        else if (key == KEY_ESC) {
            viewing = false;
        }
    }
}

// ---- START MENU ----

void launchMail(const App& app) {   // START MENU of the YouWho app
    hideCursor();

    std::string options[3] = { "Compose", "Sent Mail", "Back" };
    int selected = 0;
    bool inMenu = true;

    while (inMenu == true) {
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back         " << app.icon << " App";

        gotoxy(10, 6);
        setColor(11);
        std::cout << "Y O U W H O   M A I L";
        gotoxy(11, 8);
        setColor(8);
        std::cout << "Sent emails: " << sentEmails.size();

        for (int i = 0; i < 3; i++) {   // draw the 3 options
            gotoxy(13, 12 + i * 2);
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
        std::cout << "W/S: Move   Enter/E: Pick";

        int key = readKey(true);
        if (key == KEY_UP && selected > 0) {
            selected--;
        } else if (key == KEY_DOWN && selected < 2) {
            selected++;
        } else if (key == KEY_ENTER) {
            if (selected == 0) {
                composeEmail();
            } else if (selected == 1) {
                showSentMail();
            } else {
                inMenu = false;
            }
        } else if (key == KEY_ESC) {
            inMenu = false;
        }
    }

    setColor(7);
    clearScreen();
}
// end of mail code


// ---- MODULE 7: APPMART ----

//appmart  codes

int customAppCount = 0;     // gives every user-made app its own id

std::string createStoreApp() {      // add a new placeholder app to AppMart. Returns a message for the screen
    if (storeApps.size() >= MAX_APPS) {
        return "AppMart is full.";
    }

    clearRows(22, 23);
    gotoxy(5, 22);
    setColor(14);
    std::cout << "Enter: Create   Esc: Cancel";
    gotoxy(5, 23);
    setColor(7);
    std::cout << "App name: ";

    std::string newName = "";
    if (readField(15, 23, 14, 14, newName) == false) {      // 14 letters is the most that fits
        return "Cancelled.";
    }

    size_t first = newName.find_first_not_of(' ');      // cut the spaces at the start and the end
    if (first == std::string::npos) {
        return "Name can't be empty.";
    }
    size_t last = newName.find_last_not_of(' ');
    newName = newName.substr(first, last - first + 1);

    customAppCount++;
    App newApp;
    newApp.name = newName;
    newApp.icon = std::string(1, static_cast<char>(std::toupper(static_cast<unsigned char>(newName[0]))));
    newApp.id = "custom" + std::to_string(customAppCount);
    storeApps.push_back(newApp);

    return newName + " added to AppMart!";
}

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
            std::cout << "Remove an app on the home";
            gotoxy(8, 14);
            std::cout << "screen to find it here.";
        }

        int step = 2;           // space between rows
        if (itemCount > 7) {
            step = 1;           // squeeze the rows so a long list still fits
        }

        for (int i = 0; i < itemCount; i++) {   // one row per app 
            std::string name = items[i]->data.name;
            while (name.length() < 14) {        // name  padding
                name = name + " ";
            }

            gotoxy(6, 8 + i * step);
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
        std::cout << "W/S: Move   Enter/E: Download";
        gotoxy(5, 26);
        std::cout << "N: New app   Q: Back";

        message = "";

        int key = readKey(true);
        if (key == KEY_UP && selected > 0) {
            selected--;
        } else if (key == KEY_DOWN && selected < itemCount - 1) {
            selected++;
        } else if (key == KEY_ENTER) {
            if (itemCount > 0) {
                if (homeApps.size() >= MAX_APPS) {      // the home screen can only show MAX_APPS apps
                    message = "Home screen is full.";
                } else {
                    AppNode* node = items[selected];
                    App chosen = node->data;        
                    storeApps.removeNode(node);
                    homeApps.push_back(chosen);     
                    applyHomeSort();                
                    message = chosen.name + " added to home!";
                }
            } else {
                message = "Nothing to download.";
            }
        } else if (key == 'n') {                // N = make a new placeholder app (A is already "left")
            message = createStoreApp();
        } else if (key == KEY_ESC) {
            running = false;
        }
    }

    setColor(7);
    clearScreen();
}
// end of appmart code

// ---- PLACEHOLDER APPS ----

void launchPlaceholder(const App& app) {    // every app without its own code shows this screen
    clearScreen();

    gotoxy(5, 3);
    setColor(11);
    std::cout << "< Back         " << app.icon << " App";

    gotoxy(5, 6);
    setColor(15);
    std::cout << app.name;

    gotoxy(5, 9);
    setColor(14);
    std::cout << "This app is a placeholder.";

    gotoxy(5, 11);
    setColor(7);
    std::cout << "Check out our functional apps:";

    std::string functionalApps[7] = { "Phone", "Card Game", "BitBite", "Rock 'n' Clash", "AppMart", "EscapeProtocol", "YouWho" };
    for (int i = 0; i < 7; i++) {
        gotoxy(8, 13 + i);
        std::cout << "- " << functionalApps[i];
    }

    gotoxy(5, 21);
    setColor(8);
    std::cout << "Press [Q] to Return";

    while (true) {
        int key = readKey(true);
        if (key == KEY_ESC || key == KEY_BACKSPACE) {
            break;
        }
    }
    clearScreen();
}

// ---- MODULE 8: PHONE (SIMPLE DIALER) ----

std::string lastCalled = "";    // the last number that was called

void launchPhone(const App& app) {
    hideCursor();
    std::string number = "";
    std::string message = "";
    bool running = true;

    while (running == true) {
        clearScreen();

        gotoxy(5, 3);
        setColor(11);
        std::cout << "< Back         " << app.icon << " App";

        gotoxy(15, 6);
        setColor(14);
        std::cout << "DIALER";

        gotoxy(11, 8);      // number box, padded so old digits are erased
        setColor(15);
        std::cout << "[ " << number << std::string(12 - number.length(), ' ') << " ]";

        setColor(7);        // keypad
        gotoxy(15, 11);
        std::cout << "1  2  3";
        gotoxy(15, 13);
        std::cout << "4  5  6";
        gotoxy(15, 15);
        std::cout << "7  8  9";
        gotoxy(15, 17);
        std::cout << "*  0  #";

        if (!lastCalled.empty()) {
            gotoxy(8, 20);
            setColor(8);
            std::cout << "Last called: " << lastCalled;
        }

        gotoxy(8, 22);
        setColor(12);
        std::cout << message;
        message = "";

        gotoxy(5, 25);
        setColor(14);
        std::cout << "0-9: Type   Enter: Call";
        gotoxy(5, 26);
        std::cout << "Bksp:Erase R:Redial Esc:Back";

        int key = readKey(false);   // letters stay letters here
        if ((key >= '0' && key <= '9') || key == '*' || key == '#') {
            if (number.length() < 12) {
                number += static_cast<char>(key);
            }
        } else if (key == KEY_BACKSPACE) {
            if (!number.empty()) {
                number.erase(number.length() - 1);
            }
        } else if (key == 'r' || key == 'R') {
            if (lastCalled.empty()) {
                message = "No number to redial.";
            } else {
                number = lastCalled;
            }
        } else if (key == KEY_ENTER) {
            if (number.empty()) {
                message = "Type a number first.";
            } else {
                gotoxy(8, 22);
                setColor(10);
                std::cout << "Calling " << number << "...";
                Sleep(1500);
                lastCalled = number;
                number = "";
                message = "Call ended.";
            }
        } else if (key == KEY_ESC) {
            running = false;
        }
    }

    setColor(7);
    clearScreen();
}

// ---- HOME SCREEN CONTROLS ----

// open an app from the home screen. Uses the id, so renaming an app never breaks it
void openApp(const App& app) {
    if (app.id == "card") {
        launchCard(app);
    }
    else if (app.id == "snake") {
        launchSnake(app);
    }
    else if (app.id == "rps") {
        launchRps(app);
    }
    else if (app.id == "escape") {
        launchEscape(app);
    }
    else if (app.id == "mail") {
        launchMail(app);
    }
    else if (app.id == "store") {
        launchStore(app);
    }
    else if (app.id == "phone") {
        launchPhone(app);
    }
    else {
        launchPlaceholder(app);     // every other app, including user-made ones
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
    if (node == nullptr) {      // nothing selected
        return "No app selected.";
    }

    if (node->data.id == "store") {     
        return node->data.name + " can't be removed.";
    }

    if (storeApps.size() >= MAX_APPS) {     // AppMart can only show MAX_APPS apps
        return "AppMart is full.";
    }

    App removed = node->data;      
    homeApps.removeNode(node);
    storeApps.push_back(removed);
    return removed.name + " is back in AppMart.";
}

// rename an app: only the name inside the node changes
std::string renameHomeApp(AppNode* node) {
    if (node == nullptr) {      // nothing selected
        return "No app selected.";
    }

    clearRows(22, 23);
    gotoxy(5, 22);
    setColor(14);
    std::cout << "Enter: Save   Esc: Cancel";
    gotoxy(5, 23);
    setColor(7);
    std::cout << "New name: ";

    std::string newName = "";
    if (readField(15, 23, 14, 14, newName) == false) {      // 14 letters is the most that fits on the grid
        return "Rename cancelled.";
    }

    size_t first = newName.find_first_not_of(' ');          // cut the spaces at the start and the end
    if (first == std::string::npos) {
        return "Name can't be empty.";
    }
    size_t last = newName.find_last_not_of(' ');
    newName = newName.substr(first, last - first + 1);

    node->data.name = newName;
    applyHomeSort();            // the order may change if the list is sorted
    return "Renamed to " + newName;
}

// ---- MAIN ----

int main() {
    hideCursor();
    system("cls");

    // apps at home screen initially (name, icon, id)
    homeApps.push_back({"Phone",          ">", "phone"});
    homeApps.push_back({"Card Game",      "#", "card"});
    homeApps.push_back({"BitBite",        "S", "snake"});
    homeApps.push_back({"Rock 'n' Clash", "R", "rps"});
    homeApps.push_back({"AppMart",        "6", "store"});
    homeApps.push_back({"EscapeProtocol", "E", "escape"});
    homeApps.push_back({"YouWho",         "Y", "mail"});

    // apps on the appmart (placeholders, they only show the placeholder screen)
        storeApps.push_back({"PasteBook", "P", "facebook"});
    storeApps.push_back({"Spotibye",  "~", "spotify"});
    storeApps.push_back({"LikLok",    "T", "tiktok"});
    storeApps.push_back({"Netflicks", "N", "netflix"});

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

        // while searching the letters are typed, so W A S D only work as arrows when not searching
        int key = readKey(searching == false);

        if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
            moveSelection(selected, visCount, key);
        }
        else if (searching == true) {
            // search bar mode
            if (key == KEY_ENTER) {
                if (visCount > 0) {
                    App toOpen = visible[selected]->data;   // copy, the list may change inside the app
                    searching = false;
                    filter = "";
                    openApp(toOpen);
                    selected = indexOfId(homeApps, toOpen.id);  // keep it highlighted on the normal home screen
                }
            } else if (key == KEY_ESC) {
                searching = false;
                filter = "";
                selected = 0;
            } else if (key == KEY_BACKSPACE) {
                if (!filter.empty()) {
                    filter.erase(filter.length() - 1);
                    selected = 0;
                }
            } else if (key >= 32 && key <= 126) {     // normal typed character
                if (filter.length() < 20) {
                    filter += static_cast<char>(key);
                    selected = 0;
                }
            }
        }
        else {
            // ---- NORMAL HOME MODE ----
            if (key == KEY_ENTER) {
                if (visCount > 0) {
                    App toOpen = visible[selected]->data;
                    openApp(toOpen);
                    selected = indexOfId(homeApps, toOpen.id);
                }
            } else if (key == KEY_ESC) {
                running = false;
            } else if (key == 'z') {
                // sort
                std::string keepId = "";
                if (visCount > 0) {
                    keepId = visible[selected]->data.id;    // keeps highlight
                }

                if (homeSortMode == 1) {
                    homeSortMode = 2;
                    message = "Sorted: Z-A.";
                } else {
                    homeSortMode = 1;
                    message = "Sorted: A-Z.";
                }
                applyHomeSort();
                selected = indexOfId(homeApps, keepId);
            } else if (key == 'f' || key == '/') {
                searching = true;       // open the search bar
                filter = "";
                selected = 0;
            } else if (key == 'r') {
                if (visCount > 0) {
                    std::string keepId = visible[selected]->data.id;
                    message = renameHomeApp(visible[selected]);
                    selected = indexOfId(homeApps, keepId);     // the app may have moved after sorting
                }
            } else if (key == 'x' || key == KEY_DELETE) {
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
