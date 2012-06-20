#include "FMOD/fmod_event.hpp"
#include "FMOD/fmod_errors.h"
#include <map>
#include <string>

class FMODManager
{

private:
	FMODManager();
	~FMODManager();

public:
	static FMODManager* getInstance();
	static void kill();
	void ERRCHECK(FMOD_RESULT);
	void setMediaPath(const std::string&);
	void stopAllEvents(const std::string&);
	void loadFEV(const std::string&);
	void pushEvent(const std::string&);
	void setEvent(const std::string&);
	FMOD::Event getEvent(const std::string&);
	void getGroup(const std::string&);
	void playEvent();
	void stopEvent();
	void getValueParameter( const std::string&, float*);
	void setValueParameter( const std::string&, float*);
	void update();
	void watchEvent();

private:
	static FMODManager*	 manager;
	FMOD_RESULT			 result;
    FMOD::EventSystem	*eventsystem;
    FMOD::EventGroup	*eventgroup;
    FMOD::Event			*event;
	FMOD::EventParameter *parameter;
	std::map<std::string,FMOD::Event*>	 events;
	std::map<std::string, FMOD::Event*>::iterator it;
};