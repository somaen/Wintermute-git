/*
This file is part of WME Lite.
http://dead-code.org/redir.php?target=wmelite

Copyright (c) 2011 Jan Nedoma

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "dcgf.h"
#include "BSoundMgr.h"
#include "BRegistry.h"
#include "PathUtil.h"
#include "StringUtil.h"
#include "BGame.h"
#include "BFileManager.h"

namespace WinterMute {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_PERSISTENT(CBSoundMgr, true);

//////////////////////////////////////////////////////////////////////////
CBSoundMgr::CBSoundMgr(CBGame *inGame): CBBase(inGame) {
	m_SoundAvailable = false;

	m_VolumeSFX = m_VolumeSpeech = m_VolumeMusic = m_VolumeMaster = 100;
}


//////////////////////////////////////////////////////////////////////////
CBSoundMgr::~CBSoundMgr() {
	SaveSettings();
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::Cleanup() {
#if 0
	for (int i = 0; i < m_Sounds.GetSize(); i++) delete m_Sounds[i];
	m_Sounds.RemoveAll();

	BASS_Free();
#endif
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CBSoundMgr::SaveSettings() {
	if (m_SoundAvailable) {
		Game->m_Registry->WriteInt("Audio", "MasterVolume", m_VolumeMaster);

		Game->m_Registry->WriteInt("Audio", "SFXVolume",    m_VolumeSFX);
		Game->m_Registry->WriteInt("Audio", "SpeechVolume", m_VolumeSpeech);
		Game->m_Registry->WriteInt("Audio", "MusicVolume",  m_VolumeMusic);
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::Initialize() {
	m_SoundAvailable = false;
#if 0

#ifdef __IPHONEOS__
#define BASS_CONFIG_IOS_MIXAUDIO 34
	BASS_SetConfig(BASS_CONFIG_IOS_MIXAUDIO, 0);
#endif


	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		Game->LOG(0, "An incorrect version of BASS was loaded");
		return E_FAIL;
	}

	if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
		Game->LOG(0, "Can't initialize sound device");
		return E_FAIL;
	}
#endif

	m_VolumeMaster = Game->m_Registry->ReadInt("Audio", "MasterVolume", 100);

	m_VolumeSFX    = Game->m_Registry->ReadInt("Audio", "SFXVolume",    100);
	m_VolumeSpeech = Game->m_Registry->ReadInt("Audio", "SpeechVolume", 100);
	m_VolumeMusic  = Game->m_Registry->ReadInt("Audio", "MusicVolume",  100);

#if 0
	m_SoundAvailable = true;
	SetMasterVolumePercent(m_VolumeMaster);
#endif
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::InitLoop() {
	if (!m_SoundAvailable) return S_OK;
#if 0

	BASS_Update(500);
#endif
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CBSoundBuffer *CBSoundMgr::AddSound(const char *Filename, TSoundType Type, bool Streamed) {
	if (!m_SoundAvailable) return NULL;
#if 0

	CBSoundBuffer *sound;

	// try to switch WAV to OGG file (if available)
	AnsiString ext = PathUtil::GetExtension(Filename);
	if (StringUtil::CompareNoCase(ext, ".wav")) {
		AnsiString path = PathUtil::GetDirectoryName(Filename);
		AnsiString name = PathUtil::GetFileNameWithoutExtension(Filename);

		AnsiString newFile = PathUtil::Combine(path, name + ".ogg");
		CBFile *file = Game->m_FileManager->OpenFile(newFile.c_str());
		if (file) {
			Filename = newFile.c_str();
			Game->m_FileManager->CloseFile(file);
		}
	}

	sound = new CBSoundBuffer(Game);
	if (!sound) return NULL;

	sound->SetStreaming(Streamed);
	sound->SetType(Type);


	HRESULT res = sound->LoadFromFile(Filename);
	if (FAILED(res)) {
		Game->LOG(res, "Error loading sound '%s'", Filename);
		delete sound;
		return NULL;
	}

	// set volume appropriately
	switch (Type) {
	case SOUND_SFX:
		sound->SetVolume(m_VolumeSFX);
		break;
	case SOUND_SPEECH:
		sound->SetVolume(m_VolumeSpeech);
		break;
	case SOUND_MUSIC:
		sound->SetVolume(m_VolumeMusic);
		break;
	}

	// register sound
	m_Sounds.Add(sound);

	return sound;
#endif
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::AddSound(CBSoundBuffer *Sound, TSoundType Type) {
	if (!Sound) return E_FAIL;
#if 0
	// set volume appropriately
	switch (Type) {
	case SOUND_SFX:
		Sound->SetVolume(m_VolumeSFX);
		break;
	case SOUND_SPEECH:
		Sound->SetVolume(m_VolumeSpeech);
		break;
	case SOUND_MUSIC:
		Sound->SetVolume(m_VolumeMusic);
		break;
	}

	// register sound
	m_Sounds.Add(Sound);
#endif
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::RemoveSound(CBSoundBuffer *Sound) {
#if 0
	for (int i = 0; i < m_Sounds.GetSize(); i++) {
		if (m_Sounds[i] == Sound) {
			delete m_Sounds[i];
			m_Sounds.RemoveAt(i);
			return S_OK;
		}
	}
#endif
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::SetVolume(TSoundType Type, int Volume) {
	if (!m_SoundAvailable) return S_OK;
#if 0
	switch (Type) {
	case SOUND_SFX:
		m_VolumeSFX = Volume;
		break;
	case SOUND_SPEECH:
		m_VolumeSpeech = Volume;
		break;
	case SOUND_MUSIC:
		m_VolumeMusic  = Volume;
		break;
	}

	for (int i = 0; i < m_Sounds.GetSize(); i++) {
		if (m_Sounds[i]->m_Type == Type) m_Sounds[i]->SetVolume(Volume);
	}
#endif
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::SetVolumePercent(TSoundType Type, byte Percent) {
	return SetVolume(Type, Percent);
}


//////////////////////////////////////////////////////////////////////////
byte CBSoundMgr::GetVolumePercent(TSoundType Type) {
	int Volume;
	switch (Type) {
	case SOUND_SFX:
		Volume = m_VolumeSFX;
		break;
	case SOUND_SPEECH:
		Volume = m_VolumeSpeech;
		break;
	case SOUND_MUSIC:
		Volume = m_VolumeMusic;
		break;
	}

	return (byte )Volume;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::SetMasterVolumePercent(byte  Percent) {
	m_VolumeMaster = Percent;
#if 0
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, (uint32)(10000.0f / 100.0f * (float)Percent));
#endif
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
byte CBSoundMgr::GetMasterVolumePercent() {
#if 0
	uint32 val = BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
	return (float)val / 10000.0f * 100.0f;
#endif
	return 0;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::PauseAll(bool IncludingMusic) {
#if 0
	for (int i = 0; i < m_Sounds.GetSize(); i++) {
		if (m_Sounds[i]->IsPlaying() && (m_Sounds[i]->m_Type != SOUND_MUSIC || IncludingMusic)) {
			m_Sounds[i]->Pause();
			m_Sounds[i]->m_FreezePaused = true;
		}
	}
#endif
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::ResumeAll() {
#if 0
	for (int i = 0; i < m_Sounds.GetSize(); i++) {
		if (m_Sounds[i]->m_FreezePaused) {
			m_Sounds[i]->Resume();
			m_Sounds[i]->m_FreezePaused = false;
		}
	}
#endif
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
float CBSoundMgr::PosToPan(int X, int Y) {
	float relPos = (float)X / ((float)Game->m_Renderer->m_Width);

	float minPan = -0.7f;
	float maxPan = 0.7f;

	return minPan + relPos * (maxPan - minPan);
}

} // end of namespace WinterMute
