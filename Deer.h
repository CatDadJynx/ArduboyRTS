#pragma once

enum class DeerState : uint8_t
{
  Idle,
  Dead,
  Running
};

struct Deer
{
  static constexpr uint8_t width = 7;
  static constexpr uint8_t height = 8;
  static constexpr uint8_t defaultRegenenerationDelay = 10;
  static constexpr float movementSpeed = 0.5f;

  DeerState state;
  Point2F position;
  uint8_t regenerationDelay;
  
  Rectangle getBounds() const
  {
    return { position.x, position.y, width, height };
  }

  void hunt()
  {
    this->state = DeerState::Dead;
    this->regenerationDelay = defaultRegenenerationDelay;
  }
};
