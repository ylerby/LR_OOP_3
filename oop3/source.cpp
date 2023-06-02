#include <windows.h>

#include "header.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HDC hdc;

HPEN WhitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
HPEN BlackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
HPEN RedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
HPEN GreenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 150));

HWND GetConcolWindow()
{
    char str[128];
    LPWSTR title = (LPWSTR)"xxxxxxxxxxxxxxxxxx";
    GetConsoleTitle((LPWSTR)str, sizeof((LPWSTR)str));
    SetConsoleTitle(title);
    Sleep(100);
    HWND hwnd = FindWindow(NULL, (LPWSTR)title);
    SetConsoleTitle((LPWSTR)str);
    return hwnd;
}

bool IsHit(Car* car, Bus* CurrentBus)
{
    int carX = car->GetX(), carY = car->GetY();
    int carSide = car->GetSide();
    bool hitX = CurrentBus->GetX() + 3.4 * CurrentBus->GetSide() >= carX - carSide && CurrentBus->GetX() - CurrentBus->GetSide() / 3 <= carX + carSide;
    bool hitY = CurrentBus->GetY() + 1.1 * CurrentBus->GetSide() >= carY - carSide && CurrentBus->GetY() - CurrentBus->GetSide() / 2.5 <= carY + carSide;
    return hitX && hitY;
}

int main() {
    int x0 = 100;
    int y0 = 80;
    int Side = 100;
    int x, y;
    bool hit[CarCount] = { false, false, false };
    int j = 0;

    HWND hwnd = GetConcolWindow();

    system("color F0");
    setlocale(LC_ALL, "rus");

    if (hwnd != NULL)
    {
        hdc = GetWindowDC(hwnd);

        if (hdc != 0)
        {
            EnemyCar enemyCar1(x0 + 300, y0 + 250, 20);
            EnemyVan enemyVan1(x0 + 600, y0 + 400, 25);
            FriendCar friendCar1(x0 + 800, y0 + 420, 30);
            Car* Cars[CarCount] = { &enemyCar1, &enemyVan1, &friendCar1 };

            Bus bus(x0, y0, Side);
            BusWithBonnet busWithBonnet(x0, y0, Side);
            BusWithRoof busWithRoof(x0, y0, Side);
            BusWithRoofAndBonnet busWithRoofAndBonnet(x0, y0, Side);

            Bus* Buses[BusCount] = { &bus, &busWithBonnet, &busWithRoof, &busWithRoofAndBonnet };

            for (auto it : Cars) {
                it->Show();
            }

            Bus* CurrentBus = Buses[0];
            CurrentBus->Show();

            int transition_matrix[4][3] = {
                                            {1, 2, 3},
                                            {2, 3, 3},
                                            {0, 0, 0},
                                            {0, 2, 3}
            };

            while (true) {
                CurrentBus->Drag(50);
                Sleep(10);
                for (int i = 0; i < 3; i++) {
                    if (IsHit(Cars[i], CurrentBus) && Cars[i]->IsVisible()) {
                        Cars[i]->Hide();
                        Bus* copyBus = Buses[transition_matrix[i][j]];

                        copyBus->SetX(CurrentBus->GetX());
                        copyBus->SetY(CurrentBus->GetY());

                        j = transition_matrix[i][j];

                        CurrentBus = copyBus;
                        CurrentBus->Show();
                    }
                }
            }
        }
    }
}