#pragma once

namespace CheatState {
	inline int local_player_idx = 0;
	inline int max_clients = 0;
	inline int max_entities = 0;

	inline float znear = 0.f;
	inline float current_fov = 0.f;

	inline bool menu_open = true;

	inline VMatrix WorldToView = { };
	inline VMatrix ViewToProjection = { };
	inline VMatrix WorldToPixels = { };
	inline VMatrix m_WorldToProjection = {};
	inline int screensize_x = 0, screensize_y = 0;
}