// $Id$

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "FileContext.hh"
#include "FileOperations.hh"
#include "MSXConfig.hh"
#include "File.hh"


// class FileContext

FileContext::~FileContext()
{
}

const std::string FileContext::resolve(const std::string &filename)
{
	return resolve(getPaths(), filename);
}

const std::string FileContext::resolveCreate(const std::string &filename)
{
	try {
		return resolve(getPaths(), filename);
	} catch (FileException &e) {
		std::string path = getPaths().front();
		FileOperations::mkdirp(path);
		return path + filename;
	}
}

const std::string FileContext::resolve(const std::list<std::string> &pathList,
                                       const std::string &filename)
{
	// TODO handle url-protocols better
	
	PRT_DEBUG("Context: "<<filename);
	if ((filename.find("://") != std::string::npos) ||
	    (filename[0] == '/')) {
		// protocol specified or absolute path, don't resolve
		return filename;
	}
	
	std::list<std::string>::const_iterator it;
	for (it = pathList.begin(); it != pathList.end(); it++) {
		std::string name = FileOperations::expandTilde(*it + filename);
		unsigned pos = name.find("://");
		if (pos != std::string::npos) {
			name = name.substr(pos + 3);
		}
		struct stat buf;
		PRT_DEBUG("Context: try "<<name);
		if (!stat(name.c_str(), &buf)) {
			// no error
			PRT_DEBUG("Context: found "<<name);
			return name;
		}
	}
	// not found in any path
	throw FileException(filename + " not found in this context");
}

const std::string FileContext::resolveSave(const std::string &filename)
{
	assert(!savePath.empty());
	FileOperations::mkdirp(savePath);
	return savePath + filename;
}



// class ConfigFileContext

std::map<std::string, int> ConfigFileContext::nonames;

ConfigFileContext::ConfigFileContext(const std::string &path,
                                     const std::string &hwDescr,
                                     const std::string &userName_)
{
	paths.push_back(path);

	std::string userName(userName_);
	if (userName == "") {
		int num = ++nonames[hwDescr];
		char buf[20];
		snprintf(buf, 20, "untitled%d", num);
		userName = buf;
	}
	savePath = "~/.openMSX/persistent/" + hwDescr +
	                       '/' + userName + '/';
}

const std::list<std::string> &ConfigFileContext::getPaths()
{
	return paths;
}


// class SystemFileContext

SystemFileContext::SystemFileContext()
{
	paths.push_back("~/.openMSX/");		// user directory
	paths.push_back("/opt/openMSX/");	// system directory
}

const std::list<std::string> &SystemFileContext::getPaths()
{
	return paths;
}


// class UserFileContext

UserFileContext::UserFileContext()
	: alreadyInit(false)
{
}

UserFileContext::UserFileContext(const std::string &savePath_)
	: alreadyInit(false)
{
	savePath = std::string("~/.openMSX/persistent/") + savePath_ + '/';
}

const std::list<std::string> &UserFileContext::getPaths()
{
	if (!alreadyInit) {
		alreadyInit = true;
		try {
			Config *config = MSXConfig::instance()->
				getConfigById("UserDirectories");
			std::list<Device::Parameter*>* pathList;
			pathList = config->getParametersWithClass("");
			std::list<Device::Parameter*>::const_iterator it;
			for (it = pathList->begin();
			     it != pathList->end();
			     it++) {
				std::string path = (*it)->value;
				if (path[path.length() - 1] != '/') {
					path += '/';
				}
				paths.push_back(path);
			}
		} catch (MSXException &e) {
			// no UserDirectories specified
		}
		paths.push_back("");
	}
	return paths;
}
