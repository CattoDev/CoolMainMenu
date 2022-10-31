#ifndef PARTICLECREATOR_H
#define PARTICLECREATOR_H

#include <cocos2d.h>

class ParticleCreator {
public:
    static CCParticleSystemQuad* createBtnFloatParticles(CCPoint posVar) {
        // texture from dragEffect lol
        auto dict = CCDictionary::createWithContentsOfFileThreadSafe("dragEffect.plist");

        dict->setObject(CCString::create("0"), "emitterType");
        dict->setObject(CCString::create("-1"), "duration");
        dict->setObject(CCString::create("200"), "maxParticles");
        dict->setObject(CCString::create("1.5"), "particleLifespan");
        dict->setObject(CCString::create("-90"), "angle");
        dict->setObject(CCString::create("30"), "gravityy");

        // speed
        dict->setObject(CCString::create("0"), "speed");
        dict->setObject(CCString::create("0"), "speedVariance");

        // source position
        dict->setObject(CCString::create("0"), "sourcePositionx");
        dict->setObject(CCString::create("0"), "sourcePositiony");

        dict->setObject(CCString::createWithFormat("%f", posVar.x), "sourcePositionVariancex");
        dict->setObject(CCString::createWithFormat("%f", posVar.y), "sourcePositionVariancey");

        // particle size
        dict->setObject(CCString::create("4"), "startParticleSize");
        dict->setObject(CCString::create("4"), "finishParticleSize");

        dict->setObject(CCString::create("0"), "startParticleSizeVariance");

        // start color
        dict->setObject(CCString::create("1"), "startColorRed");
        dict->setObject(CCString::create("1"), "startColorGreen");
        dict->setObject(CCString::create("1"), "startColorBlue");
        dict->setObject(CCString::create("1"), "startColorAlpha");

        dict->setObject(CCString::create("0"), "startColorVarianceRed");
        dict->setObject(CCString::create("0"), "startColorVarianceBlue");
        dict->setObject(CCString::create("0"), "startColorVarianceGreen");
        dict->setObject(CCString::create("0"), "startColorVarianceAlpha");

        // finish color
        dict->setObject(CCString::create("1"), "finishColorRed");
        dict->setObject(CCString::create("1"), "finishColorGreen");
        dict->setObject(CCString::create("1"), "finishColorBlue");
        dict->setObject(CCString::create("0"), "finishColorAlpha");

        dict->setObject(CCString::create("1"), "finishColorVarianceRed");
        dict->setObject(CCString::create("1"), "finishColorVarianceBlue");
        dict->setObject(CCString::create("1"), "finishColorVarianceGreen");
        dict->setObject(CCString::create("0"), "finishColorVarianceAlpha");

        auto p = CCParticleSystemQuad::create();
        if(p && p->initWithDictionary(dict)) {
            dict->release();
            p->autorelease();
        }
        else {
            CC_SAFE_DELETE(p);
            return nullptr;
        }

        // position type
        *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(p) + 0x208) = 2;

        return p;
    }

    static CCParticleSystemQuad* createBtnBackgroundParticles(float maxRadius) {
        // texture from dragEffect lol
        auto dict = CCDictionary::createWithContentsOfFileThreadSafe("dragEffect.plist");

        dict->setObject(CCString::create("1"), "emitterType");
        dict->setObject(CCString::create("-1"), "duration");
        dict->setObject(CCString::create("200"), "maxParticles");
        dict->setObject(CCString::create("1.5"), "particleLifespan");
        dict->setObject(CCString::create("0"), "angle");
        dict->setObject(CCString::create("180"), "angleVariance");

        // radius
        // WHY IS IT INVERTED WTFF
        dict->setObject(CCString::create("30"), "maxRadius");
        dict->setObject(CCString::createWithFormat("%f", maxRadius), "minRadius");

        // source position
        dict->setObject(CCString::create("0"), "sourcePositionx");
        dict->setObject(CCString::create("0"), "sourcePositiony");

        //dict->setObject(CCString::createWithFormat("%f", posVar.x), "sourcePositionVariancex");
        //dict->setObject(CCString::createWithFormat("%f", posVar.y), "sourcePositionVariancey");
        dict->setObject(CCString::create("0"), "sourcePositionVariancex");
        dict->setObject(CCString::create("0"), "sourcePositionVariancey");

        // particle size
        dict->setObject(CCString::create("4"), "startParticleSize");
        dict->setObject(CCString::create("4"), "finishParticleSize");

        dict->setObject(CCString::create("0"), "startParticleSizeVariance");

        // start color
        dict->setObject(CCString::create("1"), "startColorRed");
        dict->setObject(CCString::create("1"), "startColorGreen");
        dict->setObject(CCString::create("1"), "startColorBlue");
        dict->setObject(CCString::create("1"), "startColorAlpha");

        dict->setObject(CCString::create("1"), "startColorVarianceRed");
        dict->setObject(CCString::create("1"), "startColorVarianceBlue");
        dict->setObject(CCString::create("1"), "startColorVarianceGreen");
        dict->setObject(CCString::create("0"), "startColorVarianceAlpha");

        // finish color
        dict->setObject(CCString::create("1"), "finishColorRed");
        dict->setObject(CCString::create("1"), "finishColorGreen");
        dict->setObject(CCString::create("1"), "finishColorBlue");
        dict->setObject(CCString::create("0"), "finishColorAlpha");

        dict->setObject(CCString::create("1"), "finishColorVarianceRed");
        dict->setObject(CCString::create("1"), "finishColorVarianceBlue");
        dict->setObject(CCString::create("1"), "finishColorVarianceGreen");
        dict->setObject(CCString::create("0"), "finishColorVarianceAlpha");

        auto p = CCParticleSystemQuad::create();
        if(p && p->initWithDictionary(dict)) {
            dict->release();
            p->autorelease();
        }
        else {
            CC_SAFE_DELETE(p);
            return nullptr;
        }

        // position type
        *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(p) + 0x208) = 2;

        return p;
    }
};

#endif