#include "FMODManager.h"
#include <cstdio>

FMODManager* FMODManager::manager = NULL;

FMODManager::FMODManager()
{
	ERRCHECK(result = FMOD::EventSystem_Create(&eventsystem));
	ERRCHECK(result = eventsystem->init(64, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL));
	event = NULL;
}

FMODManager::~FMODManager()
{
	manager = NULL;
}

FMODManager* FMODManager::getInstance()
{
	if(manager == NULL)
	{
		manager = new FMODManager();
	}

	return manager;
}

void FMODManager::kill()
{
	if(manager != NULL)
	{
		delete manager;
	}
}

void FMODManager::ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
		//Ecrire dans un fichier de log
    }
}

void FMODManager::setMediaPath(const std::string& name)
{
	ERRCHECK(result = eventsystem->setMediaPath(name.c_str()));
}

void FMODManager::stopAllEvents(const std::string&  groupName)
{
	ERRCHECK(eventsystem->getGroup(groupName.c_str(), FMOD_EVENT_DEFAULT, &eventgroup));
    ERRCHECK(eventgroup->freeEventData());
}

void FMODManager::loadFEV(const std::string& name)
{
	ERRCHECK(result = eventsystem->load(name.c_str(), 0, 0));
}

void FMODManager::getGroup(const std::string& name)
{
	ERRCHECK(result = eventsystem->getGroup(name.c_str(), FMOD_EVENT_DEFAULT, &eventgroup));
}

void FMODManager::pushEvent(const std::string& name)
{
	it = events.find(name);
	if(it == events.end())
	{
		ERRCHECK(result = eventgroup->getEvent(name.c_str(), FMOD_EVENT_DEFAULT, &event));
		events[std::string(name)] = event;
	}
}

void FMODManager::setEvent(const std::string& name)
{
	it=events.find(name);
}

void FMODManager::playEvent()
{
	
	ERRCHECK(result = it->second->start());
}

void FMODManager::stopEvent()
{
	ERRCHECK(result = it->second->stop());
}

void FMODManager::update()
{
	ERRCHECK(result = eventsystem->update());
}

void FMODManager::getValueParameter(const std::string& nameParameter, float* value)
{
	ERRCHECK(result = it->second->getParameter(nameParameter.c_str(), &parameter));
    ERRCHECK(result = parameter->getValue(value));
}

void FMODManager::setValueParameter(const std::string& nameParameter, float* value)
{
	ERRCHECK(result = it->second->getParameter(nameParameter.c_str(), &parameter));
    ERRCHECK(result = parameter->setValue(*value));
}
