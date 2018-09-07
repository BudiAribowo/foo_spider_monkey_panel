#pragma once

namespace mozjs
{

struct DropActionParams 
{
    int32_t playlistIdx = -1;// -1 means active playlist
    uint32_t base = 0;
    bool toSelect = true;
    uint32_t effect = DROPEFFECT_NONE;

    void Reset()
    {
        playlistIdx = -1;
        base = 0;
        toSelect = true;
        effect = DROPEFFECT_NONE;
    }
};

struct DropActionMessageParams
{
    DropActionParams actionParams;
    POINTL pt;
    uint32_t keyState;
};

}
