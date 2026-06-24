//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_OPENGLCAPSCHECKER_H
#define MCPORT_OPENGLCAPSCHECKER_H


class OpenGlCapsChecker {
private:
    static bool tryCheckOcclusionCapable;

public:
    bool checkARBOcclusion();
};


#endif //MCPORT_OPENGLCAPSCHECKER_H
