//
// Created by Keir Davis on 25/06/2026.
//
#pragma once

#ifndef MCPORT_IWORLDACCESS_H
#define MCPORT_IWORLDACCESS_H

class IWorldAccess {
public:
    virtual ~IWorldAccess() = default;

    virtual void updateAllRenderers() = 0;
};

#endif //MCPORT_IWORLDACCESS_H
