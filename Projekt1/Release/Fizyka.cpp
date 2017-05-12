#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <math.h> 
#include <algorithm>

float pi = 3.14159265359;

bool zderzenie(glm::vec3 punkt, glm::vec3 podstawa, glm::vec3 sufit, float promien)
{
	if (podstawa.x == sufit.x && podstawa.y == sufit.y && podstawa.z == sufit.z)//punkt w przestrzeni - brak kolizji
		return false;
	else if (podstawa.x == sufit.x && podstawa.z == sufit.z)//prosta rownolegla do y
	{
		if (punkt.y >= std::min(podstawa.y, sufit.y) && punkt.y <= std::max(podstawa.y, sufit.y))
		{
			float odleglosc;
			odleglosc = (punkt.x - podstawa.x)*(punkt.x - podstawa.x) + (punkt.z - podstawa.z)*(punkt.z - podstawa.z);
			if (odleglosc <= (promien * promien))
				return true;
		}
		return false;
	}
	else if (podstawa.z == sufit.z && podstawa.y == sufit.y)//prosta rownolegla do x
	{
		if (punkt.x >= std::min(podstawa.x, sufit.x) && punkt.x <= std::max(podstawa.x, sufit.x))
		{
			float odleglosc;
			odleglosc = (punkt.z - podstawa.z)*(punkt.z - podstawa.z) + (punkt.y - podstawa.y)*(punkt.y - podstawa.y);
			if (odleglosc <= promien ^ 2)
				return true;
		}
		return false;
	}
	else if (podstawa.x == sufit.x && podstawa.y == sufit.y) //prosta rownolegla do z
	{
		if (punkt.z >= std::min(podstawa.z, sufit.z) && punkt.z <= std::max(podstawa.z, sufit.z))
		{
			float odleglosc;
			odleglosc = (punkt.x - podstawa.x)*(punkt.x - podstawa.x) + (punkt.y - podstawa.y)*(punkt.y - podstawa.y);
			if (odleglosc <= promien ^ 2)
				return true;
		}
		return false;
	}
	else
	{
		float A, B, C, D;
		float A1, B1, C1, D1;
		A = podstawa.x - sufit.x;
		B = podstawa.y - sufit.y;
		C = podstawa.z - sufit.z;
		D = -1 * (A*podstawa.x + B*podstawa.y + C*podstawa.z);
		if (punkt.x*A + punkt.y*B + punkt.z*C + D <= 0)
		{
			A1 = sufit.x - podstawa.x;
			B1 = sufit.y - podstawa.y;
			C1 = sufit.z - podstawa.z;
			D1 = -1 * (A1*sufit.x + B1*sufit.y + C1*sufit.z);
			if (punkt.x*A1 + punkt.y*B1 + punkt.z*C1 + D1 <= 0)
			{
				float dA, dB, dC;
				float i, j, k;
				float odleglosc;
				dA = podstawa.x - punkt.x;
				dB = podstawa.y - punkt.y;
				dC = podstawa.z - punkt.z;
				i = dB*C1 - dC*B1;
				j = dC*A1 - dA*C1;
				k = dA*B1 - dB*A1;
				odleglosc = sqrt(i*i + j*j + k*k) / sqrt(A1*A1 + B1*B1 + C1*C1);
				if (odleglosc <= promien)
					return true;
			}
		}
	}
	return false;
}

float kat(glm::vec3 bot, glm::vec3 punkt)
{
	float a;
	if (punkt.z != bot.z && punkt.x != bot.x)
	{
		a = (punkt.x - bot.x) / (punkt.z - bot.z);
		a = atan(a);
		if (a < 0)
			a = a + pi;
		if (punkt.x < bot.x)
			a = a + pi;
	}
	else if (punkt.z != bot.z)
	{
		if (punkt.z >= bot.z)
			a = 0;
		else
			a = pi;
	}
	else
	{
		if (punkt.x >= bot.x)
			a = pi / 2;
		else
			a = 3 * pi / 2;
	}
	return a;
}

bool czy_widzi(glm::vec3 bot, float obrot, float obszar, glm::vec3 punkt)
{
	float kat_1, kat_2;
	kat_1 = obrot - obszar / 2;
	if (kat_1 < 0)
		kat_1 = 2 * pi + kat_1;
	kat_2 = obrot + obszar / 2;
	if (kat_2 > 2 * pi)
		kat_2 = kat_2 - 2 * pi;
	float a = kat(bot, punkt);
	if (((a >= kat_1) && (a <= kat_2)) || ((a >= kat_1 || a <= kat_2) && (kat_2 < kat_1)))
		return true;
	else
		return false;
}
