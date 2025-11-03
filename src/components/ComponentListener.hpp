#pragma once

namespace components
{
class ComponentListener {
public:
    virtual ~ComponentListener() = default;

    // Called when a component finishes moving (e.g. on mouse release)
    virtual void onComponentMoved() = 0;
};
} // namespace components