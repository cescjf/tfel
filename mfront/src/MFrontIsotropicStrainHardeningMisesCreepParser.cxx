/*!
 * \file   MFrontIsotropicStrainHardeningMisesCreepParser.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 jui 2007
 */

#include<string>
#include<stdexcept>

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontIsotropicStrainHardeningMisesCreepParser.hxx"

namespace mfront{

  MFrontIsotropicStrainHardeningMisesCreepParser::MFrontIsotropicStrainHardeningMisesCreepParser()
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    // Default state vars
    this->registerVariable("p");
    this->registerVariable("dp");
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->stateVarsHolder.push_back(VarHandler("StrainStensor","eel",1u,0u));
    this->stateVarsHolder.push_back(VarHandler("strain","p",1u,0u));
    this->glossaryNames.insert(MVType("eel","ElasticStrain"));
    this->glossaryNames.insert(MVType("p","EquivalentViscoplasticStrain"));
    // default local vars
    this->registerVariable("f");
    this->registerVariable("df_dseq");
    this->registerVariable("df_dp");
    this->registerVariable("se");
    this->registerVariable("seq");
    this->registerVariable("seq_e");
    this->registerVariable("n");
    this->registerVariable("mu_3");
    this->registerVariable("p_");
    this->localVarsHolder.push_back(VarHandler("DstrainDt","f",1u,0u));
    this->localVarsHolder.push_back(VarHandler("DF_DSEQ_TYPE","df_dseq",1u,0u));
    this->localVarsHolder.push_back(VarHandler("DstrainDt","df_dp",1u,0u));
    this->localVarsHolder.push_back(VarHandler("StressStensor","se",1u,0u));
    this->localVarsHolder.push_back(VarHandler("stress","seq",1u,0u));
    this->localVarsHolder.push_back(VarHandler("stress","seq_e",1u,0u));
    this->localVarsHolder.push_back(VarHandler("StrainStensor","n",1u,0u));
    this->localVarsHolder.push_back(VarHandler("strain","p_",1u,0u));
  }

  std::string
  MFrontIsotropicStrainHardeningMisesCreepParser::getName(void)
  {
    return "IsotropicStrainHardeningMisesCreep";
  }

  std::string
  MFrontIsotropicStrainHardeningMisesCreepParser::getDescription()
  {
    return "this parser is used for standard strain hardening creep behaviours "
           "of the form dp/dt=f(s,p) where p is the equivalent creep strain "
           "and s the equivalent mises stress";
  } // end of MFrontIsotropicStrainHardeningMisesCreepParser::getDescription

  void
  MFrontIsotropicStrainHardeningMisesCreepParser::writeBehaviourParserSpecificMembers(void)
  {
    using namespace std;

    this->checkBehaviourFile();

    if(this->flowRule.empty()){
      string msg("MFrontIsotropicStrainHardeningMisesCreepParser::writeBehaviourParserSpecificMembers : ");
      msg += "no flow rule declared (use the @FlowRule directive)";
      throw(runtime_error(msg));
    }

    this->behaviourFile << "void computeFlow(){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using namespace tfel::material;\n";
    writeMaterialLaws("MFrontIsotropicStrainHardeningMisesCreepParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);
    this->behaviourFile << this->flowRule << endl;
    this->behaviourFile << "}\n\n";

    this->behaviourFile << "bool NewtonIntegration(void){\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "unsigned int iter;\n";
    this->behaviourFile << "bool converge=false;\n";
    this->behaviourFile << "bool inversible=true;\n";
    this->behaviourFile << "strain newton_f;\n";
    this->behaviourFile << "strain newton_df;\n";
    this->behaviourFile << "real newton_epsilon = 100*std::numeric_limits<real>::epsilon();\n";
    this->behaviourFile << "stress mu_3 = 3*(this->mu);\n";
    this->behaviourFile << "";
    this->behaviourFile << "iter=0;\n";    
    this->behaviourFile << "this->p_=this->p+this->dp;\n"; 
    this->behaviourFile << "while((converge==false)&&\n";
    this->behaviourFile << "(iter<(" << this->className << "::iterMax))&&\n";
    this->behaviourFile << "(inversible==true)){\n";
    this->behaviourFile << "this->seq=std::max(this->seq_e-mu_3*("
			<< this->className << "::theta)*(this->dp),real(0.f));\n";
    this->behaviourFile << "this->computeFlow();\n";
    this->behaviourFile << "newton_f  = this->dp - (this->f)*(this->dt);\n";
    this->behaviourFile << "newton_df = 1-(" << this->className << "::theta)*(this->dt)*((this->df_dp)-mu_3*(this->df_dseq));\n";
    this->behaviourFile << "if(std::abs(base_cast(newton_df))";
    this->behaviourFile << ">newton_epsilon){\n";
    this->behaviourFile << "this->dp -= newton_f/newton_df;\n";
    this->behaviourFile << "this->p_=this->p + (";
    this->behaviourFile << this->className << "::theta)*(this->dp);\n";    
    this->behaviourFile << "iter+=1;\n";
    this->behaviourFile << "converge = (std::abs(tfel::math::base_cast(newton_f))<";
    this->behaviourFile << "(" << this->className << "::epsilon));\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "inversible=false;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(inversible==false){\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n\n";
    
    this->behaviourFile << "if(iter==" << this->className << "::iterMax){\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  } // end of writeBehaviourParserSpecificMembers

  void MFrontIsotropicStrainHardeningMisesCreepParser::writeBehaviourIntegrator(void)
  {
    using namespace std;
    std::vector<BoundsDescription>::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "bool\n";
    this->behaviourFile << "integrate(void){\n";
    this->behaviourFile << "if(!this->NewtonIntegration()){\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->deel = this->deto-(this->dp)*(this->n);\n";
    this->behaviourFile << "this->updateStateVars();\n";
    this->behaviourFile << "this->sig  = (this->lambda)*trace(this->eel)*StrainStensor::Id()+2*(this->mu)*(this->eel);\n";
    this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    for(p  = this->boundsDescriptions.begin();
	p != this->boundsDescriptions.end();++p){
      if(p->varCategory==BoundsDescription::StateVar){
	p->writeBoundsChecks(this->behaviourFile);
      }
    }
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  }

  void
  MFrontIsotropicStrainHardeningMisesCreepParser::writeBehaviourParserSpecificConstructorPart(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    if(this->varNames.find("young")==this->varNames.end()){
      string msg("MFrontIsotropicStrainHardeningMisesCreepParser");
      msg+="::writeBehaviourParserSpecificConstructorPart : ";
      msg += "young (the young modulus) has not been defined.";
      throw(runtime_error(msg));
    }
    if(this->varNames.find("nu")==this->varNames.end()){
      string msg("MFrontIsotropicStrainHardeningMisesCreepParser");
      msg += "::writeBehaviourParserSpecificConstructorPart : ";
      msg += "nu (the poisson ratio) has not been defined.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "this->se=2*(this->mu)*(tfel::math::deviator(this->eel+(";
    this->behaviourFile << this->className;
    this->behaviourFile << "::theta)*(this->deto)));\n";
    this->behaviourFile << "this->seq_e = sigmaeq(this->se);\n";
    this->behaviourFile << "if(this->seq_e>100*std::numeric_limits<stress>::epsilon()){\n";
    this->behaviourFile << "this->n = 1.5f*(this->se)/(this->seq_e);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "this->n = StrainStensor(strain(0));\n";
    this->behaviourFile << "}\n";
  } // end of MFrontIsotropicStrainHardeningMisesCreepParser::writeBehaviourParserSpecificConstructorPart

  MFrontIsotropicStrainHardeningMisesCreepParser::~MFrontIsotropicStrainHardeningMisesCreepParser()
  {} // end of ~MFrontIsotropicStrainHardeningMisesCreepParser

} // end of namespace mfront


