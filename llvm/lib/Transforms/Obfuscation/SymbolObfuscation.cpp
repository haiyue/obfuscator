//
// Created by 王新磊 on 2022/9/15.
//

#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/Transforms/Obfuscation/CryptoUtils.h"
#include "llvm/Transforms/Obfuscation/SymbolObfuscation.h"

#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <iostream>
#include <cstdlib>

#define DEBUG_TYPE "smobf"

using namespace llvm;
using namespace std;

static cl::opt<std::string> SymSeed("symSeed", cl::init(""),
                                    cl::desc("seed for the random of symbol obfuscation"));

//static string obfcharacters = "-_.|/\\\\`+,=()*:";
static string obfcharacters = "_1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

namespace {
  struct SymbolObfuscation : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    bool flag;

    SymbolObfuscation(): FunctionPass(ID) {}

    SymbolObfuscation(bool flag) : FunctionPass(ID) {
      this->flag = flag;
    }

    bool runOnFunction(Function &F) override;
    void symobfuscation(Function &F);
  };
}

char SymbolObfuscation::ID = 0;
static RegisterPass<SymbolObfuscation> X("symbolobf", "symbol obfuscation");
Pass *llvm::createSymbolObfuscation(bool flag) { return  new SymbolObfuscation(flag); }

bool SymbolObfuscation::runOnFunction(Function &F) {
  if (!toObfuscate(flag, &F, "smobf")) {
    return false;
  }

  symobfuscation(F);
  return true;
}

int seed = 0;
string randomString(int length) {
  string name;
  name.resize(length);
  srand(seed);
  seed++;
  for (int i = 0; i < length; ++i) {
    name[i] = obfcharacters[rand() % (obfcharacters.length())];
  }

  return "f_" + name;
}

void SymbolObfuscation::symobfuscation(Function &F) {
  //F.setName(randomString(16));
  if (F.getName().str().compare("main")==0){
    errs()<<"Skipping main\n";
  }
  else if(F.empty()==false){
    //Rename
    string newname = randomString(16);
    errs()<<"Renaming Function: "<<F.getName()<<"\n";
    errs()<<"New Function Name: "<<newname<<"\n";
    F.setName(newname);
  }
  else{
    errs()<<"Skipping External Function: "<<F.getName()<<"\n";
  }
}
