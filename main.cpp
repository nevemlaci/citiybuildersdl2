#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


//mouse handler
int handleMouseClick(SDL_MouseButtonEvent& button) {
	if (button.button == 1) {
		return 1;
	}
	if (button.button == SDL_BUTTON_RIGHT) {
		return 2;
	}
}

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Failed to initialize the SDL2 library\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return -1;
	}
	//initialize ttf lib
	TTF_Init();

	//create the window
	SDL_Window* window = SDL_CreateWindow("citybuilder",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		960, 960,
		0);

	//declare font and color
	TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);
	SDL_Color White = { 255,255,255 };

	//Load surface textures
	SDL_Surface* window_surface = SDL_GetWindowSurface(window);
	SDL_Surface* background = IMG_Load("background.bmp");
	SDL_Surface* handitem = IMG_Load("item1.png");
	SDL_Surface* text = TTF_RenderText_Solid(Sans, "Selected item: House" , White);
	
	SDL_Surface* items[5] = {
	IMG_Load("item1.png"),
	IMG_Load("item2.png"),
	IMG_Load("item3.png"),
	IMG_Load("item4.png"),
	IMG_Load("item5.png")
	};

	SDL_Surface* texts[5]{
		 TTF_RenderText_Solid(Sans, "Selected item: House" , White),
		 TTF_RenderText_Solid(Sans, "Selected item: Road 1" , White),
		 TTF_RenderText_Solid(Sans, "Selected item: Road 2" , White),
		 TTF_RenderText_Solid(Sans, "Selected item: Junction" , White),
		 TTF_RenderText_Solid(Sans, "Selected item: Park" , White)

	};

	//setting a rectangle to change player hand position
	SDL_Rect handObject;
	handObject.x = 10; 
	handObject.y = 10;
	handObject.w = 40;
	handObject.h = 40;

	//objects on the board
	SDL_Rect objectsOnBoard[576];
	SDL_Surface* tableTexture[24][24];
	bool table[24][24] = {false};
	
	//declaring some variables for the game
	bool place = false; //leftkey sets it to true for 1 frame
	bool remove = false; //rightclick sets it to true for 1 frame
	int objectAmount=0; //amount of objects placed(houses etc)
	int placex, placey; //when placing or removing gets a rounded value, rounded to the nearest lower multiple of 40
	int hand = 1; // index of the item held by player

	//mouse position
	int mousex, mousey;

	//key states array
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	
	//errors
	if (!window) {
		std::cout << "Failed to create window\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return -1;
	}else std::cout << "Window successfully loaded.\n";

	if (!window_surface) {
		std::cout << "Failed to get the surface from the window\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return -1;
	}else std::cout << "Got window surface.\n";
	
	//show instructions on console
	std::cout << "Instructions:\n\tYou switch elemnts with the numbers.\n\t1 - House\n\t2 - Road(E-W)\n\t3 - Road(N-S)\n\t4 - Junction\n\t5 - Park\n";
	
	//window events
	bool windowOpen = true;
	while (windowOpen) {
		SDL_Event e; //create the event
		SDL_GetMouseState(&mousex, &mousey); // get mouse position every frame
		SDL_PumpEvents(); // update keyState
		place = false; //set place
		remove = false; //and remove to false every frame
		while (SDL_PollEvent(&e) > 0) {

			switch (e.type) //check some cases for even type
			{
			case SDL_QUIT: //top left x button or alt+f4
				windowOpen = false;
				break;

			case SDL_MOUSEBUTTONDOWN: //mouse button down, calls a function deciding if its right or left
				if (handleMouseClick(e.button) == 1) {
					place = true;
				}
				if (handleMouseClick(e.button) == 2) {
					remove = true;
				}
			}
			
		}

		//move selected item to mouse
		handObject.x = mousex;
		handObject.y = mousey;
		
		//placing an object
		if (place) {
			placex = mousex / 40;
			placey = mousey / 40;
			if (!table[placex][placey]) {
				objectsOnBoard[objectAmount].x = placex*40;
				objectsOnBoard[objectAmount].y = placey*40;
				objectsOnBoard[objectAmount].w = 40;
				objectsOnBoard[objectAmount].h = 40;
				objectAmount++;
				table[placex][placey] = true;
				tableTexture[placex][placey] = items[hand - 1];
			}
		}

		//removing an object
		if (remove) {
			placex = mousex / 40;
			placey = mousey / 40;
			if (table[placex][placey]) {
				table[placex][placey] = false;
			}
		}

		//setting hand variable, probably could make it better
		if (keyState[SDL_SCANCODE_1]) {
			hand = 1;
		}
		if (keyState[SDL_SCANCODE_2]) {
			hand = 2;
		}
		if (keyState[SDL_SCANCODE_3]) {
			hand = 3;
		}
		if (keyState[SDL_SCANCODE_4]) {
			hand = 4;
		}
		if (keyState[SDL_SCANCODE_5]) {
			hand = 5;
		}

		//setting handitem and text
		handitem = items[hand - 1];
		text = texts[hand - 1];
		

		//drawing surfaces
		SDL_UpdateWindowSurface(window);
		SDL_BlitSurface(background, NULL, window_surface, NULL);
		for (int i = 0; i < objectAmount; i++) {
			if (table[objectsOnBoard[i].x / 40][objectsOnBoard[i].y / 40]) {
				SDL_BlitSurface(tableTexture[objectsOnBoard[i].x / 40][objectsOnBoard[i].y / 40], NULL, window_surface, &objectsOnBoard[i]);
			}
		}
		SDL_BlitSurface(handitem, NULL, window_surface, &handObject);
		SDL_BlitSurface(text, NULL, window_surface, NULL);
	}

	//on quit destroy window and quit sql
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

//future plans:
//make background random by tiling random textures
//sounds
