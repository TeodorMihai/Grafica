#include "Player.h"

Player::Player(glm::vec3 pos, int lifes, int orientation) :
	pos(pos), lifes(lifes), orientation(orientation), currentAnimation("idle"), indexAnimation(0) { }
Player::Player() : currentAnimation("idle") , indexAnimation(0) { }
Player::~Player() { }