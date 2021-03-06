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


#ifndef SELDON_FILE_MATRIX_HERMPACKED_CXX

#include "Matrix_HermPacked.hxx"

namespace Seldon
{
  

  //! Main constructor.
  /*! Builds a i x j hermitian matrix in packed form.
    \param i number of rows.
    \param j number of columns.
    \note 'j' is assumed to be equal to 'i' and is therefore discarded.
  */
  template <class T, class Prop, class Storage, class Allocator>
  Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::Matrix_HermPacked(int i, int j):
    Matrix_Base<T, Allocator>(i, i)
  {

#ifdef SELDON_CHECK_MEMORY
    try
      {
#endif

	this->data_ = Allocator::allocate((i * (i + 1)) / 2, this);

#ifdef SELDON_CHECK_MEMORY
      }
    catch (...)
      {
	this->m_ = 0;
	this->n_ = 0;
	this->data_ = NULL;
	return;
      }
    if (this->data_ == NULL)
      {
	this->m_ = 0;
	this->n_ = 0;
	return;
      }
#endif

  }


  //! Copy constructor.
  template <class T, class Prop, class Storage, class Allocator>
  Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::Matrix_HermPacked(const Matrix_HermPacked<T, Prop,
		      Storage, Allocator>& A):
    Matrix_Base<T, Allocator>()
  {
    this->m_ = 0;
    this->n_ = 0;
    this->data_ = NULL;

    this->Copy(A);
  }


  //! Clears the matrix.
  /*!
    Destructs the matrix.
    \warning On exit, the matrix is an empty 0x0 matrix.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Clear()
  {

#ifdef SELDON_CHECK_MEMORY
    try
      {
#endif

	if (this->data_ != NULL)
	  {
	    Allocator::deallocate(this->data_,
				  (this->m_ * (this->m_ + 1)) / 2);
	    this->data_ = NULL;
	  }

#ifdef SELDON_CHECK_MEMORY
      }
    catch (...)
      {
	this->m_ = 0;
	this->n_ = 0;
	this->data_ = NULL;
      }
#endif
    
    this->m_ = 0;
    this->n_ = 0;
  }


  /*********************
   * MEMORY MANAGEMENT *
   *********************/


  //! Reallocates memory to resize the matrix.
  /*!
    On exit, the matrix is a i x j matrix.
    \param i new number of rows.
    \param j new number of columns.
    \warning Depending on your allocator, data may be lost.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::Reallocate(int i, int j)
  {

    if (i != this->m_)
      {
	this->m_ = i;
	this->n_ = i;

#ifdef SELDON_CHECK_MEMORY
	try
	  {
#endif

	    this->data_ =
	      reinterpret_cast<pointer>(Allocator::reallocate(this->data_,
							      (i*(i + 1)) / 2, this));

#ifdef SELDON_CHECK_MEMORY
	  }
	catch (...)
	  {
	    this->m_ = 0;
	    this->n_ = 0;
	    this->data_ = NULL;
	    throw NoMemory("Matrix_HermPacked::Reallocate(int, int)",
			   "Unable to reallocate memory for data_.");
	  }
	if (this->data_ == NULL)
	  {
	    this->m_ = 0;
	    this->n_ = 0;
	    throw NoMemory("Matrix_HermPacked::Reallocate(int, int)",
			   "Unable to reallocate memory for data_.");
	  }
#endif

      }
  }


  //! Changes the size of the matrix and sets its data array
  //! (low level method).
  /*!
    The matrix is first cleared (memory is freed). The matrix is then resized
    to a i x j matrix, and the data array of the matrix is set to 'data'.
    'data' elements are not duplicated: the new data array of the matrix is
    the 'data' array. It is useful to create a matrix from pre-existing data.
    \param i new number of rows.
    \param j new number of columns.
    \param data new array storing elements.
    \warning 'data' has to be used carefully outside the object.
    Unless you use 'Nullify', 'data' will be freed by the destructor,
    which means that 'data' must have been allocated carefully. The matrix
    allocator should be compatible.
    \note This method should only be used by advanced users.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::SetData(int i, int j,
	    typename Matrix_HermPacked<T, Prop, Storage, Allocator>
	    ::pointer data)
  {
    this->Clear();

    this->m_ = i;
    this->n_ = i;

    this->data_ = data;
  }


  //! Clears the matrix without releasing memory.
  /*!
    On exit, the matrix is empty and the memory has not been released.
    It is useful for low level manipulations on a Matrix instance.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Nullify()
  {
    this->data_ = NULL;
    this->m_ = 0;
    this->n_ = 0;
  }


  /************************
   * CONVENIENT FUNCTIONS *
   ************************/


