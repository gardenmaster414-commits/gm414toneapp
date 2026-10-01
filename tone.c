#include <windows.h>

double choose_tone(int input)
{
    switch (input)
    {
        case 1:
            return 293.6648;

        case 2:
            return 349.2282;

        case 3:
            return 440.0000;

        case 4:
            return 493.8833;

        case 5:
            return 587.3295;

        default:
            return 0.0;
    }
}

__declspec(dllexport)
void play_tone(int input)
{
    double frequency = choose_tone(input);

    if (frequency == 0.0)
        return;

    Beep((DWORD)frequency, 500);
}