#pragma once

// DESCRIPTION
//      returns true if x belongs to the open interval (a, b)
#define in_open_inter(x, a, b) ((x) > (a) && (x) < (b)) ? true : false

// DESCRIPTION
//      returns true if x belongs to the closed interval [a, b]
#define in_closed_inter(x, a, b) ((x) >= (a) && (x) <= (b)) ? true : false