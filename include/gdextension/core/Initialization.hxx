#pragma once

struct Initialization {
    enum class Level {
        CORE,
        SERVERS,
        SCENE,
        EDITOR,
        MAX,
    };

    Level minimum_initialization_level;
    void* userdata;

    void (*initialize)(void* userdata, Level p_level);
    void (*deinitialize)(void* userdata, Level p_level);
};
