// Copyright (C) 2003-2009 Marc Duruflé
// Copyright (C) 2001-2009 Vivien Mallet
//
// This file is part of the linear-algebra library Seldon,
// http://seldon.sourceforge.net/.
//
// Seldon is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; either version 2.1 of the License, or (at your option)
// any later version.
//
// Seldon is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Seldon. If not, see http://www.gnu.org/licenses/.


#ifndef SELDON_FILE_ITERATIVE_QMRSYM_CXX

namespace Seldon
{

  //! Solves linear system using Symmetric Quasi-Minimal Residual (SQMR)
  /*!
    Solves the symmetric linear system Ax = b using the
    Quasi-Minimal Residual method.

    See: R. W. Freund and N. M. Nachtigal, A quasi-minimal residual method for
    non-Hermitian linear systems, Numerical Math., 60(1991), pp. 315-339

    \param[in] A  Complex Symmetric Matrix
    \param[in,out] x  Vector on input it is the initial guess
    on output it is the solution
    \param[in] b  Right hand side of the linear system
    \param[in] M Left preconditioner
    \param[in] iter Iteration parameters
  */
#ifdef SELDON_WITH_VIRTUAL
  template<class T, class Vector1>
  int QmrSym(const VirtualMatrix<T>& A, Vector1& x, const Vector1& b,
	     Preconditioner_Base<T>& M,
	     Iteration<typename ClassComplexType<T>::Treal>& iter)
#else
  template <class Titer, class Matrix1, class Vector1, class Preconditioner>
  int QmrSym(const Matrix1& A, Vector1& x, const Vector1& b,
	     Preconditioner& M, Iteration<Titer> & iter)
#endif
  {
    const int N = A.GetM();
    if (N <= 0)
      return 0;

    typedef typename Vector1::value_type Complexe;
    Complexe delta, ep, beta;
    typedef typename ClassComplexType<Complexe>::Treal Treal;
    Treal  rho, rho_1;
    Complexe theta_1, gamma_1;
    Complexe theta, gamma, eta;
    Complexe zero, one;
    SetComplexZero(zero);
    SetComplexOne(one);
    theta = zero; gamma = one; eta = -one; ep = zero;
    
    Vector1 r(b), y(b);
    Vector1 v(b), p_tld(b);
    Vector1 p(b), d(b), s(b);

    // we initialize iter
    int success_init = iter.Init(b);
    if (success_init != 0)
      return iter.ErrorCode();

    // r = b - Ax
    Copy(b, r);
    if (!iter.IsInitGuess_Null())
      iter.MltAdd(-one, A, x, one, r);
    else
      x.Fill(zero);

    Copy(r, v);

    M.Solve(A, v, y);
    rho = Norm2(y);

    iter.SetNumberIteration(0);
    // Loop until the stopping criteria are reached
    while (! iter.Finished(r))
      {

	if (rho == Treal(0))
	  {
	    iter.Fail(1, "Qmr breakdown #1");
	    break;
	  }

	// v = v / rho
	// y = y / rho
	Mlt(one/rho, v);
	Mlt(one/rho, y);

	delta = DotProd(v, y);
	if (delta == zero)
	  {
	    iter.Fail(3, "Qmr breakdown #2");
	    break;
	  }

	if (iter.First())
	  Copy(y, p);
	else
	  {
	    // p = y - (rho delta / ep) p
	    Mlt(-rho  * delta / ep, p);
	    Add(one, y, p);
	  }

	// product matrix vector p_tld = A*p
	iter.Mlt(A, p, p_tld);

	ep = DotProd(p, p_tld);
	if (ep == zero)
	  {
	    iter.Fail(4, "Qmr breakdown #3");
	    break;
	  }

	beta = ep / delta;
	if (beta == zero)
	  {
	    iter.Fail(5, "Qmr breakdown #4");
	    break;
	  }

	// v = -beta v + p_tld
	Mlt(-beta, v); Add(one, p_tld, v);
	M.Solve(A, v, y);

	rho_1 = rho;
	rho = Norm2(y);

	gamma_1 = gamma;
	theta_1 = theta;

	theta = rho / (gamma_1 * beta);
	gamma = one / sqrt(one + theta * theta);

	if (gamma == Treal(0))
	  {
	    iter.Fail(6, "Qmr breakdown #5");
	    break;
	  }

	eta = -eta * rho_1 * gamma * gamma / (beta * gamma_1 * gamma_1);

	if (iter.First())
	  {
	    Copy(p, d);
	    Mlt(eta, d);
	    Copy(p_tld, s);
	    Mlt(eta, s);
	  }
	else
	  {
	    Complexe tmp = (theta_1 * theta_1 * gamma * gamma);
	    Mlt(tmp, d);
	    Add(eta, p, d);
	    Mlt(tmp, s);
	    Add(eta, p_tld, s);
	  }
	Add(one, d, x);
	Add(-one, s, r);

	++iter;
      }

    return iter.ErrorCode();
  }

} // end namespace

#define SELDON_FILE_ITERATIVE_QMRSYM_CXX
#endif