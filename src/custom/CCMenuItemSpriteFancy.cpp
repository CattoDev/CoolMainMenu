#include "CCMenuItemSpriteFancy.hpp"

#define SCALEACTIONTAG 0xD
#define MOVEACTIONTAG 0xE

bool CCMenuItemSpriteFancy::init(CCNode* sprite, CCObject* target, SEL_MenuHandler callback, CCParticleSystemQuad* bgParticles) {
    if(!initWithNormalSprite(sprite, sprite, sprite, target, callback)) return false;

    m_bHover = false;
    m_bHoverActionsEnabled = true;
    m_pBtnSpr = (CCSprite*)sprite;
    m_pBGParticles = bgParticles;
    m_pButtonHoverAnimation = HoverAnimation::Scale;

    if(m_pBGParticles != nullptr)
        m_pBGParticles->stopSystem();

    // some shit that usually goes on in CCMenuItemSpriteExtra
    sprite->setAnchorPoint(CCPoint(.5, .5));
    auto size = sprite->getContentSize();
    size.width *= sprite->getScaleX();
    size.height *= sprite->getScaleY();
    setContentSize(size);
    sprite->setPosition(size / 2);

    // s
    scheduleUpdate();

    return true;
}

void CCMenuItemSpriteFancy::playFloatAnimation(float yDiff, float duration) {
    auto ease1 = CCEaseInOut::create(CCMoveBy::create(duration, CCPoint(0, yDiff)), 2);
    auto ease2 = CCEaseInOut::create(CCMoveBy::create(duration, CCPoint(0, -yDiff)), 2);

    auto seq = CCSequence::create(ease1, ease2, nullptr);
    auto ac = CCRepeatForever::create(seq);

    runAction(ac);
}

void CCMenuItemSpriteFancy::selected() {
    CCMenuItemSprite::selected();

    if(getActionByTag(SCALEACTIONTAG) != nullptr) stopAction(getActionByTag(SCALEACTIONTAG));

    unscheduleUpdate();

    auto action = CCEaseExponentialOut::create(
        CCScaleTo::create(.5, 1.2)
    );

    runAction(action);
}

void CCMenuItemSpriteFancy::unselected() {
    CCMenuItemSprite::unselected();

    if(getActionByTag(SCALEACTIONTAG) != nullptr) stopAction(getActionByTag(SCALEACTIONTAG));

    unscheduleUpdate();

    auto action = CCEaseExponentialOut::create(
        CCScaleTo::create(.5, 1)
    );

    scheduleUpdate();

    runAction(action);
}

void CCMenuItemSpriteFancy::activate() {
    // fix weird ass scaling
    unscheduleUpdate();
    
    if(getActionByTag(SCALEACTIONTAG) != nullptr) stopAction(getActionByTag(SCALEACTIONTAG));

    unselected();

    CCMenuItemSprite::activate();
}

void CCMenuItemSpriteFancy::onMouseHoverStarted() {
    if(m_pButtonHoverAnimation == HoverAnimation::Scale) {
        auto scale = CCEaseInOut::create(CCScaleTo::create(.5, .925), 2);
        scale->setTag(SCALEACTIONTAG);
        if(getActionByTag(SCALEACTIONTAG) != nullptr) stopAction(getActionByTag(SCALEACTIONTAG));
        runAction(scale);
    }
    else {
        auto move = CCEaseElasticOut::create(CCMoveTo::create(1, m_pOriginalPos + CCPoint(0, 15)), 1);
        move->setTag(MOVEACTIONTAG);
        if(getActionByTag(MOVEACTIONTAG) != nullptr) stopAction(getActionByTag(MOVEACTIONTAG));
        runAction(move);
    }

    // CCParticleSystem::resetSystem instantly removes particles that are left floating around
    // this resets it without instantly removing particles
    if(m_pBGParticles == nullptr) return;
    *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(m_pBGParticles) + 0x164) = true;
    *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(m_pBGParticles) + 0x108) = 0;
    *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(m_pBGParticles) + 0x150) = 0;
}

void CCMenuItemSpriteFancy::onMouseHoverEnded() {
    if(m_pButtonHoverAnimation == HoverAnimation::Scale) {
        auto scale = CCEaseElasticOut::create(CCScaleTo::create(.5, 1), 1);
        scale->setTag(SCALEACTIONTAG);
        if(getActionByTag(SCALEACTIONTAG) != nullptr) stopAction(getActionByTag(SCALEACTIONTAG));
        runAction(scale);
    }
    else {
        auto move = CCEaseElasticOut::create(CCMoveTo::create(1, m_pOriginalPos), 2);
        move->setTag(MOVEACTIONTAG);
        if(getActionByTag(MOVEACTIONTAG) != nullptr) stopAction(getActionByTag(MOVEACTIONTAG));
        runAction(move);
    }

    if(m_pBGParticles != nullptr) m_pBGParticles->stopSystem();
}

void CCMenuItemSpriteFancy::update(float delta) {
    CCNode::update(delta);

    auto dir = CCDirector::sharedDirector();

    if(!m_bHoverActionsEnabled) return;

    // ignore if popups are a thing aaa
    // I know this is a wack ass workaround but hey
    if(dir->getRunningScene()->getChildrenCount() > 1) {
        if(m_bHover) {
            onMouseHoverEnded();
            m_bHover = false;
        }

        return;
    }

    /* 
        hover effect
    */
    auto winSize = dir->getWinSize();
    auto view = dir->getOpenGLView();
    auto mPos = view->getMousePosition();
    auto viewPortRect = view->getViewPortRect();

    // fix Y pos wtf!!!
    mPos.y = viewPortRect.size.height - mPos.y;

    float xRatio = winSize.width / viewPortRect.size.width;
    float yRatio = winSize.height / viewPortRect.size.height;

    float posX = mPos.x * xRatio;
    float posY = mPos.y * yRatio;

    auto rect = m_pBtnSpr->getTextureRect();
    rect.origin.x = 0;
    rect.origin.y = 0;

    auto locInNode = convertToNodeSpace(CCPoint(posX, posY));

    if(rect.containsPoint(locInNode)) {
        if(!m_bHover) {
            m_bHover = true;
            onMouseHoverStarted();
        }
    }
    else {
        if(m_bHover) {
            m_bHover = false;
            onMouseHoverEnded();
        }
    }
}