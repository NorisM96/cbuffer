#include "cbuffer.h"
#include <iostream> // std::cout
#include <stdexcept> // std::out_of_range
#include <cassert> // assert

#include <vector>

//tipo custom per test
struct course {
    unsigned int credits;
    std::string name;
    course() : credits(0), name("") {}
    course(unsigned int cr, std::string n) : credits(cr), name(n) {}
    bool operator!=(const course c) const{
      return ( credits != c.credits || name != c.name );
    }
    unsigned int getCredits() const{
      return credits;
    }
};


//funzioni e costrutti usati nel test di evaluate_if
bool even(int i){
    if ((i % 2) == 0)
      return true;
    else
      return false;
}

bool heavycourse(const course c){
  if(c.getCredits() >= 8)
    return true;
  else
    return false;
}

//test per construttori di base e secondari e funzione equals

void provacostruttorieequalseenqueue(){
  int prova[4] = {1, 2, 3, 4};
  int prova2[4] = {1, 2, 4,4};
  cbuffer<int> CBprova(4, prova),CBprova2(4),CBprova3(4, prova2);
  CBprova2.enqueue(5);
  CBprova2.enqueue(5);
  CBprova2.enqueue(1);
  CBprova2.enqueue(2);
  CBprova2.enqueue(3);
  CBprova2.enqueue(4);
  if (CBprova.equals(CBprova2)){
    if(!CBprova.equals(CBprova3))
      std::cout<<"test costruttori secondari,enqueue e equals PASSATO"<<std::endl;
    else
      std::cout<<"equals non ha riconosciuto un diverso"<<std::endl;
    }
  else
    std::cout<<"equals non ha riconosciuto un uguale"<<std::endl;
}

//test per construttori di base e secondari e funzione equals

void provacostruttorestrutturadati(){
  std::string stprova1[2] = {"wat","cat"};
  std::vector<std::string> vec;
  vec.push_back("a caso");
  vec.push_back("wat");
  vec.push_back("cat");
  std::vector<std::string>::iterator inizio;
  inizio = vec.begin();
  inizio++;
  cbuffer<std::string> CSTprova1(2 , inizio , vec.end()),CSTprova2(2, stprova1);//test per construttori di base e secondari e funzione equals
  if(CSTprova2.equals(CSTprova1))
    std::cout<<"test costruttore secondario con iteratori e sequenza PASSATO"<<std::endl;
  else
    std::cout<<"costruttore secondario con errori"<<std::endl;
}

//test copy constructor

void provacopyconstructor(){
    int prova[3] = {0, 1 ,2};
    cbuffer<int> CBprova(3,prova);
    cbuffer<int> CBprova1(CBprova);
    if(CBprova.equals(CBprova1))
      std::cout<<"test copy constructor PASSATO"<<std::endl;
    else
      std::cout<<"copy constructor con errori"<<std::endl;
}

//test operazioni di manipolazione di base anche con tipo custom

void provaoperazionidibaseconcustom(){
  cbuffer<course> CBp(2),CBp2(2);
  course nodo1(8, "PAS"), nodo2(8, "BIO"), nodo3(8, "RO");
  if(CBp.isEmpty() && CBp2.isEmpty()){
    CBp.enqueue(nodo1);
    CBp.enqueue(nodo2);
    CBp.enqueue(nodo3);
    CBp.pop();
    CBp2.enqueue(nodo3);
    if(CBp.equals(CBp2)){
      if(CBp.countelem() == CBp2.countelem() == 1)
        std::cout<<"test operazioni base con custom PASSATO"<<std::endl;
      else
        std::cout<<"counta elementi errata"<<std::endl;
      }
    else
      std::cout<<"enqueue/pop errate"<<std::endl;
  }
  else
    std::cout<<"funzione controllo vuotezza errata"<<std::endl;
}

//test accesso con operatori e iteratori lettura / scrittura

void provaaccessooperatorieoutput(){
  std::cout << "Test modifica elemento [] e iteratore: " << std::endl;
  int array[4] = {1, 2, 3, 4};
  cbuffer<int> CBP(4, array);
  std::cout << CBP;
  CBP[0] = 4;
  if(*CBP.begin() != 4)
    std::cout << "operatore [] errato" << std::endl;
  else{
    *CBP.begin() = 10;
    if(CBP[0] != 10)
      std::cout << "scrittura/lettura iteratore broken" << std::endl;
    else
      std::cout << "test accesso operatori e iteratori PASSATO" << std::endl;
  }

  std::cout << CBP;
}

//test evaluate if

void provaevaluateif(){
  course c1(4,"ING"), c2(8,"PAS");
  cbuffer<course> CBprov (2);
  cbuffer<int> CBprov2 (2);
  std::cout << "PROVA EVALUATE IF CUSTOM" << std::endl;
  CBprov.enqueue(c1);
  CBprov.enqueue(c2);
  evaluate_if(CBprov,heavycourse);
  std::cout << "PROVA EVALUATE IF NORMAL" << std::endl;
  CBprov2.enqueue(1);
  CBprov2.enqueue(2);
  evaluate_if(CBprov2,even);
}

//nel main semplicemente richiamo tutti i test

int main() {
  provacostruttorieequalseenqueue();
  provacostruttorestrutturadati();
  provacopyconstructor();
  provaoperazionidibaseconcustom();
  provaaccessooperatorieoutput();
  provaevaluateif();
}
