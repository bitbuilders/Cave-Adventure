
#pragma once

class Controls;

namespace Input
{
    bool Init();

    void Shutdown();

    void ProcessInput(Controls* Controls);
}
