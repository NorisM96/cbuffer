#ifndef CBUFF_H
#define CBUFF_H

#include <ostream> // std::ostream
#include <iostream>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t
/**
@file cbuffer.h
@brief Dichiarazione della classe cbuffer
**/

/**
Classe che rappresenta un buffer circolare di un tipo t
**/
template <typename T>
class cbuffer {
public:
  typedef unsigned int size_type; ///< Definzione del tipo corrispondente a size
  typedef T value_type;

  /**
  @brief Costruttore di default (METODO FONDAMENTALE)
  Costruttore di default per istanziare un cbuffer vuoto.
  **/

  cbuffer(): _size(0), _buffer(0), first(0), last(0), freespace(0) {
  }

  /**
  @brief Costruttore secondario

  Costruttore secondario. Permette di istanziare un cbuffer con una data dimensione.
  @param size Dimensione del cbuffer da istanziare
  **/

  explicit cbuffer(size_type size) : _size(0), _buffer(0), first(0), last(0), freespace(0)  {
    _buffer = new T[size];
    _size = size;
    first = 0;
    last = 0;
    freespace = size;
  }

  /**
  @brief Costruttore secondario

  Costruttore secondario. Permette di istanziare un cbuffer con una data dimensione
  e di inizializzare le celle dell'array con il valore dato
  @param size Dimensione del cbuffer da istanziare
  @param value Valore da usare per inizizalizzare le celle dell'array
  **/

  cbuffer(size_type size, const T &value) : _size(0), _buffer(0), first(0), last(0), freespace(0)  {
    _buffer = new T[size];
    _size = size;
    first = 0;
    last = size - 1;
    freespace = 0;
    try {
      for(size_type i=0 ; i < _size; ++i)
      _buffer[i] = value;
    }
    catch(...) {
      delete[] _buffer;
      _size = 0;
      _buffer = 0;
      throw;
    }
  }

  /**
  @brief Costruttore secondario

  Costruttore secondario. Permette di istanziare un cbuffer con un array di elementi che
  gli passo(l'ho scritto principalmente per i test)
  @param size Dimensione dell'array in input
  @param arr Indirizzo cella iniziale array
  **/

  cbuffer(size_type size, const T* arr) : _size(0), _buffer(0), first(0), last(0), freespace(0)  {
    _buffer = new T[size];
    _size = size;
    first = 0;
    last = size - 1;
    freespace = 0;
    try {
      for(size_type i=0 ; i < _size; ++i)
      _buffer[i] = *(arr+i);
    }
    catch(...) {
      delete[] _buffer;
      _size = 0;
      _buffer = 0;
      throw;
    }
  }


  /**
  @brief Costruttore secondario (RICHIESTO)

  Costruttore secondario. Permette di istanziare un cbuffer partendo da una generica
  struttura dati con coppia di iteratori generici
  @param size Dimensione struttura dati e due iteratori
  @param begin iteratore inizio sequenza dati
  @param end iteratore fine sequenza dati
  **/

  template <typename I>
  cbuffer(size_type size,I begin, I end) : _buffer(0), _size(0), first(0), last(0), freespace(0) {
    try {
      _buffer = new T[size];
      first = 0;
      last = size - 1;
      _size = size;
      freespace = 0;
      for(; begin!=end; ++begin)
		    enqueue(static_cast<T>(*begin));
		}
		catch(...) {
			throw;
		}
	}

  /**
  @brief Copy constructor (METODO FONDAMENTALE)

  Costruttore di copia. Permette di istanziare un cbuffer con i valori
  presi da un altro cbuffer.
  @param other cbuffer da usare per creare quello corrente
  **/

  cbuffer(const cbuffer &other) : _size(0), _buffer(0), first(0), last(0), freespace(0)  {
    _buffer = new T[other._size];
    _size = other._size;
    freespace = other._size;
    try {
      for(size_type i=0; i<_size; ++i){
        _buffer[i] = other._buffer[i];
        freespace = freespace - 1;
      }
      first = other.first;
      last = other.last;
    }
    catch(...) {
      delete[] _buffer;
      _size = 0;
      _buffer = 0;
      first = 0;
      last = 0;
      freespace = 0;
      throw;
    }
  }

