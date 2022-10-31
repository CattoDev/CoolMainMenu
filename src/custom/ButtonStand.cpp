#include "ButtonStand.hpp"

using namespace gd;

void GameObject_setupCustomSprites(GameObject* object) {
    reinterpret_cast<void(__thiscall*)(GameObject*)>(base + 0xd7d50)(object);
}

#define MBO(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)

bool ButtonStand::init() {
    if(!CCNode::init()) return false;

    auto mainBlock = GameObject::createWithKey(1);
    addChild(mainBlock);

    auto slab1 = GameObject::createWithKey(40);
    GameObject_setupCustomSprites(slab1);
    slab1->setPosition(CCPoint(-mainBlock->getContentSize().width, -8));
    addChild(slab1);

    auto slab2 = GameObject::createWithKey(40);
    GameObject_setupCustomSprites(slab2);
    slab2->setPosition(CCPoint(mainBlock->getContentSize().width, -8));
    addChild(slab2);

    // particles hehe
    m_pParticles = CCParticleSystemQuad::create();
    CCTexture2D* tex;
    tex = CCTextureCache::sharedTextureCache()->addImage("square.png", true);
    m_pParticles->setTexture(tex);

    // virtual funcs in CCParticleSystemQuad are misaligned lol

    m_pParticles->setPositionY(mainBlock->getContentSize().height);

    addChild(m_pParticles);

    return true;
}