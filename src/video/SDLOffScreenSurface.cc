// $Id$

#include "SDLOffScreenSurface.hh"
#include "PNG.hh"

namespace openmsx {

SDLOffScreenSurface::SDLOffScreenSurface(const SDL_Surface& proto)
{
	int flags = SDL_SWSURFACE;
	setSDLFormat(*proto.format);
	const SDL_PixelFormat& format = getSDLFormat();
	surface = SDL_CreateRGBSurface(
		flags, proto.w, proto.h, format.BitsPerPixel,
		format.Rmask, format.Gmask, format.Bmask, format.Amask);
	setSDLDisplaySurface(surface);
	setSDLWorkSurface(surface);
	setBufferPtr(static_cast<char*>(surface->pixels), surface->pitch);
}

SDLOffScreenSurface::~SDLOffScreenSurface()
{
	SDL_FreeSurface(surface);
}

void SDLOffScreenSurface::saveScreenshot(const std::string& filename)
{
	lock();
	PNG::save(getSDLWorkSurface(), filename);
}

} // namespace openmsx
