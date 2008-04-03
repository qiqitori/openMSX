// $Id$

/*
 *  openmsx - the MSX emulator that aims for perfection
 *
 */

#include "Reactor.hh"
#include "CommandController.hh"
#include "CommandLineParser.hh"
#include "AfterCommand.hh"
#include "CliServer.hh"
#include "Interpreter.hh"
#include "Display.hh"
#include "RenderSettings.hh"
#include "EnumSetting.hh"
#include "MSXException.hh"
#include "Thread.hh"
#include <memory>
#include <iostream>
#include <exception>
#include <cstdlib>
#include <SDL.h>

using std::auto_ptr;
using std::cerr;
using std::endl;
using std::string;

namespace openmsx {

static void initializeSDL()
{
	int flags = SDL_INIT_TIMER;
#ifndef NDEBUG
	flags |= SDL_INIT_NOPARACHUTE;
#endif
	if (SDL_Init(flags) < 0) {
		throw FatalError(string("Couldn't init SDL: ") + SDL_GetError());
	}

// In SDL 1.2.9 and before SDL_putenv has different semantics and is not
// guaranteed to exist on all platforms.
#if SDL_VERSION_ATLEAST(1, 2, 10)
	// On Mac OS X, send key combos like Cmd+H and Cmd+M to Cocoa, so it can
	// perform the corresponding actions.
	SDL_putenv(const_cast<char*>("SDL_ENABLEAPPEVENTS=1"));
#endif
}

static void unexpectedExceptionHandler()
{
	cerr << "Unexpected exception." << endl;
}

static int main(int argc, char **argv)
{
	std::set_unexpected(unexpectedExceptionHandler);

	int err = 0;
	try {
		Thread::setMainThread();
		Reactor reactor;
		reactor.getCommandController().getInterpreter().init(argv[0]);
		AfterCommand afterCommand(reactor,
		                          reactor.getEventDistributor(),
		                          reactor.getCommandController());


		CommandLineParser parser(reactor);
		parser.parse(argc, argv);
		CommandLineParser::ParseStatus parseStatus = parser.getParseStatus();

		if (parseStatus != CommandLineParser::EXIT) {
			initializeSDL();
			if (!parser.isHiddenStartup()) {
				reactor.getDisplay().getRenderSettings().
					getRenderer().restoreDefault();
			}
			if (parseStatus != CommandLineParser::TEST) {
				CliServer cliServer(reactor.getCommandController(),
				                    reactor.getEventDistributor(),
				                    reactor.getGlobalCliComm());
				reactor.run(parser);
			}
		}
	} catch (FatalError& e) {
		cerr << "Fatal error: " << e.getMessage() << endl;
		err = 1;
	} catch (MSXException& e) {
		cerr << "Uncaught exception: " << e.getMessage() << endl;
		err = 1;
	} catch (std::exception& e) {
		cerr << "Uncaught std::exception: " << e.what() << endl;
		err = 1;
	} catch (...) {
		cerr << "Uncaught exception of unexpected type." << endl;
		err = 1;
	}
	// Clean up.
	if (SDL_WasInit(SDL_INIT_EVERYTHING)) {
		SDL_Quit();
	}
	return err;
}

} // namespace openmsx

// Enter the openMSX namespace.
int main(int argc, char **argv)
{
	exit(openmsx::main(argc, argv)); // need exit() iso return on win32/SDL
}
