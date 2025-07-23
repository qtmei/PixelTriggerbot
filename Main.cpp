#include <Windows.h>
#include <iostream>

using namespace std;

const int PIXEL_COUNT = 4, THRESHOLD = 16;
RECT screen;

void GetPixels(COLORREF* pixels)
{
	int index = 0;

	for (int x = -1; x < 2; x += 2)
		for (int y = -1; y < 2; y += 2)
			pixels[index++] = GetPixel(GetDC(GetActiveWindow()), (int)screen.right / 2 + x, (int)screen.bottom / 2 + y);
}

COLORREF GetAveragePixelColor(COLORREF* pixels) //calculate average RGB values of 4 pixels
{
	int tempR = 0, tempG = 0, tempB = 0;

	for(int i = 0; i < PIXEL_COUNT; i++)
	{
		tempR += (int)GetRValue(pixels[i]);
		tempG += (int)GetGValue(pixels[i]);
		tempB += (int)GetBValue(pixels[i]);
	}

	return RGB(tempR / PIXEL_COUNT, tempG / PIXEL_COUNT, tempB / PIXEL_COUNT);
}

bool IsOutOfThreshold(COLORREF old, COLORREF current)
{
	return (int)GetRValue(old) > (int)GetRValue(current) + THRESHOLD || (int)GetRValue(old) < (int)GetRValue(current) - THRESHOLD || (int)GetGValue(old) > (int)GetGValue(current) + THRESHOLD || (int)GetGValue(old) < (int)GetGValue(current) - THRESHOLD || (int)GetBValue(old) > (int)GetBValue(current) + THRESHOLD || (int)GetBValue(old) < (int)GetBValue(current) - THRESHOLD;
}

void Attack()
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}

int main()
{
	COLORREF *oldPixels = new COLORREF[PIXEL_COUNT], *currentPixels = new COLORREF[PIXEL_COUNT];

	cout << "© 2021 Meiware.net\nvisually examines pixels and simulates key press (no memory modification)\nhold 'Q' for triggerbot" << endl;

	while (true)
	{
		GetWindowRect(GetForegroundWindow(), &screen);

		if (!GetAsyncKeyState('Q'))
			GetPixels(oldPixels); //store old pixels
		else
		{
			GetPixels(currentPixels); //get current pixels

			if (IsOutOfThreshold(GetAveragePixelColor(currentPixels), GetAveragePixelColor(oldPixels))) //compare old and current pixels
				Attack();
		}

		Sleep(5);
	}

	delete[] currentPixels; //garbage collection
	delete[] oldPixels;

	return 0;
}
