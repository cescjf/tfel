/*!
 * \file   src/Math/Evaluator.cxx
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

#include<cmath>
#include<cassert>
#include<stdexcept>
#include<algorithm>
#include<iterator>
#include<sstream>
#include<cctype>

#include"TFEL/Math/power.hxx"
#include"TFEL/Math/Evaluator.hxx"

#ifdef __CYGWIN__
static bool isascii(const int c){
  return ((c>=0) && (c<128));
}
#endif

namespace tfel
{

  namespace math
  {

    void
    Evaluator::checkNotEndOfExpression(const std::string& method,
				       const std::string& error,
				       const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe)
    {
      if(p==pe){
	auto msg = method + " : unexpected end of line";
	if(!error.empty()){
	  msg += " ("+error+")";
	}
	throw(std::runtime_error(msg));
      }
    } // end of Evaluator::checkNotEndOfExpression

    void
    Evaluator::checkNotEndOfExpression(const std::string& method,
				       const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe)
    {
      Evaluator::checkNotEndOfExpression(method,"",p,pe);
    } // end of Evaluator::checkNotEndOfExpression

    void
    Evaluator::readSpecifiedToken(const std::string& method,
				  const std::string& value,
				  std::vector<std::string>::const_iterator& p,
				  const std::vector<std::string>::const_iterator pe)
    {
      Evaluator::checkNotEndOfExpression(method,"expected '"+value+"'",p,pe);
      if(*p!=value){
	auto msg = method;
	msg += " : unexpected token '"+*p+"'";
	msg += " (expected '"+value+"')";
	throw(std::runtime_error(msg));
      }
      ++p;
    } // end of Evaluator::readSpecifiedToken

    unsigned short
    Evaluator::convertToUnsignedShort(const std::string& method,
				      const std::string& value)
    {
      using namespace std;
      string::const_iterator ps;
      istringstream converter(value);
      for(ps=value.begin();ps!=value.end();++ps){
	if(!isdigit(*ps)){
	  throw(runtime_error(method+" : invalid entry"));
	}
      }
      unsigned short u;
      converter >> u;
      if(!converter||(!converter.eof())){
	string msg(method+" : ");
	msg += "not read value from token '"+value+"'";
	throw(runtime_error(msg));
      }
      return u;
    } // end of Evaluator::convertToUnsignedShort

    unsigned int
    Evaluator::convertToUnsignedInt(const std::string& method,
				    const std::string& value)
    {
      using namespace std;
      string::const_iterator ps;
      istringstream converter(value);
      for(ps=value.begin();ps!=value.end();++ps){
	if(!isdigit(*ps)){
	  throw(runtime_error(method+" : invalid entry"));
	}
      }
      unsigned int u;
      converter >> u;
      if(!converter||(!converter.eof())){
	string msg(method+" : ");
	msg += "not read value from token '"+value+"'";
	throw(runtime_error(msg));
      }
      return u;
    } // end of Evaluator::convertToUnsignedInt

    int
    Evaluator::convertToInt(const std::string& method,
			    const std::string& value)
    {
      using namespace std;
      string::const_iterator ps;
      istringstream converter(value);
      for(ps=value.begin();ps!=value.end();++ps){
	if(!isdigit(*ps)){
	  throw(runtime_error(method+" : invalid entry"));
	}
      }
      int u;
      converter >> u;
      if(!converter||(!converter.eof())){
	string msg(method+" : ");
	msg += "not read value from token '"+value+"'";
	throw(runtime_error(msg));
      }
      return u;
    } // end of Evaluator::convertToInt

    unsigned short
    Evaluator::readUnsignedShortValue(const std::string& method,
				      std::vector<std::string>::const_iterator& p,
				      const std::vector<std::string>::const_iterator pe)
    {
      Evaluator::checkNotEndOfExpression(method,"expected unsigned short value",p,pe);
      const auto nbr = convertToUnsignedShort(method,*p);
      ++p;
      return nbr;
    } // end of Evaluator::readUnsignedShortValue

    void
    Evaluator::checkParameterNumber(const std::vector<double>::size_type n,
				    const std::vector<double>::size_type p)
    {
      if(p!=n){
	std::string msg("Evaluator::checkParameterNumber : ");
	if(p==0){
	  msg += "no parameter given";
	} else {	
	  msg += "too many parameters given";
	}
	throw(std::runtime_error(msg));
      }
    } // end of Evaluator::checkParameterNubmer

    void
    Evaluator::checkVariableNumber(const std::vector<double>::size_type n,
				   const std::vector<double>::size_type p)
    {
      if(p!=n){
	std::string msg("Evaluator::checkVariableNumber : ");
	if(p==0){
	  msg += "no parameter given";
	} else {	
	  msg += "too many parameters given";
	}
	throw(std::runtime_error(msg));
      }
    } // end of Evaluator::checkVariableNubmer

    struct TFEL_VISIBILITY_LOCAL Evaluator::ExternalFunctionRegister
    {
      ExternalFunctionRegister() = default;
    }; // end of struct Evaluator::ExternalFunctionRegister

    Evaluator::ExternalFunctionRegister Evaluator::externalFunctionRegister;

#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    template<unsigned short N>
    static std::shared_ptr<tfel::math::parser::Expr>
    EvaluatorPowerFunctionGenerator(const std::shared_ptr<tfel::math::parser::Expr> e)
    {
      using namespace tfel::math::parser;
      using namespace tfel::math::stdfunctions;
      return std::make_shared<StandardFunction<power<N>>>(e);
    } // end of EvaluatorPowerFunctionGenerator
#endif

    static std::shared_ptr<tfel::math::parser::Expr>
    EvaluatorTreatPower(const std::vector<std::string>& params,
			std::vector<std::shared_ptr<tfel::math::parser::Expr> >& args)
    {
      using namespace tfel::math::parser;
      unsigned short nbr;
      Evaluator::checkParameterNumber(1,params.size());
      Evaluator::checkVariableNumber(1,args.size());
      nbr = Evaluator::convertToUnsignedShort("EvaluatorTreatPower",params[0]);
      switch (nbr){
      case 0:
	return std::make_shared<Number>(1.);
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      case 1:
	return EvaluatorPowerFunctionGenerator<1>(args[0]);
      case 2:
	return EvaluatorPowerFunctionGenerator<2>(args[0]);
      case 3:
	return EvaluatorPowerFunctionGenerator<3>(args[0]);
      case 4:
	return EvaluatorPowerFunctionGenerator<4>(args[0]);
      case 5:
	return EvaluatorPowerFunctionGenerator<5>(args[0]);
      case 6:
	return EvaluatorPowerFunctionGenerator<6>(args[0]);
      case 7:
	return EvaluatorPowerFunctionGenerator<7>(args[0]);
      case 8:
	return EvaluatorPowerFunctionGenerator<8>(args[0]);
      case 9:
	return EvaluatorPowerFunctionGenerator<9>(args[0]);
      case 10:
	return EvaluatorPowerFunctionGenerator<10>(args[0]);
      case 11:
	return EvaluatorPowerFunctionGenerator<11>(args[0]);
      case 12:
	return EvaluatorPowerFunctionGenerator<12>(args[0]);
      case 13:
	return EvaluatorPowerFunctionGenerator<13>(args[0]);
      case 14:
	return EvaluatorPowerFunctionGenerator<14>(args[0]);
      case 15:
	return EvaluatorPowerFunctionGenerator<15>(args[0]);
      case 16:
	return EvaluatorPowerFunctionGenerator<16>(args[0]);
#endif
      }
      throw(std::runtime_error("EvaluatorTreatPower: only exponent below 16 are supported"));
    } // end of EvaluatorTreatPower

    void
    Evaluator::treatDiff(std::vector<std::string>::const_iterator& p,
			 const std::vector<std::string>::const_iterator pe,
			 Evaluator::TGroup * const g,
			 const bool b)
    {
      using namespace tfel::math::parser;
      using size_type = std::vector<double>::size_type;
      auto throw_if = [](const bool c, const std::string& m){
	if(c){throw(std::runtime_error("Evaluator::treatDiff: "+m));}
      };
      unsigned short openedParenthesis=0u;
      int diffNumber = -1;
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff","(",p,pe);
      if(*p=="<"){
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff","expected number",p,pe);
	auto       ps  = p->begin();
	const auto pse = p->end();
	while(ps!=pse){
	  throw_if(!std::isdigit(*ps),"expected a number");
	  ++ps;
	}
	std::istringstream converter(*p);
	converter >> diffNumber;
	throw_if(diffNumber<=1,"order of differentiation must be greater or egal than 1");
	++p;
	Evaluator::readSpecifiedToken("Evaluator::treatDiff",">",p,pe);
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff","expected ')'",p,pe);
      }
      Evaluator::readSpecifiedToken("Evaluator::treatDiff","(",p,pe);
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",p,pe);
      auto f = std::string{};
      while(!((*p==",")&&(openedParenthesis==0u))){
	if(*p=="("){
	  ++openedParenthesis;
	}
	if(*p==")"){
	  throw_if(openedParenthesis==0,"unbalanced parenthesis");
	  --openedParenthesis;
	}
	f+=*p;
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",p,pe);
      }
      throw_if(f.empty(),"function definition is empty");
      ++p;
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
					 "expected variable name",p,pe);
      // now reading var list
      auto var = std::vector<std::string>{};
      while(*p!=")"){
	var.push_back(*p);
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
					   "expected  ',' or ')'",p,pe);
	if(*p!=")"){
	  throw_if(*p!=",","unexpected token '"+*p+"' (expected ',' or ')')");
	  ++p;
	  Evaluator::readSpecifiedToken("Evaluator::treatDiff",")",p,pe);
	}
      }
      throw_if(var.empty(),"empty var list");
      throw_if((var.size()>1)&&(diffNumber!=-1),
	       "only one variable name allowed when degree of differentiation specified");
      auto pev = [this,b,&f](){
	if(this->manager==nullptr){
	  if(b){
	    // variable names are fixed
	    return std::make_shared<Evaluator>(this->getVariablesNames(),f);
	  } else {
	    return std::make_shared<Evaluator>(f);
	  }
	}
	if(b){
	  // variable names are fixed
	  return std::make_shared<Evaluator>(this->getVariablesNames(),f,this->manager);
	}
	return std::make_shared<Evaluator>(f,this->manager);
      }();
      const auto& fvars = pev->getVariablesNames();
      for(const auto& v : var){
	throw_if(find(fvars.begin(),fvars.end(),v)==fvars.end(),
		 "expression '"+f+"'can't be differentiated by variable '"+v+"'");
      }
      if(diffNumber!=-1){
	var.resize(static_cast<size_type>(diffNumber),var[0]);
      }
      auto  args = std::vector<std::shared_ptr<Evaluator::TExpr>>{};
      for(const auto& v : fvars){
	if(!b){
	  this->registerVariable(v);
	}
	const auto pos = this->getVariablePosition(v);
	args.push_back(std::make_shared<Evaluator::TVariable>(pos,this->variables));
      }
      auto pvar = std::vector<size_type>{};
      for(const auto& v: var){
	pvar.push_back(static_cast<size_type>(std::find(fvars.begin(),fvars.end(),v)-fvars.begin()));
      }
      g->add(std::make_shared<Evaluator::TDifferentiatedFunctionExpr>(pev,args,pvar));
    } // end of Evaluator::treatDiff

    void
    Evaluator::FunctionGeneratorManager::checkName(const std::string& name) const
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b){throw(std::runtime_error("Evaluator::FunctionGeneratorManager::checkName: "+m));}
      };
      throw_if(this->fctGenerators.find(name)!=this->fctGenerators.end(),
	       "a function named '"+name+"' has already been declared");
      throw_if(this->bFctGenerators.find(name)!=this->bFctGenerators.end(),
	       "a binary function named '"+name+"' has already been declared");
      throw_if(this->extOpGenerators.find(name)!=this->extOpGenerators.end(),
	       "an external operator named '"+name+"' has already been declared.");
    } // end of FunctionGeneratorManager::checkName

    void
    Evaluator::FunctionGeneratorManager::insert(const std::string& name,
						const Evaluator::FunctionGenerator f)
    {
      this->checkName(name);
      this->fctGenerators.insert(make_pair(name,f));
    } // end of FunctionGeneratorManager::insert
    
    void
    Evaluator::FunctionGeneratorManager::insert(const std::string& name,
						const Evaluator::BinaryFunctionGenerator f)
    {
      this->checkName(name);
      this->bFctGenerators.insert(make_pair(name,f));
    } // end of FunctionGeneratorManager::insert

    void
    Evaluator::FunctionGeneratorManager::insert(const std::string& name,
						const Evaluator::ExternalFunctionGenerator f)
    {
      this->checkName(name);
      this->extOpGenerators.insert(make_pair(name,f));
    } // end of FunctionGeneratorManager::insert
    
    Evaluator::FunctionGeneratorManager::FunctionGeneratorManager()
    {
      this->fctGenerators.insert({{"exp",&StandardFctGenerator<exp>},
	                          {"abs",&StandardFctGenerator<fabs>},
				  {"sin",&StandardFctGenerator<sin>},
				  {"cos",&StandardFctGenerator<cos>},
				  {"tan",&StandardFctGenerator<tan>},
				  {"sqrt",&StandardFctGenerator<sqrt>},
				  {"log",&StandardFctGenerator<log>},
				  {"ln",&StandardFctGenerator<log>},
				  {"log10",&StandardFctGenerator<log10>},
				  {"asin",&StandardFctGenerator<asin>},
				  {"acos",&StandardFctGenerator<acos>},
				  {"atan",&StandardFctGenerator<atan>},
				  {"sinh",&StandardFctGenerator<sinh>},
				  {"cosh",&StandardFctGenerator<cosh>},
				  {"tanh",&StandardFctGenerator<tanh>},
				  {"H",&StandardFctGenerator<Evaluator::Heavyside>}});
      this->bFctGenerators.insert({{"max",&StandardBinaryFctGenerator<Evaluator::max>},
	                           {"min",&StandardBinaryFctGenerator<Evaluator::min>}});
      this->extOpGenerators.insert({"power",EvaluatorTreatPower});
    } // end of Evaluator::FunctionGeneratorManager::FunctionGeneratorManager

    Evaluator::FunctionGeneratorManager&
    Evaluator::getFunctionGeneratorManager()
    {
      static FunctionGeneratorManager m;
      return m;
    } // end of Evaluator::getFunctionGeneratorManager()

    bool Evaluator::isValidIdentifier(const std::string& s)
    {
      auto& f = Evaluator::getFunctionGeneratorManager();
      if(s=="diff"){
	return false;
      }
      if(f.extOpGenerators.find(s)!=f.extOpGenerators.end()){
	return false;
      }
      if(f.fctGenerators.find(s)!=f.fctGenerators.end()){
	return false;
      }
      if(f.bFctGenerators.find(s)!=f.bFctGenerators.end()){
	return false;
      }
      return true;
    } // end of Evaluator::isValidIdentifier

    double Evaluator::Heavyside(const double x)
    {
      return x<0 ? 0 : 1;
    } // end of Evaluator::Heavyside

    double Evaluator::max(const double a,const double b)
    {
      return std::max(a,b);
    } // end of Evaluator::max

    double Evaluator::min(const double a,const double b)
    {
      return std::min(a,b);
    } // end of Evaluator::min

    void Evaluator::checkCyclicDependency(std::vector<std::string>& names) const
    {
      if(this->expr==nullptr){
	throw(std::runtime_error("Evaluator::checkCyclicDependency: "
				 "uninitialized evaluator"));
      }
      this->expr->checkCyclicDependency(names);
    } // end of Evaluator::checkCyclicDependency

    void  Evaluator::checkCyclicDependency(const std::string& name) const
    {
      std::vector<std::string> names(1,name);
      if(this->expr==nullptr){
	throw(std::runtime_error("Evaluator::checkCyclicDependency: "
				 "uninitialized evaluator"));
      }
      this->expr->checkCyclicDependency(names);
    } // end of Evaluator::checkCyclicDependency

    void Evaluator::checkCyclicDependency() const
    {
      std::vector<std::string> names; 
      if(this->expr==nullptr){
	throw(std::runtime_error("Evaluator::checkCyclicDependency: "
				 "uninitialized evaluator"));
      }
     this->expr->checkCyclicDependency(names);
    } // end of Evaluator::checkCyclicDependency

    template<typename T>
    bool Evaluator::convert(const std::string& value)
    {
      std::istringstream is(value);
      T res;
      is >> res;
      if(!is||(!is.eof())){
	return false;
      }
      return true;
    } // end of convert
    
    bool Evaluator::isNumber(const std::string& value)
    {
      return convert<double>(value);
    } // end of Evaluator::isNumber

    std::vector<std::string>
    Evaluator::analyseParameters(std::vector<std::string>::const_iterator& p,
				 const std::vector<std::string>::const_iterator  pe)
    {
      using namespace std;
      vector<string> params;
      string::const_iterator ps;
      checkNotEndOfExpression("Evaluator::analyseParameters",p,pe);
      while(*p!=">"){
	// check parameter validity
	for(ps=p->begin();ps!=p->end();++ps){
	  if(!(isalpha(*ps)||isdigit(*ps))||(*ps=='_')||(*ps=='-')){
	    throw(std::runtime_error("Evaluator::analyseParameters: "
				     "invalid parameter '"+*p+"'"));
	  }
	}
	params.push_back(*p);
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::analyseParameters",p,pe);
	if(*p!=">"){
	  if(*p==","){
	    ++p;
	    Evaluator::checkNotEndOfExpression("Evaluator::analyseParameters",p,pe);
	  } else {
	    throw(std::runtime_error("Evaluator::analyseParameters: "
				     "unexpected token '"+*p+"' "
				     "(expected ',' or '>')"));
	  }
	}
      }
      ++p;
      return params;
    } // end of Evaluator::analyseParameters

    std::vector<std::shared_ptr<Evaluator::TExpr> >
    Evaluator::analyseArguments(const unsigned short nbr,
				std::vector<std::string>::const_iterator& p,
				const std::vector<std::string>::const_iterator  pe,
				const bool b)
    {
      auto res = std::vector<std::shared_ptr<Evaluator::TExpr>>{};
      if(nbr>0){
	unsigned short i;
	for(i=0;i!=nbr-1u;++i){
	  res.push_back(this->treatGroup(p,pe,b,","));
	  ++p;
	}
	res.push_back(this->treatGroup(p,pe,b,")"));
      }
      return res;
    } // end of Evaluator::analyseArguments

    std::vector<std::shared_ptr<Evaluator::TExpr> >
    Evaluator::analyseArguments(std::vector<std::string>::const_iterator& p,
				const std::vector<std::string>::const_iterator  pe,
				const bool b)
    {
      auto res = std::vector<std::shared_ptr<Evaluator::TExpr>>{};
      const auto nbr = this->countNumberOfArguments(p,pe);
      if(nbr>0){
	for(unsigned short i=0;i!=nbr-1u;++i){
	  res.push_back(this->treatGroup(p,pe,b,","));
	  ++p;
	}
	res.push_back(this->treatGroup(p,pe,b,")"));
      }
      return res;
    } // end of Evaluator::analyseArguments

    unsigned short
    Evaluator::countNumberOfArguments(std::vector<std::string>::const_iterator p,
				      const std::vector<std::string>::const_iterator pe)
    {
      using namespace std;
      unsigned short opened = 1;
      unsigned short nbr = 1;
      Evaluator::checkNotEndOfExpression("Evaluator::countNumberOfArguments",p,pe);
      if(*p==")"){
	return 0;
      }
      while(p!=pe){
	if(*p=="("){
	  ++opened;
	} else if(*p==")"){
	  --opened;
	  if(opened==0){
	    return nbr;
	  }
	} else if(*p==","){
	  if(opened==1){
	    ++nbr;
	  }
	}
	++p;
      }
      ostringstream msg;
      if(opened==1){
	msg << "Evaluator::countNumberOfArguments : group ended while " 
	    << "a parenthesis was still opened";
      } else {
	msg << "Evaluator::countNumberOfArguments : group ended while " 
	    << opened << " parenthesis were still opened";
      }
      throw(runtime_error(msg.str()));
    } // end of Evaluator::analyseArguments

    std::pair<bool,std::vector<std::string>::const_iterator>
    Evaluator::search(std::vector<std::string>::const_iterator p,
		      std::vector<std::string>::const_iterator pe,
		      const std::string& m,
		      const std::string& s)
    {
      unsigned short openedParenthesis = 0;
      bool test;
      if(s.empty()){
	test = (p!=pe);
      } else {
	test = (p!=pe)&&(*p!=s);
      }
      while(test){
	if(*p=="("){
	  ++openedParenthesis;
	}
	if(*p==")"){
	  if(openedParenthesis==0){
	    throw(std::runtime_error("Analyser::readNextGroup: "
				     "unbalanced parenthesis"));
	  }
	  --openedParenthesis;
	}
	if((*p==m)&&(openedParenthesis==0)){
	  return make_pair(true,p);
	}
	++p;
	if(s.empty()){
	  test = (p!=pe);
	} else {
	  test = (p!=pe)&&(*p!=s);
	}
      }
      return {false,p};
    } // end of Evaluator::search

    std::shared_ptr<Evaluator::TLogicalExpr>
    Evaluator::treatLogicalExpression(const std::vector<std::string>::const_iterator p,
				      const std::vector<std::string>::const_iterator pe,
				      const bool b)
    {
      using namespace std;
      using namespace tfel::math::parser;
      auto throw_if = [](const bool c, const std::string& m){
	if(c){throw(std::runtime_error("Evaluator::treatLogicalExpression: "+m));}
      };
      vector<string>::const_iterator pb(p);           
      vector<string>::const_iterator pbe(pe);
      checkNotEndOfExpression("Evaluator::treatLogicalExpression",pb,pbe);
      auto pa = this->search(pb,pbe,"&&","");
      auto po = this->search(pb,pbe,"||","");
      if((pa.second!=pe)||(po.second!=pe)){
	auto pt = pa.first ? pa.second : po.second;
	throw_if(pt==pb,"no left logical expression");
	throw_if(pt+1==pbe,"no right logical expression");
	auto lo = this->treatLogicalExpression(pb,pt,b);
	auto ro = this->treatLogicalExpression(pt+1,pbe,b);
	if(*pt=="&&"){
	  return std::make_shared<TLogicalBinaryOperation<OpAnd>>(lo,ro);
	} else if (*pt=="||"){
	  return std::make_shared<TLogicalBinaryOperation<OpOr>>(lo,ro);
	} else {
	  throw_if(true,"unkwown operator '"+*pt+"'");
	}
      }
      if(*pb=="("){
	--pbe;
	Evaluator::checkNotEndOfExpression("Evaluator::treatLogicalExpression",pb,pbe);
	throw_if(*pbe!=")","unmatched parenthesis");
	++pb;
	return this->treatLogicalExpression(pb,pbe,b);
      }
      if(*pb=="!"){
	++pb;
	return std::make_shared<TNegLogicalExpr>(this->treatLogicalExpression(pb,pbe,b));
      }
      return this->treatLogicalExpression2(pb,pbe,b);
    } // end of Evaluator::treatLogicalExpression
    
    std::vector<std::string>::const_iterator
    Evaluator::searchComparisonOperator(const std::vector<std::string>::const_iterator pb,
					const std::vector<std::string>::const_iterator pe)
    {
      using namespace std;
      unsigned short openedParenthesis = 0;
      bool found = false;
      bool test;
      map<string,Evaluator::ExternalFunctionGenerator>::const_iterator peo;
      vector<string>::const_iterator prev;
      vector<string>::const_iterator p;
      vector<string>::const_iterator po;
      p = pb;
      while(p!=pe){
	if(*p=="("){
	  ++openedParenthesis;
	}
	if(*p==")"){
	  if(openedParenthesis==0){
	    string msg("Analyser::readNextGroup : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	if(openedParenthesis==0){
	  if((*p=="==")||(*p=="<=")||
	     (*p==">")||(*p==">=")){
	    if(found){
	      ostringstream msg;
	      msg << "Evaluator::treatLogicalExpression : "
		  << "more than one logical operator found (error while parsing '";
	      copy(pb,pe,ostream_iterator<string>(msg," "));
	      msg << "')";
	      throw(runtime_error(msg.str()));
	    }
	    po = p;
	    found = true;
	  }
	  if(*p=="<"){
	    if(p==pb){
	      ostringstream msg;
	      msg << "Evaluator::treatLogicalExpression : "
		  << "no left part to logical operator (error while parsing '";
	      copy(p,pe,ostream_iterator<string>(msg," "));
	      msg << "')";
	      throw(runtime_error(msg.str()));
	    }
	    prev = p-1;
	    peo = Evaluator::getFunctionGeneratorManager().extOpGenerators.find(*prev);
	    if(peo!=Evaluator::getFunctionGeneratorManager().extOpGenerators.end()){
	      ++p;
	      Evaluator::checkNotEndOfExpression("Evaluator::searchComparisonOperator",p,pe);
	      test=true;
	      while(test){
		if(*p==">"){
		  test=false;
		} 
		++p;
		Evaluator::checkNotEndOfExpression("Evaluator::searchComparisonOperator",p,pe);
	      }
	      if(*p!="("){
		string msg("Evaluator::searchComparisonOperator : ");
		msg += "expected '(', read '"+*p+"'";
		throw(runtime_error(msg));
	      }
	      ++openedParenthesis;
	    } else {
	      if(found){
		ostringstream msg;
		msg << "Evaluator::treatLogicalExpression : "
		    << "more than one logical operator found (error while parsing '";
		copy(pb,pe,ostream_iterator<string>(msg," "));
		msg << "')";
		throw(runtime_error(msg.str()));
	      }
	      po = p;
	      found = true;
	    }
	  }
	}
	++p;
      }
      if(!found){
	ostringstream msg;
	msg << "Evaluator::treatLogicalExpression : "
	    << "no logical operator found (error while parsing '";
	copy(pb,pe,ostream_iterator<string>(msg," "));
	msg << "')";
	throw(runtime_error(msg.str()));
      }
      if(po==pb){
	ostringstream msg;
	msg << "Evaluator::treatLogicalExpression : "
	    << "no left part to logical operator (error while parsing '";
	copy(p,pe,ostream_iterator<string>(msg," "));
	msg << "')";
	throw(runtime_error(msg.str()));
      }
      if(po+1==pe){
	ostringstream msg;
	msg << "Evaluator::treatLogicalExpression : "
	    << "no right part to logical operator (error while parsing '";
	copy(pb,pe,ostream_iterator<string>(msg," "));
	msg << "')";
	throw(runtime_error(msg.str()));
      }
      return po;
    } // end of Evaluator::searchComparisonOperator

    std::shared_ptr<Evaluator::TLogicalExpr>
    Evaluator::treatLogicalExpression2(const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe,
				       const bool b)
    {
      using namespace tfel::math::parser;
      auto plo = this->searchComparisonOperator(p,pe);
      auto tmp2 = p;
      auto loexpr = this->treatGroup(tmp2,plo,b,"");
      auto tmp3 = plo+1;
      auto roexpr = this->treatGroup(tmp3,pe,b,"");
      if(*plo=="=="){
	return std::make_shared<TLogicalOperation<OpEqual>>(loexpr,roexpr);
      } else if(*plo==">"){
	return std::make_shared<TLogicalOperation<OpGreater>>(loexpr,roexpr);
      } else if(*plo==">="){
	return std::make_shared<TLogicalOperation<OpGreaterOrEqual>>(loexpr,roexpr);
      } else if(*plo=="<"){
	return std::make_shared<TLogicalOperation<OpLesser>>(loexpr,roexpr);
      } else if(*plo=="<="){
	return std::make_shared<TLogicalOperation<OpLesserOrEqual>>(loexpr,roexpr);
      }
      throw(std::runtime_error("Evaluator::treatLogicalExpression2 : "
			       "unsupported logical operator '"+*plo+"'"));
    } // end of Evaluator::treatLogicalExpression2

    std::shared_ptr<Evaluator::TExpr>
    Evaluator::treatGroup(std::vector<std::string>::const_iterator& p,
			  const std::vector<std::string>::const_iterator  pe,
			  const bool b,
			  const std::string& s)
    {
      using namespace std;
      using namespace tfel::math::parser;
      auto throw_if = [](const bool c, const std::string& m){
	if(c){throw(std::runtime_error("Evaluator::treatGroup: "+m));}
      };
      Evaluator::checkNotEndOfExpression("Evaluator::treatGroup",p,pe);
      throw_if(*p==s,"empty group");
      auto res = this->search(p,pe,"?",s);
      if(res.first){
	throw_if(p==res.second,"no conditional expression preceeding '?' character");
	throw_if(res.second+1==pe,"nothing expression following '?' character");
	throw_if(this->search(res.second+1,pe,"?",s).first,
		 "imbricated conditional expression are not supported "
		 "(use parenthesis as as workaround)");
	const auto res2 = this->search(res.second+1,pe,":",s);
	throw_if(!res2.first,"no matching ':' character to '?' character");
	throw_if(res.second+1==res2.second,"empty left conditional expression");
	throw_if(res2.second+1==pe,"empty right conditional expression");
	auto l = this->treatLogicalExpression(p,res.second,b);
	auto tmp2 = res.second+1;
	auto lexpr = this->treatGroup(tmp2,res2.second,b,"");
	p = res2.second+1;
	auto rexpr = this->treatGroup(p,pe,b,s);
	return std::make_shared<TConditionalExpr>(l,lexpr,rexpr);
      }
      return this->treatGroup2(p,pe,b,s);
    } // end of Evaluator::treatGroup

    std::shared_ptr<Evaluator::TExpr>
    Evaluator::treatGroup2(std::vector<std::string>::const_iterator& p,
			   std::vector<std::string>::const_iterator  pe,
			   const bool b,
			   const std::string& s)
    {
      using namespace std;
      map<string,Evaluator::FunctionGenerator>::const_iterator p2;
      map<string,Evaluator::BinaryFunctionGenerator>::const_iterator p3;
      map<string,Evaluator::ExternalFunctionGenerator>::const_iterator p4;
      assert(p!=pe);
      auto g = std::make_shared<TGroup>();
      bool test;
      if(s.empty()){
	test = (p!=pe);
      } else {
	test = (p!=pe)&&(*p!=s);
      }
      while(test){
	if(*p=="diff"){
	  ++p;
	  this->treatDiff(p,pe,g.get(),b);
	} else if((p4=Evaluator::getFunctionGeneratorManager().extOpGenerators.find(*p))!=
		  Evaluator::getFunctionGeneratorManager().extOpGenerators.end()){
	  ++p;
	  Evaluator::checkNotEndOfExpression("Evaluator::treatGroup2",p,pe);
	  if(*p=="<"){
	    ++p;
	    const auto& params = this->analyseParameters(p,pe);
	    Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	    auto args   = this->analyseArguments(p,pe,b);
	    g->add(std::make_shared<TExternalOperator>(p4->second,params,args));
	  } else {
	    Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	    vector<string> params;
	    auto args = this->analyseArguments(p,pe,b);
	    g->add(std::make_shared<TExternalOperator>(p4->second,params,args));
	  }
	} else if((p3=Evaluator::getFunctionGeneratorManager().bFctGenerators.find(*p))!=
		  Evaluator::getFunctionGeneratorManager().bFctGenerators.end()){
	  // call to binary function
	  ++p;
	  Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	  const auto args = this->analyseArguments(2,p,pe,b);
	  g->add(std::make_shared<TBinaryFunction>(p3->second,args[0],args[1]));
	} else if((p2=Evaluator::getFunctionGeneratorManager().fctGenerators.find(*p))!=
		  Evaluator::getFunctionGeneratorManager().fctGenerators.end()){
	  // call to function
	  ++p;
	  Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	  g->add(std::make_shared<TFunction>(p2->second,this->treatGroup(p,pe,b)));
	} else if(isNumber(*p)){
	  // number
	  istringstream converter(*p);
	  double value;
	  converter >> value;
	  g->add(std::make_shared<TNumber>(value));
	} else if(*p=="("){
	  ++p;
	  // begin group
	  g->add(this->treatGroup(p,pe,b));
	} else if(*p=="+"){
	  g->add(std::make_shared<TOperator>("+"));
	} else if(*p=="-"){
	  g->add(std::make_shared<TOperator>("-"));
	} else if(*p=="*"){
	  g->add(std::make_shared<TOperator>("*"));
	} else if(*p=="/"){
	  g->add(std::make_shared<TOperator>("/"));
	} else if(*p=="**"){
	  g->add(std::make_shared<TOperator>("**"));
	} else {
	  auto pt = p;
	  ++pt;
	  if((pt!=pe)&&(*pt=="(")){
	    if(this->manager!=nullptr){
	      this->addExternalFunctionToGroup(g.get(),p,pe,b);
	    } else {
	      string msg("Evaluator::treatGroup2 : unknown function '"+*p+"'");
	      throw(runtime_error(msg));
	    }
	  } else {
	    if(b){
	      // variable name is fixed
	      if(this->positions.find(*p)==this->positions.end()){
		vector<shared_ptr<Evaluator::TExpr> > args;
		if(this->manager==nullptr){
		  string msg("Evaluator::treatGroup2 : ");
		  msg += "unknown variable '"+*p+"'";
		  throw(runtime_error(msg));
		}
		g->add(std::make_shared<TExternalFunctionExpr>(*p,args,this->manager));

	      } else {
		g->add(std::make_shared<TVariable>(*p,*this));
	      }
	    } else {
	      g->add(std::make_shared<TVariable>(*p,*this));
	    }
	  }
	}
	++p;
	if(s.empty()){
	  test = (p!=pe);
	} else {
	  test = (p!=pe)&&(*p!=s);
	}
      }
      if((!s.empty())&&(p==pe)){
	string msg("Evaluator::treatGroup2 : unterminated group (group began with '"+s+"').");
	throw(runtime_error(msg));
      }
      return g;
    } // end of Evaluator::treatGroup2

    std::vector<double>::size_type
    Evaluator::registerVariable(const std::string& vname)
    {
      const auto p  = this->positions.find(vname); 
      const auto pe = this->positions.end();
      if(p!=pe){
	return p->second;
      }
      const auto pos = this->variables.size();
      this->positions.insert({vname,pos});
      this->variables.resize(pos+1u);
      return pos;
    } // end of Evaluator::registerVariable

    void Evaluator::setVariableValue(const std::string& vname,
				     const double value)
    {
      auto p = this->positions.find(vname);
      if(p==this->positions.end()){
	throw(std::runtime_error("Evaluator::setVariableValue: "
				 "variable '"+vname+ "' does not exist"));
      }
      this->variables[p->second] = value;
    }//end of Evaluator::setVariableValue

    void Evaluator::setVariableValue(const char* const vname,
				     const double value)
    {
      auto p = this->positions.find(vname);
      if(p==this->positions.end()){
	throw(std::runtime_error("Evaluator::setVariableValue: "
				 "variable '"+std::string(vname)+"' does not exist"));
      }
      this->variables[p->second] = value;
    }//end of Evaluator::setVariableValue
    
    void
    Evaluator::setVariableValue(const std::vector<double>::size_type pos,
				const double value)
    {
      if(pos>=this->variables.size()){
	std::ostringstream msg;
	msg << "Evaluator::setVariableValue : position '" << pos << "' is invalid ";
	if(this->variables.empty()){
	  msg << "(function has no variable).";
	} else if(this->variables.size()==1){
	  msg << "(function has one variable).";
	} else {
	  msg << "(function has only '" <<  this->variables.size() << "' variable(s)).";
	}
	throw(std::runtime_error(msg.str()));
      }
      this->variables[pos] = value;
    }//end of Evaluator::setVariableValue

    double Evaluator::getValue() const
    {
      if(this->expr==nullptr){
	throw(std::runtime_error("Evaluator::getValue: "
				 "uninitialized expression."));
      }
      return this->expr->getValue();
    }//end of Evaluator::getValue

    std::vector<std::string>
    Evaluator::getVariablesNames() const
    {
      auto res = std::vector<std::string>{};
      res.resize(this->positions.size());
      auto i = std::vector<std::string>::size_type{};
      for(const auto& p : this->positions){
	res[i] = p.first;
	++i;
      }
      return res;
    } // end of Evaluator::getVariablesNames

    std::vector<double>::size_type
    Evaluator::getNumberOfVariables() const
    {
      return this->variables.size();
    } // end of Evaluator::getNumberOfVariables() const;

    void Evaluator::analyse(const std::string& f,
			    const bool b)
    {
      tfel::math::parser::EvaluatorBase::analyse(f);
      auto       p  = this->tokens.cbegin(); 
      const auto pe = this->tokens.cend();
      auto g = this->treatGroup(p,pe,b,"");
      g->reduce();
      this->expr = g->analyse();
    }

    void
    Evaluator::addExternalFunctionToGroup(TGroup * const g,
					  std::vector<std::string>::const_iterator& p,
					  const std::vector<std::string>::const_iterator pe,
					  const bool b)
    {
      const auto fname = *p;
      ++p;
      Evaluator::readSpecifiedToken("Evaluator::addExternalFunctionToGroup","(",p,pe);
      auto args = this->analyseArguments(p,pe,b);
      g->add(std::make_shared<TExternalFunctionExpr>(fname,args,this->manager));
    } // end of Evaluator::addExternalFunctionToGroup

    Evaluator::Evaluator() = default;

    Evaluator::Evaluator(const Evaluator& src)
      : EvaluatorBase(src),
	variables(src.variables),
	positions(src.positions)
    {
      this->manager = src.manager;
      if(src.expr!=nullptr){
	this->expr = src.expr->clone(this->variables);
      }
    } // end of Evaluator::Evaluator

    Evaluator& Evaluator::operator = (const Evaluator& src)
    {
      if(this!=&src){
	this->variables = src.variables;
	this->positions = src.positions;
	this->manager   = src.manager;
	if(src.expr!=nullptr){
	  this->expr = src.expr->clone(this->variables);
	} else {
	  this->expr.reset();
	}
      }
      return *this;
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::string& f)
    {
      this->setFunction(f);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::vector<std::string>& vars,
			 const std::string& f)
    {
      this->setFunction(vars,f);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::string& f,
			 std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      this->setFunction(f,m);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::vector<std::string>& vars,
			 const std::string& f,
			 std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      this->setFunction(vars,f,m);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const double v)
    {
      this->expr = std::make_shared<parser::Number>(v);
    } // end of Evaluator::Evaluator

    void Evaluator::clear(){
      this->variables.clear();
      this->positions.clear();
      this->expr.reset();
      this->manager.reset();
    }
    
    void Evaluator::setFunction(const std::string& f)
    {
      this->clear();
      this->analyse(f,false);
    } // end of Evaluator::setFunction

    void Evaluator::setFunction(const std::vector<std::string>& vars,
				const std::string& f)
    {
      this->clear();
      this->variables.resize(vars.size());
      auto pos = std::vector<double>::size_type{0u};
      for(const auto& v: vars){
	if(this->positions.find(v)!=this->positions.end()){
	  throw(std::runtime_error("Evaluator::setFunction: "
				   "variable '"+v+"' multiply defined."));
	}
	if(!Evaluator::isValidIdentifier(v)){
	  throw(std::runtime_error("Evaluator::setFunction: "
				   "variable '"+v+"' is not valid."));
	}
	this->positions[v]=pos;
	++pos;
      }
      this->analyse(f,true);
    } // end of Evaluator::setFunction

    void
    Evaluator::setFunction(const std::string& f,
			   std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      this->clear();
      this->manager = m;
      this->analyse(f,false);
    } // end of Evaluator::setFunction

    void
    Evaluator::setFunction(const std::vector<std::string>& vars,
			   const std::string& f,
			   std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      auto throw_if = [](const bool c, const std::string& msg){
	if(c){throw(std::runtime_error("Evaluator::treatGroup: "+msg));}
      };
      this->clear();
      this->manager = m;
      this->variables.resize(vars.size());
      auto pos = std::vector<double>::size_type{};
      for(const auto& v : vars){
	throw_if(this->positions.find(v)!=this->positions.end(),
		 "variable '"+v+"' multiply defined");
	throw_if(!Evaluator::isValidIdentifier(v),
		 "variable '"+v+"' is not valid.");
	this->positions[v]=pos;
	++pos;
      }
      this->analyse(f,true);
    } // end of Evaluator::setFunction
    
    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::differentiate(const std::vector<double>::size_type pos) const
    {
      auto pev = std::make_shared<Evaluator>();
      if(this->expr==nullptr){
	throw(std::runtime_error("Evaluator::differentiate: "
				 "uninitialized expression."));
      }
      pev->variables.resize(this->variables.size());
      pev->positions = this->positions;
      if(this->variables.empty()){
	// no variable
	pev->expr = std::make_shared<parser::Number>(0.);
      } else {
	if(pos>=this->variables.size()){
	  std::ostringstream msg;
	  msg << "Evaluator::differentiate : position '" << pos << "' is invalid ";
	  if(this->variables.empty()){
	    msg << "(function has no variable).";
	  } else if(this->variables.size()==1){
	    msg << "(function has one variable).";
	  } else {
	    msg << "(function has only '" <<  this->variables.size() << "' variable(s)).";
	  }
	  throw(std::runtime_error(msg.str()));
	}
	pev->expr = this->expr->differentiate(pos,pev->variables);
      }
      return pev;
    } // end of Evaluator::differentiate

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::differentiate(const std::string& v) const
    {
      return this->differentiate(this->getVariablePosition(v));
    } // end of std::shared_ptr<ExternalFunction>

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::resolveDependencies() const
    {
      this->checkCyclicDependency();
      auto f = std::make_shared<Evaluator>(*this);
      f->expr = f->expr->resolveDependencies(f->variables);
      return f;
    } // end of Evaluator::resolveDependencies() const

    void Evaluator::removeDependencies()
    {
      this->checkCyclicDependency();
      this->expr = this->expr->resolveDependencies(this->variables);
    } // end of Evaluator::removeDependencies() const

    std::shared_ptr<tfel::math::parser::ExternalFunctionManager>
    Evaluator::getExternalFunctionManager()
    {
      return this->manager;
    } // end of Evaluator::getExternalFunctionManager
    
    std::vector<double>::size_type
    Evaluator::getVariablePosition(const std::string& name) const
    {
      const auto p = this->positions.find(name);
      if(p==this->positions.end()){
	throw(std::runtime_error("Evaluator::getVariablePosition: "
				 "unknown variable '"+name+"'"));
      }
      return p->second;
    } // end of Evaluator::getVariablePosition(const std::string&)

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::createFunctionByChangingParametersIntoVariables(std::vector<std::string>& nparams,
							       const std::vector<double>&,
							       const std::vector<std::string>& params,
							       const std::map<std::string,
							       std::vector<double>::size_type>&) const
    {
      std::set<std::string> ev_params;
      nparams.clear();
      this->getParametersNames(ev_params);
      for(const auto& p:params){
	if(ev_params.find(p)!=ev_params.end()){
	  if(std::find(nparams.begin(),nparams.end(),p)==nparams.end()){
	    nparams.push_back(p);
	  }
	}
      }
      return this->createFunctionByChangingParametersIntoVariables(nparams);
    } // end of Evaluator::createFunctionByChangingParametersIntoVariables

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::createFunctionByChangingParametersIntoVariables(const std::vector<std::string>& params) const
    {
      using namespace tfel::math::parser;
      auto throw_if = [](const bool b, const std::string& m){
	if(b){throw(std::runtime_error("Evaluator::createFunctionByChanging"
				       "ParametersIntoVariables: "+m));}
      };
      std::set<std::string> ev_params;
      throw_if(this->variables.size()!=this->positions.size(),
	       "internal error: variables size is not equal to the positions size");
      this->getParametersNames(ev_params);
      for(const auto& p : params){
	throw_if(ev_params.find(p)==ev_params.end(),"no parameter '"+p+"'");
	throw_if(this->positions.find(p)!=this->positions.end(),
		 "'"+p+"' is alredy a variable of this function");
      }
      auto  pev = std::make_shared<Evaluator>();
      pev->variables.resize(this->variables.size()+params.size());
      pev->positions = this->positions;
      auto i=this->variables.size();
      for(const auto& p : params){
	throw_if(!(pev->positions.insert({p,i}).second),
		 "internal error (variable '"+p+"' alredy declared)");
	++i;
      }
      pev->manager = this->manager;
      pev->expr = this->expr->createFunctionByChangingParametersIntoVariables(pev->variables,
									      params,pev->positions);      
      return pev;
    } // end of Evaluator::createFunctionByChangingParametersIntoVariables

    void Evaluator::getParametersNames(std::set<std::string>& n) const
    {
      if(this->expr==nullptr){
	throw(std::runtime_error("Evaluator::getParametersNames: "
				 "uninitialized evaluator"));
      }
      return this->expr->getParametersNames(n);
    } // end of Evaluator::getParametersNames

    Evaluator::~Evaluator() = default;
    
  } // end of namespace math
  
} // end of namespace tfel
