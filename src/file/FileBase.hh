// $Id$

#ifndef FILEBASE_HH
#define FILEBASE_HH

#include <string>
#include "openmsx.hh"

namespace openmsx {

class FileBase
{
public:
	FileBase();
	virtual ~FileBase();

	virtual void read (byte* buffer, unsigned num) = 0;
	virtual void write(const byte* buffer, unsigned num) = 0;

	// If you override mmap(), make sure to call munmap() in
	// your destructor.
	virtual byte* mmap(bool writeBack = false);
	virtual void munmap();

	virtual unsigned getSize() = 0;
	virtual void seek(unsigned pos) = 0;
	virtual unsigned getPos() = 0;
	virtual void truncate(unsigned size);
	virtual void flush() = 0;

	virtual const std::string getURL() const = 0;
	virtual const std::string getLocalName() = 0;
	virtual const std::string getOriginalName();
	virtual bool isReadOnly() const = 0;
	virtual time_t getModificationDate() = 0;

protected:
	byte* mmem;

private:
	bool mmapWrite;
	unsigned mmapSize;
};

} // namespace openmsx

#endif
