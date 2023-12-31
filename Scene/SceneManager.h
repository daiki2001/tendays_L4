#pragma once
#include "Scene.h"
#include "../Input/KeyInput.h"
#include <memory>
#include <stack>

class BaseScene;
class SceneManager
{
public: // メンバ関数
	SceneManager();
	~SceneManager() {}

	void Loop(const KeyInput& input) const;
	void SceneChange(Scene scene, bool stackClear);
	int PopScene();

private: // メンバ変数
	std::stack<std::shared_ptr<BaseScene>> sceneStack;
};
