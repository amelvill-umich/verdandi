// Copyright (C) 2001-2011 Vivien Mallet
// Copyright (C) 2003-2011 Marc Duruflé
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


// To be included by Seldon.hxx

#ifndef SELDON_FILE_MATRIX_SYMMETRIC_HXX

#include "../share/Common.hxx"
#include "../share/Properties.hxx"
#include "../share/Storage.hxx"
#include "../share/Errors.hxx"
#include "../share/Allocator.hxx"

namespace Seldon
{


  //! Symmetric matrix stored in a full matrix.
  template <class T, class Prop, class Storage, class Allocator
	    = typename SeldonDefaultAllocator<Storage, T>::allocator>
  class Matrix_Symmetric: public Matrix_Base<T, Allocator>
  {
    // typedef declaration.
  public:
    typedef typename Allocator::value_type value_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::const_pointer const_pointer;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::value_type entry_type;
    typedef typename Allocator::value_type access_type;
    typedef typename Allocator::value_type const_access_type;

    // Attributes.
  protected:
    pointer* me_;

    // Methods.
  public:
    // Constructor.
    Matrix_Symmetric();
    explicit Matrix_Symmetric(int i, int j);
    Matrix_Symmetric(const Matrix_Symmetric<T, Prop, Storage, Allocator>& A);

    // Destructor.
    ~Matrix_Symmetric();
    void Clear();

    // Basic methods.
    int GetDataSize() const;
    int64_t GetMemorySize() const;
    
    // Memory management.
    void Reallocate(int i, int j);
    void SetData(int i, int j, pointer data);
    void Nullify();
    void Resize(int i, int j);

    // Element access and affectation.
    reference operator() (int i, int j);
    const_reference operator() (int i, int j) const;
    const_reference Val(int i, int j) const;
    reference Val(int i, int j);
    reference Get(int i, int j);
    const_reference Get(int i, int j) const;
    reference operator[] (int i);
    const_reference operator[] (int i) const;
    Matrix_Symmetric<T, Prop, Storage, Allocator>&
    operator= (const Matrix_Symmetric<T, Prop, Storage, Allocator>& A);
    void Set(int i, int j, const T& x);
    void Copy(const Matrix_Symmetric<T, Prop, Storage, Allocator>& A);

    // Convenient functions.
    void Zero();
    void SetIdentity();
    void Fill();
    template <class T0>
    void Fill(const T0& x);
    template <class T0>
    Matrix_Symmetric<T, Prop, Storage, Allocator>& operator= (const T0& x);
    void FillRand();
    void Print() const;
    void Print(int a, int b, int m, int n) const;
    void Print(int l) const;

    // Input/output functions.
    void Write(string FileName) const;
    void Write(ostream& FileStream) const;
    void WriteText(string FileName) const;
    void WriteText(ostream& FileStream) const;
    void Read(string FileName);
    void Read(istream& FileStream);
    void ReadText(string FileName);
    void ReadText(istream& FileStream);

#ifdef SELDON_WITH_VIRTUAL
    typedef typename ClassComplexType<T>::Treal Treal;
    typedef typename ClassComplexType<T>::Tcplx Tcplx;

    virtual void ApplySor(Vector<T>& x, const Vector<T>& r,
			  const typename ClassComplexType<T>::Treal& omega,
			  int nb_iter, int stage_ssor) const;

    virtual void ApplySor(const class_SeldonTrans&, Vector<T>& x, const Vector<T>& r,
			  const typename ClassComplexType<T>::Treal& omega,
			  int nb_iter, int stage_ssor) const;
        
    virtual void MltAddVector(const Treal& alpha, const Vector<Treal>& x,
			      const Treal& beta, Vector<Treal>& y) const;

    virtual void MltAddVector(const Tcplx& alpha, const Vector<Tcplx>& x,
			      const Tcplx& beta, Vector<Tcplx>& y) const;

    virtual void MltAddVector(const Treal& alpha, const SeldonTranspose&,
			      const Vector<Treal>& x,
			      const Treal& beta, Vector<Treal>& y) const;

    virtual void MltAddVector(const Tcplx& alpha, const SeldonTranspose&,
			      const Vector<Tcplx>& x,
			      const Tcplx& beta, Vector<Tcplx>& y) const;
    
    virtual void MltVector(const Vector<Treal>& x, Vector<Treal>& y) const;
    virtual void MltVector(const Vector<Tcplx>& x, Vector<Tcplx>& y) const;
    
    virtual void MltVector(const SeldonTranspose&,
			   const Vector<Treal>& x, Vector<Treal>& y) const;

    virtual void MltVector(const SeldonTranspose&,
			   const Vector<Tcplx>& x, Vector<Tcplx>& y) const;
    
    virtual bool IsSymmetric() const;
#endif

  };


  //! Column-major symmetric full-matrix class.
  template <class T, class Prop, class Allocator>
  class Matrix<T, Prop, ColSym, Allocator>:
    public Matrix_Symmetric<T, Prop, ColSym, Allocator>
  {
    // typedef declaration.
  public:
    typedef typename Allocator::value_type value_type;
    typedef Prop property;
    typedef ColSym storage;
    typedef Allocator allocator;

  public:
    Matrix();
    explicit Matrix(int i, int j);

    template <class T0>
    Matrix<T, Prop, ColSym, Allocator>& operator= (const T0& x);
    Matrix<T, Prop, ColSym, Allocator>& operator= (const Matrix<T, Prop,
                                                         ColSym,
                                                         Allocator>& A);
    template<class T0>
    Matrix<T, Prop, ColSym, Allocator>& operator*= (const T0& x);

  };


  //! Row-major symmetric full-matrix class.
  template <class T, class Prop, class Allocator>
  class Matrix<T, Prop, RowSym, Allocator>:
    public Matrix_Symmetric<T, Prop, RowSym, Allocator>
  {
    // typedef declaration.
  public:
    typedef typename Allocator::value_type value_type;
    typedef Prop property;
    typedef RowSym storage;
    typedef Allocator allocator;

  public:
    Matrix();
    explicit Matrix(int i, int j);

    template <class T0>
    Matrix<T, Prop, RowSym, Allocator>& operator= (const T0& x);
    Matrix<T, Prop, RowSym, Allocator>& operator= (const Matrix<T, Prop,
                                                         RowSym,
                                                         Allocator>& A);
    template<class T0>
    Matrix<T, Prop, RowSym, Allocator>& operator*= (const T0& x);

  };


} // namespace Seldon.

#define SELDON_FILE_MATRIX_SYMMETRIC_HXX
#endif
