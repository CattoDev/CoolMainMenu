#include "CCMenuMoveAndRemove.hpp"

void CCMenuMoveAndRemove::update(float delta) {
    CCMoveBy::update(delta);

    if(isDone()) {
        m_pTarget->removeAllChildren();
    }
}