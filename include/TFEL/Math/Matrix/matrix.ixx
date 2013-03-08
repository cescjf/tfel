/*!
 * \file   matrix.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   05 oct 2006
 */

#ifndef _LIB_TFEL_MATRIX_IXX_
#define _LIB_TFEL_MATRIX_IXX_ 

namespace tfel{

  namespace math{

    template<typename T>
    matrix<T>::matrix(const matrix<T>& src)
      : tfel::math::vector<T>(src),
	MatrixProperties<T>(src)
    {}

    template<typename T>
    matrix<T>::matrix()
      : tfel::math::vector<T>(),
	MatrixProperties<T>(0,0)
    {} // end of matrix::matrix

    template<typename T>
    matrix<T>::matrix(const typename matrix<T>::size_type n,
		      const typename matrix<T>::size_type m)
      : tfel::math::vector<T>(n*m),
	MatrixProperties<T>(n,m)
    {}

    template<typename T>
    matrix<T>::matrix(const typename matrix<T>::size_type n,
			      const typename matrix<T>::size_type m,
			      const T& v)
      : tfel::math::vector<T>(n*m,v),
	MatrixProperties<T>(n,m)
    {}

    template<typename T>
    void
    matrix<T>::resize(const typename matrix<T>::size_type n,
		      const typename matrix<T>::size_type m)
    {
      vector<T>::resize(n*m);
      MatrixProperties<T>::operator=(MatrixProperties<T>(n,m));
    } // end of matrix<T>::resize

    template<typename T>
    matrix<T>&
    matrix<T>::operator=(const matrix<T>& src)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   src.getRunTimeProperties());
#endif
      tfel::math::vector<T>::operator=(src);
      return *this;
    }

    template<typename T>
    matrix<T>&
    matrix<T>::operator+=(const matrix<T>& src)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   src.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) += src(i,j);
	}
      }
      return *this;
    }

    template<typename T>
    matrix<T>&
    matrix<T>::operator-=(const matrix<T>& src)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   src.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) -= src(i,j);
	}
      }
      return *this;
    }

    template<typename T>
    T& 
    matrix<T>::operator()(const typename matrix<T>::size_type i,
				  const typename matrix<T>::size_type j)
    {
      return tfel::math::vector<T>::operator[](i*(this->nb_cols)+j);
    }
    
    template<typename T>
    const T& 
    matrix<T>::operator()(const typename matrix<T>::size_type i,
				  const typename matrix<T>::size_type j) const
    {
      return tfel::math::vector<T>::operator[](i*(this->nb_cols)+j);
    }

    template<typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      matrix<T>&
    >::type
    matrix<T>::operator=(const MatrixExpr<matrix<T2>,Expr>& expr)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   expr.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) = expr(i,j);
	}
      }
      return *this;
    }

    template<typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      matrix<T>&
    >::type
    matrix<T>::operator+=(const MatrixExpr<matrix<T2>,Expr>& expr)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   expr.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) += expr(i,j);
	}
      }
      return *this;
    }

    template<typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      matrix<T>&
    >::type
    matrix<T>::operator-=(const MatrixExpr<matrix<T2>,Expr>& expr)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   expr.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) -= expr(i,j);
	}
      }
      return *this;
    }

    template<typename T>
    void matrix<T>::swap(matrix<T>& a)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   a.getRunTimeProperties());
#endif
      tfel::math::vector<T>::swap(a);
    }

    template<typename T>
    TFEL_MATH_INLINE
    typename matrix<T>::size_type
    matrix<T>::getNbRows() const
    {
      return this->nb_rows;
    }
    
    template<typename T>
    TFEL_MATH_INLINE
    typename matrix<T>::size_type
    matrix<T>::getNbCols() const
    {
      return this->nb_cols;
    }

    template<typename T>
    TFEL_MATH_INLINE
#ifdef NO_RUNTIME_CHECK_BOUNDS
    const typename matrix<T>::RunTimeProperties
#else
    const typename matrix<T>::RunTimeProperties&
#endif
    matrix<T>::getRunTimeProperties(void) const
    {
#ifdef NO_RUNTIME_CHECK_BOUNDS
      return RunTimeProperties();
#else
      return *this;
#endif
    } // end of matrix<T>::getRunTimeProperties(void) const

    template<typename T>
    MatrixProperties<T>::MatrixProperties(const typename MatrixProperties<T>::IndexType n,
					  const typename MatrixProperties<T>::IndexType m)
      :	nb_rows(n),
	nb_cols(m)
    {} // end of MatrixProperties<T>::MatrixProperties
    
    template<typename T>
    MatrixProperties<T>::MatrixProperties(const MatrixProperties& src)
      : nb_rows(src.nb_rows),
	nb_cols(src.nb_cols)
    {} // end of MatrixProperties<T>::MatrixProperties

    template<typename T>
    MatrixProperties<T>&
    MatrixProperties<T>::operator=(const MatrixProperties<T>& s)
    {
      this->nb_rows = s.nb_rows;
      this->nb_cols = s.nb_cols;
      return *this;
    }  // end of MatrixProperties<T>::operator=

    template<typename T>
    bool
    MatrixProperties<T>::operator == (const MatrixProperties& s) const
    {
      return (this->nb_rows==s.nb_rows)&&(this->nb_cols==s.nb_cols);
    } // end of MatrixProperties<T>::operator == 

    template<typename T>
    bool
    MatrixProperties<T>::operator != (const MatrixProperties& s) const
    {
      return (this->nb_rows!=s.nb_rows)||(this->nb_cols!=s.nb_cols);
    } // end of MatrixProperties<T>::operator == 

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATRIX_IXX */
