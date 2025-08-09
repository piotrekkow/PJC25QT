#pragma once

class Lane
{
    float width_;   // meters
    // vehicle restrictions, more stuff later

public:
    Lane(float width = 3.5f);
    float getWidth() const { return width_; }
};
