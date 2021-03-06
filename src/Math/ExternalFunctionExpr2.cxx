/*!
 * \file   src/Math/ExternalFunctionExpr2.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr2.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
      
      ExternalFunctionExpr2::ExternalFunctionExpr2(std::shared_ptr<ExternalFunction> ff,
						   std::vector<std::shared_ptr<Expr> >& fargs)
	: f(std::move(ff)),
	  args(fargs)	
      {
	using namespace std;
	if(f->getNumberOfVariables()!=args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr2::ExternalFunctionExpr2 : "
	      << "invalid number of arguments for function (" 
	      << this->args.size() << " given, "
	      << f->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
        }
      } // end of ExternalFunctionExpr2::ExternalFunctionExpr2

      double
      ExternalFunctionExpr2::getValue() const
      {
	using namespace std;
	using namespace tfel::math::parser;
	vector<shared_ptr<Expr> >::const_iterator p;
	vector<shared_ptr<Expr> >::size_type i;
	for(p=this->args.begin(),i=0u;p!=this->args.end();++p,++i){
	  const double val = (*p)->getValue();
	  this->f->setVariableValue(i,val);
	}
	return this->f->getValue();
      } // end of ExternalFunctionExpr2::getValue

      void
      ExternalFunctionExpr2::checkCyclicDependency(std::vector<std::string>& names) const
      {
	using namespace std;
	vector<shared_ptr<Expr> >::const_iterator p;
	vector<string> a;
	for(p=this->args.begin();p!=this->args.end();++p){
	  vector<string> n(a);
	  (*p)->checkCyclicDependency(n);
	  mergeVariablesNames(names,n);
	}
      } // end of ExternalFunctionExpr2::checkCyclicDependency

      void
      ExternalFunctionExpr2::getParametersNames(std::set<std::string>& p) const
      {
	using namespace std;
	vector<shared_ptr<Expr> >::const_iterator pa;
	for(pa=this->args.begin();pa!=this->args.end();++pa){
	  (*pa)->getParametersNames(p);
	}
      } // end of ExternalFunctionExpr2::getParametersNames(std::set<std::string>&) const;

      std::shared_ptr<Expr>
      ExternalFunctionExpr2::differentiate(const std::vector<double>::size_type pos,
					   const std::vector<double>& v) const
      {
	std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
        auto p = this->args.begin();
	unsigned short i = 0;
	if(this->args.empty()){
	  return std::make_shared<Number>(0.);
	}
	auto p3 = this->args.begin();
	auto p4 = nargs.begin();
        for(;p3!=this->args.end();++p3,++p4){
	  *p4 = (*p3)->clone(v);
	}
        auto df_ = std::make_shared<ExternalFunctionExpr2>(this->f->differentiate(i),nargs);
	std::shared_ptr<Expr> df  =
	  std::make_shared<BinaryOperation<OpMult>>(df_,(*p)->differentiate(pos,v));
	++p;
	++i;
        while(p!=this->args.end()){
	  df_  = std::make_shared<ExternalFunctionExpr2>(this->f->differentiate(i),nargs);
	  std::shared_ptr<Expr> df2 =
	    std::make_shared<BinaryOperation<OpMult>>(df_,(*p)->differentiate(pos,v));
	  df = std::make_shared<BinaryOperation<OpPlus>>(df,df2);
	  ++p;
	  ++i;
        }
	return df;
      } // end of ExternalFunctionExpr2::differentiate

      std::shared_ptr<Expr>
      ExternalFunctionExpr2::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
									     const std::vector<std::string>& params,
									     const std::map<std::string,
									     std::vector<double>::size_type>& pos) const
      {
	using namespace std;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->createFunctionByChangingParametersIntoVariables(v,params,pos);
	}
        return std::make_shared<ExternalFunctionExpr2>(this->f,nargs);
      } // end of ExternalFunctionExpr2::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<Expr>
      ExternalFunctionExpr2::clone(const std::vector<double>& v) const
      {
	using namespace std;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->clone(v);
	}
        return std::make_shared<ExternalFunctionExpr2>(this->f,nargs);	
      } // end of ExternalFunctionExpr2::clone

      std::shared_ptr<Expr>
      ExternalFunctionExpr2::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace std;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->resolveDependencies(v);
	}
        return std::make_shared<ExternalFunctionExpr2>(this->f->resolveDependencies(),nargs);	
      } // end of ExternalFunctionExpr2::resolveDependencies

      ExternalFunctionExpr2::~ExternalFunctionExpr2() = default;

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
