/*!
 * \file   mfront/src/FortranMaterialPropertyInterface.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 déc 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<algorithm>
#include<stdexcept>

#include"MFront/MFrontHeader.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/FortranMaterialPropertyInterface.hxx"

namespace mfront
{

  std::string
  FortranMaterialPropertyInterface::getName()
  {
    return "fortran";
  }

  FortranMaterialPropertyInterface::FortranMaterialPropertyInterface()
    : CMaterialPropertyInterfaceBase()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  FortranMaterialPropertyInterface::treatKeyword(const std::string& key,
						 const std::vector<std::string>& i,
						 tokens_iterator current,
						 const tokens_iterator)
  {
    auto throw_if = [](const bool b, const std::string& m){
      if(b){throw(std::runtime_error("FortranMaterialPropertyInterface::treatKeyword : "+m));}
    };
    if(std::find(i.begin(),i.end(),"fortran")!=i.end()){
      throw_if(key!="@Module","unsupported key '"+key+"'");
    }
    return {false,current};
  }
  
  void
  FortranMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
							  const MaterialPropertyDescription& mpd) const

  {
    const auto lib  = "Fortran"+getMaterialLawLibraryNameBase(mpd);
    const auto name = this->getSrcFileName(mpd.material,mpd.className);
    const auto f = makeLowerCase(mpd.material.empty()
				 ? mpd.className : mpd.material+"_"+mpd.className);
    insert_if(d[lib].ldflags,"-lm");
    insert_if(d[lib].sources,name+".cxx");
    insert_if(d[lib].epts,{f,f+"_checkBounds"});
  } // end of FortranMaterialPropertyInterface::getTargetsDescription

  std::string
  FortranMaterialPropertyInterface::getHeaderFileName(const std::string&,
						      const std::string&) const
  {
    return "";
  } // end of FortranMaterialPropertyInterface::getHeaderFileName

  std::string
  FortranMaterialPropertyInterface::getSrcFileName(const std::string& material,
						   const std::string& className) const
  {
    if(material.empty()){
      return className+"-fortran";
    }
    return material+"_"+className+"-fortran";
  } // end of FortranMaterialPropertyInterface::getSrcFileName

  void
  FortranMaterialPropertyInterface::writeInterfaceSpecificVariables(std::ostream& os,
								    const VariableDescriptionContainer& inputs) const
  {
    for(const auto& i : inputs){
      os << "const mfront_fortran_real8 " << i.name
	 << " =  *(_mfront_var_" << i.name << ");\n";
    }
  } // end of FortranMaterialPropertyInterface::writeInterfaceSpecificVariables

  void
  FortranMaterialPropertyInterface::writeParameterList(std::ostream& file,
						       const VariableDescriptionContainer& inputs) const{
    if(!inputs.empty()){
      for(auto p=inputs.begin();p!=inputs.end();){
	file << "const mfront_fortran_real8 * const _mfront_var_" << p->name;
	if((++p)!=inputs.end()){
	  file << ",\n";
	}
      }
    } else {
      file << "void";
    }
  } // end of FortranMaterialPropertyInterface::writeParameterList

  void
  FortranMaterialPropertyInterface::writeSrcPreprocessorDirectives(std::ostream& os,
								   const MaterialPropertyDescription& mpd) const
  {
    const auto name = (!mpd.material.empty()) ? mpd.material+"_"+mpd.className : mpd.className;
    const auto f77_func = (name.find('_')!=std::string::npos) ? "F77_FUNC_" : "F77_FUNC";
    writeExportDirectives(os);
    writeF77FUNCMacros(os);
    os << "#define " << this->getFunctionName(mpd)
       << " " << f77_func << "("
       << makeLowerCase(name) << ","
       << makeUpperCase(name) << ")\n"
       << "#define " << this->getCheckBoundsFunctionName(mpd)
       << " F77_FUNC_(" << makeLowerCase(name) << "_checkbounds,"
       << makeUpperCase(name) << "_CHECKBOUNDS)\n\n"
       << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n"
       << "typedef double mfront_fortran_real8;\n\n"
       << "\nMFRONT_SHAREDOBJ double "
       << this->getFunctionName(mpd) << "(";
    this->writeParameterList(os,mpd.inputs);
    os << ");\n"
       << "MFRONT_SHAREDOBJ int "
       << this->getCheckBoundsFunctionName(mpd) << "(";
    this->writeParameterList(os,mpd.inputs);
    os << ");\n\n"
       << "#ifdef __cplusplus\n"
       << "}\n"
       << "#endif /* __cplusplus */\n\n";
  } // end of FortranMaterialPropertyInterface::writeSrcPreprocessorDirectives

  void FortranMaterialPropertyInterface::writeBeginHeaderNamespace(std::ostream&) const
  {} // end of FortranMaterialPropertyInterface::writeBeginHeaderNamespace
  
  void FortranMaterialPropertyInterface::writeEndHeaderNamespace(std::ostream&) const
  {} // end of FortranMaterialPropertyInterface::writeEndHeaderNamespace()

  void FortranMaterialPropertyInterface::writeBeginSrcNamespace(std::ostream& os) const
  {
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";
  } // end of FortranMaterialPropertyInterface::writeBeginSrcNamespace
  
  void FortranMaterialPropertyInterface::writeEndSrcNamespace(std::ostream& os) const
  {
    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";
  } // end of FortranMaterialPropertyInterface::writeEndSrcNamespace()

  std::string
  FortranMaterialPropertyInterface::getFunctionName(const MaterialPropertyDescription& mpd) const
  {
    const auto material  = mpd.material;
    const auto className = mpd.className;
    return (material.empty() ? makeUpperCase(className) :
	    makeUpperCase(material+"_"+className)) + "_F77";
  } // end of FortranMaterialPropertyInterface::getFunctionName

  bool FortranMaterialPropertyInterface::requiresCheckBoundsFunction() const
  {
    return false;
  }

  std::string
  FortranMaterialPropertyInterface::getCheckBoundsFunctionName(const MaterialPropertyDescription& mpd) const
  {
    const auto material  = mpd.material;
    const auto className = mpd.className;
    return (material.empty() ? makeUpperCase(className) :
	    makeUpperCase(material+"_"+className)) + "_CHECKBOUNDS_F77";
  } // end of FortranMaterialPropertyInterface::getCheckBoundsFunctionName
  
  FortranMaterialPropertyInterface::~FortranMaterialPropertyInterface() = default;

} // end of namespace mfront
