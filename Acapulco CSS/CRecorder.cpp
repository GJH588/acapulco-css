#include "CRecorder.h"
#include <string>

struct Frame
{
	float viewangles[2];
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	unsigned char impulse;
	short mousedx;
	short mousedy;

	Frame(CUserCmd* cmd)
	{
		this->viewangles[0] = cmd->viewangles.x;
		this->viewangles[1] = cmd->viewangles.y;
		this->forwardmove = cmd->forwardmove;
		this->sidemove = cmd->sidemove;
		this->upmove = cmd->upmove;
		this->buttons = cmd->buttons;
		this->impulse = cmd->impulse;
		this->mousedx = cmd->mousedx;
		this->mousedy = cmd->mousedy;
	}

	void Replay(CUserCmd* cmd)
	{
		cmd->viewangles.x = this->viewangles[0];
		cmd->viewangles.y = this->viewangles[1];
		cmd->forwardmove = this->forwardmove;
		cmd->sidemove = this->sidemove;
		cmd->upmove = this->upmove;
		cmd->buttons = this->buttons;
		cmd->impulse = this->impulse;
		cmd->mousedx = this->mousedx;
		cmd->mousedy = this->mousedy;
	}
};

typedef std::vector<Frame> FrameContainer;

class Recorder
{
private:
	bool is_recording_active = false;
	FrameContainer recording_frames;

public:
	void StartRecording()
	{
		this->is_recording_active = true;
	}

	void StopRecording()
	{
		this->is_recording_active = false;
	}

	bool IsRecordingActive() const
	{
		return this->is_recording_active;
	}

	FrameContainer& GetActiveRecording()
	{
		return this->recording_frames;
	}
};

class Playback
{
private:
	bool is_playback_active = false;
	size_t current_frame = 0;
	FrameContainer& active_demo = FrameContainer();

public:
	void StartPlayback(FrameContainer& frames)
	{
		this->is_playback_active = true;
		this->active_demo = frames;
	};

	void StopPlayback()
	{
		this->is_playback_active = false;
		this->current_frame = 0;
	};

	bool IsPlaybackActive() const
	{
		return this->is_playback_active;
	}

	size_t GetCurrentFrame() const
	{
		return this->current_frame;
	};

	void SetCurrentFrame(size_t frame)
	{
		this->current_frame = frame;
	};

	FrameContainer& GetActiveDemo() const
	{
		return this->active_demo;
	}
};

Playback playback;
Recorder recorder;

void CRecorder::Draw()
{
	ImGui::Begin("Recorder");
	{
		ImGui::Columns(2, NULL, true);
		{
			if (ImGui::Button("Begin recording (F12)", ImVec2(-1, 16))) { recorder.StartRecording(); }
			if (ImGui::Button("Stop recording (F11)", ImVec2(-1, 16))) { recorder.StopRecording(); }

			if (ImGui::Button("Begin playback (F10)", ImVec2(-1, 16))) { playback.StartPlayback(recorder.GetActiveRecording()); }
			if (ImGui::Button("Stop playback (F12)", ImVec2(-1, 16))) { playback.StopPlayback(); }

			if (ImGui::Button("Clear frames (F2)", ImVec2(-1, 16))) { recorder.GetActiveRecording().clear(); }
		}
		ImGui::NextColumn();
		{
			FrameContainer& recording = recorder.GetActiveRecording();

			int currentFrame = playback.GetCurrentFrame();
			int maxFrames = recording.size(); 

			ImGui::Text(std::string("Current frame: " + std::to_string(currentFrame) + " / " + std::to_string(maxFrames)).c_str());

			ImGui::Text(recorder.IsRecordingActive() ? "Recording: yes" : "Recording: no");
			ImGui::Text(playback.IsPlaybackActive() ? "Playing: yes" : "Playing: no");
		}
		ImGui::Columns(1);
	}
	ImGui::End();
}

void CRecorder::Hook(CUserCmd* cmd)
{
	if (!Settings::Misc::Recorder)
		return;

	bool isPlaybackActive = playback.IsPlaybackActive();
	bool isRecordingActive = recorder.IsRecordingActive();

	FrameContainer& recording = recorder.GetActiveRecording();

	if (GetAsyncKeyState(VK_F12))
		recorder.StartRecording();

	if (GetAsyncKeyState(VK_F11))
		recorder.StopRecording();

	if (GetAsyncKeyState(VK_F10))
		playback.StartPlayback(recorder.GetActiveRecording());

	if (GetAsyncKeyState(VK_F9))
		playback.StopPlayback();

	if (GetAsyncKeyState(VK_F2))
		recorder.GetActiveRecording().clear();

	if (isRecordingActive)
		recording.push_back({ cmd });

	if (isPlaybackActive) 
	{
		const size_t current_playback_frame = playback.GetCurrentFrame();

		try 
		{
			recording.at(current_playback_frame).Replay(cmd);
			pEngine->SetViewAngles(cmd->viewangles);

			if (current_playback_frame + 1 == recording.size()) 
			{
				playback.StopPlayback();
			}
			else 
			{
				playback.SetCurrentFrame(current_playback_frame + 1);
			}
		}
		catch (std::out_of_range) 
		{
			playback.StopPlayback();
		}
	}
}