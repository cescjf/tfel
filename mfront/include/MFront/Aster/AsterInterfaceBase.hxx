/*! 
 * \file  AsterInterfaceBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_ASTER_ASTERINTERFACEBASE_H_
#define _LIB_MFRONT_ASTER_ASTERINTERFACEBASE_H_ 

#include<string>

#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/Material/MaterialException.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/AsterException.hxx"

namespace aster{

  /*!
   * \class  AsterInterfaceBase
   * \brief  Base class for Aster interfaces
   * \author Helfer Thomas
   * \date   12/12/2011
   */
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInterfaceBase
  {

    /*!
     * \brief throw an AsterException. This method shall be called when
     * the number of materials properties declared by the beahviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    static void
    throwUnMatchedNumberOfMaterialProperties(const std::string&,
					     const unsigned short n1,
					     const AsterInt n2);
    
    /*!
     * \brief throw an AsterException. This method shall be called when
     * the number of state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    static void
    throwUnMatchedNumberOfStateVariables(const std::string&,
					 const unsigned short n1,
					 const AsterInt n2);

    /*!
     * \brief display the error message out of an AsterException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the AsterException to be treated
     */
    static void
    treatAsterException(const std::string&,
			const AsterException&);

    /*!
     * \brief display the error message out of a material exception to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the material exception to be treated
     */
    static void
    treatMaterialException(const std::string&,
			   const tfel::material::MaterialException&);
    
    /*!
     * \brief display the error message out of a generic tfel
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void
    treatTFELException(const std::string&,
		       const tfel::exception::TFELException&);

    /*!
     * \brief display the error message out of a generic standard
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void
    treatStandardException(const std::string&,
			   const std::exception&);

    /*!
     * \brief display the error message when an unknown exception is caught
     * \param[in] b : behaviour name
     */
    static void
    treatUnknownException(const std::string&);

    /*!
     * \brief throw an AsterException if the time step is negative
     * \param[in] b : behaviour name
     */
    static void
    throwNegativeTimeStepException(const std::string&);

    /*!
     * \brief throw an AsterException if the behaviour integration
     * failed
     * \param[in] b : behaviour name
     */
    static void
    throwBehaviourIntegrationFailedException(const std::string&);

    /*!
     * \brief throw an AsterException if the a consistant tangent
     * operator has to be given.
     * \param[in] b : behaviour name
     */
    static void
    throwConsistantTangentOperatorIsNotAvalaible(const std::string&);

    /*!
     * \brief display an error message if the value of the NTENS
     * parameter is not valid
     */
    static void
    displayInvalidNTENSValueErrorMessage();

  }; // end of struct AsterInterfaceBase

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_ASTERINTERFACEBASE_H */
