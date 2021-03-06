#include "RendererBase.h"
#include "Engine.h"
#include <math.h>

void RendererBase::Initialize(){}
void RendererBase::PrecacheTexture(const std::string& name, const std::string& path){}
void RendererBase::RenderImage(const char* textureID, Vector2 worldposition){}
void RendererBase::RenderImage(const char* textureID, Vector2 worldposition, double scale, double rotation, SDL_RendererFlip flip){}
void RendererBase::RenderDrawRect(Vector2 worldposition, Vector2 dimensions, SDL_Color color){}
void RendererBase::RenderFillRect(Vector2 worldposition, Vector2 dimensions, SDL_Color color){}
void RendererBase::RenderPresent(){}
void RendererBase::RenderClear(){}
void RendererBase::Cleanup(){}
RendererBase::~RendererBase(){}

void RendererBase::SetRenderView(int x, int y)
{
	this->RenderView.x = x;
	this->RenderView.y = y;
}

void RendererBase::SetRenderView(Vector2 pos)
{
	this->SetRenderView(pos.x, pos.y);
}

bool RendererBase::TextureExists(const std::string& Texture){ return false; }

// Font Stuff
void RendererBase::PrecacheFont(const std::string& name, const std::string& path, int fontsize){}
void RendererBase::RenderFont(const std::string& message, const char* fontID, Vector2 worldposition, SDL_Color color){}
void RendererBase::RenderFont(const std::string& message, const char* fontID, Vector2 worldposition, double rotation, SDL_RendererFlip flip, SDL_Color color){}


// Vector Translation
Vector2 RendererBase::WorldToLocalVector(Vector2 in)
{
	//TODO: Improve this
	// Displace coordinates.
	in.x += floor(RenderView.x+(Engine::Properties.Width/2));
	in.y += floor(RenderView.y+(Engine::Properties.Height/2));
	return in;
}

Vector2 RendererBase::LocalToWorldVector(Vector2 in)
{
	//TODO: Improve this
	// Displace coordinates.
	in.x -= floor(RenderView.x+(Engine::Properties.Width/2));
	in.y -= floor(RenderView.y+(Engine::Properties.Height/2));
	return in;
}
