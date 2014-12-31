#ifndef PLUGGINGCONTROLLER_HH
#define PLUGGINGCONTROLLER_HH

#include "RecordedCommand.hh"
#include "InfoTopic.hh"
#include "EmuTime.hh"
#include "noncopyable.hh"
#include "string_ref.hh"
#include <vector>
#include <memory>

namespace openmsx {

class MSXMotherBoard;
class Connector;
class Pluggable;
class CliComm;

/**
 * Central administration of Connectors and Pluggables.
 */
class PluggingController : private noncopyable
{
public:
	explicit PluggingController(MSXMotherBoard& motherBoard);
	~PluggingController();

	/** Connectors must be (un)registered
	  */
	void registerConnector(Connector& connector);
	void unregisterConnector(Connector& connector);

	/** Return the Connector with given name or
	  * nullptr if there is none with this name.
	  */
	Connector* findConnector(string_ref name) const;

	/** Add a Pluggable to the registry.
	 */
	void registerPluggable(std::unique_ptr<Pluggable> pluggable);

	/** Return the Pluggable with given name or
	  * nullptr if there is none with this name.
	  */
	Pluggable* findPluggable(string_ref name) const;

	/** Access to the MSX specific CliComm, so that Connectors can get it.
	 */
	CliComm& getCliComm();

	/** Convenience method: get current time.
	 */
	EmuTime::param getCurrentTime() const;

private:
	Connector& getConnector(string_ref name) const;
	Pluggable& getPluggable(string_ref name) const;

	MSXMotherBoard& motherBoard;
	std::vector<Connector*> connectors;
	std::vector<std::unique_ptr<Pluggable>> pluggables;

	class PlugCmd final : public RecordedCommand {
	public:
		PlugCmd(CommandController& commandController,
			StateChangeDistributor& stateChangeDistributor,
			Scheduler& scheduler,
			PluggingController& pluggingController);
		void execute(array_ref<TclObject> tokens, TclObject& result,
			     EmuTime::param time) override;
		std::string help(const std::vector<std::string>& tokens) const override;
		void tabCompletion(std::vector<std::string>& tokens) const override;
		bool needRecord(array_ref<TclObject> tokens) const override;
	private:
		PluggingController& pluggingController;
	} plugCmd;

	class UnplugCmd final : public RecordedCommand {
	public:
		UnplugCmd(CommandController& commandController,
			  StateChangeDistributor& stateChangeDistributor,
			  Scheduler& scheduler,
			  PluggingController& pluggingController);
		void execute(array_ref<TclObject> tokens, TclObject& result,
			     EmuTime::param time) override;
		std::string help(const std::vector<std::string>& tokens) const override;
		void tabCompletion(std::vector<std::string>& tokens) const override;
	private:
		PluggingController& pluggingController;
	} unplugCmd;

	class PluggableInfo final : public InfoTopic {
	public:
		PluggableInfo(InfoCommand& machineInfoCommand,
			      PluggingController& pluggingController);
		void execute(array_ref<TclObject> tokens,
			     TclObject& result) const override;
		std::string help(const std::vector<std::string>& tokens) const override;
		void tabCompletion(std::vector<std::string>& tokens) const override;
	private:
		PluggingController& pluggingController;
	} pluggableInfo;

	class ConnectorInfo final : public InfoTopic {
	public:
		ConnectorInfo(InfoCommand& machineInfoCommand,
			      PluggingController& pluggingController);
		void execute(array_ref<TclObject> tokens,
			     TclObject& result) const override;
		std::string help(const std::vector<std::string>& tokens) const override;
		void tabCompletion(std::vector<std::string>& tokens) const override;
	private:
		PluggingController& pluggingController;
	} connectorInfo;

	class ConnectionClassInfo final : public InfoTopic {
	public:
		ConnectionClassInfo(InfoCommand& machineInfoCommand,
				    PluggingController& pluggingController);
		void execute(array_ref<TclObject> tokens,
			     TclObject& result) const override;
		std::string help(const std::vector<std::string>& tokens) const override;
		void tabCompletion(std::vector<std::string>& tokens) const override;
	private:
		PluggingController& pluggingController;
	} connectionClassInfo;
};

} // namespace openmsx

#endif