  /**
  @brief Operatore di assegnamento (METODO FONDAMENTALE)

  Operatore di assegnamento. Permette la copia tra cbuffer.
  @param other cbuffer sorgente
  @return riferimento a this
**/

cbuffer &operator=(const cbuffer &other) {
  if (this != &other) {
    // Proviamo a copiare i nuovi dati in un cbuffer di appoggio
    // Se la copia fallisce, viene lanciata una eccezione
    cbuffer tmp(other);
    // Se la copia riesce, scambiamo i dati di this con quelli del cbuffer di appoggio
    this->swap(tmp);
    // All'uscita dell'if, tmp viene automaticamente distrutto
  }
  return *this;
}

/**
@brief Distruttore (METODO FONDAMENTALE)

Distruttore. Rimuove la memoria allocata da cbuffer.
**/

~cbuffer() {
  delete[] _buffer;
  _buffer = 0;
  _size = 0;
  last = 0;
  first = 0;
  freespace = 0;
}

/**
@brief Funzione che ritorna se la coda è vuota o no
Funzione che ritorna il valore di empty
**/

bool isEmpty() const{
  return freespace == _size;
}

/**
@brief Accoda un valore al cbuffer
Accoda un valore al cbuffer
@param value valore da accodare
**/

bool enqueue(const T &value){
  assert(_size != 0);
  if(first == last + 1){ //il buffer è pieno e gli indici sono in mezzo (circolare)
    _buffer[first] = value;
    first = (first + 1) % (_size - 1);
    last = (last + 1);
  }
  else if(((last >= first && last < (_size-1)) && !isEmpty())|| (first > last)){  //buffer ancora da riempire(lineare)
    last = last + 1;
    _buffer[last] = value;
    freespace--;
  }
  else if((last == (_size - 1)) && first == 0){ //aggiunta elemento buffer pieno (circolare)
    last = 0;
    first = 1;
    _buffer[last] = value;
  }
  else if(last == (_size - 1)) //buffer non pieno (inizio non in posizione 0)(circolare)
  {
    last = 0;
    _buffer[last] = value;
    freespace--;
  }
  else{   //primo elemento inserito nell'array
    _buffer[last] = value;
    freespace--;
  }
}


/**
@brief Metodo che conta i valori contenuti in cbuffer
Funzione che conta i valori contenuti in cbuffer
**/

size_type countelem() const{
  return _size - freespace;
}

/**
@brief Funzione che toglie la testa da cbuffer(logicamente)
Funzione che toglie la testa da cbuffer(logicamente)
**/

bool pop(){
  assert(!isEmpty()); //non posso applicare pop a code con un solo elemento!
  if(first == last)
    freespace = _size;
  else{
    first = (first + 1)%_size;
    freespace++;
  }
}

/**
@brief Funzione che confronta cbuffer chiamante con quella passata
e restituisce se sono uguali
Funzione che confronta cbuffer chiamante con quella passata
e restituisce se sono uguali
@param CB da controllare
@return risultato confronto
**/

bool equals(const cbuffer<T> &CB){
    bool equal = true;
    if(_size != CB._size)
      return false;
    typename cbuffer<T>::const_iterator i1=begin(), ie1=end(), i2 = CB.begin(), ie2 = CB.end();
    while(equal){
      if((i1 == ie1 && i2 != ie2) || (i1 != ie1 && i2 == ie2)) //uno dei due buffer ha finito ma l'altro no
         return false;
       else if (i1 == ie1 && i2 == ie2) //hanno entrambi finito e elementi =
         return true;
       else if(*i1 != *i2)  //elementi diversi
         return  false;
      else{      //elementi uguali
        ++i1;
        ++i2;
      }
    }
}

/**
@brief Accesso ai dati in lettura/scrittura (stile C++ con operatore)

Metodo getter per leggere il valore index-esimo dell'array.

@pre E' necessario che index < size
@param index Indice della cella dell'array da leggere
@return Il valore della cella index-esima
**/

T &operator[](size_type index) {
  assert(index < (countelem())); // asserzione se viene violata il programma termina
  return _buffer[((first+index)%_size)];
}

/**
@brief Accesso ai dati in lettura (stile C++ con operatore)
Metodo setter per scrivere il valore index-esimo dell'array.
@pre E' necessario che index < size
@param index Indice della cella dell'array da scrivere
**/

const T &operator[](size_type index) const {
  assert(index < (countelem())); // asserzione (l'indice sfora la dimensione impostata del buffer)
  return _buffer[((first+index)%_size)];
}

/**
@brief Scambia il contenuto di due cbuffer
Scambia il contenuto di due cbuffer.
@param other cbuffer con il quale scambiare i dati
**/

void swap(cbuffer &other) {
  std::swap(other._size, this->_size);
  std::swap(other._buffer, this->_buffer);
  std::swap(other.first, this->first);
  std::swap(other.last, this->last);
  std::swap(other.freespace, this->freespace);
}

class const_iterator;

class iterator {
  T *ptr;
  T *firstelem;
  size_type sizec;
  int pos;

public:
  typedef std::forward_iterator_tag iterator_category;
  typedef T                         value_type;
  typedef ptrdiff_t                 difference_type;
  typedef T*                        pointer;
  typedef T&                        reference;

