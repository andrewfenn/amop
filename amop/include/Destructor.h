#ifndef __AMOP_DESTRUCTOR_HH
#define __AMOP_DESTRUCTOR_HH

namespace amop
{
    //! Trait class for binding to destructor
    /*!
        This trait class is used for binding destructor to your mock object:

        \code
            mock.call(Destructor());
        \endcode

        \remarks
            In fact, it is not binding to your constructor directly, 
            but it is binding to the delete operator of your interface. 
            However, because it is a pure virutal class, there are only 2 way to call the destructor, 
            one is delete it by using the delete operator, 
            another one is call it directly ( mock->~YourInterface() ). 
            Normally latter method is rarely happened. 
    */
    class Destructor
    {
    };
}

#endif
