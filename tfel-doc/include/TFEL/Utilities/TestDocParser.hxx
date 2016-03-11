/*! 
 * \file  tfel-doc/include/TFEL/Utilities/TestDocParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_UTILITIES_TESTDOCPARSER_H_
#define LIB_TFEL_UTILITIES_TESTDOCPARSER_H_ 

#include"TFEL/Utilities/ParserBase.hxx"
#include"TFEL/Utilities/TestDocumentation.hxx"

namespace tfel{

  namespace utilities {

    /*!
     * Structure in charge of reading a test documentation file
     */
    struct TestDocParser
      : public ParserBase
    {
      /*!
       * \param f : file to be read
       */
      TestDocParser(const std::string&);
      /*!
       * add the documentation of the tests
       */
      virtual void
      addDocumentation(std::map<std::string,std::vector<TestDocumentation> >& tests);
      /*!
       * desctructor
       */
      virtual ~TestDocParser() noexcept;
    private:
      
      void
      treatTest(TestDocumentation& t,
		std::string& c,
		const_iterator& p);

      void
      getStringValue(std::string& s,
		     const std::string& name,
		     const std::string& v,
		     const_iterator& p);

      void
      treatDescription(std::map<std::string,std::string>& dm,
		       std::string& n,
		       const_iterator& p);
      
      void
      treatIndex(std::map<std::string,std::vector<std::string> >& i,
		 const std::string& n,
		 const_iterator& p,
		 const bool b);
      
    }; // end of struct TestDocParser

  } // end of namespace utilities

} // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_TESTDOCPARSER_H */

