/*!
 * \file   FunctionExpr.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   27 d�c 2006
 */

#ifndef _LIB_TFEL_FUNCTIONEXPR_HXX_
#define _LIB_TFEL_FUNCTIONEXPR_HXX_ 

#include<string>
#include"TFEL/Utilities/Name.hxx"

namespace tfel{

  namespace math{

    /*
     * Forward declaration 
     * (needed since FunctionExpr is used in FunctionConcept declaration to define
     * function composition).
     */
    template<typename F>
    struct FunctionConcept; 

    template<typename Expr>
    class FunctionExpr
      : public FunctionConcept<FunctionExpr<Expr> >,
	public Expr
    {

      FunctionExpr& operator=(const FunctionExpr&);

    public:

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionExpr<")+Name<Expr>::getName()+string(">");
      }

      explicit TFEL_MATH_INLINE FunctionExpr()
	: Expr()
      {}

      TFEL_MATH_INLINE FunctionExpr(const FunctionExpr& src)
	: Expr(src)
      {}

      explicit TFEL_MATH_INLINE 
      FunctionExpr(const typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE 
      FunctionExpr(const typename Expr::first_arg&  a_, 
		   const typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}
      
      using Expr::operator();
      
      ~FunctionExpr()
      {}

    }; // end of FunctionExpr

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_FUNCTIONEXPR_HXX */
