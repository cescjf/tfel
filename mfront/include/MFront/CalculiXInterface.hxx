/*!
 * \file   mfront/include/MFront/CalculiXInterface.hxx
 * \brief  This file declares the CalculiXInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CALCULIXINTERFACE_HXX
#define LIB_MFRONT_CALCULIXINTERFACE_HXX 

#include<string>
#include<iosfwd>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/UMATInterfaceBase.hxx"

namespace mfront{

  /*!
   * \brief the interface the CalculiX Standard finite element solver
   */
  struct CalculiXInterface
    : public UMATInterfaceBase
  {
    /*!
     * This enum defines the various finite strain strategies
     * available to resuse HPP laws into finite strain computations.
     *
     * The "finite rotation small strain" strategy is defined in the
     * reference of the Code-Aster finite element software:
     * [R5.03.22] Loi de comportement en grandes rotations et petites déformations
     *
     * The logarithmic strain strategy has been introduced by Miehe,
     * Apel and Lambrecht:
     * Miehe C., Apel N., Lambrecht M.: Anisotropic additive plasticity in the logarithm strain space : modular
     * kinematic formulation and implementation based on incremental minimization principles for
     * standard materials., Computer Methods in Applied Mechanics and Engineering, 191,
     * pp.5383-5425, 2002.
     * This strategy is also developped int the reference of the Code-Aster finite element software:
     * [R5.03.24] Modèles de grandes déformations GDEF_LOG et GDEF_HYPO_ELAS
     */
    enum FiniteStrainStrategy{
      UNDEFINEDSTRATEGY,
      NATIVEFINITESTRAINSTRATEGY,
      FINITEROTATIONSMALLSTRAIN,
      MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN
    }; // end of enum FiniteStrainStrategy
    /*!
     * \return the name of the interface
     */
    static std::string 
    getName();
    /*!
     * \param[in,out] mb: behaviour description
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool,tokens_iterator>
    treatKeyword(BehaviourDescription&,
		 const std::string&,
		 const std::vector<std::string>&,
		 tokens_iterator,
		 const tokens_iterator) override;
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
    virtual ~CalculiXInterface();
    
  protected:

    virtual std::string
    getInterfaceName() const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void
    writeBehaviourDataDrivingVariableSetter(std::ostream&,
					    const DrivingVariable&,
					    const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void
    writeIntegrationDataDrivingVariableSetter(std::ostream&,
					      const DrivingVariable&,
					      const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the instruction of exporting a thermodynamic force in an array
     * \param[out] out : output stream
     * \param[in]  a   : array name
     * \param[in]  f   : thermodynamic force
     * \param[in]  o   : thermodynamic force offset
     */
    virtual void 
    exportThermodynamicForce(std::ostream&,
			     const std::string&,
			     const ThermodynamicForce&,
			     const SupportedTypes::TypeSize) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ostream&,
				   const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a thermodynamic force
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void
    writeBehaviourDataThermodynamicForceSetter(std::ostream&,
					       const ThermodynamicForce&,
					       const SupportedTypes::TypeSize) const override;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxBehaviourTypeSymbols(std::ostream&,
				    const std::string&,
				    const BehaviourDescription&) const override;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxBehaviourKinematicSymbols(std::ostream&,
					 const std::string&,
					 const BehaviourDescription&) const override;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     * \param[in] sfeh: name of the function used to handle the stress
     *                  expansion
     */
    virtual void writeUMATFunctionBase(std::ostream&,
				       const BehaviourDescription&,
				       const std::string&,
				       const std::string&) const;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     */
    virtual void
    writeUMATSmallStrainFunction(std::ostream&,
				 const BehaviourDescription&,
				 const std::string&) const;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     */
    virtual void
    writeUMATFiniteStrainFunction(std::ostream&,
				  const BehaviourDescription&,
				  const std::string&) const;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     */
    virtual void
    writeUMATFiniteRotationSmallStrainFunction(std::ostream&,
					       const BehaviourDescription&,
					       const std::string&) const;
    /*!
     * \return the name of the generated library
     * \param[in] mb : behaviour description
     */
    virtual std::string
    getLibraryName(const BehaviourDescription&) const override;
    /*!
     * \brief write a  specialisation of the CalculiXTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     */
    virtual void
    writeCalculiXBehaviourTraits(std::ostream&,
			       const BehaviourDescription&) const;
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
    /*!
     * \param[in] out : output file
     */
    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const override;

    virtual std::string
    getModellingHypothesisTest(const Hypothesis) const override;

    virtual std::map<UMATInterfaceBase::Hypothesis,std::string>
    gatherModellingHypothesesAndTests(const BehaviourDescription&) const override;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and whose second members is an offset giving the number of
     * imposed material properties.
     * \param[in] mb : behaviour description
     * \param[in] h  : modelling hypothesis
     */
    virtual std::pair<std::vector<UMATMaterialProperty>,
		      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription&,
				const Hypothesis) const override;
    /*!
     * \return the name of the function generated by the interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string getFunctionName(const std::string&) const override;
    /*!
     * \return the list of modelling hypotheses treated by the interface
     * \param[in] mb : behaviour description
     */
    virtual std::set<Hypothesis>
    getModellingHypothesesToBeTreated(const BehaviourDescription&) const override;
    /*!
     * \return the input file example
     * \param[in] mb: behaviour description
     * \param[in] fd: file description
     * \param[in] b: if true, write the example for
     * CalculiX-Standard. If false, write the example for
     * CalculiX-Explicit
     */
    virtual void 
    writeInputFileExample(const BehaviourDescription&,
			  const FileDescription&,
			  const bool) const;
    
    virtual void writeDepvar(std::ostream&,int&,
			     const VariableDescription&,
			     const std::string&) const;
    //! \return true if the interface handles external state variables
    virtual bool areExternalStateVariablesSupported() const;
    //! \return true if the interface handles temperature increment
    virtual bool isTemperatureIncrementSupported() const;
    //! selected finite strain strategy
    FiniteStrainStrategy fss = UNDEFINEDSTRATEGY;
    /*!
     * \brief boolean stating the we want a comparison of the user
     * defined tangent operator with a numerical approximation.
     */
    bool compareToNumericalTangentOperator = false;
    /*!
     * \brief perturbation value used for the computation of the
     * numerical approximation of the tangent operator
     */
    double strainPerturbationValue = 1.e-6;
    /*!
     * \brief comparison critera value used for comparing of the user
     * defined tangent operator with a numerical approximation.
     */
    double tangentOperatorComparisonCriterion = 1.e7;
  }; // end of CalculiXInterface

} // end of namespace mfront

#endif /* LIB_MFRONT_CALCULIXINTERFACE_HXX */
