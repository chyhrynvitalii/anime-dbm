#pragma once

// returns true if x belongs to [low, up], otherwise returns false
#define in_closed_range(x, low, up) ((x) >= (low) && (x) <= (up)) ? true : false