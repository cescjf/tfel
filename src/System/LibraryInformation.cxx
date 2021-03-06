/*!
 * \file   LibraryInformation.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   28/03/2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 *
 * \note This file contains code that has been extracted from the
 * boost/dll library version 1.63 and has been originally written by
 * Antony Polukhin, Renato Tegon Forti and Antony Polukhin.
 */

#include<cassert>
#include<cstring>
#include<fstream>
#include<stdexcept>
#include<algorithm>
#include"TFEL/System/LibraryInformation.hxx"

namespace tfel{

  namespace system{

#include "LibraryInformationImplementation.cxx"
#include "LibraryInformation-elf_info.cxx"
#include "LibraryInformation-pe_info.cxx"
#include "LibraryInformation-macho_info.cxx"
    
    LibraryInformation::LibraryInformation(const std::string& f,
					   const bool b)
    {
      auto throw_if = [](const bool c,const std::string& m){
	if(c){throw(std::runtime_error("LibraryInformation::LibraryInformation: "+m));}
      };
      auto throw_if_in_32bits = [throw_if]() {
	constexpr const auto c = sizeof(void*) == 4;
        throw_if(c,"not native format (64bit binary)");
      };
      auto throw_if_in_windows = [throw_if]() {
#if (defined(_WIN32) || defined(_WIN64))
	throw_if(true,"not native format (not a PE binary)");
#endif
      };
      auto throw_if_in_linux = [throw_if]() {
#if !((defined(_WIN32)    || defined(_WIN64))   || \
      (defined(macintosh) || defined(Macintosh) || \
       ((defined(__APPLE__) && defined(__MACH__)))))
	throw_if(true,"not native format (not an ELF binary)");
#endif
      };
      auto throw_if_in_macos = [throw_if]() {
#if (defined(macintosh) || defined(Macintosh) || \
     (defined(__APPLE__) && defined(__MACH__)))
	throw_if(true,"not native format (not an Mach-O binary)");
#endif
      };
      //
      auto file = std::make_shared<std::ifstream>(f);
      throw_if(!(*file),"no library named '"+f+"' found");
      // 
      if (details::elf_info32::parsing_supported(*file)) {
	if (b) { throw_if_in_windows(); throw_if_in_macos(); }
	this->impl = std::make_shared<details::elf_info32>(file);
      } else if (details::elf_info64::parsing_supported(*file)) {
	if (b) { throw_if_in_windows(); throw_if_in_macos(); throw_if_in_32bits(); }
	this->impl = std::make_shared<details::elf_info64>(file);
      } else if (details::pe_info32::parsing_supported(*file)) {
      	if (b) { throw_if_in_linux(); throw_if_in_macos(); }
      	this->impl = std::make_shared<details::pe_info32>(file);
      } else if (details::pe_info64::parsing_supported(*file)) {
      	if (b) { throw_if_in_linux(); throw_if_in_macos(); throw_if_in_32bits(); }
      	this->impl = std::make_shared<details::pe_info64>(file);
      } else if (details::macho_info32::parsing_supported(*file)) {
      	if (b) { throw_if_in_linux(); throw_if_in_windows(); }
      	this->impl = std::make_shared<details::macho_info32>(file);
      } else if (details::macho_info64::parsing_supported(*file)) {
      	if (b) { throw_if_in_linux(); throw_if_in_windows(); throw_if_in_32bits(); }
      	this->impl = std::make_shared<details::macho_info64>(file);
      } else {
	throw_if(true,"unsupported binary format");
      }
    } // end of LibraryInformation::LibraryInformation

    std::vector<std::string> LibraryInformation::sections()
    {
      return this->impl->sections();
    } // end of LibraryInformation::section

    std::vector<std::string> LibraryInformation::symbols(){
      return this->impl->symbols();
    } // end of LibraryInformation::symbols

    std::vector<std::string> LibraryInformation::symbols(const char* n){
      return this->impl->symbols(n);
    } // end of LibraryInformation::symbols

    std::vector<std::string> LibraryInformation::symbols(const std::string& n){
      return this->symbols(n.c_str());
    } // end of LibraryInformation::symbols

    LibraryInformation::~LibraryInformation() noexcept = default;
    
  } // end of namespace system
    
} // end of namespace tfel
