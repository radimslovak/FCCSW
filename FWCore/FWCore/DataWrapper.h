#ifndef FWCORE_DATAWRAPPER_H
#define FWCORE_DATAWRAPPER_H

#include <type_traits>

//Include files
#include "GaudiKernel/DataObject.h"
#include "albers/CollectionBase.h"

class GAUDI_API DataWrapperBase : public DataObject {
 public:
   // ugly hack to circumvent the usage of boost::any yet
   // DataSvc would need a templated register method
   virtual albers::CollectionBase* collectionBase() = 0;
   virtual ~DataWrapperBase(){};
};

template<class T>
class GAUDI_API DataWrapper : public DataWrapperBase {
 public:

  DataWrapper() : DataWrapperBase(), m_data(nullptr){};
  virtual ~DataWrapper();

  const T* getData() {return m_data;}
  void setData(T* data) {m_data = data;}
  /// try to cast to collectionBase; may return nullptr;
  virtual albers::CollectionBase* collectionBase();

 private:
  T* m_data;

};

template<class T>
DataWrapper<T>::~DataWrapper<T>() {
  if (m_data!=nullptr) delete m_data;
}

template<class T>
albers::CollectionBase* DataWrapper<T>::collectionBase(){
  if (std::is_base_of<albers::CollectionBase,T>::value){
    return reinterpret_cast<albers::CollectionBase*>(m_data);
  }
  return nullptr;
}


#endif