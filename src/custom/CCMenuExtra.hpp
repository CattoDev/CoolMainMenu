#include "../includes.h"

class CCMenuExtra : public CCMenu {
public:
    // fix bg being set to full opacity
    void updateDisplayedOpacity(GLubyte parentOpacity) override {
        _displayedOpacity = _realOpacity * parentOpacity / 255.0;

        if(_cascadeOpacityEnabled) {
            CCObject* pObj;
            CCARRAY_FOREACH(m_pChildren, pObj)
            {
                CCRGBAProtocol* item = dynamic_cast<CCRGBAProtocol*>(pObj);
                if(item)
                {
                    GLubyte childOpacity = _displayedOpacity;
                    if(dynamic_cast<CCNode*>(item)->getTag() == 0xA) {
                        childOpacity = childOpacity > 100 ? 100 : childOpacity;
                    }

                    item->updateDisplayedOpacity(childOpacity);
                }
            }
        }
    }

    static CCMenuExtra* create() {
        auto pRet = new CCMenuExtra();

        if(pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};