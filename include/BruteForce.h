#pragma once
#include "env/Primitive.h"


namespace AccelStruct {

    class BruteForce {

    public:

        Primitive* list = nullptr;

    public:

        BruteForce() {}
        BruteForce(Primitive* prim) {
            list = prim;
        }

    };

};