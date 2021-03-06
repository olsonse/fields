#define BOOST_TEST_MODULE  fields_AddField

#include <fields/Fields.h>

#include <xylose/Vector.h>

#include <boost/test/unit_test.hpp>

namespace {

  /* To be able to add two fields of the same typedef, we have to wrap them
   * inside a class that changes their type.  One simple way of doing this is to
   * include another template parameter that has no effect on the code, but just
   * changes the type of the base class.
   * */
  template<class T, unsigned int id = 0>
  class BgI : public virtual fields::BaseField,
              public fields::BgField<T> {};

  using xylose::Vector;
  using xylose::V3;
  using fields::AddField;

  typedef  BgI<double,0U> s0;
  typedef  BgI<double,1U> s1;
  typedef  BgI< Vector<double,3>, 0U> v0;
  typedef  BgI< Vector<double,3>, 1U> v1;

  typedef AddField< s0, s1 > s01;
  typedef AddField< v0, v1 > v01;
  typedef AddField< v0, s0 > v0s0;


  /* NOTE THAT operator() functions WILL NOT INSTANTIATE for this typedef. */
  typedef AddField< s0, v0 > s0v0;

}/* namespace (anon) */

BOOST_AUTO_TEST_CASE( two_fields ) {
  s01 s;
  s.s0::bg = -10.0;
  s.s1::bg = -100.0;
  {
    double sr = 0;
    s(sr,V3(0.,0.,0.));
    BOOST_MESSAGE( "testing addition of two scalar fields..." );
    BOOST_CHECK_EQUAL( sr, -110.0 );
    BOOST_CHECK_EQUAL( s(V3(0.,0.,0.)), -110.0 );
  }

  {
    v01 v;
    v.v0::bg = -5.0;
    v.v1::bg = V3(-50.0,0.,10.);
    Vector<double,3> vr(0.0);
    v(vr,V3(0.,0.,0.));
    BOOST_MESSAGE( "testing addition to two vector fields..." );
    BOOST_CHECK_EQUAL( vr, V3(-55.,-5.,5.) );
    BOOST_CHECK_EQUAL( v(V3(0.,0.,0.)), V3(-55.,-5.,5.) );
  }

  {
    v0s0 vs;
    vs.v0::bg = 10.0;
    vs.s0::bg = 100.0;
    Vector<double,3> vr(0.0);
    vs(vr,V3(0.,0.,0.));
    BOOST_MESSAGE( "testing addition of vector and scalar fields..." );
    BOOST_CHECK_EQUAL( vr, V3(110.,110.,110.) );
    BOOST_CHECK_EQUAL( vs(V3(0.,0.,0.)), V3(110.,110.,110.) );
  }


  {
    s0v0 sv;
    sv.v0::bg = 10.0;
    sv.s0::bg = 100.0;
    Vector<double,3> vr(0.0);
    #if 0
      // NOTE:  these operator() functions cannot instantiate because the
      // AddField was done with F0:=scalar and F1:=vector.
      sv(vr,V3(0,0,0));
      cout << "vector+scaler result of 'double operator(...)'      = " << sv(V3(0,0,0)) << endl;
      cout << "vector+scaler result of 'void operator(result,...)' = " << vr << endl;
    #endif
  }
}

