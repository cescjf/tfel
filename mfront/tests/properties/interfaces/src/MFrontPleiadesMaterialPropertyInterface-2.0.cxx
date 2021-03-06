/*!
 * \file   mfront/tests/properties/interfaces/src/MFrontPleiadesMaterialPropertyInterface-2.0.cxx
 * \brief
 * \author Helfer Thomas
 * \date   06 mai 2008
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

#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/MaterialPropertyInterfaceProxy.hxx"
#include"MFront/MFrontPleiadesMaterialPropertyInterface-2.0.hxx"

namespace mfront {

  std::string MFrontPleiadesMaterialPropertyInterface::getName() {
    return "pleiades-2.0";
    // TODO: make a MFrontPleiadesMaterialPropertyInterface.cxx.in and use @PACKAGE_VERSION@
  }

  MFrontPleiadesMaterialPropertyInterface::MFrontPleiadesMaterialPropertyInterface() = default;

  std::pair<bool,MFrontPleiadesMaterialPropertyInterface::const_iterator>
  MFrontPleiadesMaterialPropertyInterface::treatKeyword(const std::string& k,
							const std::vector<std::string>& i,
							const_iterator current,
							const const_iterator) {
    if(std::find(i.begin(),i.end(),"pleiades-2.0")!=i.end()){
      throw(std::runtime_error("MFrontPleiadesMaterialPropertyInterface::treatKeyword: "
			       "unsupported key '"+k+"'"));
    }
    return {false,current};
  } // end of treatKeyword

  MFrontPleiadesMaterialPropertyInterface::~MFrontPleiadesMaterialPropertyInterface() = default;

  void
  MFrontPleiadesMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
								 const MaterialPropertyDescription& mpd) const
  {
    const auto lib  = "libPleiades"+getMaterialLawLibraryNameBase(mpd);
    const auto name = mpd.material.empty() ? mpd.className : mpd.material+"_"+mpd.className;
    const auto hn   = "include/Pleiades/Metier/MaterialProperty/"+name+"-pleiades.hh";
    d[lib].ldflags.push_back("-lm");
    d[lib].cppflags.push_back("`pleiades-config --includes`\n");
    d[lib].sources.push_back(name+"-pleiades.cpp");
    d[lib].epts.push_back(name);
    d.headers.push_back(hn.substr(8));
  } // end of MFrontPleiadesMaterialPropertyInterface::getTargetsDescription

  void MFrontPleiadesMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
								 const FileDescription& fd) const{
    this->writeHeaderFile(mpd,fd);
    this->writeSrcFile(mpd,fd);
  } // end of MFrontPleiadesMaterialPropertyInterface::writeOutputFiles

  void MFrontPleiadesMaterialPropertyInterface::writeHeaderFile(const MaterialPropertyDescription& mpd,
								const FileDescription& fd) const {
    using namespace tfel::system;
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/Metier");
    systemCall::mkdir("include/Pleiades/Metier/MaterialProperty");
    const auto fn  = "include/Pleiades/Metier/MaterialProperty/"+name+"-pleiades.hh";
    std::ofstream os(fn);
    if(!os){
      throw(std::runtime_error("MaterialLawParser::writeOutputFiles: "
			       "unable to open '"+fn+"' for writing output file."));
    }
    os.exceptions(std::ios::badbit|std::ios::failbit);
    os << "/*!\n"
       << "* \\file   " << fn  << '\n'
       << "* \\brief  " << "this file declares the "
       << name << " MaterialLaw.\n"
       << "*         File generated by "
       << MFrontHeader::getVersionName() << " "
       << "version " << MFrontHeader::getVersionNumber()
       << '\n';
    if(!fd.authorName.empty()){
      os << "* \\author " << fd.authorName << '\n';
    }
    if(!fd.date.empty()){
      os << "* \\date   " << fd.date       << '\n';
    }
    if(!fd.description.empty()){
      os << fd.description << '\n';
    }
    os << " */\n\n"
       << "#ifndef _PLEIADES_" << makeUpperCase(name) << "_HH\n"
       << "#define _PLEIADES_" << makeUpperCase(name) << "_HH\n\n";
    //  writeExportDirectives(os);

    os << "#include<cmath>\n";
    if(!mpd.includes.empty()){
      os << mpd.includes << '\n';
    }
    os << "#include \"Pleiades/Metier/MaterialProperty/MaterialPropertyBase.hh\"\n\n"
       << "namespace Pleiades\n{\n\n"
       << "class " << name << '\n'
       << ": public MaterialPropertyBase\n"
       << "{\n\n"
       << "public:\n"
       << name << "();\n"
       << "void declare();\n "
       << "void compute();\n "
       << "double operator()(";
    if(!mpd.inputs.empty()){
      for(auto p4=mpd.inputs.begin();p4!=mpd.inputs.end();){
	os << "const double& " << p4->name ;
	if((++p4)!=mpd.inputs.end()){
	  os << ",";
	}
      }
    } else {
      os << "void";
    }
    os << ") const;\n\n";
    // Disable copy constructor and assignement operator
    os << "private:\n\n"
       << "//! Copy constructor (disabled)\n"
       << name << "(const "
       << name << "&);\n\n"
       << "//! Assignement operator (disabled)\n"
       << name << "&\n"
       << "operator=(const "
       << name << "&);\n\n"
       << "}; // end of class " << name << "\n\n"
       << "} // end of namespace Pleiades\n\n"
       << "#endif /* _PLEIADES_" << makeUpperCase(name) << "_HH */\n";
    os .close();
  } // end of MFrontPleiadesMaterialPropertyInterface::writeHeaderFile()

  void MFrontPleiadesMaterialPropertyInterface::writeSrcFile(const MaterialPropertyDescription& mpd,
							     const FileDescription& fd) const {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("MFrontPleiadesMaterialPropertyInterface::writeSrcFile: "+m));}
    };
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    const auto fn  = "src/"+name+"-pleiades.cpp";
    std::ofstream os(fn);
    throw_if(!os,"unable to open '"+fn+"' for writing output file.");
    os.exceptions(std::ios::badbit|std::ios::failbit);
    os << "/*!\n"
       << "* \\file   " << fn  << '\n'
       << "* \\brief  " << "this file implements the "
       << name << " MaterialLaw.\n"
       << "*         File generated by "
       << MFrontHeader::getVersionName() << " "
       << "version " << MFrontHeader::getVersionNumber()
       << '\n';
    if(!fd.authorName.empty()){
      os << "* \\author " << fd.authorName << '\n';
    }
    if(!fd.date.empty()){
      os << "* \\date   " << fd.date       << '\n';
    }
    os << " */\n\n"
       << "#include<string>\n"
       << "#include<cmath>\n\n"
       << "#include\"Pleiades/Examplars/ClassProxy.hh\"\n"
       << "#include\"Pleiades/Exceptions/pexceptions.hh\"\n"
       << "#include\"Pleiades/Metier/Glossaire/pglossaire.hh\"\n"
       << "#include\"Pleiades/Metier/MaterialProperty/" << name << "-pleiades.hh\"\n"
       << "namespace Pleiades {\n\n"
       << name << "::" << name <<"(){}\n\n";
    // declare method
    os << "void " << name << "::declare() {\n"
       << "using namespace Pleiades::PGlossaire;\n";
    std::string oname;
    if(mpd.output.hasGlossaryName()){
      oname = "GlossaireField::"+mpd.output.getExternalName();
    } else if (mpd.output.hasEntryName()){
      oname = "\"" + mpd.output.getExternalName() + "\"";
    } else {
      os << "AFF(\"WARNING !!! Glossary name not defined for output field: "
	 << mpd.output.name << " in " << name << "\");\n";
      oname = "\""+mpd.output.name+"\"";
    }
    os << "declareField<double>(\"OutputField\", OUT);\n"
       << "setFieldName(\"OutputField\", " << oname << ");\n";

    for(const auto& i : mpd.inputs){
      const auto iname = [&i](){
	if(i.hasGlossaryName()){
	  return "GlossaireField::"+i.getExternalName();
	} else if(i.hasEntryName()){
	  return "\""+i.getExternalName()+"\"";
	}
	return "\""+i.name+"\"";
      }();
      os << "declareField<double>(\"" << i.name << "\", " << "IN" << ");\n"
	 << "setFieldName(\"" << i.name << "\", " << iname << ");\n";
    }
    os << "}\n\n";

    // Compute
    os << "void " << name << "::compute() {\n"
       << "apply(*this, getOutputField<double>(\"OutputField\")";
    for(const auto& i : mpd.inputs){
      os << ", getField<double>(\"" << i.name << "\")";
    }
    os << ");\n"
       << "} // end of " << name << "::compute\n\n";
    // Law
    os << "double " << name << "::operator()(";
    if(!mpd.inputs.empty()){
      for(auto pi=mpd.inputs.begin();pi!=mpd.inputs.end();){
	os << "const double& " << pi->name;
	if((++pi)!=mpd.inputs.end()){
	  os << ",";
	}
      }
    } else {
      os << "void";
    }
    os << ") const {\n"
       << "using namespace std;\n"
       << "using namespace Pleiades::PGlossaire;\n";

    // material laws
    writeMaterialLaws(os,mpd.materialLaws);
    // static variables
    writeStaticVariables(os,mpd.staticVars,fd.fileName);
    if(!mpd.parameters.empty()){
      for(const auto& p : mpd.parameters){
	throw_if(!p.hasAttribute(VariableDescription::defaultValue),
		 "internal error (can't find value of "
		 "parameter " + p.name + ")");
	os << "static constexpr double " << p.name << " = "
	   << p.getAttribute<double>(VariableDescription::defaultValue) << ";\n";
      }
    }
    os << "double " << mpd.output.name << ";\n";
    if((hasPhysicalBounds(mpd.inputs))||(hasBounds(mpd.inputs))){
      os << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
    }
    auto get_ename = [](const VariableDescription& v){
      if(v.hasGlossaryName()){
	return "GlossaireField::"+v.getExternalName();
      } else if(v.hasEntryName()){
	return "\""+v.getExternalName()+"\"";
      }
      return "\""+v.name+"\"";
    };    
    if(hasPhysicalBounds(mpd.inputs)){
      os << "// treating physical bounds\n";
      for(const auto&i : mpd.inputs){
	if(!i.hasPhysicalBounds()){
	  continue;
	}
	const auto& b = i.getPhysicalBounds();
	const auto fname = get_ename(i);
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  os << "if(" << i.name << " < "<< b.lowerBound << "){\n"
	     << "string msg (\"" << name << "::compute : \");\n"
	     << "msg += " << fname << ";\n"
	     << "msg += \" is below its physical lower bound.\";\n"
	     << "PLEIADES_THROW(msg);\n"
	     << "}\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  os << "if(" << i.name << " > "<< b.upperBound << "){\n"
	     << "string msg (\"" << name << "::compute : \");\n"
	     << "msg += " << fname << ";\n"
	     << "msg += \" is over its physical upper bound.\";\n"
	     << "PLEIADES_THROW(msg);\n"
	     << "}\n";
	} else {
	  os << "if((" << i.name << " < "<< b.lowerBound << ")||"
	     << "(" << i.name << " > "<< b.upperBound << ")){\n"
	     << "if(" << i.name << " < " << b.lowerBound << "){\n"
	     << "string msg (\"" << name << "::compute : \");\n"
	     << "msg += " << fname << ";\n"
	     << "msg += \" is below its physical lower bound.\";\n"
	     << "PLEIADES_THROW(msg);\n"
	     << "} else {\n"
	     << "string msg (\"" << name << "::compute : \");\n"
	     << "msg += " << fname << ";\n"
	     << "msg += \" is over its physical upper bound.\";\n"
	     << "PLEIADES_THROW(msg);\n"
	     << "}\n"
	     << "}\n";
	}
      }
    }
    if(hasBounds(mpd.inputs)){
      os << "// treating standard bounds\n";
      for(const auto&i : mpd.inputs){
	if(!i.hasBounds()){
	  continue;
	}
	const auto fname = get_ename(i);
	const auto& b = i.getBounds();
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  os << "if(" << i.name << " < "<< b.lowerBound << "){\n"
	     << "string msg(\"" << name << "::compute : value of\");\n"
	     << "msg += " << fname << ";\n"
	     << "msg += \" is below its lower bound\";\n"
	     << "treatOutOfBounds(msg);\n"
	     << "}\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  os << "if(" << i.name << " > "<< b.upperBound << "){\n"
	     << "string msg(\"" << name << "::compute : value of\");\n"
	     << "msg += " << fname << ";\n"
	     << "msg += \" is over its upper bound\";\n"
	     << "treatOutOfBounds(msg);\n"
	     << "}\n";
	} else {
	  os << "if((" << i.name << " < "<< b.lowerBound << ")||"
	     << "(" << i.name << " > "<< b.upperBound << ")){\n"
	     << "string msg(\"" << name << "::compute : value of\");\n"
	     << "msg += " << fname << ";\n"
	     << "msg += \" is out of bounds\";\n"
	     << "treatOutOfBounds(msg);\n"
	     << "}\n";
	}
      }
    }
    if((hasPhysicalBounds(mpd.inputs))||(hasBounds(mpd.inputs))){
      os << "#endif /* NO_PLEIADES_BOUNDS_CHECK */\n";
    }
    os << mpd.f.body
       << "return " << mpd.output.name << ";\n"
       << "} // end of " << name << "::law\n\n"
       << "GENERATE_PROXY(IMaterialProperty," << name << ");\n"
       << "} // end of namespace Pleiades\n";
    os.close();
  } // end of MFrontPleiadesMaterialPropertyInterface::writeSrcFile()

  MaterialPropertyInterfaceProxy<MFrontPleiadesMaterialPropertyInterface> pleiadesLawProxy;

} // end of namespace mfront
