/*!
 * \file   vectorResultType.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   06 Oct 2006
 */

#ifndef _LIB_TFEL_VECTORRESULTTYPE_H_
#define _LIB_TFEL_VECTORRESULTTYPE_H_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/vector.hxx"

namespace tfel{

  namespace math{

        /*
     * Partial specialisation for functions.
     */
    template<typename T, typename Op>
    class UnaryResultType_<VectorTag,FunctionTag,vector<T>,Op>
    {
      typedef typename UnaryResultType<T,Op>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

    /*
     * Partial specialisation for negation.
     */
    template<typename T>
    struct UnaryResultType<vector<T>,OpNeg>
    {
      typedef typename UnaryResultType<T,OpNeg>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename T, typename Scal>
    class ResultType_<VectorTag,ScalarTag,vector<T>,Scal,OpMult>
    {
      typedef typename ResultType<T,Scal,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename T, typename Scal>
    class ResultType_<VectorTag,ScalarTag,vector<T>,Scal,OpDiv>
    {
      typedef typename ResultType<T,Scal,OpDiv>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal, typename T>
    class ResultType_<ScalarTag,VectorTag,Scal,vector<T>,OpMult>
    {
      typedef typename ResultType<Scal,T,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template<typename T, typename T2>
    class ResultType<vector<T>,vector<T2>,OpPlus>
    {
      typedef typename ResultType<T,T2,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template<typename T, typename T2>
    class ResultType<vector<T>,vector<T2>,OpMinus>
    {
      typedef typename ResultType<T,T2,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template<typename T, typename T2>
    class ResultType<vector<T>,vector<T2>,OpMult>
    {
      typedef typename ResultType<T,T2,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      vector<ResBase_> >::type type;
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_VECTORRESULTTYPE_H */
