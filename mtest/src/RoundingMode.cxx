/*!
 * \file   RoundingMode.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 mai 2017
 */

#include<random>
#include<ostream>
#include<stdexcept>
#include"MFront/MFrontLogStream.hxx"
#include"MTest/RoundingMode.hxx"

namespace mtest{

  enum MTestRoundingMode{
    MTEST_UPWARD,
    MTEST_DOWNWARD,
    MTEST_TOWARDZERO,
    MTEST_TONEAREST,
    MTEST_RANDOM
  }; // end of enum MTestRoundingMode

  static MTestRoundingMode& getRoundingMode(){
    static MTestRoundingMode m = MTEST_TONEAREST;
    return m;
  } // end of getRoundingMode
  
  void setRoundingMode(const std::string& m)
  {
    if(m=="DownWard"){
      getRoundingMode() = MTEST_DOWNWARD;
    } else if(m=="UpWard"){
      getRoundingMode() = MTEST_UPWARD;
    } else if(m=="ToNearest"){
      getRoundingMode() = MTEST_TONEAREST;
    } else if(m=="TowardZero"){
      getRoundingMode() = MTEST_TOWARDZERO;
    } else if(m=="Random"){
      getRoundingMode() = MTEST_RANDOM;
    } else {
      throw(std::runtime_error("mtest::setRoundingMode: "
			       "unsupported rounding mode '"+m+"'"));
    }
    setRoundingMode();
  } // end of setRoundingMode

  void setRoundingMode()
  {
    auto report = [](const char *m){
      if(mfront::getVerboseMode()>=mfront::VERBOSE_DEBUG){
	mfront::getLogStream() << "mtest::setRoundingMode: "
	<< "setting rounding mode to '" << m << "'\n";
      }
    };
    if(getRoundingMode()==MTEST_DOWNWARD){
      report("DownWard");
      std::fesetround(FE_DOWNWARD);
    } else if(getRoundingMode()== MTEST_UPWARD){
      report("UpWard");
      std::fesetround(FE_UPWARD);
    } else if(getRoundingMode()== MTEST_TONEAREST){
      report("ToNearest");
      std::fesetround(FE_TONEAREST);
    } else if(getRoundingMode()== MTEST_TOWARDZERO){
      report("TowardZero");
      std::fesetround(FE_TOWARDZERO);
    } else {
      std::random_device rd;
      std::mt19937 rng(rd());
      std::uniform_int_distribution<int> d(0,3);
      switch(d(rng)){
      case 0:
	report("DownWard");
	std::fesetround(FE_DOWNWARD);
	break;
      case 1:
	report("UpWard");
	std::fesetround(FE_UPWARD);
	break;
      case 2:
	report("ToNearest");
	std::fesetround(FE_TONEAREST);
	break;
      default:
	report("TowardZero");
	std::fesetround(FE_TOWARDZERO);
      }
    }
  } // end of setRoundingMode
  
} // end of namespace mtest
