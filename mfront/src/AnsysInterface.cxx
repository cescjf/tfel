/*!
 * \file   mfront/src/AnsysInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Config/GetInstallPath.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/Ansys/AnsysOrthotropicBehaviour.hxx"
#include"MFront/AnsysInterface.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront{

  static void
  writeUMATArguments(std::ostream& out,
		     const BehaviourDescription::BehaviourType& t,
		     const AnsysInterface::FiniteStrainStrategy fss,
		     const bool f)
  {
    if(f){
      out << "(const ansys::AnsysInt *const matId,\n"
	  << " const ansys::AnsysInt *const elemId,\n"
	  << " const ansys::AnsysInt *const kDomIntPt,\n"
	  << " const ansys::AnsysInt *const kLayer,\n"
	  << " const ansys::AnsysInt *const kSectPt,\n"
	  << " const ansys::AnsysInt *const ldsetp,\n"
	  << " const ansys::AnsysInt *const isubst,\n"
	  << "       ansys::AnsysInt *const keycut,\n"
	  << " const ansys::AnsysInt *const nDirect,\n"
	  << " const ansys::AnsysInt *const nShear,\n"
	  << " const ansys::AnsysInt *const NTENS,\n"
	  << " const ansys::AnsysInt *const NSTATV,\n"
	  << " const ansys::AnsysInt *const NPROPS,\n"
	  << " const ansys::AnsysReal *const TIME,\n"
	  << " const ansys::AnsysReal *const DTIME,\n"
	  << " const ansys::AnsysReal *const TEMP,\n"
	  << " const ansys::AnsysReal *const DTEMP,\n"
	  << "       ansys::AnsysReal *const STRESS,\n"
	  << "       ansys::AnsysReal *const STATEV,\n"
	  << "       ansys::AnsysReal *const DDSDDE,\n"
	  << "       ansys::AnsysReal *const SEDEL,\n"
	  << "       ansys::AnsysReal *const SEDPL,\n"
	  << "       ansys::AnsysReal *const EPSEQ,\n"
	  << " const ansys::AnsysReal *const STRAN,\n"
	  << " const ansys::AnsysReal *const DSTRAN,\n"
	  << "       ansys::AnsysReal *const EPSPL,\n"
	  << " const ansys::AnsysReal *const PROPS,\n"
	  << " const ansys::AnsysReal *const coords,\n"
	  << " const ansys::AnsysReal *const DROT,\n"
	  << " const ansys::AnsysReal *const F0,\n"
	  << " const ansys::AnsysReal *const F1,\n"
	  << " const ansys::AnsysReal *const tsstif,\n"
	  << "       ansys::AnsysReal *const EPSZZ,\n"
	  << " const ansys::AnsysReal *const var1,\n"
	  << " const ansys::AnsysReal *const var2,\n"
	  << " const ansys::AnsysReal *const var3,\n"
	  << " const ansys::AnsysReal *const var4,\n"
	  << " const ansys::AnsysReal *const var5,\n"
	  << " const ansys::AnsysReal *const var6,\n"
	  << " const ansys::AnsysReal *const var7,\n"
	  << " const ansys::AnsysReal *const var8)";
    } else {
      out << "(const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << "       ansys::AnsysInt *const keycut,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const,\n"
	  << " const ansys::AnsysInt *const NTENS,\n"
	  << " const ansys::AnsysInt *const NSTATV,\n"
	  << " const ansys::AnsysInt *const NPROPS,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const DTIME,\n"
	  << " const ansys::AnsysReal *const TEMP,\n"
	  << " const ansys::AnsysReal *const DTEMP,\n"
	  << "       ansys::AnsysReal *const STRESS,\n"
	  << "       ansys::AnsysReal *const STATEV,\n"
	  << "       ansys::AnsysReal *const DDSDDE,\n"
	  << "       ansys::AnsysReal *const SEDEL,\n"
	  << "       ansys::AnsysReal *const SEDPL,\n"
	  << "       ansys::AnsysReal *const EPSEQ,\n";
      if(t==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	out << " const ansys::AnsysReal *const STRAN,\n"
	    << " const ansys::AnsysReal *const DSTRAN,\n";
      } else {
	out << " const ansys::AnsysReal *const,\n"
	    << " const ansys::AnsysReal *const,\n";
      }
      out << "       ansys::AnsysReal *const EPSPL,\n"
	  << " const ansys::AnsysReal *const PROPS,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const DROT,\n";
      if((t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)||
	 ((fss!=AnsysInterface::NATIVEFINITESTRAINSTRATEGY)&&
	  (fss!=AnsysInterface::UNDEFINEDSTRATEGY))){
	out << " const ansys::AnsysReal *const F0,\n"
	    << " const ansys::AnsysReal *const F1,\n";
      } else {
	out << " const ansys::AnsysReal *const,\n"
	    << " const ansys::AnsysReal *const,\n";
      }
      out << " const ansys::AnsysReal *const,\n"
	  << "       ansys::AnsysReal *const EPSZZ,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const,\n"
	  << " const ansys::AnsysReal *const)";
    }
  } // end of writeUMATArguments

  static void
  writeUMATArguments(std::ostream& out)
  {
    out << "(const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< "       ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysInt *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< "       ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const,\n"
	<< " const ansys::AnsysReal *const)";
  } // end of writeUMATArguments

  std::string AnsysInterface::getLibraryName(const BehaviourDescription& mb) const
  {
    auto lib = std::string{};
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = this->getInterfaceName()+mb.getMaterialName();
      } else {
	lib = this->getInterfaceName()+"Behaviour";
      }
    } else {
      lib = this->getInterfaceName()+mb.getLibrary();
    }
    return lib;
  } // end of AnsysInterface::getLibraryName

  void AnsysInterface::writeUMATxxAdditionalSymbols(std::ostream&,
						    const std::string&,
						    const Hypothesis,
						    const BehaviourDescription&,
						    const FileDescription&) const
  {} // end of AnsysInterface::writeUMATxxAdditionalSymbols

  void AnsysInterface::writeUMATxxSpecificSymbols(std::ostream&,
						  const std::string&,
						  const BehaviourDescription&,
						  const FileDescription&) const
  {}
  
  unsigned short
  AnsysInterface::getStateVariablesOffset(const BehaviourDescription& mb,
					  const Hypothesis h) const{
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if((h==ModellingHypothesis::AXISYMMETRICAL)||
	 (h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::PLANESTRESS)){
	return 2u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return 6u;
      }
      throw(std::runtime_error("AnsysInterface::getStateVariablesOffset: "
			       "invalid hypothesis"));
    }
    return 0u;
  }
    
  std::string AnsysInterface::getFunctionName(const std::string& name) const
  {
    return name;
  } // end of AnsysInterface::getFunctionName

  std::string AnsysInterface::getFunctionNameForHypothesis(const std::string& name,
								const Hypothesis h) const
  {
    const auto s = [h]() -> std::string {
      if(h==ModellingHypothesis::AXISYMMETRICAL){
	return "axis";
      } else if(h==ModellingHypothesis::PLANESTRAIN){
	return "pstrain";
      } else if(h==ModellingHypothesis::PLANESTRESS){
	return "pstress";
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return "3D";
      }
      throw(std::runtime_error("AnsysInterface::getFunctionNameForHypothesis: "
			       "invalid hypothesis."));
    }();
    return name+"_"+s;
  } // end of AnsysInterface::getFunctionNameForHypothesis
  
  std::set<AnsysInterface::Hypothesis>
  AnsysInterface::getModellingHypothesesToBeTreated(const BehaviourDescription& mb) const
  {
    auto h = std::set<Hypothesis>{};
    const auto& bh = mb.getModellingHypotheses();
    if(bh.find(ModellingHypothesis::AXISYMMETRICAL)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICAL);
    }
    if(bh.find(ModellingHypothesis::PLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::PLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::PLANESTRESS)!=bh.end()){
      h.insert(ModellingHypothesis::PLANESTRESS);
    }
    if(bh.find(ModellingHypothesis::TRIDIMENSIONAL)!=bh.end()){
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    if(h.empty()){
      throw(std::runtime_error("AnsysInterface::getModellingHypothesesToBeTreated : "
			       "no hypotheses selected. This means that the given beahviour "
			       "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
			       "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
			       "make sense to use the Ansys interface"));
    }
    return h;
  } // end of AnsysInterface::getModellingHypothesesToBeTreated

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  AnsysInterface::buildMaterialPropertiesList(const BehaviourDescription& mb,
						   const Hypothesis h) const
  {
    using namespace std;
    auto throw_if = [](const bool c,const std::string& m){
      if(c){throw(std::runtime_error("AnsysInterface:buildMaterialPropertiesList: "+m));}
    };
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      const auto ah = this->getModellingHypothesesToBeTreated(mb);
      std::set<Hypothesis> uh;
      for(const auto &lh : ah){
	if(!mb.hasSpecialisedMechanicalData(lh)){
	  uh.insert(lh);
	}
      }
      throw_if(uh.empty(),"internal error : the mechanical behaviour says that not "
	       "all handled mechanical data are specialised, but we found none.");
      // material properties for all the selected hypothesis
      auto mpositions = vector<pair<vector<UMATMaterialProperty>,
				    SupportedTypes::TypeSize>>{};
      for(const auto & lh : uh){
	mpositions.push_back(this->buildMaterialPropertiesList(mb,lh));
      }
      auto ph=uh.begin();
      auto pum = mpositions.begin();
      const auto& mfirst = *pum;
      ++ph;
      ++pum;
      for(;ph!=uh.end();++ph,++pum){
	const auto& d = mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	const auto& mps = d.getMaterialProperties();
	for(const auto & mp : mps){
	  const auto& mp1 = findUMATMaterialProperty(mfirst.first,
						     mb.getExternalName(h,mp.name));
	  const auto& mp2 = findUMATMaterialProperty(pum->first,
						     mb.getExternalName(h,mp.name));
	  auto o1 = mp1.offset;
	  o1+=pum->second;
	  auto o2 = mp2.offset;
	  o2+=mfirst.second;
	  throw_if(o1!=o2,"incompatible offset for material property '"+mp.name+
		   "' (aka '"+mp1.name+"'). This is one pitfall of the umat interface. "
		   "To by-pass this limitation, you may want to explicitely "
		   "specialise some modelling hypotheses");
	}
      }
      return mfirst;
    }
    auto res = pair<vector<UMATMaterialProperty>,
		    SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    throw_if((h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
	     (h!=ModellingHypothesis::AXISYMMETRICAL)&&
	     (h!=ModellingHypothesis::PLANESTRAIN)&&
	     (h!=ModellingHypothesis::PLANESTRESS)&&
	     (h!=ModellingHypothesis::TRIDIMENSIONAL),
	     std::string("unsupported modelling hypothesis ")+
	     ((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS) ? "(default)" : 
	      "'"+ModellingHypothesis::toString(h)+"'"));
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus","youn",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio","nu",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio12","nu12",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio23","nu23",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio13","nu13",false);
	this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	if (h==ModellingHypothesis::TRIDIMENSIONAL){
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus23","g23",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus13","g13",false);
	} else if((h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)&&
		  (h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
		  (h!=ModellingHypothesis::AXISYMMETRICAL)&&
		  (h!=ModellingHypothesis::PLANESTRAIN)&&
		  (h!=ModellingHypothesis::PLANESTRESS)){
	  throw_if(true,"unsupported modelling hypothesis");
	}
      } else {
	throw_if(true,"unsupported behaviour symmetry type");
      }
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion",
					     "alph",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1",
					     "alp1",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2",
					     "alp2",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3",
					     "alp3",false);
      } else {
	throw(std::runtime_error("AnsysInterface::buildMaterialPropertiesList : "
				 "unsupported behaviour symmetry type"));
      }
    }
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if((h==ModellingHypothesis::AXISYMMETRICAL)||
	 (h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::PLANESTRESS)){
	this->appendToMaterialPropertiesList(mprops,"real","FirstOrthotropicAxis_1","v1x",false);
	this->appendToMaterialPropertiesList(mprops,"real","FirstOrthotropicAxis_2","v1y",false);
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	this->appendToMaterialPropertiesList(mprops,"real","FirstOrthotropicAxis_1","v1x",false);
	this->appendToMaterialPropertiesList(mprops,"real","FirstOrthotropicAxis_2","v1y",false);
	this->appendToMaterialPropertiesList(mprops,"real","FirstOrthotropicAxis_2","v1z",false);
	this->appendToMaterialPropertiesList(mprops,"real","SecondOrthotropicAxis_1","v2x",false);
	this->appendToMaterialPropertiesList(mprops,"real","SecondOrthotropicAxis_2","v2y",false);
	this->appendToMaterialPropertiesList(mprops,"real","SecondOrthotropicAxis_2","v2z",false);
      } else {
	throw_if(true,"unsupported modellinh hypothesis");
      }
    }
    if(!mprops.empty()){
      const auto& m = mprops.back();
      res.second  = m.offset;
      res.second += SupportedTypes::getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of AnsysInterface::buildMaterialPropertiesList

  void AnsysInterface::writeAnsysBehaviourTraits(std::ostream& out,
						 const BehaviourDescription& mb,
						 const Hypothesis h) const
  {
    using namespace std;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb,h);
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    } else {
      out << "template<typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    }
    out << ">\n";
    out << "struct AnsysTraits<tfel::material::" << mb.getClassName() << "<";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "H";
    } else {
      out << "tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h);
    }
    out << ",Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >\n{\n";
    out << "//! behaviour type\n";
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " AnsysBehaviourType btype = ansys::SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " AnsysBehaviourType btype = ansys::FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else {
      throw(std::runtime_error("AnsysInterface::writeAnsysBehaviourTraits : "
			       "unsupported behaviour type"));
    }
    out << "//! space dimension\n";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "static " << constexpr_c << " unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    } else {
      out << "static " << constexpr_c << " unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<"
	  << "tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h)
	  << ">::value;\n";
    }
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c << " unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c << " unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array\n";
    out << "static " << constexpr_c << " unsigned short DrivingVariableSize = " << mvs.first <<  ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c << " unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";\n";
    if(mb.getAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,false)){
      out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " AnsysSymmetryType type = ansys::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " AnsysSymmetryType type = ansys::ORTHOTROPIC;\n";
    } else {
      throw(std::runtime_error("AnsysInterface::writeAnsysBehaviourTraits: "
			       "unsupported behaviour type.\n"
			       "The ansys interface only support isotropic or orthotropic "
			       "behaviour at this time."));
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if(!mprops.first.empty()){
      const auto& m = mprops.first.back();
      msize  = m.offset;
      msize += SupportedTypes::getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c << " unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " AnsysSymmetryType etype = ansys::ISOTROPIC;\n";
      if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 1u;\n"; 
      } else {
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
      out << "static " << constexpr_c << " unsigned short orthotropicAxesOffset = 0u;\n"; 
    } else if (mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " AnsysSymmetryType etype = ansys::ORTHOTROPIC;\n";
      if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
    	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset "
    	    << "= AnsysOrthotropicElasticPropertiesOffset<N>::value;\n";
      } else {
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 3u;\n"; 
      } else {
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
      out << "static " << constexpr_c << " unsigned short orthotropicAxesOffset = AnsysOrthotropicAxesOffset<N>::value;\n"; 
    } else {
      throw(std::runtime_error("AnsysInterface::writeAnsysBehaviourTraits: "
			       "unsupported behaviour type.\n"
			       "The ansys interface only support isotropic or "
			       "orthotropic behaviour at this time."));
    }
    out << "}; // end of class AnsysTraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis,std::string>
  AnsysInterface::gatherModellingHypothesesAndTests(const BehaviourDescription& mb) const
  {
    auto res = std::map<Hypothesis,std::string>{};
    if((mb.getSymmetryType()==mfront::ORTHOTROPIC)&&
       ((mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false))||
	(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)))){
      auto h = this->getModellingHypothesesToBeTreated(mb);
      for(const auto & mh : h){
	res.insert({mh,this->getModellingHypothesisTest(mh)});
      }
      return res;
    }
    return UMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  } // end of AnsysInterface::gatherModellingHypothesesAndTests

  std::string
  AnsysInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return "*NTENS==4";
    } else if(h==ModellingHypothesis::PLANESTRESS){
      return "*NTENS==3";
    } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "*NTENS==6";
    }
    throw(std::runtime_error("AnsysInterface::getModellingHypothesisTest : "
			     "unsupported modelling hypothesis"));
  } // end of AnsysInterface::gatherModellingHypothesesAndTests
  
  void
  AnsysInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "ModellingHypothesis::Hypothesis h;\n"
	<< "if( *NTENS == 6 ){\n"
	<< "  h = ModellingHypothesis::TRIDIMENSIONAL;\n"
	<< "} else if(*NTENS==3){\n"
	<< "  h = ModellingHypothesis::PLANESTRESS;\n"
	<< "} else if(*NTENS==4){\n"
	<< "  h = ModellingHypothesis::GENERALISEDPLANESTRAIN;\n"
	<< "} else {\n"
	<< "  return;\n"
	<< "}\n"
	<< "mg.setModellingHypothesis(h);\n";
  } // end of AnsysInterface::writeMTestFileGeneratorSetModellingHypothesis

  void
  AnsysInterface::writeInputFileExample(const BehaviourDescription& mb,
					const FileDescription& fd) const{ 
    auto throw_if = [](const bool c,const std::string& m){
      if(c){throw(std::runtime_error("AnsysInterface::writeInputFileExample: "+m));}
    };
    const auto name =  mb.getLibrary()+mb.getClassName();
    const auto mn = this->getLibraryName(mb)+"_"+mb.getClassName();
    const auto fn = "ansys/"+name+".dat";
    std::ofstream out{fn};
    throw_if(!out,"could not open file '"+fn+"'");
    // header
    out << "/comm, \n"
    	<< "/comm, File generated by MFront from the "  << fd.fileName << " source\n"
    	<< "/comm, Example of how to use the " << mb.getClassName() << " behaviour law\n"
    	<< "/comm, Author "  << fd.authorName << '\n'
    	<< "/comm, Date   "  << fd.date       << '\n'
    	<< "/comm,\n\n";
    // loop over hypothesis
    for(const auto & h : this->getModellingHypothesesToBeTreated(mb)){
      const auto& d = mb.getBehaviourData(h);
      const auto mps = this->buildMaterialPropertiesList(mb,h);
      // material properties
      auto msize = SupportedTypes::TypeSize{};
      if(!mps.first.empty()){
    	const auto& m = mps.first.back();
    	msize  = m.offset;
    	msize += SupportedTypes::getTypeSize(m.type,m.arraySize);
      }
      // internal state variables
      const auto& persistentVarsHolder = d.getPersistentVariables();
      auto vs = SupportedTypes::TypeSize{};
      for(const auto& v : persistentVarsHolder){
    	vs+=SupportedTypes::getTypeSize(v.type,v.arraySize);
      }
      const auto vsize = vs.getValueForModellingHypothesis(h)+ 
    	this->getStateVariablesOffset(mb,h);
      // external state variables
      const auto& externalStateVarsHolder = d.getExternalStateVariables();
      throw_if(externalStateVarsHolder.size()!=1u,
	       "The ansys interface does not support external state  variables");
      // user material declaration
      out << "/com, Example for the '" << ModellingHypothesis::toString(h) << "' modelling hypothesis\n";
      out << "/com, List of material properties\n";
      for(const auto& mp: mps.first){
	out << "/com, -" << mp.name << "\n";
      }
      out << "tb,user,<mat_id>,<numer of temperatures>,"
	  << msize.getValueForModellingHypothesis(h) << "\n"
	  << "/com, you shall now declare your material properties\n"
	  << "/com, using the tbtemp an tbdata commands.\n"
	  << "/com, See the \"ANSYS USER Material Subroutine\" "
	  << "guide for details.\n";
      // number of state variables
      out << "/com, Declaration of state variables\n"
	  << "tb,state,<mat_id>,," << vsize << "\n"
	  << "\n\n";
    }
  } // end of AnsysInterface::writeInputFileExample
  
  std::string AnsysInterface::getName()
  {
    return "ansys";
  }
  
  std::string AnsysInterface::getInterfaceName() const
  {
    return "Ansys";
  } // end of AnsysInterface::getInterfaceName

  std::pair<bool,AnsysInterface::tokens_iterator>
  AnsysInterface::treatKeyword(BehaviourDescription&,
				const std::string& key,
				const std::vector<std::string>& i,
				tokens_iterator current,
				const tokens_iterator end)
  {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AnsysInterface::treatKeyword: "+m));}
    };
    if(!i.empty()){
      if(std::find(i.begin(),i.end(),this->getName())!=i.end()){
	auto keys = {"@AnsysFiniteStrainStrategy",
		     "@AnsysGenerateMTestFileOnFailure",
		     "@AnsysCompareToNumericalTangentOperator",
		     "@AnsysTangentOperatorComparisonCriterium",
		     "@AnsysTangentOperatorComparisonCriterion",
		     "@AnsysStrainPerturbationValue"};
      throw_if(std::find(keys.begin(),keys.end(),key)==keys.end(),
		 "unsupported key '"+key+"'");
      } else {
	return {false,current};
      }
    }
    if (key=="@AnsysFiniteStrainStrategy"){
      auto read = [&throw_if](const std::string& s){
	if(s=="Native"){
	  return NATIVEFINITESTRAINSTRATEGY;
	} else if(s=="FiniteRotationSmallStrain"){
	  return FINITEROTATIONSMALLSTRAIN;
	} else if(s=="MieheApelLambrechtLogarithmicStrain"){
	  return MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN;
	}
	throw_if(true,"unsupported strategy '"+s+"'\n"
		 "The only supported strategies are 'Native',"
		 "'FiniteRotationSmallStrain' and "
		 "'MieheApelLambrechtLogarithmicStrain'");
      };
      throw_if(this->fss!=UNDEFINEDSTRATEGY,
	       "a finite strain strategy has already been defined");
      throw_if(current==end,"unexpected end of file");
      this->fss = read(current->value);
      throw_if(++current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+'\'');
      ++(current);
      return {true,current};
    }
    if (key=="@AnsysGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return {true,current};      
    } else if(key=="@AnsysCompareToNumericalTangentOperator"){
      this->compareToNumericalTangentOperator  = this->readBooleanValue(key,current,end);
      return make_pair(true,current);
    } else if ((key=="@AnsysTangentOperatorComparisonCriterium")||
	       (key=="@AnsysTangentOperatorComparisonCriterion")){
      throw_if(!this->compareToNumericalTangentOperator,
	       "comparison to tangent operator is not enabled at this stage.\n"
	       "Use the @AnsysCompareToNumericalTangentOperator directive before "
	       "@AnsysTangentOperatorComparisonCriterion");
      throw_if(current==end,"unexpected end of file");
      this->tangentOperatorComparisonCriterion = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    } else if (key=="@AnsysStrainPerturbationValue"){
      throw_if(!this->compareToNumericalTangentOperator,
	       "time stepping is not enabled at this stage.\n"
	       "Use the @AnsysUseTimeSubStepping directive before "
	       "@AnsysStrainPerturbationValue");
      throw_if(current==end,"unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    }
    return {false,current};
  } // end of treatKeyword

  void AnsysInterface::endTreatment(const BehaviourDescription& mb,
				    const FileDescription& fd) const
  {
    using namespace tfel::system;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AnsysInterface::endTreatment: "+m));}
    };
    throw_if((mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	     (this->fss!=UNDEFINEDSTRATEGY),
	     "finite strain strategy is only supported for small strain behaviours");
    throw_if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)),
	     "the ansys interface only supports small and "
	     "finite strain behaviours");
    if(this->compareToNumericalTangentOperator){
      throw_if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
	       "unsupported feature @AnsysSaveTangentOperator "
	       "and @AnsysCompareToNumericalTangentOperator : "
	       "those are only valid for small strain beahviours");
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    for(const auto h : mh){
      const auto& d = mb.getBehaviourData(h);
      throw_if(d.getExternalStateVariables().size()!=1u,
	       "external state variables are not supported "
	       "by ANSYS's usermat interface");
    }
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Ansys");
    systemCall::mkdir("ansys");

    std::ofstream out;
    {
      // copy usermat.cpp locally
      MFrontLockGuard lock;
      out.open("ansys/usermat.cpp");
      if(out){
	const auto root = tfel::getInstallPath();
	const auto fn = root+"/share/doc/mfront/ansys/usermat.cpp";
	std::ifstream in{fn};
	if(in){
	  out << in.rdbuf();
	  in.close();
	} else {
	  std::cerr << "AnsysInterface::endTreatment: "
		    << "could not open file '" << fn << "'" << std::endl;
	}
      } else {
	  std::cerr << "AnsysInterface::endTreatment: "
		    << "could not open file 'ansys/usermat.cpp'" << std::endl;
      }
      out.close();
    }
    
    // header
    auto fname = "ansys"+name+".hxx";
    out.open("include/MFront/Ansys/"+fname);
    throw_if(!out,"could not open file '"+fname+"'");
    
    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file declares the ansys interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    const auto header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << "\n"
	<< "#define "<< header << "\n\n"
	<< "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
	<< "#include\"MFront/Ansys/Ansys.hxx\"\n"
	<< "#include\"MFront/Ansys/AnsysData.hxx\"\n\n";
    out << "#ifdef __cplusplus\n"
	<< "#include\"MFront/Ansys/AnsysTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Ansys/AnsysOrthotropicBehaviour.hxx\"\n";
    }
    if(this->fss!=UNDEFINEDSTRATEGY){
      out << "#include\"MFront/Ansys/AnsysFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
	<< "namespace ansys{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(mh)){
      this->writeAnsysBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->writeAnsysBehaviourTraits(out,mb,h);
      }
    }

    out << "} // end of namespace ansys\n\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#ifdef __cplusplus\n"
	<< "extern \"C\"{\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    this->writeSetParametersFunctionsDeclarations(out,name,mb);

    for(const auto h: mh){
      out << "MFRONT_SHAREDOBJ void\n"
	  << this->getFunctionNameForHypothesis(name,h);
      writeUMATArguments(out);
      out << ";\n\n";
    }

    out << "#ifdef __cplusplus\n"
	<< "}\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#endif /* " << header << " */\n";

    out.close();

    fname  = "ansys"+name+".cxx";
    out.open("src/"+fname);
    throw_if(!out,"could not open file '"+fname+"'");

    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file implements the ansys interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    this->getExtraSrcIncludes(out,mb);

    if(this->compareToNumericalTangentOperator){
      out << "#include<cmath>\n"
	  << "#include<vector>\n"
	  << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
	<< "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Ansys/AnsysRotation.hxx\"\n\n";
    }
    out << "#include\"MFront/Ansys/AnsysStressFreeExpansionHandler.hxx\"\n\n"
	<< "#include\"MFront/Ansys/AnsysInterface.hxx\"\n\n"
	<< "#include\"MFront/Ansys/ansys" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
    
    out << "extern \"C\"{\n\n";
 
    this->generateUMATxxGeneralSymbols(out,name,mb,fd);
    if(!mb.areAllMechanicalDataSpecialised(mh)){
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out,name,uh,mb,fd);
    }
    for(const auto &h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->generateUMATxxSymbols(out,name,h,mb,fd);
      }
    }
    
    this->writeSetParametersFunctionsImplementations(out,name,mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);

    for(const auto h: mh){
      if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	 ((this->fss==UNDEFINEDSTRATEGY)||(this->fss==NATIVEFINITESTRAINSTRATEGY))){
	this->writeUMATSmallStrainFunction(out,mb,name,h);
      } else if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
		((this->fss!=UNDEFINEDSTRATEGY)&&(this->fss!=NATIVEFINITESTRAINSTRATEGY))){
	if(this->fss==FINITEROTATIONSMALLSTRAIN){
	  this->writeUMATFiniteRotationSmallStrainFunction(out,mb,name,h);
	} else {
	  throw_if(true,"unsupported finite strain strategy !");
	}
      } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	this->writeUMATFiniteStrainFunction(out,mb,name,h);
      } else {
	throw_if(true,"the ansys interface only supports small "
		 "and finite strain behaviours");
      }
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb,fd);
  } // end of AnsysInterface::endTreatment
  
  void AnsysInterface::writeUMATFunctionBase(std::ostream& out,
					 const BehaviourDescription& mb,
					 const std::string& name,
					 const std::string& sfeh,
					 const Hypothesis h) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AnsysInterface::writeUMATFunctionBase: "+m));}
    };
    std::string dv0,dv1,sig;
    const auto btype = mb.getBehaviourType();
    out << "static void\n" << name << "_base" << this->getFunctionNameForHypothesis("",h);
    writeUMATArguments(out,btype,this->fss,false);
    out << "{\n";
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      const auto mpoffset = [this,h,&mb](){
	auto o = size_t{};
	if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	  if((h==ModellingHypothesis::PLANESTRESS)||
	     (h==ModellingHypothesis::AXISYMMETRICAL)||
	     (h==ModellingHypothesis::PLANESTRAIN)){
	    o+=ansys::AnsysOrthotropicElasticPropertiesOffset<2u>::value;
	  } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	    o+=ansys::AnsysOrthotropicElasticPropertiesOffset<3u>::value;
	  }
	}
	if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	  if((h==ModellingHypothesis::PLANESTRESS)||
	     (h==ModellingHypothesis::AXISYMMETRICAL)||
	     (h==ModellingHypothesis::PLANESTRAIN)){
	    o+=ansys::AnsysOrthotropicElasticPropertiesOffset<2u>::value;
	  } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	    o+=3u;
	  }
	}
	return o;
      }();
      const auto smpoffset = std::to_string(mpoffset);
      if(btype==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
    	// turning the deformation and the deformation gradient
    	// increment to the material frame
    	if ((h==ModellingHypothesis::PLANESTRESS)||
	    (h==ModellingHypothesis::AXISYMMETRICAL)||
	    (h==ModellingHypothesis::PLANESTRAIN)){
	  out << "if(*NPROPS<"+smpoffset+"+2u){\n"
	      << "std::cerr << \"" << name << this->getFunctionNameForHypothesis("",h) << ": \"\n"
	      << "          << \"invalid number of material properties\\n\";\n"
	      << "*keycut = 1;\n"
	      << "return;\n"
	      << "}\n"
	      << "ansys::AnsysRotation2D<ansys::AnsysReal> R(PROPS+"+smpoffset+");\n"
    	      << "ansys::AnsysReal e[4u];\n"
    	      << "ansys::AnsysReal de[4u];\n"
    	      << "ansys::AnsysReal sm[4u];\n"
    	      << "R.rotateStrainsForward(STRAN,e);\n"
    	      << "R.rotateStrainsForward(DSTRAN,de);\n"
    	      << "R.rotateStressesForward(STRESS,sm);\n";
    	} else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	  out << "if(*NPROPS<"+smpoffset+"+6u){\n"
	      << "std::cerr << \"" << name << this->getFunctionNameForHypothesis("",h) << ": \"\n"
	      << "          << \"invalid number of state variables\\n\";\n"
	      << "*keycut = 1;\n"
	      << "return;\n"
	      << "}\n"
	      << "ansys::AnsysRotation3D<ansys::AnsysReal> R(PROPS+"+smpoffset+");\n"
    	      << "ansys::AnsysReal e[6u];\n"
    	      << "ansys::AnsysReal de[6u];\n"
    	      << "ansys::AnsysReal sm[6u];\n"
    	      << "R.rotateStrainsForward(STRAN,e);\n"
    	      << "R.rotateStrainsForward(DSTRAN,de);\n"
    	      << "R.rotateStressesForward(STRESS,sm);\n";
    	} else {
    	  throw_if(true,"unsupported hypothesis");
    	}
    	dv0 = "e";
    	dv1 = "de";
    	sig = "sm";
      } else if(btype==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
    	// turning the deformation gradients to the material frame
    	if ((h==ModellingHypothesis::PLANESTRESS)||
	    (h==ModellingHypothesis::AXISYMMETRICAL)||
	    (h==ModellingHypothesis::PLANESTRAIN)){
	  out << "if(*NPROPS<"+smpoffset+"+2u){\n"
	      << "std::cerr << \"" << name << this->getFunctionNameForHypothesis("",h) << ": \"\n"
	      << "          << \"invalid number of material properties\\n\";\n"
	      << "*keycut = 1;\n"
	      << "return;\n"
	      << "}\n"
	      << "ansys::AnsysRotation2D<ansys::AnsysReal> R(PROPS+"+smpoffset+");\n"
    	      << "ansys::AnsysReal Fm0[9u];\n"
	      << "ansys::AnsysReal Fm1[9u];\n"
    	      << "ansys::AnsysReal sm[4u];\n"
    	      << "R.rotateDeformationGradientForward(F0,Fm0);\n"
    	      << "R.rotateDeformationGradientForward(F1,Fm1);\n"
     	      << "R.rotateStressesForward(STRESS,sm);\n";
    	} else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	  out << "if(*NPROPS<"+smpoffset+"+6u){\n"
	      << "std::cerr << \"" << name << this->getFunctionNameForHypothesis("",h) << ": \"\n"
	      << "          << \"invalid number of state variables\\n\";\n"
	      << "*keycut = 1;\n"
	      << "return;\n"
	      << "}\n"
	      << "ansys::AnsysRotation3D<ansys::AnsysReal> R(PROPS+"+smpoffset+");\n"
    	      << "ansys::AnsysReal Fm0[9u];\n"
	      << "ansys::AnsysReal Fm1[9u];\n"
     	      << "ansys::AnsysReal sm[6u];\n"
    	      << "R.rotateDeformationGradientForward(F0,Fm0);\n"
    	      << "R.rotateDeformationGradientForward(F1,Fm1);\n"
     	      << "R.rotateStressesForward(STRESS,sm);\n";
    	} else {
    	  throw_if(true,"unsupported hypothesis");
    	}
	dv0 = "Fm0";
	dv1 = "Fm1";
	sig = "sm";
      } else {
    	throw_if(true,"the ansys interface only supports small "
    		 "and finite strain behaviours");
      }
    } else {
      if(btype==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	dv0 = "STRAN";
	dv1 = "DSTRAN";
      } else if(btype==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	dv0 = "F0";
	dv1 = "F1";
      } else {
    	throw_if(true,"the ansys interface only supports small "
    		 "and finite strain behaviours");
      }
      sig = "STRESS";
    }
    out << "ansys::AnsysData d = {" << sig << ",DDSDDE,STATEV,\n"
	<< "                      SEDEL,SEDPL,EPSEQ,EPSPL,EPSZZ,\n"
	<< "                      *NTENS,*NPROPS,*NSTATV,*DTIME,DROT,\n"
	<< "                      " << dv0 << "," << dv1 << ",TEMP,DTEMP,PROPS,"
	<< getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	<< "if(ansys::AnsysInterface<tfel::material::ModellingHypothesis::"
	<< ModellingHypothesis::toUpperCaseString(h) << ",tfel::material::" << mb.getClassName() 
	<< ">::exe(d)!=0){\n"
	<< "*keycut = 1;\n"
	<< "return;\n"
	<< "}\n";
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if(h==ModellingHypothesis::PLANESTRESS){
	out << "ansys::AnsysReal D[16u] = {DDSDDE[0],DDSDDE[3],0,DDSDDE[6],\n"
	    << "                           DDSDDE[1],DDSDDE[4],0,DDSDDE[7],\n"
	    << "                           0,0,0,0,\n"
	    << "                           DDSDDE[2],DDSDDE[5],0,DDSDDE[8]};\n"
	    << "R.rotateStressesBackward(sm,STRESS);\n"
	    << "R.rotateTangentOperatorBackward(D);\n"
	    << "DDSDDE[0]=D[0];\n"
	    << "DDSDDE[1]=D[4];\n"
	    << "DDSDDE[2]=D[12];\n"
	    << "DDSDDE[3]=D[1];\n"
	    << "DDSDDE[4]=D[5];\n"
	    << "DDSDDE[5]=D[13];\n"
	    << "DDSDDE[6]=D[3];\n"
	    << "DDSDDE[7]=D[7];\n"
	    << "DDSDDE[8]=D[15];\n";
      } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
		 (h==ModellingHypothesis::PLANESTRAIN)||
		 (h==ModellingHypothesis::TRIDIMENSIONAL)){
	out << "R.rotateStressesBackward(sm,STRESS);\n"
	    << "R.rotateTangentOperatorBackward(DDSDDE);\n";
      } else {
	throw_if(true,"unsupported hypothesis");
      }
    }
    if(getDebugMode()){
      out << "std::cout << \"Dt :\" << std::endl;\n"
	  << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
	  << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
	  << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n"
	  << "std::cout << std::endl;\n";
    }
    out << "}\n\n";
  } // end of AnsysInterface::writeUMATFunctionBase

  void AnsysInterface::writeUMATFiniteStrainFunction(std::ostream& out,
						     const BehaviourDescription& mb,
						     const std::string& name,
						     const Hypothesis h) const
  {
    const std::string sfeh = "nullptr";
    this->writeUMATFunctionBase(out,mb,name,sfeh,h);
    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionNameForHypothesis(name,h);
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,this->fss,true);
    out << "{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("",h)
	<< "(matId,elemId,kDomIntPt,kLayer,kSectPt,ldsetp,isubst,keycut,\n"
	<< " nDirect,nShear,NTENS,NSTATV,NPROPS,TIME,DTIME,TEMP,DTEMP,\n"
	<< " STRESS,STATEV,DDSDDE,SEDEL,SEDPL,EPSEQ,STRAN,DSTRAN,\n"
	<< " EPSPL,PROPS,coords,DROT,F0,F1,tsstif,EPSZZ,\n"
	<< " var1,var2,var3,var4,var5,var6,var7,var8);\n"
	<< "}\n\n";
  }
  
  void AnsysInterface::writeUMATSmallStrainFunction(std::ostream& out,
						     const BehaviourDescription& mb,
						     const std::string& name,
						     const Hypothesis h) const
  {
    const std::string sfeh = "ansys::AnsysStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out,mb,name,sfeh,h);
    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionNameForHypothesis(name,h);
    writeUMATArguments(out,BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,this->fss,true);
    out << "{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "std::vector<ansys::AnsysReal> deto0(*NTENS);\n"
	  << "std::vector<ansys::AnsysReal> sig0(*NTENS);\n"
	  << "std::vector<ansys::AnsysReal> sv0(*NSTATV);\n"
	  << "std::copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n"
	  << "std::copy(STRESS,STRESS+*(NTENS),sig0.begin());\n"
	  << "std::copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if(this->generateMTestFile){
      this->generateMTestFile1(out);
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("",h)
	<< "(matId,elemId,kDomIntPt,kLayer,kSectPt,ldsetp,isubst,keycut,\n"
	<< " nDirect,nShear,NTENS,NSTATV,NPROPS,TIME,DTIME,TEMP,DTEMP,\n"
	<< " STRESS,STATEV,DDSDDE,SEDEL,SEDPL,EPSEQ,STRAN,DSTRAN,\n"
	<< " EPSPL,PROPS,coords,DROT,F0,F1,tsstif,EPSZZ,\n"
	<< " var1,var2,var3,var4,var5,var6,var7,var8);\n";
    if(this->generateMTestFile){
      out << "if(*keycut!=0){\n";
      this->generateMTestFile2(out,mb.getBehaviourType(),
			       name,"",mb);
      out << "}\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "// computing the tangent operator by pertubation\n"
	  << "std::vector<ansys::AnsysReal> nD((*NTENS)*(*NTENS));\n"
	  << "std::vector<ansys::AnsysReal> deto(*NTENS);\n"
	  << "std::vector<ansys::AnsysReal> sigf(*NTENS);\n"
	  << "std::vector<ansys::AnsysReal> sigb(*NTENS);\n"
	  << "std::vector<ansys::AnsysReal> sv(*NSTATV);\n"
	  << "std::vector<ansys::AnsysReal> D((*NTENS)*(*NTENS));\n"
	  << "ansys::AnsysReal keycut0 = 0;\n"
	  << "ansys::AnsysReal m;\n"
	  << "ansys::AnsysReal mDt;\n"
	  << "ansys::AnsysReal mnDt;\n"
	  << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
	  << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "std::copy(sig0.begin(),sig0.end(),sigf.begin());\n"
	  << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] += " << this->strainPerturbationValue << ";\n"
	  << "ansys::AnsysData d2 = {&sigf[0],&D[0],&sv[0],\n"
	  << "                         SEDEL,SEDPL,EPSEQ,EPSPL,EPSZZ,\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,DROT,\n"
	  << "                         STRAN,&deto[0],TEMP,DTEMP,PROPS,"
	  << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	  << "if(ansys::AnsysInterface<tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h) << ","
	  << "tfel::material::" << mb.getClassName() << ">::exe(d2)!=0){\n"
	  << "return;\n"
	  << "}\n"
	  << "ansys::AnsysReal keycut0 = 0;\n"
	  << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "std::copy(sig0.begin(),sig0.end(),sigb.begin());\n"
	  << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] -= " << this->strainPerturbationValue << ";\n"
	  << "ansys::AnsysData d3 = {&sigf[0],&D[0],&sv[0],\n"
	  << "                         SEDEL,SEDPL,EPSEQ,EPSPL,EPSZZ,\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,DROT,\n"
	  << "                         STRAN,&deto[0],TEMP,DTEMP,PROPS,"
	  << "if(ansys::AnsysInterface<tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h) << ","
	  << "tfel::material::" << mb.getClassName() << ">::exe(d3)!=0){\n"
	  << "return;\n"
	  << "}\n"
	  << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
	  << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*" << this->strainPerturbationValue << ");\n"
	  << "}\n"
	  << "}\n"
	  << "// comparison\n"
	  << "m=0.;\n"
	  << "mDt=0.;\n"
	  << "mnDt=0.;\n"
	  << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n"
	  << "mDt=std::max(mDt,*(DDSDDE+i));\n"
	  << "mnDt=std::max(mnDt,nD[i]);\n"
	  << "m=std::max(m,std::abs(nD[i]-*(DDSDDE+i)));\n"
	  << "}\n"
	  << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n"
	  << "std::cout << \"||nDt-Dt|| = \" << m << \" (\" << 100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< std::endl;\n"
	  << "std::cout << \"Dt :\" << std::endl;\n"
	  << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
	  << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
	  << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n"
	  << "std::cout << \"nDt :\" << std::endl;\n"
	  << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
	  << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
	  << "std::cout << nD[j*(*NTENS)+i] << \" \";\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n";
    }
    if(this->fss==NATIVEFINITESTRAINSTRATEGY){
	out << "ansys::AnsysFiniteStrain::applyNativeFiniteStrainCorrection(DDSDDE,DFGRD1,STRESS,*NTENS);\n";
    }
    out << "}\n\n";
  }
  
  void
  AnsysInterface::writeUMATFiniteRotationSmallStrainFunction(std::ostream& out,
							      const BehaviourDescription& mb,
							      const std::string& name,
							      const Hypothesis h) const
  {
    if(h==ModellingHypothesis::PLANESTRESS){
      throw(std::runtime_error("AnsysInterface::writeUMATFiniteRotationSmallStrainFunction: "
			       "plane stress is not supported yet"));
    }
    const auto ps = h==ModellingHypothesis::PLANESTRESS ? "true" : "false";
    const std::string sfeh = "ansys::AnsysStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out,mb,name,sfeh,h);
    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionNameForHypothesis(name,h);
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,this->fss,true);
    out << "{\n"
	<< "using namespace ansys;\n"
	<< "AnsysReal eto[6];\n"
      	<< "AnsysReal deto[6];\n"
	<< "AnsysReal CSE[36];\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
      out << "{\n"
	  << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    
    out << "static_cast<void>(STRAN);\n"
	<< "static_cast<void>(DSTRAN);\n"
	<< "AnsysFiniteStrain::computeGreenLagrangeStrain(eto,DFGRD0,*NTENS," << ps << ");\n"
	<< "AnsysFiniteStrain::computeGreenLagrangeStrain(deto,DFGRD1,*NTENS," << ps << ");\n"
	<< "AnsysFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(STRESS,DFGRD0,*NTENS," << ps << ",0);\n"
	<< "for(int i=0;i!=*NTENS;++i){\n"
	<< "deto[i] -= eto[i];\n"
	<< "}\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "}\n";
    }
    out	<< name << "_base" << this->getFunctionNameForHypothesis("",h)
	<< "(matId,elemId,kDomIntPt,kLayer,kSectPt,ldsetp,isubst,keycut,\n"
	<< " nDirect,nShear,NTENS,NSTATV,NPROPS,TIME,DTIME,TEMP,DTEMP,\n"
	<< " STRESS,STATEV,DDSDDE,SEDEL,SEDPL,EPSEQ,STRAN,DSTRAN,\n"
	<< " EPSPL,PROPS,coords,DROT,F0,F1,tsstif,EPSZZ,\n"
	<< " var1,var2,var3,var4,var5,var6,var7,var8);\n"
	<< "if(*keycut!=0){\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "BehaviourProfiler::Timer post_timer("
	  << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "AnsysFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,DFGRD1,*NTENS," << ps << ",0);\n";
    out << "AnsysFiniteStrain::computeAnsysTangentOperatorFromCSE(DDSDDE,CSE,DFGRD1,STRESS,*NTENS," << ps << ");\n";
    out << "}\n";
    out << "}\n\n";
  }
  
  void
  AnsysInterface::writeUMATxxBehaviourTypeSymbols(std::ostream& out,
						   const std::string& name,
						   const BehaviourDescription& mb) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AnsysInterface::writeUMATxxBehaviourTypeSymbols: "+m));}
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourType = " ;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->fss==UNDEFINEDSTRATEGY){
	out << "1u;\n\n";
      } else {
	out << "2u;\n\n";
      }
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "2u;\n\n";
    } else {
      throw_if(true,"unsupported behaviour type");
    }
  } // end of AnsysInterface::writeUMATxxBehaviourTypeSymbols

  void
  AnsysInterface::writeUMATxxBehaviourKinematicSymbols(std::ostream& out,
						   const std::string& name,
						   const BehaviourDescription& mb) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AnsysInterface::writeUMATxxBehaviourKinematicSymbols: "+m));}
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourKinematic = " ;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->fss==UNDEFINEDSTRATEGY){
	out << "1u;\n\n";
      } else {
	out << "3u;\n\n";
      }
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "3u;\n\n";
    } else {
      throw_if(true,"unsupported behaviour type");
    }
  } // end of AnsysInterface::writeUMATxxBehaviourKinematicSymbols
  
  void 
  AnsysInterface::writeInterfaceSpecificIncludes(std::ostream& out,
						  const BehaviourDescription&) const
  {
    out << "#include\"MFront/Ansys/Ansys.hxx\"\n"
	<< "#include\"MFront/Ansys/AnsysConvert.hxx\"\n\n";
  } // end of AnsysInterface::writeInterfaceSpecificIncludes

  std::vector<std::pair<std::string,std::string>>
  AnsysInterface::getBehaviourDataConstructorAdditionalVariables() const{
    return {{"DR","increment of rigid body rotation"}};
  } // end of AnsysInterface::getBehaviourDataConstructorAdditionalVariables

  void 
  AnsysInterface::writeBehaviourDataMainVariablesSetters(std::ostream& os,
							  const BehaviourDescription& mb) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov,of;
    os << "void set"
       << iprefix << "BehaviourDataDrivingVariables(const Type* const " << iprefix << "stran)\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataDrivingVariableSetter(os,v.first,ov);
      ov += SupportedTypes::getTypeSize(v.first.type,1u);
    }
    os << "}\n\n";
    os << "void set"
       << iprefix << "BehaviourDataThermodynamicForces(const Type* const " << iprefix << "stress_,\n"
       << "                                                        const Type* const )\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataThermodynamicForceSetter(os,v.second,of);
      of += SupportedTypes::getTypeSize(v.second.type,1u);
    }
    os << "}\n\n";
  } // end of AnsysInterface::writeBehaviourDataMainVariablesSetters

  void 
  AnsysInterface::writeBehaviourDataDrivingVariableSetter(std::ostream& os,
							   const DrivingVariable& v,
							   const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(!o.isNull()){
      throw(std::runtime_error("AnsysInterface::writeBehaviourDataMainVariablesSetter : "
			       "only one driving variable supported"));
    }
    if(v.increment_known){
      os << "ansys::ImportDrivingVariables<hypothesis>::exe(this->" << v.name << ","
	 << iprefix << "stran);\n";
    } else {
      os << "ansys::ImportDrivingVariables<hypothesis>::exe(this->" << v.name << "0,"
	 << iprefix << "stran);\n";
    }
  } // end of AnsysInterface::writeBehaviourDataDrivingVariableSetter

  void 
  AnsysInterface::writeIntegrationDataDrivingVariableSetter(std::ostream& os,
							   const DrivingVariable& v,
							   const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(!o.isNull()){
      throw(std::runtime_error("AnsysInterface::writeIntegrationDataMainVariablesSetter : "
			       "only one driving variable supported"));
    }
    if(v.increment_known){
      os << "ansys::ImportDrivingVariables<hypothesis>::exe(this->d" << v.name << ","
	 << iprefix << "dstran);\n";
    } else {
      os << "ansys::ImportDrivingVariables<hypothesis>::exe(this->" << v.name << "1,"
	 << iprefix << "dstran);\n";
    }
  } // end of AnsysInterface::writeIntegrationDataDrivingVariableSetter
  
  void 
  AnsysInterface::writeBehaviourDataThermodynamicForceSetter(std::ostream& os,
							      const ThermodynamicForce& f,
							      const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(SupportedTypes::getTypeFlag(f.type)==SupportedTypes::Stensor){
      os << "ansys::ImportThermodynamicForces<hypothesis>::exe(this->" << f.name << ",";
      if(!o.isNull()){
	os << iprefix << "stress_+" << o << ");\n";
      } else {
	os << iprefix << "stress_);\n";
      }
    } else {
      throw(std::runtime_error("AnsysInterface::writeBehaviourDataMainVariablesSetters : "
			       "unsupported forces type"));
    }
  } // end of AnsysInterface::writeBehaviourDataThermodynamicForceSetter
  
  void 
  AnsysInterface::completeBehaviourDataConstructor(std::ostream& out,
						    const Hypothesis h,
						    const BehaviourDescription& mb) const
  {
    auto do_nothing = [&out](){
      out << "static_cast<void>(ANSYSDR);\n";
    };
    /* 
     * We apply the rotation associated to the Jauman corotationnal frame only if:
     * - the behaviour symmetry is isotropic
     * - the behaviour is written in small strain
     * - the finite strain strategy is either undefined or `Native`
     */
    const bool c = ((mb.getSymmetryType()==mfront::ISOTROPIC)&&
		    (mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
		    ((this->fss==UNDEFINEDSTRATEGY)||(this->fss==NATIVEFINITESTRAINSTRATEGY)));
    if(!c){
      do_nothing();
      return;
    }
    // checking if there are variables that need to be rotated
    const auto& d = mb.getBehaviourData(h);
    bool b = false; // have persistent variables that have to be updated
    for(const auto& v:d.getPersistentVariables()){
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if((flag==SupportedTypes::Stensor)||
	 (flag==SupportedTypes::Tensor)){
	b = true;
	break;
      }
    }
    if(!b){
      do_nothing();
      return;
    }
    // rotate variables
    out << "const tfel::math::tmatrix<3u,3u,real> ansys_dr = {ANSYSDR[0],ANSYSDR[1],ANSYSDR[2],\n"
      "                                                       ANSYSDR[3],ANSYSDR[4],ANSYSDR[5],\n"
      "                                                       ANSYSDR[6],ANSYSDR[7],ANSYSDR[8]};\n";
    for(const auto& v:d.getPersistentVariables()){
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if((flag==SupportedTypes::Stensor)||
	 (flag==SupportedTypes::Tensor)){
	if(v.arraySize==1u){
	  out << "this->" << v.name << ".changeBasis(ansys_dr);\n";
	} else {
	  for(unsigned short i=0;i!=v.arraySize;++i){
	    out << "this->" << v.name << "[" << i << "].changeBasis(ansys_dr);\n";
	  }
	}
      }
    }
  } // end of AnsysInterface::completeBehaviourDataConstructor

  void AnsysInterface::exportThermodynamicForce(std::ostream& out,
						const std::string& a,
						const ThermodynamicForce& f,
						const SupportedTypes::TypeSize o) const
   {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
    if(flag==SupportedTypes::Stensor){
      if(!o.isNull()){
	out << "ansys::ExportThermodynamicForces<hypothesis>::exe("
	    << a << "+" << o << ",this->sig);\n";
      } else {
	out << "ansys::ExportThermodynamicForces<hypothesis>::exe(" << a << ",this->sig);\n";
      }
    } else {
      throw(std::runtime_error("AnsysInterface::exportThermodynamicForce: "
			       "unsupported forces type"));
    }
  } // end of AnsysInterface::exportThermodynamicForce

  bool AnsysInterface::areExternalStateVariablesSupported() const
  {
    return false;
  } // end of AnsysInterface::areExternalStateVariablesSupported()

  void AnsysInterface::getTargetsDescription(TargetsDescription& d,
					     const BehaviourDescription& bd)
  {
    const auto lib  = this->getLibraryName(bd);
    const auto name = bd.getLibrary()+bd.getClassName(); 
#ifdef _WIN32
    const std::string tfel_config = "tfel-config.exe";
#else /* WIN32 */
    const std::string tfel_config = "tfel-config";
#endif /* WIN32 */
    insert_if(d[lib].cppflags,"$(shell "+tfel_config+" --includes)");
    insert_if(d[lib].sources,"ansys"+name+".cxx");
    d.headers.push_back("MFront/Ansys/ansys"+name+".hxx");
    insert_if(d[lib].ldflags,"-lAnsysInterface");
    if(this->generateMTestFile){
      insert_if(d[lib].ldflags,"-lMTestFileGenerator");
    }
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    for(const auto h : this->getModellingHypothesesToBeTreated(bd)){
      insert_if(d[lib].epts,this->getFunctionNameForHypothesis(name,h));
    }
  } // end of AnsysInterface::getTargetsDescription

  AnsysInterface::~AnsysInterface() = default;
  
} // end of namespace mfront

