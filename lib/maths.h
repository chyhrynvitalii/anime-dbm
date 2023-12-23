#pragma once

#define in_open_range(x, low, up)   ((x) > (low) && (x) < (up)) ? true : false
#define in_closed_range(x, low, up) ((x) >= (low) && (x) <= (up)) ? true : false