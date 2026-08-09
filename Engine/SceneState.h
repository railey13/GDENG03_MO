#pragma once

// ---------------------------------------------------------------------------
// SceneState
//   Tracks whether the editor is in Edit mode (changes are permanent) or
//   Play mode (changes are temporary and will be reverted on Stop).
// ---------------------------------------------------------------------------
enum class SceneState
{
	Edit,   // Default authoring state — inspector edits persist
	Play,   // Runtime state — inspector edits and spawns are reverted on Stop
};
