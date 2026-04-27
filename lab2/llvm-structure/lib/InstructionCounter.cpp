#include "InstructionCounter.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

PreservedAnalyses InstructionCounter::run(Function &F,
                                          FunctionAnalysisManager &FAM)
{
  //******************************** ASSIGNMENT ********************************

  for (auto &B : F)
  {
    // 2. 기본 블록 내의 모든 명령어(Instruction)를 순회합니다.
    for (auto &I : B)
    {
      // 3. 명령어의 이름(Opcode Name)을 가져옵니다 (예: add, br, call 등).
      auto OpcodeName = I.getOpcodeName();

      // 4. 해당 명령어의 카운트를 증가시킵니다.
      // InstructionCounter가 StringMap<unsigned> 또는 유사한 구조라고 가정합니다.
      InstructionCounter[OpcodeName]++;
    }
  }

  //****************************** ASSIGNMENT END ******************************

  errs() << "Instruction hit counts\n";
  errs() << "-----------------------------\n";
  for (auto &InstructionCount : InstructionCounter)
  {
    errs() << InstructionCount.getKey() << " : " << InstructionCount.getValue()
           << "\n";
  }
  errs() << "-----------------------------\n";

  return PreservedAnalyses::all();
}

/*******************************************************************************
 *                        Pass Plugin Registration
 ******************************************************************************/

// Hiding callback functions for registration steps inside anonymous namespace.
namespace
{
  /**
   * Register pass to the pipeline.
   *
   * Registered pass can be called from `opt` by its name.
   */
  bool regInstructionCounterToPipeline(StringRef Name, FunctionPassManager &FPM,
                                       ArrayRef<PassBuilder::PipelineElement>)
  {
    if (Name != "instruction-counter")
      return false;
    FPM.addPass(InstructionCounter());
    return true;
  }

  void PBHook(PassBuilder &PB)
  {
    PB.registerPipelineParsingCallback(regInstructionCounterToPipeline);
  }

  PassPluginLibraryInfo getInstructionCounterPluginInfo()
  {
    return {LLVM_PLUGIN_API_VERSION, "Advanced Compilers - Instruction Counter",
            LLVM_VERSION_STRING, PBHook};
  }
} // namespace

// Pass registeration.
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo()
{
  return getInstructionCounterPluginInfo();
}