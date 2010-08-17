#include "game/statistic.h"

Statistic::Statistic () :
deaths (0),
crashes (0),
killed (0),
kills (0),
selfs (0),
jump (0),
smiles ({
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}) {
}