  //! Sets all elements to zero.
  /*!
    \warning It fills the memory with zeros. If the matrix stores complex
    structures, use 'Fill' instead.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Zero()
  {
    Allocator::memoryset(this->data_, char(0),
			 this->GetDataSize() * sizeof(value_type));
  }


  //! Sets the matrix to the identity.
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::SetIdentity()
  {
    T one, zero;
    SetComplexOne(one);
    SetComplexZero(zero);
    
    this->Fill(zero);

    for (int i = 0; i < min(this->m_, this->n_); i++)
      this->Val(i,i) = one;
  }


  //! Fills the matrix with 0, 1, 2, ...
  /*!
    On exit, the matrix is filled with 0, 1, 2, 3, ... The order of
    those numbers depends on the storage.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Fill()
  {
    for (int i = 0; i < this->GetDataSize(); i++)
      SetComplexReal(i, this->data_[i]);
  }


  //! Fills the matrix with a given value.
  /*!
    \param x value to fill the matrix with.
    \note If the imaginary part of x is non-null, the upper part will
    contain x, whereas lower part will contain conj(x).
  */
  template <class T, class Prop, class Storage, class Allocator>
  template <class T0>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Fill(const T0& x)
  {
    T x_;
    SetComplexReal(x, x_);
    for (int i = 0; i < this->GetDataSize(); i++)
      this->data_[i] = x_;
  }


  //! Fills the matrix with a given value.
  /*!
    \param x value to fill the matrix with.
    \note If the imaginary part of x is non-null, the upper part will
    contain x, whereas lower part will contain conj(x).
  */
  template <class T, class Prop, class Storage, class Allocator>
  template <class T0>
  Matrix_HermPacked<T, Prop, Storage, Allocator>&
  Matrix_HermPacked<T, Prop, Storage, Allocator>::operator= (const T0& x)
  {
    this->Fill(x);

    return *this;
  }


  //! Fills the matrix randomly.
  /*!
    \note The random generator is very basic.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::FillRand()
  {
#ifndef SELDON_WITHOUT_REINIT_RANDOM
    srand(time(NULL));
#endif
    for (int i = 0; i < this->GetDataSize(); i++)
      SetComplexReal(rand(), this->data_[i]);
  }


  //! Displays the matrix on the standard output.
  /*!
    Displays elements on the standard output, in text format.
    Each row is displayed on a single line and elements of
    a row are delimited by tabulations.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Print() const
  {
    for (int i = 0; i < this->m_; i++)
      {
	for (int j = 0; j < this->n_; j++)
	  cout << (*this)(i, j) << "\t";
	cout << endl;
      }
  }


  //! Displays a sub-matrix on the standard output.
  /*!
    The sub-matrix is defined by its upper-left corner (a, b)
    and its bottom-right corner (m, n). So, elements with indices
    in [a, m] x [b, n] are displayed on the standard output,
    in text format. Each row is displayed on a single line and
    elements of a row are delimited by tabulations.
    \param a row index of the upper-left corner.
    \param b column index of the upper-left corner.
    \param m row index of the bottom-right corner.
    \param n column index of the bottom-right corner.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::Print(int a, int b, int m, int n) const
  {
    for (int i = a; i < min(this->m_, a+m); i++)
      {
	for (int j = b; j < min(this->n_, b+n); j++)
	  cout << (*this)(i, j) << "\t";
	cout << endl;
      }
  }


  //! Displays a square sub-matrix on the standard output.
  /*!
    The sub-matrix is defined by its bottom-right corner (l, l).
    So, elements with indices in [0, 0] x [l, l] are displayed
    on the standard output, in text format. Each row is displayed
    on a single line and elements of a row are delimited
    by tabulations.
    \param l dimension of the square matrix to be displayed.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Print(int l) const
  {
    Print(0, 0, l, l);
  }


  /**************************
   * INPUT/OUTPUT FUNCTIONS *
   **************************/


