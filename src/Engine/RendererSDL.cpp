#include "RendererSDL.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Engine.h"

void RendererSDL::Initialize()
{
	RenderView = Vector2(0, 0);
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", SDL_GetError() ); return;
	}

	GameWindow = SDL_CreateWindow(Engine::Properties.Name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Engine::Properties.Width, Engine::Properties.Height, SDL_WINDOW_SHOWN);
	if(GameWindow == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError()); return;
	}

	GameRenderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(GameRenderer == nullptr)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}


void RendererSDL::PrecacheTexture(const std::string& name, const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == nullptr)
	{
		printf("Surface Load Fail: %s -> %s", name.c_str(), SDL_GetError());
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(GameRenderer, surface);
	if(texture == nullptr)
	{
		printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	int texWidth, texHeight;
	TextureRepository[name] = new SDLTextureWrapper(texture);
	SDL_QueryTexture(TextureRepository[name]->Data, NULL, NULL, &texWidth, &texHeight);
	TextureRepository[name]->Extrai["Width"] = texWidth;
	TextureRepository[name]->Extrai["Height"] = texHeight;
	TexSizeRepository[name] = Vector2(texWidth, texHeight);
	SDL_FreeSurface(surface);
}

void RendererSDL::RenderImage(const char* textureID, Vector2 position)
{
	this->RenderImage(textureID, position, 1.0, 0, SDL_FLIP_NONE);
}

void RendererSDL::RenderImage(const char* textureID, Vector2 position, double scale, double rotation, SDL_RendererFlip flip)
{
	std::string strID = std::string(textureID);
	Vector2 pos = VectorToRenderVector(position);
	if(TextureRepository.count(strID) > 0
	&& TextureRepository[strID]->Data == nullptr)
	{
		printf("Texture render attempt does not exist! -> %s\n", textureID);
		return;
	}
	SDL_Rect TexDimensions = {pos.x, pos.y, (int)floor(TextureRepository[strID]->Extrai["Width"]*scale),
											(int)floor(TextureRepository[strID]->Extrai["Height"]*scale)};
	SDL_RenderCopyEx(GameRenderer, TextureRepository[strID]->Data, NULL, &TexDimensions, rotation, NULL, flip);
}

void RendererSDL::RenderDrawRect(Vector2 position, Vector2 dimensions, SDL_Color color)
{
	Vector2 pos = VectorToRenderVector(position);
	SDL_Rect rect = {pos.x, pos.y, dimensions.x, dimensions.y};
	SDL_SetRenderDrawColor(GameRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(GameRenderer, &rect);
}

void RendererSDL::RenderFillRect(Vector2 position, Vector2 dimensions, SDL_Color color)
{
	Vector2 pos = VectorToRenderVector(position);
	SDL_Rect rect = {pos.x, pos.y, dimensions.x, dimensions.y};
	SDL_SetRenderDrawColor(GameRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(GameRenderer, &rect);
}

void RendererSDL::RenderPresent()
{
	SDL_RenderPresent(GameRenderer);
}

void RendererSDL::RenderClear()
{
	SDL_SetRenderDrawColor(GameRenderer, 255, 255, 255, 255);
	SDL_RenderClear(GameRenderer);
}
void RendererSDL::Cleanup()
{
	for(auto iter = TextureRepository.begin(); iter != TextureRepository.end(); iter++)
	{
		SDL_DestroyTexture(iter->second->Data);
		iter->second->Data = nullptr;
		delete iter->second;
		iter->second = nullptr;
	}
	SDL_DestroyRenderer(GameRenderer);
	SDL_DestroyWindow(GameWindow);
	IMG_Quit();
}

Vector2 RendererSDL::VectorToRenderVector(Vector2 in)
{
	//TODO: This is needless. -> Too Lazy to Remove.
	// Displace coordinates.
	in.x += RenderView.x;
	in.y += RenderView.y;
	return in;
}

bool RendererSDL::TextureExists(const std::string& Texture)
{
	return (TextureRepository[Texture] != nullptr);
}