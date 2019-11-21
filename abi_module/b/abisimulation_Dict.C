// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME abisimulation_Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/seaquest/users/apun/abi_project/abi_module/abisimulation.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *abisimulation_Dictionary();
   static void abisimulation_TClassManip(TClass*);
   static void *new_abisimulation(void *p = 0);
   static void *newArray_abisimulation(Long_t size, void *p);
   static void delete_abisimulation(void *p);
   static void deleteArray_abisimulation(void *p);
   static void destruct_abisimulation(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::abisimulation*)
   {
      ::abisimulation *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::abisimulation));
      static ::ROOT::TGenericClassInfo 
         instance("abisimulation", "", 35,
                  typeid(::abisimulation), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &abisimulation_Dictionary, isa_proxy, 3,
                  sizeof(::abisimulation) );
      instance.SetNew(&new_abisimulation);
      instance.SetNewArray(&newArray_abisimulation);
      instance.SetDelete(&delete_abisimulation);
      instance.SetDeleteArray(&deleteArray_abisimulation);
      instance.SetDestructor(&destruct_abisimulation);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::abisimulation*)
   {
      return GenerateInitInstanceLocal((::abisimulation*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::abisimulation*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *abisimulation_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::abisimulation*)0x0)->GetClass();
      abisimulation_TClassManip(theClass);
   return theClass;
   }

   static void abisimulation_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_abisimulation(void *p) {
      return  p ? new(p) ::abisimulation : new ::abisimulation;
   }
   static void *newArray_abisimulation(Long_t nElements, void *p) {
      return p ? new(p) ::abisimulation[nElements] : new ::abisimulation[nElements];
   }
   // Wrapper around operator delete
   static void delete_abisimulation(void *p) {
      delete ((::abisimulation*)p);
   }
   static void deleteArray_abisimulation(void *p) {
      delete [] ((::abisimulation*)p);
   }
   static void destruct_abisimulation(void *p) {
      typedef ::abisimulation current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::abisimulation

namespace {
  void TriggerDictionaryInitialization_abisimulation_Dict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "abisimulation_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class abisimulation;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "abisimulation_Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef __ABISIMULATION_H__
#define __ABISIMULATION_H__

#include <fun4all/SubsysReco.h>
#include <vector>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TTree.h>

class PHCompositeNode;
class RawClusterContainer;
class RawCluster;
class SvtxTrackMap;
class JetMap;
class JetEvalStack;
class JetRecoEval;
class SvtxTrackEval;
class PHG4TruthInfoContainer;
class PHHepMCGenEvent;
//class RawTowerGeom;
//class RawTowerGeomContainer;
class GlobalVertex;
class PHG4HitContainer;
class SQDimuonTruthInfoContainer;
class MCDimuon;

class abisimulation : public SubsysReco
{
 public:
  abisimulation(const std::string &name = "abivertex.root");

  int Init(PHCompositeNode *);
  int InitRun(PHCompositeNode *);
  int process_event(PHCompositeNode *);
  int End(PHCompositeNode *);

 
 private:
  void ResetVars();
  void InitTree();
  int GetNodes(PHCompositeNode *topNode);
  PHG4TruthInfoContainer* _truth;
  SQDimuonTruthInfoContainer* _dimuoninfo;
  //MCDimuon* _dimuoninfo;
  TFile *file;
  TTree *truth_tree;
  std::string outfilename;

  //all truth particles
  /* float truthpx, truthpy, truthpz; */
  /* float truthp; */
  /* float truthphi; */
  /* float trutheta; */
  /* float truthpt; */
  /* float truthenergy; */
  /* int truthpid; */
  /* int numparticlesinevent; */
  /* int process_id; */
 
  //Vertex variable
  float truth_vtxx[2],truth_vtxy[2],truth_vtxz[2], truthpx[2], truthpy[2], truthpz[2] ,dimuon_CosTheta,dimuon_phi,dimuon_xs, truthe[2],dimuon_m;
  int truthpid[2];


  const float pi = 3.1415926;
};

#endif  // __ABIVERTEX_H__

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"abisimulation", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("abisimulation_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_abisimulation_Dict_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_abisimulation_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_abisimulation_Dict() {
  TriggerDictionaryInitialization_abisimulation_Dict_Impl();
}