  //! Writes the matrix in a file.
  /*!
    Stores the matrix in a file in binary format.
    The number of rows (integer) and the number of columns (integer)
    are written, and matrix elements are then written in the same order
    as in memory (e.g. row-major storage).
    \param FileName output file name.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::Write(string FileName) const
  {

    ofstream FileStream;
    FileStream.open(FileName.c_str(), ofstream::binary);

#ifdef SELDON_CHECK_IO
    // Checks if the file was opened.
    if (!FileStream.is_open())
      throw IOError("Matrix_HermPacked::Write(string FileName)",
		    string("Unable to open file \"") + FileName + "\".");
#endif

    this->Write(FileStream);

    FileStream.close();

  }


  //! Writes the matrix to an output stream.
  /*!
    Writes the matrix to an output stream in binary format.
    The number of rows (integer) and the number of columns (integer)
    are written, and matrix elements are then written in the same order
    as in memory (e.g. row-major storage).
    \param FileStream output stream.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::Write(ostream& FileStream) const
  {

#ifdef SELDON_CHECK_IO
    // Checks if the file is ready.
    if (!FileStream.good())
      throw IOError("Matrix_HermPacked::Write(ofstream& FileStream)",
                    "Stream is not ready.");
#endif

    FileStream.write(reinterpret_cast<char*>(const_cast<int*>(&this->m_)),
		     sizeof(int));
    FileStream.write(reinterpret_cast<char*>(const_cast<int*>(&this->n_)),
		     sizeof(int));

    FileStream.write(reinterpret_cast<char*>(this->data_),
		     this->GetDataSize() * sizeof(value_type));

#ifdef SELDON_CHECK_IO
    // Checks if data was written.
    if (!FileStream.good())
      throw IOError("Matrix_HermPacked::Write(ofstream& FileStream)",
                    string("Output operation failed.")
		    + string(" The output file may have been removed")
		    + " or there is no space left on device.");
#endif

  }


  //! Writes the matrix in a file.
  /*!
    Stores the matrix in a file in text format.
    Only matrix elements are written (not dimensions).
    Each row is written on a single line and elements of
    a row are delimited by tabulations.
    \param FileName output file name.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::WriteText(string FileName) const
  {
    ofstream FileStream;
    FileStream.precision(cout.precision());
    FileStream.flags(cout.flags());
    FileStream.open(FileName.c_str());

#ifdef SELDON_CHECK_IO
    // Checks if the file was opened.
    if (!FileStream.is_open())
      throw IOError("Matrix_HermPacked::WriteText(string FileName)",
		    string("Unable to open file \"") + FileName + "\".");
#endif

    this->WriteText(FileStream);

    FileStream.close();
  }


  //! Writes the matrix to an output stream.
  /*!
    Writes the matrix to an output stream in text format.
    Only matrix elements are written (not dimensions).
    Each row is written on a single line and elements of
    a row are delimited by tabulations.
    \param FileStream output stream.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::WriteText(ostream& FileStream) const
  {

#ifdef SELDON_CHECK_IO
    // Checks if the stream is ready.
    if (!FileStream.good())
      throw IOError("Matrix_HermPacked::WriteText(ofstream& FileStream)",
                    "Stream is not ready.");
#endif

    int i, j;
    for (i = 0; i < this->GetM(); i++)
      {
	for (j = 0; j < this->GetN(); j++)
	  FileStream << (*this)(i, j) << '\t';
	FileStream << endl;
      }

#ifdef SELDON_CHECK_IO
    // Checks if data was written.
    if (!FileStream.good())
      throw IOError("Matrix_HermPacked::WriteText(ofstream& FileStream)",
                    string("Output operation failed.")
		    + string(" The output file may have been removed")
		    + " or there is no space left on device.");
#endif

  }


  //! Reads the matrix from a file.
  /*!
    Reads a matrix stored in binary format in a file.
    The number of rows (integer) and the number of columns (integer)
    are read, and matrix elements are then read in the same order
    as it should be in memory (e.g. row-major storage).
    \param FileName input file name.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::Read(string FileName)
  {
    ifstream FileStream;
    FileStream.open(FileName.c_str(), ifstream::binary);

#ifdef SELDON_CHECK_IO
    // Checks if the file was opened.
    if (!FileStream.good())
      throw IOError("Matrix_HermPacked::Read(string FileName)",
		    string("Unable to open file \"") + FileName + "\".");
#endif

    this->Read(FileStream);

    FileStream.close();
  }


  //! Reads the matrix from an input stream.
  /*!
    Reads a matrix in binary format from an input stream.
    The number of rows (integer) and the number of columns (integer)
    are read, and matrix elements are then read in the same order
    as it should be in memory (e.g. row-major storage).
    \param FileStream input stream.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::Read(istream& FileStream)
  {

#ifdef SELDON_CHECK_IO
    // Checks if the stream is ready.
    if (!FileStream.good())
      throw IOError("Matrix_HermPacked::Read(ifstream& FileStream)",
                    "Stream is not ready.");
#endif

    int new_m, new_n;
    FileStream.read(reinterpret_cast<char*>(&new_m), sizeof(int));
    FileStream.read(reinterpret_cast<char*>(&new_n), sizeof(int));
    this->Reallocate(new_m, new_n);

    FileStream.read(reinterpret_cast<char*>(this->data_),
		    this->GetDataSize() * sizeof(value_type));

#ifdef SELDON_CHECK_IO
    // Checks if data was read.
    if (!FileStream.good())
      throw IOError("Matrix_HermPacked::Read(ifstream& FileStream)",
                    string("Input operation failed.")
		    + string(" The input file may have been removed")
		    + " or may not contain enough data.");
#endif

  }


  //! Reads the matrix from a file.
  /*!
    Reads a matrix stored in text format in a file.
    \param FileName input file name.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>::ReadText(string FileName)
  {
    ifstream FileStream;
    FileStream.open(FileName.c_str());

#ifdef SELDON_CHECK_IO
    // Checks if the file was opened.
    if (!FileStream.is_open())
      throw IOError("Matrix_Pointers::ReadText(string FileName)",
		    string("Unable to open file \"") + FileName + "\".");
#endif

    this->ReadText(FileStream);

    FileStream.close();
  }


  //! Reads the matrix from an input stream.
  /*!
    Reads a matrix in text format from an input stream.
    \param FileStream input stream.
  */
  template <class T, class Prop, class Storage, class Allocator>
  void Matrix_HermPacked<T, Prop, Storage, Allocator>
  ::ReadText(istream& FileStream)
  {
    // clears previous matrix
    Clear();

#ifdef SELDON_CHECK_IO
    // Checks if the stream is ready.
    if (!FileStream.good())
      throw IOError("Matrix_Pointers::ReadText(ifstream& FileStream)",
                    "Stream is not ready.");
#endif

    // we read first line
    string line;
    getline(FileStream, line);

    if (FileStream.fail())
      {
	// empty file ?
	return;
      }

    // converting first line into a vector
    istringstream line_stream(line);
    Vector<T> first_row;
    first_row.ReadText(line_stream);

    // and now the other rows
    Vector<T> other_rows;
    other_rows.ReadText(FileStream);

    // number of rows and columns
    int n = first_row.GetM();
    int m = 1 + other_rows.GetM()/n;

#ifdef SELDON_CHECK_IO
    // Checking number of elements
    if (other_rows.GetM() != (m-1)*n)
      throw IOError("Matrix_Pointers::ReadText(ifstream& FileStream)",
                    "The file should contain same number of columns.");
#endif

    this->Reallocate(m,n);
    // filling matrix
    for (int j = 0; j < n; j++)
      this->Val(0, j) = first_row(j);

    int nb = 0;
    for (int i = 1; i < m; i++)
      {
	for (int j = 0; j < i; j++)
	  nb++;

	for (int j = i; j < n; j++)
	  this->Val(i, j) = other_rows(nb++);
      }
  }


