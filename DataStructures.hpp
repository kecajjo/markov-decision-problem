#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

enum class ActionType{
    Up,
    Right,
    Down,
    Left,
    Unknown
};

enum class TileType{
    Normal,
    Start,
    Terminal,
    Prohibitet,
    Special,
    Invalid
};

#endif