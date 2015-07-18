
#pragma once
#include "_OLD/game/Game.h"

class Game : public Z::Game
{
public:
    bool initialize() final override;
    void shutdown() final override;
    void runFrame(double time) final override;
    void onPointerPressed(int id, const glm::vec2& pos) override;
    void onPointerMoved(int id, const glm::vec2& pos) override;
    void onPointerReleased(int id, const glm::vec2& pos) override;
    void onPointerCancelled(int id, const glm::vec2& pos) override;
};
