/*! 
 * \file   mfront/mtest/CalculiXSmallStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  03/08/2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<limits>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"MFront/CalculiX/CalculiX.hxx"
#include"MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx"

#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/CalculiXSmallStrainBehaviour.hxx"

namespace mtest
{

  CalculiXSmallStrainBehaviour::CalculiXSmallStrainBehaviour(const Hypothesis h,
							     const std::string& l,
							     const std::string& b)
    : CalculiXStandardBehaviour(h,l,b)
  {}

  void
  CalculiXSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    std::fill(v.begin(),v.end(),real(0));
  }

  std::pair<bool,real>
  CalculiXSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
					       CurrentState& s,
					       BehaviourWorkSpace& wk,
					       const real dt,
					       const StiffnessMatrixType ktype,
					       const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace calculix;
    using tfel::math::vector;
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    auto throw_if = [](const bool c, const std::string& m){
      if(c){throw(std::runtime_error("CalculiXSmallStrainBehaviour::"
				     "call_behaviour: "+m));}
    };
    throw_if(ktype!=StiffnessMatrixType::CONSISTENTTANGENTOPERATOR,
	     "CalculiX behaviours only provide the "
	     "consistent tangent operator");
    const CalculiXInt nprops = -100-static_cast<CalculiXInt>(s.mprops1.size());
    fill(wk.D.begin(),wk.D.end(),0.);
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    copy(s.iv0.begin(),s.iv0.end(),wk.ivs.begin());
    const auto nstatv = static_cast<CalculiXInt>(wk.ivs.size());
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ue1(real(0));
    stensor<3u,real> us(real(0));
    copy(s.e0.begin(),s.e0.end(),ue0.begin());
    copy(s.e1.begin(),s.e1.end(),ue1.begin());
    copy(s.s0.begin(),s.s0.end(),us.begin());
    // thermal strain
    for(CalculiXInt i=0;i!=6;++i){
      ue0(i) -= s.e_th0(i);
      ue1(i) -= s.e_th1(i);
    }
    if(this->stype==1u){
      ue0.changeBasis(s.r);
      ue1.changeBasis(s.r);
      us.changeBasis(s.r);
    }
    // CalculiX standard convention
    for(CalculiXInt i=3;i!=6;++i){
      ue0(i) /= sqrt2;
      ue1(i) /= sqrt2;
      us(i)  /= sqrt2;
    }
    auto ndt = std::numeric_limits<CalculiXReal>::max();
    const auto iel      = CalculiXInt(0);
    const auto iint     = CalculiXInt(0);
    const auto ithermal = CalculiXInt(1);
    const auto icmd     = CalculiXInt(0);
    const auto ielas    = CalculiXInt(1);
    const auto mi       = CalculiXInt(1);
    const auto ioren    = CalculiXInt(0);
    const auto T = s.esv0(0)+s.desv(0);
    (this->fct)(nullptr,&iel,&iint,&nprops,
		s.mprops1.empty() ? nullptr : &s.mprops1[0],
		&ue1(0),&ue0(1),
		nullptr,nullptr,nullptr,nullptr,nullptr,
		&ithermal,&T,&dt,nullptr,nullptr,&icmd,&ielas,
		&mi,&nstatv,
		s.iv0.empty() ? nullptr : &s.iv0(0),
		wk.ivs.empty() ? nullptr : &wk.ivs(0),
		&us(0),&(wk.D(0,0)),&ioren,nullptr,
		nullptr,&ndt,nullptr,0);
    if(ndt<1.){
      return {false,ndt};
    }
    const auto rb = transpose(s.r);
    // treating the consistent tangent operator
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      const auto K = this->convertTangentOperator(&(wk.D(0,0)));
      if(this->stype==1u){
	const auto nK = change_basis(K,rb);
	for(unsigned short i=0;i!=6u;++i){
	  for(unsigned short j=0;j!=6u;++j){
	    Kt(i,j)=nK(i,j);
	  }
	}
      } else {
	for(unsigned short i=0;i!=6u;++i){
	  for(unsigned short j=0;j!=6u;++j){
	    Kt(i,j)=K(i,j);
	  }
	}
      }
    } else {
      throw_if(true,"unsupported modelling hypothesis");
    }
    if(b){
      // treating internal state variables
      if(!s.iv0.empty()){
	copy_n(wk.ivs.begin(),s.iv1.size(),s.iv1.begin());
      }
      // turning stresses in TFEL conventions
      for(CalculiXInt i=3;i!=6;++i){
	us[i] *= sqrt2;
      }
      if(this->stype==1u){
	us.changeBasis(rb);
      }
      copy(us.begin(),us.begin()+s.s1.size(),s.s1.begin());
    }
    return {true,ndt};
  }

  CalculiXSmallStrainBehaviour::~CalculiXSmallStrainBehaviour() = default;
  
} // end of namespace mtest