  iterator() : ptr(0), sizec(0), firstelem(0), pos(0) {
  }

  iterator(const iterator &other) : ptr(other.ptr), sizec(other.sizec), firstelem(other.firstelem) {
  }


  //operatore di assegnamento
  iterator& operator=(const iterator &other) {
    ptr = other.ptr;
    sizec = other.sizec;
    firstelem = other.firstelem;
    pos = other.pos;
    return *this;
  }



  ~iterator() {}

  // Ritorna il dato riferito dall'iteratore (dereferenziamento)
  reference operator*() const {
    return *ptr;
  }

  // Ritorna il puntatore al dato riferito dall'iteratore
  pointer operator->() const {
    return ptr;
  }

  // Operatore di iterazione post-incremento
  iterator operator++(int) {
    const_iterator tmp(*this);
    ++ptr;
    ++pos;
    if(const_iterator(ptr, sizec, firstelem, pos)  == const_iterator(firstelem + sizec, sizec, firstelem, pos)){ //se arrivo a end
      if(pos != sizec)  //caso end fisica != end logica
        tmp.ptr = firstelem;
      }
    return tmp;
  }

  // Operatore di iterazione pre-incremento
  iterator& operator++() {
    ++ptr;
    pos++;
    if(iterator(ptr, sizec, firstelem, pos)  == iterator(firstelem + sizec, sizec, firstelem, pos)){ //se arrivo a end
      if(pos != sizec) //caso end fisica != end logica
       ptr = firstelem;
    }
    return *this;
  }

  // Uguaglianza
  bool operator==(const iterator &other) const {
    return (ptr == other.ptr && firstelem == other.firstelem && sizec == other.sizec && pos == other.pos);

  }

  // Diversita'
  bool operator!=(const iterator &other) const {
    return (ptr != other.ptr ||  firstelem != other.firstelem || sizec != other.sizec || pos != other.pos);
  }

  //Funzione getter che restituisce la posizione logica del'iteratore nell'array
  int getPos() const {
    return pos;
  }

  friend class const_iterator;

  // Uguaglianza
  bool operator==(const const_iterator &other) const {
    return (ptr == other.ptr && firstelem == other.firstelem && sizec == other.sizec && pos == other.pos);
  }

  // Diversita'
  bool operator!=(const const_iterator &other) const {
    return (ptr != other.ptr ||  firstelem != other.firstelem || sizec != other.sizec || pos != other.pos);
  }

private:
  friend class cbuffer;

  iterator(T*p, size_type sizecb, T*fe, int posc) : ptr(p), sizec(sizecb), firstelem(fe),pos(posc) {

  }


}; // classe iterator

class const_iterator {
  const T *ptr;
  const T *firstelem;
  size_type sizec;
  int pos;

public:
  typedef std::forward_iterator_tag iterator_category;
  typedef T                         value_type;
  typedef ptrdiff_t                 difference_type;
  typedef const T*                  pointer;
  typedef const T&                  reference;


  const_iterator() : ptr(0), sizec(0), firstelem(0), pos(0) {
  }

  const_iterator(const const_iterator &other)
  : ptr(other.ptr), firstelem(other.firstelem), sizec(other.sizec), pos(other.pos) {

  }

  const_iterator(const iterator &other)
  : ptr(other.ptr), firstelem(other.firstelem), sizec(other.sizec), pos(other.pos)  {

  }

  const_iterator& operator=(const const_iterator &other) {
    ptr = other.ptr;
    firstelem = other.firstelem;
    sizec = other.sizec;
    pos = other.pos;
    return *this;
  }

  const_iterator& operator=(const iterator &other) {
    ptr = other.ptr;
    firstelem = other.firstelem;
    sizec = other.sizec;
    return *this;
  }

  ~const_iterator() {
  }

