#include "Common.h"

GameManager::GameManager(ID3D10Device* m_GraphicsDevice){}

void GameManager::Init(){}
void GameManager::Update(float dt){}
void GameManager::Render(){}

bool GameManager::KeyPress(){return false;}
bool GameManager::MousePress(){return false;}
bool GameManager::MouseMove(){return false;}
bool GameManager::MouseWheel( WPARAM wParam ){return false;}
void GameManager::CheckInput(float dt){}
