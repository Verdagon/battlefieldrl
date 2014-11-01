#pragma once

#include <type_traits>
#include <functional>
#include <cassert>
#include <iostream>

#define flare(args...) (flare_impl(__FILE__, __LINE__, ## args))

template<typename ...Params>
inline void flare_impl_values(Params && ... args);

template<typename First, typename ...Rest>
inline void flare_impl_values(First && firstValue, Rest && ... restValues) {
  std::cout << " " << std::forward<First>(firstValue);
  flare_impl_values(std::forward<Rest>(restValues)...);
}

template<>
inline void flare_impl_values() {
  std::cout << std::endl;
}

template<typename ...T>
inline void flare_impl(const char *file, int line, T && ... values) {
  std::cout << "flare " << file << ":" << line;
  flare_impl_values(std::forward<T>(values)...);
}


// Begin ecatmur's code
template<typename T> struct remove_class { };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...)> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) volatile> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const volatile> { using type = R(A...); };

template<typename T>
struct get_signature_impl { using type = typename remove_class<
    decltype(&std::remove_reference<T>::type::operator())>::type; };
template<typename R, typename... A>
struct get_signature_impl<R(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(&)(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(*)(A...)> { using type = R(A...); };
template<typename T> using get_signature = typename get_signature_impl<T>::type;
// End ecatmur's code

// Begin typecase code
template<typename Base, typename FirstSubclass, typename... RestOfSubclasses>
void typecase(
        Base *base,
        FirstSubclass &&first,
        RestOfSubclasses &&... rest) {

    using Signature = get_signature<FirstSubclass>;
    using Function = std::function<Signature>;

    if (typecaseHelper(base, (Function)first)) {
        return;
    }
    else {
        typecase(base, rest...);
    }
}
template<typename Base>
void typecase(Base *) {
    assert(false);
}
template<typename Base, typename T>
bool typecaseHelper(Base *base, std::function<void(T *)> func) {
    if (T *first = dynamic_cast<T *>(base)) {
        func(first);
        return true;
    }
    else {
        return false;
    }
}
// End typecase code

class Coord {
public:
  int row, col;

  inline Coord() :
      row(0), col(0) {
  }

  inline Coord(int initRow, int initCol) :
      row(initRow), col(initCol) {
  }

  inline Coord(const Coord & that) :
      row(that.row), col(that.col) {
  }

  inline bool operator==(Coord that) const {
    return this->row == that.row && this->col == that.col;
  }
  inline bool operator!=(Coord that) const {
    return !(*this == that);
  }

  inline Coord operator+(Coord that) const {
    return Coord(this->row + that.row, this->col + that.col);
  }

  inline bool inside(Coord that) const {
    return row >= 0 && col >= 0 && row < that.row && col < that.col;
  }
};

inline std::ostream & operator<<(std::ostream & out, Coord coord) {
  return out << "C(" << coord.row << "," << coord.col << ")";
}

class CoordIterator {
  Coord current;
  Coord extent;

  CoordIterator(Coord initCurrent, Coord initExtent) :
      current(initCurrent), extent(initExtent) { }

public:
  inline bool operator!=(const CoordIterator & that) const {
    assert(this->extent == that.extent);
    return this->current != that.current;
  }
  inline void operator++() {
    bool atEnd = (current.row == extent.row);
    assert(!atEnd);

    current.col++;
    if (current.col == extent.col) {
      current.col = 0;
      current.row++;
    }
  }
  inline void operator++(int) { operator++(); }
  inline Coord operator*() const { return current; }

  friend CoordIterator begin(Coord coord);
  friend CoordIterator end(Coord coord);
};


inline CoordIterator begin(Coord coord) {
  return CoordIterator(Coord(0, 0), coord);
}
inline CoordIterator end(Coord coord) {
  return CoordIterator(Coord(coord.row, 0), coord);
}



template<typename T>
class Grid {
private:
  Coord mSize;
  T ** mArray;

public:
  Grid() :
      mSize(0, 0),
      mArray(nullptr) {
  }

  Grid(Coord size, const T & defaultValue = T()) :
      mSize(size) {
    mArray = new T*[mSize.row];
    for (int row = 0; row < mSize.row; row++) {
      mArray[row] = new T[mSize.col];
      for (int col = 0; col < mSize.col; col++) {
        mArray[row][col] = defaultValue;
      }
    }
  }

  ~Grid() {
    if (mArray != nullptr) {
      for (int row = 0; row < mSize.row; row++) {
        delete[] mArray[row];
      }
      delete[] mArray;
      mArray = nullptr;
    }
  }

  Grid(Grid && that) :
      mSize(that.mSize),
      mArray(that.mArray) {
    that.mSize = Coord(0, 0);
    that.mArray = nullptr;
  }

  // Unimplemented
  template<typename Y>
  Grid(const Y & that) = delete;

  // Unimplemented
  template<typename Y>
  void operator=(Y && that) = delete;



  inline Coord size() const {
    return mSize;
  }

  inline T & operator[](Coord coord) {
    return mArray[coord.row][coord.col];
  }
  inline const T & operator[](Coord coord) const {
    return mArray[coord.row][coord.col];
  }

  class iterator {
    Grid * grid;
    CoordIterator coordIter;
  public:
    void operator++() {
      coordIter++;
    }
    void operator!=(const iterator & that) {
      assert(this->grid == that.grid);
      return this->coordIter == that.coordIter;
    }
    T & operator*() const {
      return (*grid)[coordIter];
    }
    T * operator->() const {
      return &(*grid)[coordIter];
    }
  };

  class const_iterator {
    const Grid * grid;
    CoordIterator coordIter;
  public:
    void operator++() {
      coordIter++;
    }
    void operator!=(const iterator & that) {
      assert(this->grid == that.grid);
      return this->coordIter == that.coordIter;
    }
    const T & operator*() const {
      return (*grid)[coordIter];
    }
    const T * operator->() const {
      return &(*grid)[coordIter];
    }
  };
};

template<typename T>
class viterBase {
public:
  virtual T & operator*() const = 0;
  virtual T * operator->() const = 0;
  virtual void operator++() = 0;
  virtual bool operator!=(const viterBase<T> & that) const = 0;
};

template<typename T>
class viter : public viterBase<T> {
  viterBase<T> * mIter;
public:
  virtual T & operator*() const override {
    return mIter->operator*();
  }
  virtual T * operator->() const override {
    return mIter->operator->();
  }
  virtual void operator++() override {
    mIter->operator++();
  }
  virtual bool operator!=(const viterBase<T> & that) const override {
    return mIter->operator!=(that);
  }
};

template<typename T, typename TIter>
class viterSpecialized : public viterBase<T> {
  TIter mIter;

public:
  viterSpecialized(TIter && iter) :
      mIter(std::forward<TIter>(iter)) {
  }
  virtual T & operator*() const override {
    return mIter.operator*();
  }
  virtual T * operator->() const override {
    return mIter.operator->();
  }
  virtual void operator++() override {
    mIter.operator++();
  }
  virtual bool operator!=(const viterBase<T> & that) const override {
    const viterImpl<T, TIter> * that2 = dynamic_cast<const viterImpl<T, TIter> *>(&that);
    assert(that2 != nullptr);
    return mIter != that2->mIter;
  }
};

template<typename TIter>
viter<TIter::value_type> viterify(TIter && t) {
  return viter<T>(std::forward<TIter>(t));
}

template<typename T>
class vrange {
public:
  using value_type = T;

private:
  viter<T> mBegin;
  viter<T> mEnd;

public:
  range(viter<T> begin, viter<T> end) :
      mBegin(begin),
      mEnd(end) { }

  viter<T> begin() const {
    return mBegin;
  }

  viter<T> end() const {
    return mEnd;
  }
};

template<typename TRange>
viter<TRange::value_type> vrangify(TRange && range) {
  return vrangify(range.begin(), range.end());
}

template<typename TIter>
viter<TIter::value_type> vrangify(TIter && begin, TIter && end) {
  return vrange<T>(
      viterify(std::forward<TIter>(begin)),
      viterify(std::forward<TIter>(end)));
}
