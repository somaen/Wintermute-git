/*
This file is part of WME Lite.
http://dead-code.org/redir.php?target=wmelite

Copyright (c) 2011 Jan Nedoma

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "dcgf.h"
#include "BRenderSDL.h"
#include "FreeImage.h"
#include "MathUtil.h"


//////////////////////////////////////////////////////////////////////////
CBRenderSDL::CBRenderSDL(CBGame* inGame) : CBRenderer(inGame)
{
	m_Renderer = NULL;
	m_Win = NULL;

	m_BorderLeft = m_BorderRight = m_BorderTop = m_BorderBottom = 0;
	m_RatioX = m_RatioY = 1.0f;
}

//////////////////////////////////////////////////////////////////////////
CBRenderSDL::~CBRenderSDL()
{
	if (m_Renderer) SDL_DestroyRenderer(m_Renderer);
	if (m_Win) SDL_DestroyWindow(m_Win);
	
	SDL_Quit();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::InitRenderer(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return E_FAIL;
		
	
	m_Width = width;
	m_Height = height;

	m_RealWidth = width;
	m_RealHeight = height;


	// find suitable resolution
#ifdef __IPHONEOS__
	m_RealWidth = 480;
	m_RealHeight = 320;

	int numModes = SDL_GetNumDisplayModes(0);
	for (int i = 0; i < numModes; i++)
	{
		SDL_DisplayMode mode;
		SDL_GetDisplayMode(0, i, &mode);

		if (mode.w > mode.h)
		{
			m_RealWidth = mode.w;
			m_RealHeight = mode.h;
			break;
		}
	}	
#else
	m_RealWidth = Game->m_Registry->ReadInt("Debug", "ForceResWidth", m_Width);
	m_RealHeight = Game->m_Registry->ReadInt("Debug", "ForceResHeight", m_Height);
#endif

	/*	
	m_RealWidth = 480;
	m_RealHeight = 320;
	*/

	float origAspect = (float)m_Width / (float)m_Height;
	float realAspect = (float)m_RealWidth / (float)m_RealHeight;

	float ratio;
	if (origAspect < realAspect)
	{
		// normal to wide
		ratio = (float)m_RealHeight / (float)m_Height;
	}
	else
	{
		// wide to normal
		ratio = (float)m_RealWidth / (float)m_Width;
	}

	m_BorderLeft = (m_RealWidth - (m_Width * ratio)) / 2;
	m_BorderRight = m_RealWidth - (m_Width * ratio) - m_BorderLeft;

	m_BorderTop = (m_RealHeight - (m_Height * ratio)) / 2;
	m_BorderBottom = m_RealHeight - (m_Height * ratio) - m_BorderTop;



	m_RatioX = (float)(m_RealWidth - m_BorderLeft - m_BorderRight) / (float)m_Width;
	m_RatioY = (float)(m_RealHeight - m_BorderTop - m_BorderBottom) / (float)m_Height;


	Uint32 flags = SDL_WINDOW_SHOWN;
#ifdef __IPHONEOS__
	flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS;
#endif

	m_Win = SDL_CreateWindow("WME Lite",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_RealWidth, m_RealHeight,
		flags);
	
	if (!m_Win) return E_FAIL;

	SDL_ShowCursor(SDL_DISABLE);

