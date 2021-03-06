/*!
 * \file   include/TFEL/Tests/TestResult.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_TESTS_TESTRESULT_HXX
#define LIB_TFEL_TESTS_TESTRESULT_HXX 1

#include<iosfwd>
#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace tests
  {
    
   /*! 
     * result of test
     */
    struct TFELTESTS_VISIBILITY_EXPORT TestResult
      : protected std::vector<TestResult>
    {
      using std::vector<TestResult>::const_iterator;
      /*! 
       * Default onstructor
       */
      TestResult();
      /*! 
       * Constructor
       * \param b : tells if the test is a success
       */
      TestResult(const bool);
      /*! 
       * Constructor
       * \param b : tells if the test is a success
       * \param c : detail
       */
      TestResult(const bool,const char* const);
      /*! 
       * Constructor
       * \param b : tells if the test is a success
       * \param c : detail
       */
      TestResult(const bool,const std::string&);
      //! copy constructor
      TestResult(const TestResult&);
      //! copy constructor
      TestResult(TestResult&&);
      //! assignement operator
      TestResult& operator = (const TestResult&);
      //! assignement operator
      TestResult& operator = (TestResult&&);
      /*! 
       * report success
       */
      bool success() const;
      /*! 
       * get result details
       */
      const std::string&
      details() const;
      /*!
       *  \return a read-only (constant) iterator that points to the
       *  first element in the vector.
       */
      const_iterator
      begin() const;
      /*!
       *  \return an iterator that points one past
       *  the last element in the vector
       */
      const_iterator
      end() const;
      /*!
       * This is a typical stack operation.  The function creates an
       * element at the end of the vector and assigns the given data
       * to it.
       * Due to the nature of a vector this operation can be
       * done in constant time if the vector has preallocated space
       * available.
       * \param r : result to be appended
       */
      void append(const TestResult&);
      /*!
       * \brief set test duration
       * \param d test duration
       */
      void setTestDuration(const double);
      /*!
       * \return test duration
       */
      double duration() const;
      //! destructor
      ~TestResult();
    private:
      //! detail
      std::string d;
      //! test duration
      double test_duration = 0.;
      //! success of the test
      bool s = true;
    }; // end of struct 
    
    std::ostream&
    operator << (std::ostream&,const TestResult&);

  } // end of namespace tests

} // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTRESULT_HXX */
