/*! 
 * \file   mfront/src/ExcelMaterialPropertyInternalInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \date   03 nov. 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/DSLUtilities.hxx"
#include"MFront/ExcelMaterialPropertyInternalInterface.hxx"

namespace mfront
{

  std::string
  ExcelMaterialPropertyInternalInterface::getName()
  {
    return "excel-internal";
  }

  ExcelMaterialPropertyInternalInterface::ExcelMaterialPropertyInternalInterface()
    : CMaterialPropertyInterface()
  {}

  std::string
  ExcelMaterialPropertyInternalInterface::getGeneratedLibraryName(const MaterialPropertyDescription& mpd) const
  {
    return "Excel"+mfront::getMaterialLawLibraryNameBase(mpd);
  } // end of ExcelMaterialPropertyInternalInterface::getGeneratedLibraryName

  std::string
  ExcelMaterialPropertyInternalInterface::getHeaderFileName(const std::string& m,
							    const std::string& c) const
  {
    return CMaterialPropertyInterface::getHeaderFileName(m,c)+"-Excel";
  } // end of ExcelMaterialPropertyInternalInterface::getHeaderFileName

  std::string
  ExcelMaterialPropertyInternalInterface::getSrcFileName(const std::string& m,
							 const std::string& c) const
  {
    return CMaterialPropertyInterface::getSrcFileName(m,c)+"-Excel";
  } // end of ExcelMaterialPropertyInternalInterface::getSrcFileName
  
  void ExcelMaterialPropertyInternalInterface::writeHeaderPreprocessorDirectives(std::ostream& os,
										 const MaterialPropertyDescription&) const
  {
    os << "#ifndef MFRONT_EXCEL_CALLING_CONVENTION\n"
       << "#ifdef _WIN32\n"
       << "#define MFRONT_EXCEL_CALLING_CONVENTION __stdcall\n"
       << "#else /* _WIN32 */\n"
       << "#define MFRONT_EXCEL_CALLING_CONVENTION\n"
       << "#endif /* _WIN32 */\n"
       << "#endif /* MFRONT_EXCEL_CALLING_CONVENTION */\n"; 
  } // end of ExcelMaterialPropertyInternalInterface::writePreprocessorDirectives

  std::string
  ExcelMaterialPropertyInternalInterface::getCallingConvention() const{
    return "MFRONT_EXCEL_CALLING_CONVENTION";
  }

  ExcelMaterialPropertyInternalInterface::~ExcelMaterialPropertyInternalInterface() = default;

} // end of namespace mfront