#ifdef __IPHONEOS__
	// SDL defaults to OGL ES2, which doesn't work on old devices
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles");
#else
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
#endif

	m_Renderer = SDL_CreateRenderer(m_Win, -1, 0);
	if (!m_Renderer) return E_FAIL;

	m_Active = true;

	m_Windowed = Game->m_Registry->ReadBool("Video", "Windowed", true);
	if (!m_Windowed) SDL_SetWindowFullscreen(m_Win, SDL_TRUE);


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::Flip()
{
	SDL_RenderPresent(m_Renderer);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::Fill(BYTE r, BYTE g, BYTE b, RECT* rect)
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, 0xFF);
	SDL_RenderClear(m_Renderer);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::Fade(WORD Alpha)
{
	DWORD dwAlpha = 255 - Alpha;
	return FadeToColor(dwAlpha<<24);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::FadeToColor(DWORD Color, RECT* rect)
{
	SDL_Rect fillRect;

	if (rect)
	{
		fillRect.x = rect->left;
		fillRect.y = rect->top;
		fillRect.w = rect->right - rect->left;
		fillRect.h = rect->bottom - rect->top;		
	}
	else
	{
		RECT rc;
		Game->GetCurrentViewportRect(&rc);
		fillRect.x = rc.left;
		fillRect.y = rc.top;
		fillRect.w = rc.right - rc.left;
		fillRect.h = rc.bottom - rc.top;
	}
	ModTargetRect(&fillRect);

	BYTE r = D3DCOLGetR(Color);
	BYTE g = D3DCOLGetG(Color);
	BYTE b = D3DCOLGetB(Color);
	BYTE a = D3DCOLGetA(Color);

	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(m_Renderer, &fillRect);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::DrawLine(int X1, int Y1, int X2, int Y2, DWORD Color)
{
	BYTE r = D3DCOLGetR(Color);
	BYTE g = D3DCOLGetG(Color);
	BYTE b = D3DCOLGetB(Color);
	BYTE a = D3DCOLGetA(Color);

	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);

	POINT point1, point2;
	point1.x = X1;
	point1.y = Y1;
	PointToScreen(&point1);

	point2.x = X2;
	point2.y = Y2;
	PointToScreen(&point2);

	SDL_RenderDrawLine(m_Renderer, point1.x, point1.y, point2.x, point2.y);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
CBImage* CBRenderSDL::TakeScreenshot()
{
	SDL_Rect viewport;

	SDL_RenderGetViewport(m_Renderer, &viewport);
	
	SDL_Surface* surface = SDL_CreateRGBSurface(0, viewport.w, viewport.h, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, 0x00000000);
	if (!surface) return NULL;

	if (SDL_RenderReadPixels(m_Renderer, NULL, surface->format->format, surface->pixels, surface->pitch) < 0) return NULL;


	FIBITMAP* dib = FreeImage_Allocate(viewport.w, viewport.h, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	
	int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
	
	for (unsigned y = 0; y < FreeImage_GetHeight(dib); y++)
	{
		BYTE* bits = FreeImage_GetScanLine(dib, y);
		BYTE* src = (BYTE*)surface->pixels + (viewport.h - y - 1) * surface->pitch;
		memcpy(bits, src, bytespp * viewport.w);
	}
	
	return new CBImage(Game, dib);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::SwitchFullscreen()
{
	if (m_Windowed) SDL_SetWindowFullscreen(m_Win, SDL_TRUE);
	else SDL_SetWindowFullscreen(m_Win, SDL_FALSE);

	m_Windowed = !m_Windowed;

	Game->m_Registry->WriteBool("Video", "Windowed", m_Windowed);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
const char* CBRenderSDL::GetName()
{
	if (m_Name.empty())
	{
		if (m_Renderer)
		{
			SDL_RendererInfo info;
			SDL_GetRendererInfo(m_Renderer, &info);
			m_Name = AnsiString(info.name);
		}
	}
	return m_Name.c_str();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderSDL::SetViewport(int left, int top, int right, int bottom)
{
	SDL_Rect rect;
	rect.x = left + m_BorderLeft;
	rect.y = top + m_BorderTop;
	rect.w = (right - left) * m_RatioX;
	rect.h = (bottom - top) * m_RatioY;
	
	// TODO fix!!!
#ifndef __IPHONEOS__	
	SDL_RenderSetViewport(GetSdlRenderer(), &rect);
#endif
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CBRenderSDL::ModTargetRect(SDL_Rect* rect)
{
	SDL_Rect viewportRect;
	SDL_RenderGetViewport(GetSdlRenderer(), &viewportRect);

	rect->x = MathUtil::Round((rect->x + m_BorderLeft - viewportRect.x) * m_RatioX);
	rect->y = MathUtil::Round((rect->y + m_BorderTop - viewportRect.y) * m_RatioY);
	rect->w = MathUtil::RoundUp(rect->w * m_RatioX);
	rect->h = MathUtil::RoundUp(rect->h * m_RatioY);
}

//////////////////////////////////////////////////////////////////////////
void CBRenderSDL::PointFromScreen(POINT* point)
{
	SDL_Rect viewportRect;
	SDL_RenderGetViewport(GetSdlRenderer(), &viewportRect);

	point->x = point->x / m_RatioX - m_BorderLeft + viewportRect.x;
	point->y = point->y / m_RatioY - m_BorderTop + viewportRect.y;
}


//////////////////////////////////////////////////////////////////////////
void CBRenderSDL::PointToScreen(POINT* point)
{
	SDL_Rect viewportRect;
	SDL_RenderGetViewport(GetSdlRenderer(), &viewportRect);

	point->x = MathUtil::RoundUp(point->x * m_RatioX) + m_BorderLeft - viewportRect.x;
	point->y = MathUtil::RoundUp(point->y * m_RatioY) + m_BorderTop - viewportRect.y;

}
