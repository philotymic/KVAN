// -*- c++ -*-
#ifndef __KVAN_STRUCT_DESCRIPTOR__HH__
#define __KVAN_STRUCT_DESCRIPTOR__HH__

#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <any>
#include <initializer_list>
using namespace std;

#include <kvan/addl-type-traits.h>
#include <kvan/enum-io.h>
#include <kvan/string-utils.h>

typedef vector<string> LOBKey;
class StructDescriptor;
template <class T> inline StructDescriptor get_struct_descriptor();

class StructVisitor
{
public:
  virtual void visit_enum(const LOBKey& path, const string& enum_s) = 0;
  virtual void visit_string(const LOBKey& path, const string& s) = 0;
  virtual void visit_fundamental(const LOBKey& path, const string& v) = 0;
  virtual void visit_start_map(const LOBKey& path) = 0;
  virtual void visit_end_map() = 0;
  virtual void visit_delimiter() = 0;
  virtual void visit_start_array() = 0;
  virtual void visit_end_array() = 0;
};

class MemberDescriptor
{
public:
  string member_name;
  MemberDescriptor(const char* member_name);

  virtual void set_value__(void* o, const string& new_value,
			   const LOBKey& path,
			   int curr_member_index) = 0;
  virtual void visit_member(StructVisitor* visitor,
			    LOBKey* curr_vpath,
			    const any& o) = 0;
  virtual void visit_member_array_element(StructVisitor* visitor,
					  LOBKey* curr_vpath,
					  const any& o) = 0;
};

template <class MT, class T>
class MemberDescriptorT : public MemberDescriptor
{
private:
  MT T::*mptr;

public:
  MemberDescriptorT(const char* member_name, MT T::*mptr);
  void set_value__(void* o, const string& new_value,
		   const LOBKey& path, int curr_member_index) override;

  void visit_member_array_element(StructVisitor* visitor, LOBKey* curr_vpath,
				  const any& o) override;
  void visit_member(StructVisitor* visitor, LOBKey* curr_vpath,
		    const any& o) override;
};

class StructDescriptor
{
public:  
  void visit_members(StructVisitor* sv, LOBKey* curr_vpath, const any& o);
  void set_value__(void* o, const string& new_value,
		   const LOBKey& path, size_t curr_index);

public:
  vector<string> member_names;
  vector<shared_ptr<MemberDescriptor>> member_descriptors;
  map<string, int> member_lookup;

  StructDescriptor(initializer_list<shared_ptr<MemberDescriptor>> l);
  void set_value(void* o, const LOBKey& path, const string& new_value);
};

template <class T> inline
StructDescriptor get_struct_descriptor() {
  static_assert(assert_false<T>::value, "provide spec");
  return StructDescriptor();
}

template <class MT, class T> inline
shared_ptr<MemberDescriptor> make_member_descriptor(const char* n, MT T::* mptr)
{
  return make_shared<MemberDescriptorT<MT, T>>(n, mptr);
}

#include <kvan/struct-descriptor-inlines.h>

#endif
