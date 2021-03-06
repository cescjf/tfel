/*!
 * \file   mfront/include/MFront/IsotropicStrainHardeningMisesCreepDSL.hxx
 * \brief  This file declares the IsotropicStrainHardeningMisesCreepDSL class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_HXX
#define LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_HXX 

#include<string>
#include"MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront{

  struct IsotropicStrainHardeningMisesCreepDSL
    : public IsotropicBehaviourDSLBase
  {
    IsotropicStrainHardeningMisesCreepDSL();

    static std::string getName();

    static std::string getDescription();
    
    virtual void
    endsInputFileProcessing() override;

    virtual ~IsotropicStrainHardeningMisesCreepDSL();

  protected:
    
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

#endif /* LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_HXX */

