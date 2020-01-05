struct  Foo : SomeBase<Foo>
{
    
};


template <typename Derived>
struct  SomeBase
{
    void foo()
    {
        for (auto& item : *static_cast<Derived*>(this))
        {

        }
    }
};
