#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "Shuttle.h"

Shuttle::Shuttle() : GameObject("shuttle", "box", "VertexNormal"), lives(3) , timeSinceDeath(0), mode(0), isAlive(true) { }
