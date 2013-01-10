/*!
 * \file   SignalManager.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Nov 2007
 */

#ifndef _LIB_TFEL_SIGNALMANAGER_HXX_
#define _LIB_TFEL_SIGNALMANAGER_HXX_ 

#include<map>
#include<string>

#include<signal.h>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/SignalHandler.hxx"

namespace tfel
{

  namespace system
  {
    
    struct TFELSYSTEM_VISIBILITY_EXPORT SignalManager
    {
      static void
      printBackTrace(const int);
      static SignalManager&
      getSignalManager(void);
      unsigned short
      registerHandler(const int,SignalHandler *const,struct sigaction&);
      unsigned short
      registerHandler(const int,SignalHandler *const);
      void
      setSpecificHandler(const int,SignalHandler *const);
      void
      removeHandler(const unsigned short);
      ~SignalManager();
    private:
      TFEL_VISIBILITY_LOCAL
      static void
      callGdb(const int,const char* const);
      TFEL_VISIBILITY_LOCAL
      SignalManager();
      TFEL_VISIBILITY_LOCAL
      SignalManager(const SignalManager &);
      TFEL_VISIBILITY_LOCAL
      SignalManager&
      operator = (const SignalManager &);
      TFEL_VISIBILITY_LOCAL
      void eraseHandlers(void);
      TFEL_VISIBILITY_LOCAL
      static void treatAction(int);
      std::map<int,std::map<unsigned short,SignalHandler *> > callBacks;
      std::map<int,SignalHandler *> sHandlers;
      unsigned short handlerNbr;
    }; // end of struct SignalManager

  } // end of system
  
} // end of tfel

#endif /* _LIB_TFEL_SIGNALMANAGER_HXX */

