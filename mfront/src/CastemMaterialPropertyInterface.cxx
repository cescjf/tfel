/*!
 * \file   mfront/src/CastemMaterialPropertyInterface.cxx
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
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/MaterialPropertyParametersHandler.hxx"
#include"MFront/CastemMaterialPropertyInterface.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99)
#include<sstream>
namespace std{
  template<typename T>
  std::string to_string(const T& v){
    std::ostringstream s;
    s << v;
    return s.str();
  }
}
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront
{
  
  std::string
  CastemMaterialPropertyInterface::getName(void)
  {
    return "castem";
  }

  CastemMaterialPropertyInterface::CastemMaterialPropertyInterface()
  {}
  
  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CastemMaterialPropertyInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    return {false,current};
  } // end of treatKeyword

  CastemMaterialPropertyInterface::~CastemMaterialPropertyInterface()
  {}

  void
  CastemMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
							 const MaterialPropertyDescription& mpd)
  {
    const auto lib = "Castem"+getMaterialLawLibraryNameBase(mpd.library,
							    mpd.material);
    const auto name    = this->getCastemFunctionName(mpd.material,
						     mpd.className);
    insert_if(d[lib].ldflags,"-lm");
    insert_if(d[lib].cppflags,CASTEM_CPPFLAGS);
    insert_if(d[lib].sources,this->getSourceFileName(name));
    d.headers.push_back("include/"+this->getHeaderFileName(name));    
    insert_if(d[lib].epts,name);
  } // end of CastemMaterialPropertyInterface::getTargetsDescription

  std::string
  CastemMaterialPropertyInterface::getCastemFunctionName(const std::string& material,
							 const std::string& className)
  {
    if(material.empty()){
      return className;
    }
    return material+"_"+className;
  }

  std::string
  CastemMaterialPropertyInterface::getHeaderFileName(const std::string& name)
  {
    return name + "-castem.hxx";
  }

  std::string
  CastemMaterialPropertyInterface::getSourceFileName(const std::string& name)
  {
    return name + "-castem.cxx";
  }

  void
  CastemMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						    const FileDescription& fd)
  {
    this->writeHeaderFile(mpd,fd);
    this->writeSrcFile(mpd,fd);
  } // end of CastemMaterialPropertyInterface::writeOutputFiles

  void
  CastemMaterialPropertyInterface::writeHeaderFile(const MaterialPropertyDescription& mpd,
						   const FileDescription& fd)
  {
    const auto name = this->getCastemFunctionName(mpd.material,mpd.className);
    const auto fn = "include/"+this->getHeaderFileName(name);
    std::ofstream out{fn};
    if(!out){
      throw(std::runtime_error("CastemMaterialPropertyInterface::writeOutputFiles: "
			       "unable to open '"+fn+"'"));
    }
    out.exceptions(std::ios::badbit|std::ios::failbit);
    out << "/*!\n";
    out << "* \\file   " << fn  << "\n";
    out << "* \\brief  " << "this file declares the " 
	<< name << " MaterialLaw.\n";
    out << "*         File generated by ";
    out << MFrontHeader::getVersionName() << " ";
    out << "version " << MFrontHeader::getVersionNumber();
    out << '\n';
    if(!fd.authorName.empty()){
      out << "* \\author " << fd.authorName << '\n';
    }
    if(!fd.date.empty()){
      out << "* \\date   " << fd.date       << '\n';
    }
    if(!fd.description.empty()){
      out << fd.description << '\n';
    }
    out << " */\n\n";

    out << "#ifndef " << makeUpperCase(name) << "_CASTEM_HH\n";
    out << "#define " << makeUpperCase(name) << "_CASTEM_HH\n\n";
    writeExportDirectives(out);
    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";
    out << "MFRONT_SHAREDOBJ double\n";
    out << name << "(";
    out << "const double * const";
    out << ");\n\n";
    if(!mpd.parameters.empty()){
      out << "MFRONT_SHAREDOBJ int\n";
      out << name << "_setParameter(const char *const,";
      out << "const double";
      out << ");\n\n";
    }
    out << "#ifdef __cplusplus\n";
    out << "} // end of extern \"C\"\n";
    out << "#endif /* __cplusplus */\n\n";
    out << "#endif /* " << makeUpperCase(name) << "_CASTEM_HH */\n";
    out.close();
  } // end of CastemMaterialPropertyInterface::writeHeaderFile(void)

  void
  CastemMaterialPropertyInterface::writeSrcFile(const MaterialPropertyDescription& mpd,
						const FileDescription& fd)
  {
    const auto name = this->getCastemFunctionName(mpd.material,mpd.className);
    const auto fn   = "src/"+this->getSourceFileName(name);
    std::ofstream out{fn};
    if(!out){
      throw(std::runtime_error("CastemMaterialPropertyInterface::writeOutputFiles: "
			       "unable to open '"+fn+"'"));
    }
    out.exceptions(std::ios::badbit|std::ios::failbit);
    const auto& file=fd.fileName;
    const auto& author=fd.authorName;
    const auto& date=fd.date;
    const auto& includes=mpd.includes;
    const auto& output=mpd.output;
    const auto& inputs=mpd.inputs;
    const auto& materialLaws=mpd.materialLaws;
    const auto& glossaryNames=mpd.glossaryNames;
    const auto& entryNames=mpd.entryNames;
    const auto& staticVars=mpd.staticVars;
    const auto& params=mpd.parameters;
    const auto& function=mpd.f;
    const auto& bounds=mpd.bounds;
    const auto& physicalBounds=mpd.physicalBounds;
    out << "/*!\n";
    out << "* \\file   " << fn  << '\n';
    out << "* \\brief  " << "this file implements the " 
			<< name << " MaterialLaw.\n";
    out << "*         File generated by ";
    out << MFrontHeader::getVersionName() << " ";
    out << "version " << MFrontHeader::getVersionNumber();
    out << '\n';
    if(!author.empty()){
      out << "* \\author " << author << '\n';
    }
    if(!date.empty()){
      out << "* \\date   " << date       << '\n';
    }
    out << " */\n\n";
    out << "#include<algorithm>\n";
    out << "#include<iostream>\n";
    out << "#include<fstream>\n";
    out << "#include<cstring>\n";
    out << "#include<cstdlib>\n";
    out << "#include<string>\n";
    out << "#include<cmath>\n";
    if(!includes.empty()){
      out << includes << "\n\n";
    }
    out << "#include\"" << name << "-castem.hxx\"\n\n";
    writeMaterialPropertyParametersHandler(out,mpd,
					   name,"double","castem");
    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "MFRONT_SHAREDOBJ const char *\n";
    out << name << "_src = \""
	<< tfel::utilities::tokenize(file,tfel::system::dirSeparator()).back()
	<< "\";\n\n";
    if(!inputs.empty()){
      out << "MFRONT_SHAREDOBJ const char *\n";
      out << name << "_args[" << inputs.size();
      out << "] = {";
      for(auto p3=inputs.begin();p3!=inputs.end();){
	std::string iname;
	auto p4=glossaryNames.find(p3->name);
	if(p4!=glossaryNames.end()){
	  iname = "\""+p4->second+"\"";
	} else if((p4=entryNames.find(p3->name))!=entryNames.end()){
	  iname = "\""+p4->second+"\"";
	} else {
	  iname = "\""+p3->name+"\"";
	}
	out << iname;
	if(++p3!=inputs.end()){
	  out << ",";
	}
      }
      out << "};\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short\n";
    out << name << "_nargs = " << inputs.size() << "u;\n\n";

    if(!params.empty()){
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      out << "MFRONT_SHAREDOBJ int\n";
      out << name << "_setParameter(const char *const p,";
      out << "const double v";
      out << "){\n";
      for(auto pp=params.begin();pp!=params.end();++pp){
	out << "if(strcmp(\"" << *pp << "\",p)==0){\n";
	out << "castem::" <<  hn << "::get" << hn << "()." << *pp << " = v;\n";
	out << "return 1;\n";
	out << "}\n";
      }	
      out << "return 0;\n";
      out << "}\n\n";
    }

    out << "MFRONT_SHAREDOBJ double\n";
    out << name << "(";
    if(!inputs.empty()){
      out << "const double * const castem_params";
    } else {
      out << "const double * const";
    }
    out << ")\n{\n";
    out << "using namespace std;\n";
    out << "typedef double real;\n";
    // material laws
    writeMaterialLaws("CastemMaterialPropertyInterface::writeOutputFile",
		      out,materialLaws);
    // static variables
    writeStaticVariables("CastemMaterialPropertyInterface::writeOutputFile",
			 out,staticVars,file);
    // parameters
    if(!params.empty()){
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      out << "if(!castem::" <<  hn << "::get" << hn << "().ok){\n"
		    << "return std::nan(castem::"<< name << "MaterialPropertyHandler::get"
		    << name << "MaterialPropertyHandler().msg.c_str());\n"
		    << "}\n";
      writeAssignMaterialPropertyParameters(out,mpd,name,
					    "double","castem");
    }
    if(!inputs.empty()){
      auto p3=inputs.begin();
      for(auto i=0u;p3!=inputs.end();++p3,++i){
	out << "const double " << p3->name << " = ";
	if(i==0){
	  out << "*(castem_params);\n";
	} else {
	  out << "*(castem_params+"+std::to_string(i)+"u);\n";
	}
      }
    }
    out << "real " << output << ";\n";
    if((!physicalBounds.empty())||
       (!bounds.empty())){
      out << "#ifndef NO_CASTEM_BOUNDS_CHECK\n";
    }
    if(!physicalBounds.empty()){
      out << "// treating physical bounds\n";
      for(auto p5=physicalBounds.begin();
	  p5!=physicalBounds.end();++p5){
	if(p5->boundsType==VariableBoundsDescription::Lower){
	  out << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is below its physical lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  out << "return nan(\""
			      << name << " : "
			      << p5->varName << " is not physically valid.\");\n";
	  out << "}\n";
	} else if(p5->boundsType==VariableBoundsDescription::Upper){
	  out << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is below its physical upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  out << "return nan(\""
			      << name << " : "
			      << p5->varName << " is not physically valid.\");\n";
	  out << "}\n";
	} else {
	  out << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			      << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	  out << "if(" << p5->varName<< " < " << p5->lowerBound << "){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is below its physical lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  out << "} else {\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is over its physical upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  out << "}\n";
	  out << "return nan(\""
			      << name << " : "
			      << p5->varName << " is not physically valid.\");\n";
	  out << "}\n";
	}
      }
    }
    if(!bounds.empty()){
      out << "// treating standard bounds\n";
      for(auto p5=bounds.begin();
	  p5!=bounds.end();++p5){
	if(p5->boundsType==VariableBoundsDescription::Lower){
	  out << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	  out << "const char * const policy = "
			      << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n";
	  out << "if(policy!=nullptr){\n";
	  out << "if(strcmp(policy,\"STRICT\")==0){\n";
	  out << "return nan(\""
			      << name << " : "
			      << p5->varName << " is out of bounds.\");\n";
	  out << "} else if (strcmp(policy,\"WARNING\")==0){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is below its lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  out << "}\n";
	  out << "}\n";
	  out << "}\n";
	} else if(p5->boundsType==VariableBoundsDescription::Upper){
	  out << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	  out << "const char * const policy = "
			      << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n";
	  out << "if(policy!=nullptr){\n";
	  out << "if(strcmp(policy,\"STRICT\")==0){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  out << "return nan(\""
			      << name << " : "
			      << p5->varName << " is out of bounds.\");\n";
	  out << "} else if (strcmp(policy,\"WARNING\")==0){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  out << "}\n";
	  out << "}\n";
	  out << "}\n";
	} else {
	  out << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			      << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	  out << "const char * const policy = "
			      << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n";
	  out << "if(policy!=nullptr){\n";
	  out << "if(strcmp(policy,\"STRICT\")==0){\n";
	  out << "if(" << p5->varName<< " < " << p5->lowerBound << "){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is below its lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  out << "} else {\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  out << "}\n";
	  out << "return nan(\""
			      << name << " : "
			      << p5->varName << " is out of bounds.\");\n";
	  out << "} else if (strcmp(policy,\"WARNING\")==0){\n";
	  out << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is below its lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  out << "} else {\n";
	  out << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  out << "}\n";
	  out << "}\n";
	  out << "}\n";
	  out << "}\n";
	}
      }
    }
    if((!physicalBounds.empty())||
       (!bounds.empty())){
      out << "#endif /* NO_CASTEM_BOUNDS_CHECK */\n";
    }
    out << function.body;
    out << "return " << output << ";\n";
    out << "} // end of " << name << "\n\n";
    out << "#ifdef __cplusplus\n";
    out << "} // end of extern \"C\"\n";
    out << "#endif /* __cplusplus */\n\n";
    out.close();
  } // end of CastemMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront
