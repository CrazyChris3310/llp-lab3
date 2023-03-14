// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef REQ_SCHEMA_HXX
#define REQ_SCHEMA_HXX

#ifndef XSD_CXX11
#define XSD_CXX11
#endif

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 4000000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>

#include <xsd/cxx/xml/dom/serialization-header.hxx>
#include <xsd/cxx/tree/serialization.hxx>
#include <xsd/cxx/tree/serialization/byte.hxx>
#include <xsd/cxx/tree/serialization/unsigned-byte.hxx>
#include <xsd/cxx/tree/serialization/short.hxx>
#include <xsd/cxx/tree/serialization/unsigned-short.hxx>
#include <xsd/cxx/tree/serialization/int.hxx>
#include <xsd/cxx/tree/serialization/unsigned-int.hxx>
#include <xsd/cxx/tree/serialization/long.hxx>
#include <xsd/cxx/tree/serialization/unsigned-long.hxx>
#include <xsd/cxx/tree/serialization/boolean.hxx>
#include <xsd/cxx/tree/serialization/float.hxx>
#include <xsd/cxx/tree/serialization/double.hxx>
#include <xsd/cxx/tree/serialization/decimal.hxx>

namespace xml_schema
{
  // anyType and anySimpleType.
  //
  typedef ::xsd::cxx::tree::type type;
  typedef ::xsd::cxx::tree::simple_type< char, type > simple_type;
  typedef ::xsd::cxx::tree::type container;

  // 8-bit
  //
  typedef signed char byte;
  typedef unsigned char unsigned_byte;

  // 16-bit
  //
  typedef short short_;
  typedef unsigned short unsigned_short;

  // 32-bit
  //
  typedef int int_;
  typedef unsigned int unsigned_int;

  // 64-bit
  //
  typedef long long long_;
  typedef unsigned long long unsigned_long;

  // Supposed to be arbitrary-length integral types.
  //
  typedef long long integer;
  typedef long long non_positive_integer;
  typedef unsigned long long non_negative_integer;
  typedef unsigned long long positive_integer;
  typedef long long negative_integer;

  // Boolean.
  //
  typedef bool boolean;

  // Floating-point types.
  //
  typedef float float_;
  typedef double double_;
  typedef double decimal;

  // String types.
  //
  typedef ::xsd::cxx::tree::string< char, simple_type > string;
  typedef ::xsd::cxx::tree::normalized_string< char, string > normalized_string;
  typedef ::xsd::cxx::tree::token< char, normalized_string > token;
  typedef ::xsd::cxx::tree::name< char, token > name;
  typedef ::xsd::cxx::tree::nmtoken< char, token > nmtoken;
  typedef ::xsd::cxx::tree::nmtokens< char, simple_type, nmtoken > nmtokens;
  typedef ::xsd::cxx::tree::ncname< char, name > ncname;
  typedef ::xsd::cxx::tree::language< char, token > language;

  // ID/IDREF.
  //
  typedef ::xsd::cxx::tree::id< char, ncname > id;
  typedef ::xsd::cxx::tree::idref< char, ncname, type > idref;
  typedef ::xsd::cxx::tree::idrefs< char, simple_type, idref > idrefs;

  // URI.
  //
  typedef ::xsd::cxx::tree::uri< char, simple_type > uri;

  // Qualified name.
  //
  typedef ::xsd::cxx::tree::qname< char, simple_type, uri, ncname > qname;

  // Binary.
  //
  typedef ::xsd::cxx::tree::buffer< char > buffer;
  typedef ::xsd::cxx::tree::base64_binary< char, simple_type > base64_binary;
  typedef ::xsd::cxx::tree::hex_binary< char, simple_type > hex_binary;

  // Date/time.
  //
  typedef ::xsd::cxx::tree::time_zone time_zone;
  typedef ::xsd::cxx::tree::date< char, simple_type > date;
  typedef ::xsd::cxx::tree::date_time< char, simple_type > date_time;
  typedef ::xsd::cxx::tree::duration< char, simple_type > duration;
  typedef ::xsd::cxx::tree::gday< char, simple_type > gday;
  typedef ::xsd::cxx::tree::gmonth< char, simple_type > gmonth;
  typedef ::xsd::cxx::tree::gmonth_day< char, simple_type > gmonth_day;
  typedef ::xsd::cxx::tree::gyear< char, simple_type > gyear;
  typedef ::xsd::cxx::tree::gyear_month< char, simple_type > gyear_month;
  typedef ::xsd::cxx::tree::time< char, simple_type > time;

  // Entity.
  //
  typedef ::xsd::cxx::tree::entity< char, ncname > entity;
  typedef ::xsd::cxx::tree::entities< char, simple_type, entity > entities;

  typedef ::xsd::cxx::tree::content_order content_order;
  // Namespace information and list stream. Used in
  // serialization functions.
  //
  typedef ::xsd::cxx::xml::dom::namespace_info< char > namespace_info;
  typedef ::xsd::cxx::xml::dom::namespace_infomap< char > namespace_infomap;
  typedef ::xsd::cxx::tree::list_stream< char > list_stream;
  typedef ::xsd::cxx::tree::as_double< double_ > as_double;
  typedef ::xsd::cxx::tree::as_decimal< decimal > as_decimal;
  typedef ::xsd::cxx::tree::facet facet;

  // Flags and properties.
  //
  typedef ::xsd::cxx::tree::flags flags;
  typedef ::xsd::cxx::tree::properties< char > properties;

  // Parsing/serialization diagnostics.
  //
  typedef ::xsd::cxx::tree::severity severity;
  typedef ::xsd::cxx::tree::error< char > error;
  typedef ::xsd::cxx::tree::diagnostics< char > diagnostics;

  // Exceptions.
  //
  typedef ::xsd::cxx::tree::exception< char > exception;
  typedef ::xsd::cxx::tree::bounds< char > bounds;
  typedef ::xsd::cxx::tree::duplicate_id< char > duplicate_id;
  typedef ::xsd::cxx::tree::parsing< char > parsing;
  typedef ::xsd::cxx::tree::expected_element< char > expected_element;
  typedef ::xsd::cxx::tree::unexpected_element< char > unexpected_element;
  typedef ::xsd::cxx::tree::expected_attribute< char > expected_attribute;
  typedef ::xsd::cxx::tree::unexpected_enumerator< char > unexpected_enumerator;
  typedef ::xsd::cxx::tree::expected_text_content< char > expected_text_content;
  typedef ::xsd::cxx::tree::no_prefix_mapping< char > no_prefix_mapping;
  typedef ::xsd::cxx::tree::serialization< char > serialization;

  // Error handler callback interface.
  //
  typedef ::xsd::cxx::xml::error_handler< char > error_handler;

  // DOM interaction.
  //
  namespace dom
  {
    // Automatic pointer for DOMDocument.
    //
    using ::xsd::cxx::xml::dom::unique_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
    // DOM user data key for back pointers to tree nodes.
    //
    const XMLCh* const tree_node_key = ::xsd::cxx::tree::user_data_keys::node;
#endif
  }
}

// Forward declarations.
//
class constant_t;
class condition_t;
class predicate_t;
class ret_val_t;
class select_t;
class map_t;
class insert_t;
class delete_t;
class update_t;
class create_t;
class drop_t;
class request_t;
class message_t;
class entry;