  ///////////////////////////
  // MATRIX<COLHERMPACKED> //
  ///////////////////////////


  /*******************
   * OTHER FUNCTIONS *
   *******************/


  //! Multiplies the matrix by a given value.
  /*!
    \param x multiplication coefficient
    \warning imaginary part of x should be null to keep an hermitian matrix
  */
  template <class T, class Prop, class Allocator>
  template <class T0>
  Matrix<T, Prop, ColHermPacked, Allocator>&
  Matrix<T, Prop, ColHermPacked, Allocator>::operator*= (const T0& x)
  {
    for (int i = 0; i < this->GetDataSize();i++)
      this->data_[i] *= x;

    return *this;
  }


  //! Reallocates memory to resize the matrix and keeps previous entries.
  /*!
    On exit, the matrix is a i x j matrix.
    \param i new number of rows.
    \param j new number of columns.
    \warning The previous entries are kept, extra-entries may not be not
    initialized (depending of the allocator).
  */
  template <class T, class Prop, class Allocator>
  void Matrix<T, Prop, ColHermPacked, Allocator>
  ::Resize(int i, int j)
  {

    // Storing the old values of the matrix.
    int nold = this->GetDataSize();
    Vector<T, VectFull, Allocator> xold(nold);
    for (int k = 0; k < nold; k++)
      xold(k) = this->data_[k];

    // Reallocation.
    this->Reallocate(i, j);

    // Filling the matrix with its old values.
    int nmin = min(nold, this->GetDataSize());
    for (int k = 0; k < nmin; k++)
      this->data_[k] = xold(k);
  }


