/*!
 * \file   mfront/include/MFront/EuroplexusInterface.hxx
 * \brief  This file declares the EuroplexusInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_EUROPLEXUSINTERFACE_H_
#define LIB_MFRONT_EUROPLEXUSINTERFACE_H_ 

#include<string>
#include<iosfwd>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/UMATInterfaceBase.hxx"

namespace mfront{

  /*!
   * \brief the interface the Europlexus Standard finite element solver
   */
  struct EuroplexusInterface
    : public UMATInterfaceBase
  {
    //! return the name of the interface
    static std::string 
    getName(void);
    /*!
     * \param[in] k  : keyword treated
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) override;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    virtual void
    endTreatment(const BehaviourDescription&,
		 const FileDescription&) const override;
    /*!
     * \param[out] d  : target description
     * \param[out] bd : behaviour description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const BehaviourDescription&) override;
    //! destructor
    virtual ~EuroplexusInterface();
    
  protected:
    //! \return the name of the generated library
    virtual std::string
    getLibraryName(const BehaviourDescription&) const override;
    
    virtual std::string
    getInterfaceName(void) const override;
    /*!
     * write the initialisation of the driving variables
     * \param[out] os: output file
     * \param[in]  mb: mechanical behaviour description
     */
    virtual void
    writeBehaviourDataMainVariablesSetters(std::ofstream&,
					   const BehaviourDescription&) const override;
    /*!
     * \brief write the instruction of exporting a thermodynamic force in an array
     * \param[out] out : output stream
     * \param[in]  a   : array name
     * \param[in]  f   : thermodynamic force
     * \param[in]  o   : thermodynamic force offset
     */
    virtual void 
    exportThermodynamicForce(std::ofstream&,
			     const std::string&,
			     const ThermodynamicForce&,
			     const SupportedTypes::TypeSize) const override;
    /*!
     * \return the name of the function generated by the Cyrano interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string
    getFunctionName(const std::string&) const override;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and whose second members is an offset giving the number of
     * imposed material properties.
     * \param[in] mb : mechanical behaviour description
     * \param[in] h  : modelling hypothesis
     */
    virtual std::pair<std::vector<UMATMaterialProperty>,
		      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription&,
				const Hypothesis) const override;
    /*!
     * \param[in] out : output file
     */
    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const override;
    /*!
     * \brief write a  specialisation of the EuroplexusTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void
    writeEuroplexusBehaviourTraits(std::ostream&,
			      const BehaviourDescription&,
			      const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ofstream&,
				   const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a thermodynamic force
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void
    writeBehaviourDataThermodynamicForceSetter(std::ofstream&,
					       const ThermodynamicForce&,
					       const SupportedTypes::TypeSize) const override;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    writeUMATxxAdditionalSymbols(std::ostream&,
				 const std::string&,
				 const Hypothesis,
				 const BehaviourDescription&,
				 const FileDescription&) const override;
    virtual std::string
    getModellingHypothesisTest(const Hypothesis) const override;

    virtual std::map<UMATInterfaceBase::Hypothesis,std::string>
    gatherModellingHypothesesAndTests(const BehaviourDescription&) const override;

    virtual std::set<tfel::material::ModellingHypothesis::Hypothesis>
    getModellingHypothesesToBeTreated(const BehaviourDescription&) const override;

  }; // end of EuroplexusInterface

} // end of namespace mfront

#endif /* LIB_MFRONT_EUROPLEXUSINTERFACE_H_ */
