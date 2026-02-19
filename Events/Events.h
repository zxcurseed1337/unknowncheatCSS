#pragma once


namespace CheatEvents {
	bool ModifyCmd(CUserCmd* cmd);

	void FrameStageNotify(ClientFrameStage_t stage);
	void LevelInitPostEntity();
	void LevelShutdown();

}