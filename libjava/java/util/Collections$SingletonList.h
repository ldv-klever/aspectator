
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __java_util_Collections$SingletonList__
#define __java_util_Collections$SingletonList__

#pragma interface

#include <java/util/AbstractList.h>
#include <gcj/array.h>


class java::util::Collections$SingletonList : public ::java::util::AbstractList
{

public: // actually package-private
  Collections$SingletonList(::java::lang::Object *);
public:
  jint size();
  ::java::lang::Object * get(jint);
  jboolean contains(::java::lang::Object *);
  jboolean containsAll(::java::util::Collection *);
  jint hashCode();
  jint indexOf(::java::lang::Object *);
  jint lastIndexOf(::java::lang::Object *);
  ::java::util::List * subList(jint, jint);
  JArray< ::java::lang::Object * > * toArray();
  ::java::lang::String * toString();
private:
  static const jlong serialVersionUID = 3093736618740652951LL;
  ::java::lang::Object * __attribute__((aligned(__alignof__( ::java::util::AbstractList)))) element;
public:
  static ::java::lang::Class class$;
};

#endif // __java_util_Collections$SingletonList__
