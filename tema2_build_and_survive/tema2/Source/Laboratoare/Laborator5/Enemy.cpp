#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "Enemy.h"
Enemy::Enemy() : GameObject("enemy", "box", "VertexNormal"), lives(4), period(0), timeSinceDeath(0), dimension(1.0) { }