  ///////////////////////////
  // MATRIX<ROWHERMPACKED> //
  ///////////////////////////


  /*******************
   * OTHER FUNCTIONS *
   *******************/


  //! Multiplies the matrix by a given value.
  /*!
    \param x multiplication coefficient
  */
  template <class T, class Prop, class Allocator>
  template <class T0>
  Matrix<T, Prop, RowHermPacked, Allocator>&
  Matrix<T, Prop, RowHermPacked, Allocator>::operator*= (const T0& x)
  {
    for (int i = 0; i < this->GetDataSize();i++)
      this->data_[i] *= x;

    return *this;
  }


  //! Reallocates memory to resize the matrix and keeps previous entries.
  /*!
    On exit, the matrix is a i x j matrix.
    \param i new number of rows.
    \param j new number of columns.
    \warning The previous entries are kept, extra-entries may not be
    initialized (depending of the allocator).
  */
  template <class T, class Prop, class Allocator>
  void Matrix<T, Prop, RowHermPacked, Allocator>
  ::Resize(int i, int j)
  {
    // Storing the old values of the matrix.
    int nold = this->GetDataSize(), iold = this->m_;
    Vector<T, VectFull, Allocator> xold(nold);
    for (int k = 0; k < nold; k++)
      xold(k) = this->data_[k];

    // Reallocation.
    this->Reallocate(i, j);

    // Filling the matrix with its old values.
    int imin = min(iold, i);
    nold = 0;
    int n = 0;
    for (int k = 0; k < imin; k++)
      {
	for (int l = k; l < imin; l++)
	  this->data_[n+l-k] = xold(nold+l-k);

	n += i - k;
	nold += iold - k;
      }
  }


} // namespace Seldon.

#define SELDON_FILE_MATRIX_HERMPACKED_CXX
#endif
