#include "../includes.h"

class CCMenuMoveAndRemove : public CCMoveTo {
public:
    void update(float) override;

    static CCMenuMoveAndRemove* create(float duration, CCPoint const& pos) {
        auto pRet = new CCMenuMoveAndRemove();

        if(pRet->initWithDuration(duration, pos)) {
            pRet->autorelease();
            return pRet;
        }
        else {
            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
    }
};