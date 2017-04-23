#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "Bullet.h"
Bullet::Bullet() : GameObject("sphere", "", "VertexNormal") , power(1), distance(0) , timeSinceLaunch(0) { }
