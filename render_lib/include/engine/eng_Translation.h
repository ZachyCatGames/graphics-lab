#pragma once
#include <engine/eng_Component.h>
#include <engine/eng_Vector3D.h>

namespace eng {

/**
 * Positional and rotation information.
 */
class Translation : public Component {
public:
    Vector3DF position;
}; // class Translation

} // namespace eng
