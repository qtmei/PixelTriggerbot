#include <Windows.h>
#include <iostream>

using namespace std;

RECT screen;

void GetPixels(COLORREF* pixels)
{
	int index = 0;

	for (int x = 0; x < 2; x++)
		for (int y = 0; y < 2; y++)
			pixels[index++] = GetPixel(GetDC(GetActiveWindow()), (int)screen.right / 2 + x, (int)screen.bottom / 2 + y);
}

COLORREF GetAveragePixelColor(COLORREF* pixels)
{
	int tempR = 0, tempG = 0, tempB = 0;

	for(int i = 0; i < 4; i++)
	{
		tempR += (int)GetRValue(pixels[i]);
		tempG += (int)GetGValue(pixels[i]);
		tempB += (int)GetBValue(pixels[i]);
	}

	return RGB(tempR / 4, tempG / 4, tempB / 4);
}

bool IsOutOfThreshold(COLORREF old, COLORREF current, int threshold)
{
	return (int)GetRValue(old) > (int)GetRValue(current) + threshold || (int)GetRValue(old) < (int)GetRValue(current) - threshold || (int)GetGValue(old) > (int)GetGValue(current) + threshold || (int)GetGValue(old) < (int)GetGValue(current) - threshold || (int)GetBValue(old) > (int)GetBValue(current) + threshold || (int)GetBValue(old) < (int)GetBValue(current) - threshold;
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
	COLORREF *oldPixels = new COLORREF[4], *currentPixels = new COLORREF[4];

	cout << "© 2021 Meiware.net\nvisually examines pixels and simulates key press (no memory modification)\nhold 'Q' for triggerbot" << endl;

	while (true)
	{
		GetWindowRect(GetForegroundWindow(), &screen);

		if (!GetAsyncKeyState('Q'))
			GetPixels(oldPixels); //store old pixels
		else
		{
			GetPixels(currentPixels); //get current pixels

			if (IsOutOfThreshold(GetAveragePixelColor(currentPixels), GetAveragePixelColor(oldPixels), 16)) //compare old and current pixels
				Attack();
		}

		Sleep(4);
	}

	delete[] currentPixels;
	delete[] oldPixels;

	return 0;
}
