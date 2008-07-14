#include <UnitTest++.h>
#include "CheckOffsetFunc.h"

using namespace amop;

//------------------------------------------------------------------
class Interface
{
public:
	~Interface(){};

	virtual int foo1() = 0;
	virtual void foo2() = 0;
	virtual void foo3() = 0;
	virtual void foo4() = 0;
	virtual void foo5() = 0;
	virtual int foo6() = 0;
	virtual void foo7() = 0;
};

//------------------------------------------------------------------
TEST(TestGetOffset)
{
  CHECK_EQUAL(1u, detail::inner::offset::getOffset(&Interface::foo2));
  CHECK_EQUAL(0u, detail::inner::offset::getOffset(&Interface::foo1));
  
  CHECK_EQUAL(4u, detail::inner::offset::getOffset(&Interface::foo5));
  CHECK_EQUAL(5u, detail::inner::offset::getOffset(&Interface::foo6));
  CHECK_EQUAL(6u, detail::inner::offset::getOffset(&Interface::foo7));	
}

