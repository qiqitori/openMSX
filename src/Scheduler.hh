// $Id$

#ifndef __SCHEDULER_HH__
#define __SCHEDULER_HH__

#include "emutime.hh"
#include "sortedlist.hh"

class MSXZ80;

class SchedulerNode
{
	public:
		SchedulerNode (const Emutime &time, MSXDevice &dev) : timeStamp(time), device(dev) {}
		const Emutime &getTime() const { return timeStamp; }
		MSXDevice &getDevice() const { return device; }
		bool operator< (const SchedulerNode &n) const { return getTime() < n.getTime(); }
	private: 
		const Emutime timeStamp;	// copy of original timestamp
		MSXDevice &device;		// alias
};

class Scheduler
{
	private:
		SortedList<SchedulerNode> scheduleList;
		Emutime currentTime;
		int stateIRQline;
		bool keepRunning;
	public:
		Scheduler(void);
		~Scheduler(void);
		const Emutime &getCurrentTime();
		const Emutime &getFirstStamp();
		void removeFirstStamp();
		void insertStamp(Emutime &timestamp, MSXDevice &activedevice);
		//void setLaterSP(Emutime &latertimestamp, MSXDevice *activedevice);
		void raiseIRQ();
		void lowerIRQ();
		int getIRQ();
		void scheduleEmulation();
		void stopEmulation();

		static MSXZ80 *nowRunning; //temporary hack for Z80: DO NOT USE
};

#endif
