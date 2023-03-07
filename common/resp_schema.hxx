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

#ifndef RESP_SCHEMA_HXX
#define RESP_SCHEMA_HXX

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
class column_t;
class header_t;
class body_t;
class response_t;

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

class column_t: public ::xml_schema::type
{
  public:
  // name
  //
  typedef ::xml_schema::string name_type;
  typedef ::xsd::cxx::tree::traits< name_type, char > name_traits;

  const name_type&
  name () const;

  name_type&
  name ();

  void
  name (const name_type& x);

  void
  name (::std::unique_ptr< name_type > p);

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

  // Constructors.
  //
  column_t (const name_type&,
            const type_type&);

  column_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  column_t (const column_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual column_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  column_t&
  operator= (const column_t& x);

  virtual 
  ~column_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< name_type > name_;
  ::xsd::cxx::tree::one< type_type > type_;
};

class header_t: public ::xml_schema::type
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

  // column
  //
  typedef ::column_t column_type;
  typedef ::xsd::cxx::tree::sequence< column_type > column_sequence;
  typedef column_sequence::iterator column_iterator;
  typedef column_sequence::const_iterator column_const_iterator;
  typedef ::xsd::cxx::tree::traits< column_type, char > column_traits;

  const column_sequence&
  column () const;

  column_sequence&
  column ();

  void
  column (const column_sequence& s);

  // Constructors.
  //
  header_t (const table_type&);

  header_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  header_t (const header_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual header_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  header_t&
  operator= (const header_t& x);

  virtual 
  ~header_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< table_type > table_;
  column_sequence column_;
};

class body_t: public ::xml_schema::type
{
  public:
  // column
  //
  typedef ::xml_schema::string column_type;
  typedef ::xsd::cxx::tree::traits< column_type, char > column_traits;

  const column_type&
  column () const;

  column_type&
  column ();

  void
  column (const column_type& x);

  void
  column (::std::unique_ptr< column_type > p);

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
  body_t (const column_type&,
          const value_type&);

  body_t (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  body_t (const body_t& x,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  virtual body_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  body_t&
  operator= (const body_t& x);

  virtual 
  ~body_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< column_type > column_;
  ::xsd::cxx::tree::one< value_type > value_;
};

class response_t: public ::xml_schema::type
{
  public:
  // status
  //
  typedef ::xml_schema::string status_type;
  typedef ::xsd::cxx::tree::traits< status_type, char > status_traits;

  const status_type&
  status () const;

  status_type&
  status ();

  void
  status (const status_type& x);

  void
  status (::std::unique_ptr< status_type > p);

  // message
  //
  typedef ::xml_schema::string message_type;
  typedef ::xsd::cxx::tree::traits< message_type, char > message_traits;

  const message_type&
  message () const;

  message_type&
  message ();

  void
  message (const message_type& x);

  void
  message (::std::unique_ptr< message_type > p);

  // header
  //
  typedef ::header_t header_type;
  typedef ::xsd::cxx::tree::optional< header_type > header_optional;
  typedef ::xsd::cxx::tree::traits< header_type, char > header_traits;

  const header_optional&
  header () const;

  header_optional&
  header ();

  void
  header (const header_type& x);

  void
  header (const header_optional& x);

  void
  header (::std::unique_ptr< header_type > p);

  // body
  //
  typedef ::body_t body_type;
  typedef ::xsd::cxx::tree::optional< body_type > body_optional;
  typedef ::xsd::cxx::tree::traits< body_type, char > body_traits;

  const body_optional&
  body () const;

  body_optional&
  body ();

  void
  body (const body_type& x);

  void
  body (const body_optional& x);

  void
  body (::std::unique_ptr< body_type > p);

  // Constructors.
  //
  response_t (const status_type&,
              const message_type&);

  response_t (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  response_t (const response_t& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual response_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  response_t&
  operator= (const response_t& x);

  virtual 
  ~response_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< status_type > status_;
  ::xsd::cxx::tree::one< message_type > message_;
  header_optional header_;
  body_optional body_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

void
operator<< (::xercesc::DOMElement&, const column_t&);

void
operator<< (::xercesc::DOMElement&, const header_t&);

void
operator<< (::xercesc::DOMElement&, const body_t&);

void
operator<< (::xercesc::DOMElement&, const response_t&);

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // RESP_SCHEMA_HXX