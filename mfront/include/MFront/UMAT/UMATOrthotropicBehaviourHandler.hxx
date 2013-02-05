/*! 
 * \file  UMATOrthotropicBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_UMAT_UMATORTHOTROPICBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_UMAT_UMATORTHOTROPICBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_UMAT_UMATBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif

namespace umat
{
  
  template<unsigned short N,template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicBehaviourHandler;

  template<template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicBehaviourHandler<1u,Behaviour>
    : private UMATBehaviourHandler<1u,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const,
	       const UMATReal *const DDSOE,
	       const UMATReal *const STRAN ,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const UMATInt  *const NDI)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<1u,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<1u,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<1u,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	  Traits::useTimeSubStepping,
	  typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	  typename UMATBehaviourHandler::template Integrator<bs,ba>
	  >::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,DDSOE,STRAN,
		      DSTRAN,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,
		      STRESS,NDI);
      handler.exe(STRESS,STATEV);
    } // end of UMATOrthotropicBehaviourHander<1u,Behaviour>::exe
  }; // end of struct UMATOrthotropicBehaviourHander<1u,Behaviour>

  template<template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicBehaviourHandler<2u,Behaviour>
    : private UMATBehaviourHandler<2u,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const DROT  ,
	       const UMATReal *const DDSOE,
	       const UMATReal *const STRAN ,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const UMATInt  *const NDI) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<2u,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<2u,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<2u,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      UMATReal s[4];
      UMATReal e[4];
      UMATReal de[4];
      UMATRotationMatrix2D m(PROPS+7,DROT);
      m.rotateStressesForward(STRESS,s);
      m.rotateStrainsForward(STRAN,e);
      m.rotateStrainsForward(DSTRAN,de);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,DDSOE,
		      e,de,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,NDI);
      handler.exe(s,STATEV);
      m.rotateStressesBackward(s,STRESS);
    } // end of UMATOrthotropicBehaviourHander<2u,Behaviour>::exe
  }; // end of UMATOrthotropicBehaviourHander<2u,Behaviour>

  template<template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicBehaviourHandler<3u,Behaviour>
    : private UMATBehaviourHandler<3u,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const DROT,
	       const UMATReal *const DDSOE,
	       const UMATReal *const STRAN,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const UMATInt  *const NDI) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<3u,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<3u,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<3u,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      UMATReal  s[6];
      UMATReal  e[6];
      UMATReal de[6];
      // Passage au repère matériau
      UMATRotationMatrix3D m(PROPS+9,DROT);
      m.rotateStressesForward(STRESS,s);
      m.rotateStrainsForward(STRAN,e);
      m.rotateStrainsForward(DSTRAN,de);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,DDSOE,e,
		      de,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,NDI);
      handler.exe(s,STATEV);
      m.rotateStressesBackward(s,STRESS);
    } // end of UMATOrthotropicBehaviourHandler<3u,Behaviour>::exe
	
  }; // end of struct UMATOrthotropicBehaviourHandler<3u,Behaviour>

} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATORTHOTROPICBEHAVIOURHANDLER_H */
