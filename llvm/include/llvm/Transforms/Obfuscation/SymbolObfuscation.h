//
// Created by 王新磊 on 2022/9/15.
//

#ifndef OBFUSCATOR_SYMBOLOBFUSCATION_H
#define OBFUSCATOR_SYMBOLOBFUSCATION_H

#include "llvm/Pass.h"

namespace llvm {
    Pass* createSymbolObfuscation(bool flag);
}

#endif // OBFUSCATOR_SYMBOLOBFUSCATION_H
