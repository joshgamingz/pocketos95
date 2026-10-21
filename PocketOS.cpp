#include <iostream>
#include <vector>
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

// color settings
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Clear pixels in bezel
void clearScreen() {
    for (int y = PHONE_TOP; y <= PHONE_BOTTOM; ++y) {
        gotoxy(PHONE_LEFT, y);
        cout << "                                   ";
    }
}

// print phone frame
void drawPhoneFrame() {
    gotoxy(0, 0);
    setColor(8); // Dark gray frame
    cout << "  .-------------------------------------.\n";
    cout << "  |                (  O  )              |\n";
    cout << "  |-------------------------------------|\n";
    for (int i = 0; i < 25; ++i) {
        cout << "  |                                     |\n";
    }
    cout << "  |-------------------------------------|\n";
    cout << "  |                 [ O ]               |\n";
    cout << "  '-------------------------------------'\n";
    setColor(7);
}

// home screen display
void renderHomeScreen(const vector<App>& apps, int selected) {
    // Status bar
    gotoxy(5, 3);
    setColor(11); // Cyan text
    cout << "10:55 PM        5G [|||]";

    // App grid (2 Columns)
    for (size_t i = 0; i < apps.size(); ++i) {
        short col = (i % 2 == 0) ? 6 : 18;
        short row = 6 + static_cast<short>(i / 2) * 3;

        gotoxy(col, row);
        if (static_cast<int>(i) == selected) {
            setColor(240); // Highlight selected app (Black text on White)
            cout << "[" << apps[i].icon << " " << apps[i].name << "]";
        } else {
            setColor(7); // Normal white text
            cout << " " << apps[i].icon << " " << apps[i].name << " ";
        }
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
void launchCard(const App& app) {
	hideCursor();
    clearScreen();
    int selected = 0;
    int score = 0;
    bool running = true;
    
     vector<App> apps = {
        {"Phone",   ">"},
        {"YouWho?", "#"},
        {"Camera",  "O"},
        {"Gallery", "*"},
        {"Music",   "~"},
        {"Settings","@"}
    };
    
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
	
	
    while (running) {
        gotoxy(10,8);
	cout << randomNum;
	int newNum = rand() % 10 + 1;
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
			   }
               
			}else{
			   	
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

    vector<App> apps = {
        {"Phone",   ">"},
        {"YouWho?", "#"},
        {"Camera",  "O"},
        {"Gallery", "*"},
        {"Music",   "~"},
        {"Settings","@"}
    };

    int selected = 0;
    bool running = true;

    drawPhoneFrame();

    while (running) {
        renderHomeScreen(apps, selected);

        int ch = _getch();
        if (ch == 224 || ch == 0) { // Arrow key prefix handling
            switch (_getch()) {
                case KEY_UP:
                    if (selected >= 2) selected -= 2;
                    break;
                case KEY_DOWN:
                    if (selected + 2 < static_cast<int>(apps.size())) selected += 2;
                    break;
                case KEY_LEFT:
                    if (selected % 2 == 1) selected -= 1;
                    break;
                case KEY_RIGHT:
                    if (selected % 2 == 0 && selected + 1 < static_cast<int>(apps.size())) selected += 1;
                    break;
            }
        } else if (ch == KEY_ENTER) {
        	if(apps[selected].name == "YouWho?"){
               launchCard(apps[selected]);
               
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