#include <memory>    // ::std::unique_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search
#include <utility>   // std::move

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class constant_t: public ::xml_schema::type
{
  public:
  // type
  //
  typedef ::xml_schema::string type_type;
  typedef ::xsd::cxx::tree::traits< type_type, char > type_traits;

  const type_type&
  type () const;

  type_type&
  type ();

  void
  type (const type_type& x);

  void
  type (::std::unique_ptr< type_type > p);

  // value
  //
  typedef ::xml_schema::string value_type;
  typedef ::xsd::cxx::tree::traits< value_type, char > value_traits;

  const value_type&
  value () const;

  value_type&
  value ();

  void
  value (const value_type& x);

  void
  value (::std::unique_ptr< value_type > p);

  // Constructors.
  //
  constant_t (const type_type&,
              const value_type&);

  constant_t (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  constant_t (const constant_t& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual constant_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  constant_t&
  operator= (const constant_t& x);

  virtual 
  ~constant_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< type_type > type_;
  ::xsd::cxx::tree::one< value_type > value_;
};

class condition_t: public ::xml_schema::type
{
  public:
  // operator
  //
  typedef ::xml_schema::string operator_type;
  typedef ::xsd::cxx::tree::traits< operator_type, char > operator_traits;

  const operator_type&
  operator_ () const;

  operator_type&
  operator_ ();

  void
  operator_ (const operator_type& x);

  void
  operator_ (::std::unique_ptr< operator_type > p);

  // field
  //
  typedef ::xml_schema::string field_type;
  typedef ::xsd::cxx::tree::traits< field_type, char > field_traits;

  const field_type&
  field () const;

  field_type&
  field ();

  void
  field (const field_type& x);

  void
  field (::std::unique_ptr< field_type > p);

  // constant
  //
  typedef ::constant_t constant_type;
  typedef ::xsd::cxx::tree::traits< constant_type, char > constant_traits;

  const constant_type&
  constant () const;

  constant_type&
  constant ();

  void
  constant (const constant_type& x);

  void
  constant (::std::unique_ptr< constant_type > p);

  // Constructors.
  //
  condition_t (const operator_type&,
               const field_type&,
               const constant_type&);

  condition_t (const operator_type&,
               const field_type&,
               ::std::unique_ptr< constant_type >);

  condition_t (const ::xercesc::DOMElement& e,
               ::xml_schema::flags f = 0,
               ::xml_schema::container* c = 0);

  condition_t (const condition_t& x,
               ::xml_schema::flags f = 0,
               ::xml_schema::container* c = 0);

  virtual condition_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  condition_t&
  operator= (const condition_t& x);

  virtual 
  ~condition_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< operator_type > operator__;
  ::xsd::cxx::tree::one< field_type > field_;
  ::xsd::cxx::tree::one< constant_type > constant_;
};

class predicate_t: public ::xml_schema::type
{
  public:
  // condition
  //
  typedef ::condition_t condition_type;
  typedef ::xsd::cxx::tree::sequence< condition_type > condition_sequence;
  typedef condition_sequence::iterator condition_iterator;
  typedef condition_sequence::const_iterator condition_const_iterator;
  typedef ::xsd::cxx::tree::traits< condition_type, char > condition_traits;

  const condition_sequence&
  condition () const;

  condition_sequence&
  condition ();

  void
  condition (const condition_sequence& s);

  // Constructors.
  //
  predicate_t ();

  predicate_t (const ::xercesc::DOMElement& e,
               ::xml_schema::flags f = 0,
               ::xml_schema::container* c = 0);

  predicate_t (const predicate_t& x,
               ::xml_schema::flags f = 0,
               ::xml_schema::container* c = 0);

  virtual predicate_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  predicate_t&
  operator= (const predicate_t& x);

  virtual 
  ~predicate_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  condition_sequence condition_;
};

class ret_val_t: public ::xml_schema::type
{
  public:
  // constant
  //
  typedef ::constant_t constant_type;
  typedef ::xsd::cxx::tree::optional< constant_type > constant_optional;
  typedef ::xsd::cxx::tree::traits< constant_type, char > constant_traits;

  const constant_optional&
  constant () const;

  constant_optional&
  constant ();

  void
  constant (const constant_type& x);

  void
  constant (const constant_optional& x);

  void
  constant (::std::unique_ptr< constant_type > p);

  // map
  //
  typedef ::map_t map_type;
  typedef ::xsd::cxx::tree::optional< map_type > map_optional;
  typedef ::xsd::cxx::tree::traits< map_type, char > map_traits;

  const map_optional&
  map () const;

  map_optional&
  map ();

  void
  map (const map_type& x);

  void
  map (const map_optional& x);

  void
  map (::std::unique_ptr< map_type > p);

  // Constructors.
  //
  ret_val_t ();

  ret_val_t (const ::xercesc::DOMElement& e,
             ::xml_schema::flags f = 0,
             ::xml_schema::container* c = 0);

  ret_val_t (const ret_val_t& x,
             ::xml_schema::flags f = 0,
             ::xml_schema::container* c = 0);

  virtual ret_val_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  ret_val_t&
  operator= (const ret_val_t& x);

  virtual 
  ~ret_val_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  constant_optional constant_;
  map_optional map_;
};

class select_t: public ::xml_schema::type
{
  public:
  // table
  //
  typedef ::xml_schema::string table_type;
  typedef ::xsd::cxx::tree::sequence< table_type > table_sequence;
  typedef table_sequence::iterator table_iterator;
  typedef table_sequence::const_iterator table_const_iterator;
  typedef ::xsd::cxx::tree::traits< table_type, char > table_traits;

  const table_sequence&
  table () const;

  table_sequence&
  table ();

  void
  table (const table_sequence& s);

  // predicate
  //
  typedef ::predicate_t predicate_type;
  typedef ::xsd::cxx::tree::optional< predicate_type > predicate_optional;
  typedef ::xsd::cxx::tree::traits< predicate_type, char > predicate_traits;

  const predicate_optional&
  predicate () const;

  predicate_optional&
  predicate ();

  void
  predicate (const predicate_type& x);

  void
  predicate (const predicate_optional& x);

  void
  predicate (::std::unique_ptr< predicate_type > p);

  // Constructors.
  //
  select_t ();

  select_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  select_t (const select_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual select_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  select_t&
  operator= (const select_t& x);

  virtual 
  ~select_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  table_sequence table_;
  predicate_optional predicate_;
};

class map_t: public ::xml_schema::type
{
  public:
  // entry
  //
  typedef ::entry entry_type;
  typedef ::xsd::cxx::tree::sequence< entry_type > entry_sequence;
  typedef entry_sequence::iterator entry_iterator;
  typedef entry_sequence::const_iterator entry_const_iterator;
  typedef ::xsd::cxx::tree::traits< entry_type, char > entry_traits;

  const entry_sequence&
  entry () const;

  entry_sequence&
  entry ();

  void
  entry (const entry_sequence& s);

  // Constructors.
  //
  map_t ();

  map_t (const ::xercesc::DOMElement& e,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  map_t (const map_t& x,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  virtual map_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  map_t&
  operator= (const map_t& x);

  virtual 
  ~map_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  entry_sequence entry_;
};

class insert_t: public ::xml_schema::type
{
  public:
  // into
  //
  typedef ::xml_schema::string into_type;
  typedef ::xsd::cxx::tree::traits< into_type, char > into_traits;

  const into_type&
  into () const;

  into_type&
  into ();

  void
  into (const into_type& x);

  void
  into (::std::unique_ptr< into_type > p);

  // values
  //
  typedef ::map_t values_type;
  typedef ::xsd::cxx::tree::traits< values_type, char > values_traits;

  const values_type&
  values () const;

  values_type&
  values ();

  void
  values (const values_type& x);

  void
  values (::std::unique_ptr< values_type > p);

  // Constructors.
  //
  insert_t (const into_type&,
            const values_type&);

  insert_t (const into_type&,
            ::std::unique_ptr< values_type >);

  insert_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  insert_t (const insert_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual insert_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  insert_t&
  operator= (const insert_t& x);

  virtual 
  ~insert_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< into_type > into_;
  ::xsd::cxx::tree::one< values_type > values_;
};

class delete_t: public ::xml_schema::type
{
  public:
  // from
  //
  typedef ::xml_schema::string from_type;
  typedef ::xsd::cxx::tree::traits< from_type, char > from_traits;

  const from_type&
  from () const;

  from_type&
  from ();

  void
  from (const from_type& x);

  void
  from (::std::unique_ptr< from_type > p);

  // predicate
  //
  typedef ::predicate_t predicate_type;
  typedef ::xsd::cxx::tree::optional< predicate_type > predicate_optional;
  typedef ::xsd::cxx::tree::traits< predicate_type, char > predicate_traits;

  const predicate_optional&
  predicate () const;

  predicate_optional&
  predicate ();

  void
  predicate (const predicate_type& x);

  void
  predicate (const predicate_optional& x);

  void
  predicate (::std::unique_ptr< predicate_type > p);

  // Constructors.
  //
  delete_t (const from_type&);

  delete_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  delete_t (const delete_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual delete_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  delete_t&
  operator= (const delete_t& x);

  virtual 
  ~delete_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< from_type > from_;
  predicate_optional predicate_;
};

class update_t: public ::xml_schema::type
{
  public:
  // table
  //
  typedef ::xml_schema::string table_type;
  typedef ::xsd::cxx::tree::traits< table_type, char > table_traits;

  const table_type&
  table () const;

  table_type&
  table ();

  void
  table (const table_type& x);

  void
  table (::std::unique_ptr< table_type > p);

  // fields
  //
  typedef ::map_t fields_type;
  typedef ::xsd::cxx::tree::traits< fields_type, char > fields_traits;

  const fields_type&
  fields () const;

  fields_type&
  fields ();

  void
  fields (const fields_type& x);

  void
  fields (::std::unique_ptr< fields_type > p);

  // predicate
  //
  typedef ::predicate_t predicate_type;
  typedef ::xsd::cxx::tree::optional< predicate_type > predicate_optional;
  typedef ::xsd::cxx::tree::traits< predicate_type, char > predicate_traits;

  const predicate_optional&
  predicate () const;

  predicate_optional&
  predicate ();

  void
  predicate (const predicate_type& x);

  void
  predicate (const predicate_optional& x);

  void
  predicate (::std::unique_ptr< predicate_type > p);

  // Constructors.
  //
  update_t (const table_type&,
            const fields_type&);

  update_t (const table_type&,
            ::std::unique_ptr< fields_type >);

  update_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  update_t (const update_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual update_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  update_t&
  operator= (const update_t& x);

  virtual 
  ~update_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< table_type > table_;
  ::xsd::cxx::tree::one< fields_type > fields_;
  predicate_optional predicate_;
};

class create_t: public ::xml_schema::type
{
  public:
  // table
  //
  typedef ::xml_schema::string table_type;
  typedef ::xsd::cxx::tree::traits< table_type, char > table_traits;

  const table_type&
  table () const;

  table_type&
  table ();

  void
  table (const table_type& x);

  void
  table (::std::unique_ptr< table_type > p);

  // fields
  //
  typedef ::map_t fields_type;
  typedef ::xsd::cxx::tree::traits< fields_type, char > fields_traits;

  const fields_type&
  fields () const;

  fields_type&
  fields ();

  void
  fields (const fields_type& x);

  void
  fields (::std::unique_ptr< fields_type > p);

  // Constructors.
  //
  create_t (const table_type&,
            const fields_type&);

  create_t (const table_type&,
            ::std::unique_ptr< fields_type >);

  create_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  create_t (const create_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual create_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  create_t&
  operator= (const create_t& x);

  virtual 
  ~create_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< table_type > table_;
  ::xsd::cxx::tree::one< fields_type > fields_;
};

class drop_t: public ::xml_schema::type
{
  public:
  // table
  //
  typedef ::xml_schema::string table_type;
  typedef ::xsd::cxx::tree::traits< table_type, char > table_traits;

  const table_type&
  table () const;

  table_type&
  table ();

  void
  table (const table_type& x);

  void
  table (::std::unique_ptr< table_type > p);

  // Constructors.
  //
  drop_t (const table_type&);

  drop_t (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  drop_t (const drop_t& x,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  virtual drop_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  drop_t&
  operator= (const drop_t& x);

  virtual 
  ~drop_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< table_type > table_;
};

class request_t: public ::xml_schema::type
{
  public:
  // type
  //
  typedef ::xml_schema::string type_type;
  typedef ::xsd::cxx::tree::traits< type_type, char > type_traits;

  const type_type&
  type () const;

  type_type&
  type ();

  void
  type (const type_type& x);

  void
  type (::std::unique_ptr< type_type > p);

  // select
  //
  typedef ::select_t select_type;
  typedef ::xsd::cxx::tree::optional< select_type > select_optional;
  typedef ::xsd::cxx::tree::traits< select_type, char > select_traits;

  const select_optional&
  select () const;

  select_optional&
  select ();

  void
  select (const select_type& x);

  void
  select (const select_optional& x);

  void
  select (::std::unique_ptr< select_type > p);

  // delete
  //
  typedef ::delete_t delete_type;
  typedef ::xsd::cxx::tree::optional< delete_type > delete_optional;
  typedef ::xsd::cxx::tree::traits< delete_type, char > delete_traits;

  const delete_optional&
  delete_ () const;

  delete_optional&
  delete_ ();

  void
  delete_ (const delete_type& x);

  void
  delete_ (const delete_optional& x);

  void
  delete_ (::std::unique_ptr< delete_type > p);

  // insert
  //
  typedef ::insert_t insert_type;
  typedef ::xsd::cxx::tree::optional< insert_type > insert_optional;
  typedef ::xsd::cxx::tree::traits< insert_type, char > insert_traits;

  const insert_optional&
  insert () const;

  insert_optional&
  insert ();

  void
  insert (const insert_type& x);

  void
  insert (const insert_optional& x);

  void
  insert (::std::unique_ptr< insert_type > p);

  // create
  //
  typedef ::create_t create_type;
  typedef ::xsd::cxx::tree::optional< create_type > create_optional;
  typedef ::xsd::cxx::tree::traits< create_type, char > create_traits;

  const create_optional&
  create () const;

  create_optional&
  create ();

  void
  create (const create_type& x);

  void
  create (const create_optional& x);

  void
  create (::std::unique_ptr< create_type > p);

  // update
  //
  typedef ::update_t update_type;
  typedef ::xsd::cxx::tree::optional< update_type > update_optional;
  typedef ::xsd::cxx::tree::traits< update_type, char > update_traits;

  const update_optional&
  update () const;

  update_optional&
  update ();

  void
  update (const update_type& x);

  void
  update (const update_optional& x);

  void
  update (::std::unique_ptr< update_type > p);

  // drop
  //
  typedef ::drop_t drop_type;
  typedef ::xsd::cxx::tree::optional< drop_type > drop_optional;
  typedef ::xsd::cxx::tree::traits< drop_type, char > drop_traits;

  const drop_optional&
  drop () const;

  drop_optional&
  drop ();

  void
  drop (const drop_type& x);

  void
  drop (const drop_optional& x);

  void
  drop (::std::unique_ptr< drop_type > p);

  // Constructors.
  //
  request_t (const type_type&);

  request_t (const ::xercesc::DOMElement& e,
             ::xml_schema::flags f = 0,
             ::xml_schema::container* c = 0);

  request_t (const request_t& x,
             ::xml_schema::flags f = 0,
             ::xml_schema::container* c = 0);

  virtual request_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  request_t&
  operator= (const request_t& x);

  virtual 
  ~request_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< type_type > type_;
  select_optional select_;
  delete_optional delete__;
  insert_optional insert_;
  create_optional create_;
  update_optional update_;
  drop_optional drop_;
};

class message_t: public ::xml_schema::type
{
  public:
  // connect
  //
  typedef ::xml_schema::boolean connect_type;
  typedef ::xsd::cxx::tree::traits< connect_type, char > connect_traits;

  const connect_type&
  connect () const;

  connect_type&
  connect ();

  void
  connect (const connect_type& x);

  // database
  //
  typedef ::xml_schema::string database_type;
  typedef ::xsd::cxx::tree::optional< database_type > database_optional;
  typedef ::xsd::cxx::tree::traits< database_type, char > database_traits;

  const database_optional&
  database () const;

  database_optional&
  database ();

  void
  database (const database_type& x);

  void
  database (const database_optional& x);

  void
  database (::std::unique_ptr< database_type > p);

  // request
  //
  typedef ::request_t request_type;
  typedef ::xsd::cxx::tree::optional< request_type > request_optional;
  typedef ::xsd::cxx::tree::traits< request_type, char > request_traits;

  const request_optional&
  request () const;

  request_optional&
  request ();

  void
  request (const request_type& x);

  void
  request (const request_optional& x);

  void
  request (::std::unique_ptr< request_type > p);

  // Constructors.
  //
  message_t (const connect_type&);

  message_t (const ::xercesc::DOMElement& e,
             ::xml_schema::flags f = 0,
             ::xml_schema::container* c = 0);

  message_t (const message_t& x,
             ::xml_schema::flags f = 0,
             ::xml_schema::container* c = 0);

  virtual message_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  message_t&
  operator= (const message_t& x);

  virtual 
  ~message_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< connect_type > connect_;
  database_optional database_;
  request_optional request_;
};

class entry: public ::xml_schema::type
{
  public:
  // key
  //
  typedef ::xml_schema::string key_type;
  typedef ::xsd::cxx::tree::traits< key_type, char > key_traits;

  const key_type&
  key () const;

  key_type&
  key ();

  void
  key (const key_type& x);

  void
  key (::std::unique_ptr< key_type > p);

  // value
  //
  typedef ::constant_t value_type;
  typedef ::xsd::cxx::tree::traits< value_type, char > value_traits;

  const value_type&
  value () const;

  value_type&
  value ();

  void
  value (const value_type& x);

  void
  value (::std::unique_ptr< value_type > p);

  // Constructors.
  //
  entry (const key_type&,
         const value_type&);

  entry (const key_type&,
         ::std::unique_ptr< value_type >);

  entry (const ::xercesc::DOMElement& e,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  entry (const entry& x,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  virtual entry*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  entry&
  operator= (const entry& x);

  virtual 
  ~entry ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< key_type > key_;
  ::xsd::cxx::tree::one< value_type > value_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

// Parse a URI or a local file.
//

::std::unique_ptr< ::message_t >
message (const ::std::string& uri,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (const ::std::string& uri,
         ::xml_schema::error_handler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (const ::std::string& uri,
         ::xercesc::DOMErrorHandler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse std::istream.
//

::std::unique_ptr< ::message_t >
message (::std::istream& is,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::std::istream& is,
         ::xml_schema::error_handler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::std::istream& is,
         ::xercesc::DOMErrorHandler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::std::istream& is,
         const ::std::string& id,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::std::istream& is,
         const ::std::string& id,
         ::xml_schema::error_handler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::std::istream& is,
         const ::std::string& id,
         ::xercesc::DOMErrorHandler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::InputSource.
//

::std::unique_ptr< ::message_t >
message (::xercesc::InputSource& is,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::xercesc::InputSource& is,
         ::xml_schema::error_handler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::xercesc::InputSource& is,
         ::xercesc::DOMErrorHandler& eh,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::DOMDocument.
//

::std::unique_ptr< ::message_t >
message (const ::xercesc::DOMDocument& d,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::message_t >
message (::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d,
         ::xml_schema::flags f = 0,
         const ::xml_schema::properties& p = ::xml_schema::properties ());

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

void
operator<< (::xercesc::DOMElement&, const constant_t&);

void
operator<< (::xercesc::DOMElement&, const condition_t&);

void
operator<< (::xercesc::DOMElement&, const predicate_t&);

void
operator<< (::xercesc::DOMElement&, const ret_val_t&);

void
operator<< (::xercesc::DOMElement&, const select_t&);

void
operator<< (::xercesc::DOMElement&, const map_t&);

void
operator<< (::xercesc::DOMElement&, const insert_t&);

void
operator<< (::xercesc::DOMElement&, const delete_t&);

void
operator<< (::xercesc::DOMElement&, const update_t&);

void
operator<< (::xercesc::DOMElement&, const create_t&);

void
operator<< (::xercesc::DOMElement&, const drop_t&);

void
operator<< (::xercesc::DOMElement&, const request_t&);

void
operator<< (::xercesc::DOMElement&, const message_t&);

// Serialize to std::ostream.
//

void
message (::std::ostream& os,
         const ::message_t& x, 
         const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
         const ::std::string& e = "UTF-8",
         ::xml_schema::flags f = 0);

void
message (::std::ostream& os,
         const ::message_t& x, 
         ::xml_schema::error_handler& eh,
         const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
         const ::std::string& e = "UTF-8",
         ::xml_schema::flags f = 0);

void
message (::std::ostream& os,
         const ::message_t& x, 
         ::xercesc::DOMErrorHandler& eh,
         const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
         const ::std::string& e = "UTF-8",
         ::xml_schema::flags f = 0);

// Serialize to xercesc::XMLFormatTarget.
//

void
message (::xercesc::XMLFormatTarget& ft,
         const ::message_t& x, 
         const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
         const ::std::string& e = "UTF-8",
         ::xml_schema::flags f = 0);

void
message (::xercesc::XMLFormatTarget& ft,
         const ::message_t& x, 
         ::xml_schema::error_handler& eh,
         const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
         const ::std::string& e = "UTF-8",
         ::xml_schema::flags f = 0);

void
message (::xercesc::XMLFormatTarget& ft,
         const ::message_t& x, 
         ::xercesc::DOMErrorHandler& eh,
         const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
         const ::std::string& e = "UTF-8",
         ::xml_schema::flags f = 0);

// Serialize to an existing xercesc::DOMDocument.
//

void
message (::xercesc::DOMDocument& d,
         const ::message_t& x,
         ::xml_schema::flags f = 0);

// Serialize to a new xercesc::DOMDocument.
//

::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument >
message (const ::message_t& x, 
         const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
         ::xml_schema::flags f = 0);

void
operator<< (::xercesc::DOMElement&, const entry&);

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // REQ_SCHEMA_HXX
