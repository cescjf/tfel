/*! 
 * \file  mfront/mtest/CastemEvolution.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"MTest/CastemEvolution.hxx"

namespace mtest{

  CastemEvolution::CastemEvolution(const std::string& l,
				   const std::string& fn,
				   const EvolutionManager& evm_)
    : evm(evm_)
  {
    using namespace tfel::system;
    using ELM = tfel::system::ExternalLibraryManager;
    auto& elm = ELM::getExternalLibraryManager();
    this->f = elm.getCastemFunction(l,fn);
    if(elm.getCastemFunctionNumberOfVariables(l,fn)!=0u){
      elm.getCastemFunctionVariables(this->vnames,l,fn);
      this->args.resize(this->vnames.size());
    }
  } // end of CastemEvolution::CastemEvolution

  real
  CastemEvolution::operator()(const real t) const
  {
    using namespace std;
    vector<string>::size_type i;
    for(i=0;i!=this->vnames.size();++i){
      auto pev = this->evm.find(vnames[i]);
      if(pev==this->evm.end()){
	throw(runtime_error("CastemEvolution::operator(): "
			    "can't evaluate argument '"+vnames[i]+"'"));
      } else {
	const Evolution& ev = *(pev->second);
	args[i] = ev(t);
      }
    }
    return this->f(&args[0]);
  } // end of CastemEvolution::operator()

  bool
  CastemEvolution::isConstant() const
  {
    using namespace std;
    vector<string>::size_type i;
    for(i=0;i!=this->vnames.size();++i){
      auto pev = this->evm.find(vnames[i]);
      if(pev==this->evm.end()){
	throw(runtime_error("CastemEvolution::operator(): "
			    "can't evaluate argument '"+vnames[i]+"'"));
      } else {
	const Evolution& ev = *(pev->second);
	if(!ev.isConstant()){
	  return false;
	}
      }
    }
    return true;
  } // end of CastemEvolution::isConstant

  void CastemEvolution::setValue(const real)
  {
    throw(std::runtime_error("CastemEvolution::setValue : "
			     "this method does not makes "
			     "sense for castem evolution"));
  }
  
  void CastemEvolution::setValue(const real,const real)
  {
    throw(std::runtime_error("CastemEvolution::setValue : "
			     "this method does not makes "
			     "sense for castem evolution"));
  }
  
  CastemEvolution::~CastemEvolution() = default;

}
