#pragma once

#include <irrKlang.h>
using namespace irrklang;
#include <map>

class CSoundEngine
{
protected:
	// Destructor

	// The library map of all the sounds created
	std::map<std::string, std::string> soundMap;

public:
	static CSoundEngine* soundEngine;
	ISoundEngine* theSoundEngine;
	static CSoundEngine* Getinstance() {
		if (soundEngine == nullptr)
			soundEngine = new CSoundEngine;
		return soundEngine;
	};
	// Constructor
	CSoundEngine();
	virtual ~CSoundEngine();
	// Init this class and it will create the Sound Engine
	bool Init(void);

	// Get the handler to the sound engine
	ISoundEngine* GetSoundEngine(void);
	ISoundEngine* SetListenerPosition(vec3df _position, vec3df _target);

	// Add a sound to this library
	void AddSound(const std::string& _soundIndex, const std::string& _soundSource);
	// Get a sound from this map 
	std::string GetSound(const std::string& _soundIndex);
	// Remove a sound from this map
	bool RemoveSound(const std::string& _soundIndex);
	// Play a sound from this map
	void PlayASound(const std::string& _soundIndex);

	void Play3D(const std::string& _soundIndex, vec3df _position);

	bool CurentlyPlaying(const std::string & _soundIndex);

	void setSoundVolume(float volume);

	void SetmaxDist3D(float volume);

	void SetminDist3D(float volume);

	void StopSound();

		
};

