/*!
 * \file   basic_wstream.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 nov 2007
 */

#ifndef _LIB_TFEL_BASIC_WSTREAM_HXX_
#define _LIB_TFEL_BASIC_WSTREAM_HXX_ 

#include<string>
#include<cctype>

namespace tfel
{

  namespace system
  {

    template<typename Child,typename Traits>
    struct basic_wstream
    {
      operator int (void);

      operator int (void) const;

      basic_wstream& 
      operator<<(const char&);

      basic_wstream& 
      operator<<(const bool&);
      
      basic_wstream& 
      operator<<(const short&);
      
      basic_wstream& 
      operator<<(const unsigned short&);

      basic_wstream& 
      operator<<(const int&);
      
      basic_wstream& 
      operator<<(const unsigned int&);

      basic_wstream& 
      operator<<(const long&);
      
      basic_wstream& 
      operator<<(const unsigned long&);

      basic_wstream& 
      operator<<(const float&);

      basic_wstream& 
      operator<<(const double&);

      basic_wstream& 
      operator<<(const long double&);

      basic_wstream& 
      operator<<(const void*&);

      basic_wstream& 
      operator<<(const char*);

      basic_wstream& 
      operator<<(const std::string&);

      void
      write(const char* const,const size_t);

      void
      write(const bool* const,const size_t);
      
      void
      write(const short* const,const size_t);
      
      void
      write(const unsigned short* const,const size_t);

      void
      write(const int* const,const size_t);
      
      void
      write(const unsigned int* const,const size_t);

      void
      write(const long* const,const size_t);
      
      void
      write(const unsigned long* const,const size_t);

      void
      write(const float* const,const size_t);

      void
      write(const double* const,const size_t);

      void
      write(const long double* const,const size_t);

    protected :

      basic_wstream();

      basic_wstream(const basic_wstream&);

      basic_wstream&
      operator =(const basic_wstream&);

      static void
      write(int,const void *const,const size_t);

    }; // end of struct basic_rstream

    struct BlockingStreamWriter
    {
      static void write(int,const void *const,const size_t);
    }; // end of BlockingStreamWriter

    struct NonBlockingStreamWriter
    {
      static void write(int,const void *const,const size_t);
    }; // end of NonBlockingStreamWriter

  } // end of namespace System  
  
} // end of namespace tfel  

#include"TFEL/System/basic_wstream.ixx"
 
#endif /* _LIB_TFEL_BASIC_WSTREAM_HXX */
