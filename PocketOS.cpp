#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

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
    cout << "  |                 (  O  )               |\n";
    cout << "  |-------------------------------------|\n";
    for (int i = 0; i < 25; ++i) {
        cout << "  |                                     |\n";
    }
    cout << "  |-------------------------------------|\n";
    cout << "  |                  [ O ]                |\n";
    cout << "  '-------------------------------------'\n";
    setColor(7);
}

// home screen display
void renderHomeScreen(const AppList& apps, AppNode* selectedApp) {
    // Status bar
    gotoxy(5, 3);
    setColor(11); // Cyan text
    cout << "10:55 PM        5G [|||]";

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

    // Bottom dock
    gotoxy(5, 15);
    setColor(8);
    cout << "---------------------";
    gotoxy(5, 16);
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