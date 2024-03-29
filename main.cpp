#include "SDL.h"
#include <vector>
#include <iostream>

#define resoluutioX 1000
#define resoluutioY 600

#define FPS 60

int FOV = 45;
int kulma = 0;

// etäisyys ruudun ja nollakohdan välillä
float Z;

struct vec3d {
	int x, y, z;

	// muuta perspektiivi 3d malliksi
	static vec3d muutaPerspektiivi(vec3d alkuperäinen, float Z0) {

		vec3d palautettava;

		palautettava.x = alkuperäinen.x * (Z0 / (Z0 + alkuperäinen.z));
		palautettava.y = alkuperäinen.y * (Z0 / (Z0 + alkuperäinen.z));
		palautettava.z = alkuperäinen.z;

		return palautettava;
	}

	static vec3d käänny(vec3d alkuperäinen, int kulma) {
		
		vec3d palautettava;

		palautettava.x = (alkuperäinen.x - 50) * cos(kulma * 3.14159265 / 180.0) + (alkuperäinen.z - 50) * -sin(kulma * 3.14159265 / 180.0);
		palautettava.y = alkuperäinen.y;
		palautettava.z = (alkuperäinen.x - 50) * sin(kulma * 3.14159265 / 180.0) + (alkuperäinen.z - 50) * cos(kulma * 3.14159265 / 180.0);
		
		return palautettava;
	}

};

struct kolmio {
	vec3d k[3];
};

struct verkko {

	std::vector <kolmio>kolmiot = {

		// ETUPUOLI
		{0, 0, 0,		100, 100, 0,	0, 100, 0},
		{100, 0, 0,		100, 100 ,0,	0, 100, 0},

		// OIKEA SIVU
		{100, 0 ,0,		100, 100, 100,	100, 100, 0},
		{100, 0, 100,	100, 100, 100,	100, 100, 0},

		// TAKAPUOLI
		{100, 0, 100,	0, 100, 100,	100, 100, 100},
		{0, 0, 100,		100, 0, 100,	100, 100, 100},

		// VASEN SIVU
		{0, 0, 100,		0, 0, 0,		0, 100, 100},
		{0, 0, 0,		0, 100, 0,		0, 0, 100},

		// YLÄPUOLI
		{0, 0, 100,		100, 0, 100,	100, 0, 0},
		{100, 0, 0,		0, 0, 0,		0, 0, 100},

		// ALAPUOLI
		{0, 100, 100,	100, 100, 0,	0, 100, 0},
		{100, 100, 100,	100, 100, 0,	0, 100, 0}
	};
};

verkko kuutio;

// päivitä annettu renderi
void päivitäRenderi(SDL_Renderer* renderi, float Z, int kulma) {
	
	SDL_SetRenderDrawColor(renderi, 0, 0, 0, 0);
	
	SDL_RenderClear(renderi);

	SDL_SetRenderDrawColor(renderi, 255, 255, 255, 255);

	// piirrä kolmio
	for (kolmio& kolmio : kuutio.kolmiot) {

		vec3d vpiste1 = vec3d::käänny(kolmio.k[0], kulma);
		vec3d vpiste2 = vec3d::käänny(kolmio.k[1], kulma);
		vec3d vpiste3 = vec3d::käänny(kolmio.k[2], kulma);

		vec3d piste1 = vec3d::muutaPerspektiivi(vpiste1, Z);
		vec3d piste2 = vec3d::muutaPerspektiivi(vpiste2, Z);
		vec3d piste3 = vec3d::muutaPerspektiivi(vpiste3, Z);

		SDL_Point pisteet[4] = {
			{piste1.x * 3 + 500, piste1.y * 3 + 150},
			{piste2.x * 3 + 500, piste2.y * 3 + 150},
			{piste3.x * 3 + 500, piste3.y * 3 + 150},
			{piste1.x * 3 + 500, piste1.y * 3 + 150},
		};

		SDL_RenderDrawLines(renderi, pisteet, 4);

	}

	SDL_RenderPresent(renderi);
}

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* ikkuna = SDL_CreateWindow("3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, 0);
	SDL_Renderer* renderi = SDL_CreateRenderer(ikkuna, -1, 0);

	bool keskeytetty = false;
	SDL_Event event;

	while (!keskeytetty) {

		float Z0 = (resoluutioX / 2.0) / tan((FOV / 2.0) * 3.14159265 / 180.0);
		kulma += 1;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				keskeytetty = true;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_UP) {
					kulma += 1;
				}
				if (event.key.keysym.sym == SDLK_DOWN) {
					kulma -= 1;
				}
			}
		}

		päivitäRenderi(renderi, Z0, kulma);

		SDL_Delay(1000 / FPS);

	}

	SDL_DestroyWindow(ikkuna);
	SDL_Quit();

	return 0;

}