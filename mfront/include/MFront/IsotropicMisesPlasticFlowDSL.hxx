/*!
 * \file   mfront/include/MFront/IsotropicMisesPlasticFlowDSL.hxx
 * \brief  This file declares the IsotropicMisesPlasticFlowDSL class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX
#define LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX 

#include<string>
#include"MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront{

  /*!
   * \brief a dsl dedicated to isotropic mises plastic flows
   */
  struct IsotropicMisesPlasticFlowDSL
    : public IsotropicBehaviourDSLBase
  {
    //! \return the name of the dsl
    static std::string getName();
    //! \return a description of the dsl
    static std::string getDescription();
    //! constructor
    IsotropicMisesPlasticFlowDSL();

    virtual void
    endsInputFileProcessing() override;
    //! desctructor
    virtual ~IsotropicMisesPlasticFlowDSL();

  protected:

    virtual double getDefaultThetaValue() const override;
    
    virtual void
    writeBehaviourParserSpecificInitializeMethodPart(std::ostream&,
						     const Hypothesis) const override;

    virtual void
    writeBehaviourParserSpecificMembers(std::ostream&,
					const Hypothesis) const override;

    virtual void
    writeBehaviourIntegrator(std::ostream&,
			     const Hypothesis) const override;

    virtual void
    writeBehaviourComputeTangentOperator(std::ostream&,
					 const Hypothesis) const override;
  };

} // end of namespace mfront  

#endif /* LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX */

