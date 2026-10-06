#include "pch.h"
#include "Audio/Audio.h"
namespace sr {
	bool Audio::Initialize()
	{
		FMOD_RESULT result = FMOD::System_Create(&m_fmodSystem);
		if (!CheckFMODResult(result))
			return false;

		void* extradriverdata = nullptr;
		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
		if (!CheckFMODResult(result))
			return false;

		return true;
	}

	void Audio::Shutdown()
	{
		FMOD_RESULT result = m_fmodSystem->release();
		CheckFMODResult(result);
	}

	void Audio::Update()
	{
		FMOD_RESULT result = m_fmodSystem->update();
		CheckFMODResult(result);
	}

	bool Audio::AddSound(const std::string& name, const std::string& filename)
	{
		// check if key exists in sounds map
		if (m_sounds.contains(name))
		{
			std::cerr << "Audio System : name already exists " << name << std::endl;
			return false;
		}

		// create sound from key
		FMOD::Sound* sound = nullptr;
		FMOD_RESULT result = m_fmodSystem->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound);
		if (!CheckFMODResult(result)) return false;

		// insert sound into map
		m_sounds.insert({ name, sound });

		return true;
	}

	bool Audio::PlaySound(const std::string& name)
	{
		// check if sound exists in sounds map
		if (!m_sounds.contains(name))
		{
			std::cerr << "Audio System : name doesn't exists " << name << std::endl;
			return false;
		}

		// play sound from key
		FMOD_RESULT result = m_fmodSystem->playSound(m_sounds[name], nullptr, false, nullptr);
		if (!CheckFMODResult(result))
			return false;

		return true;
	}

	bool Audio::CheckFMODResult(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cerr << FMOD_ErrorString(result) << std::endl;
			return false;
		}

		return true;
	}
}