 while (true) {
        if (_kbhit()) {
            char c = _getch();

            return c - '0';
        }
    }
}
int showMenuSettings() {
    system("cls");