  // Ritorna il dato riferito dall'iteratore (dereferenziamento)
  reference operator*() const {
    return *ptr;
  }

  // Ritorna il puntatore al dato riferito dall'iteratore
  pointer operator->() const {
    return ptr;
  }

  // Operatore di iterazione post-incremento
  const_iterator operator++(int) {
    const_iterator tmp(*this);
    ++ptr;
    ++pos;
    if(const_iterator(ptr, sizec, firstelem, pos)  == const_iterator(firstelem + sizec, sizec, firstelem, pos)){
      if(pos != sizec)
        tmp.ptr = firstelem;
      }
    return tmp;
  }

  // Operatore di iterazione pre-incremento
  const_iterator& operator++() {
    ++ptr;
    ++pos;
    if(const_iterator(ptr, sizec, firstelem, pos)  == const_iterator(firstelem + sizec, sizec, firstelem, pos)){   //se arrivo a end
      if(pos != sizec)  //caso  end fisica != end logica
        ptr = firstelem;
      }
    return *this;
  }

  // Uguaglianza
  bool operator==(const const_iterator &other) const {
    return (ptr == other.ptr && firstelem == other.firstelem && sizec == other.sizec && pos == other.pos);

  }
  // Diversita'
  bool operator!=(const const_iterator &other) const {
    return (ptr != other.ptr ||  firstelem != other.firstelem || sizec != other.sizec || pos != other.pos);
  }

  friend class iterator;

  // Uguaglianza
  bool operator==(const iterator &other) const {
    return (ptr == other.ptr && firstelem == other.firstelem && sizec == other.sizec && pos == other.pos);
  }

  // Diversita'
  bool operator!=(const iterator &other) const {
    return (ptr != other.ptr ||  firstelem != other.firstelem || _size != other.sizec || pos != other.pos);
  }
  //Funzione getter che restituisce la posizione logica del'iteratore nell'array
  int getPos() const {
    return pos;
  }
private:
  friend class cbuffer;

  // Costruttore privato usabile da cbuffer
  const_iterator(const T*p,const size_type sizecb, const T*fe, int posizione) : ptr(p), sizec(sizecb), firstelem(fe), pos(posizione) {

  }

}; // classe const_iterator

// Ritorna l'iteratore all'first della sequenza dati
iterator begin() {
  return iterator(_buffer + first, _size, _buffer,0);
}

// Ritorna l'iteratore alla last della sequenza dati
iterator end() {
  return iterator(_buffer + last + 1, _size, _buffer,countelem());
}


//Ritorna l'iteratore costante a first della sequenza dati
const_iterator begin() const {
  return const_iterator(_buffer + first, _size, _buffer,0);
}

// Ritorna l'iteratore costante a last della sequenza dati
const_iterator end() const {
  return const_iterator(_buffer + last + 1,_size, _buffer,countelem());
}


private:

  T *_buffer; ///< Puntatore al buffer
  size_type _size; ///< Dimensione dell'array
  size_type first; ///< Posizione nell'array dell'elemento più vecchio
  size_type last; ///< Posizione nell'array dell'elemento più nuovo
  size_type freespace;///< N elementi mancanti a riempire il buffer
};

/**
	Ridefinizione dell'operatore di stream per la stampa
	del cbuffer

	@param os oggetto stream di output
	@param cbuff cbuffer da stampare
	@return reference allo stream di output
*/

template <typename T>
std::ostream &operator<<(std::ostream &os,
	const cbuffer<T> &cbuff) {
    assert(!cbuff.isEmpty());
    typename cbuffer<T>::const_iterator i, ie;
    for(i = cbuff.begin(), ie = cbuff.end(); i != ie;++i)
  		os << *i << std::endl;
}

/**
  Funzione globale che prende un cbuffer e un predicato unario
  e stampa il risultato della sua applicazione su ogni elemento
  del cbuffer

	@param CB cbuffer su cui applicare il predicato
	@param functor funtore del predicata
*/


template <typename T,typename fctr>
void evaluate_if(const cbuffer<T> &CB,fctr functor){
  typename cbuffer<T>::const_iterator i, ie;
  for(i = CB.begin(), ie = CB.end(); i != ie;++i)
    if( functor ( *i ))
      std::cout<<"["<<i.getPos()<<"] : true"<<std::endl;
    else
      std::cout<<"["<<i.getPos()<<"] : false"<<std::endl;
}
#endif
