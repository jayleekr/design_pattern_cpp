
# 1. 중요한 개념

## 1.1 Curiously Recurring Template Pattern(CRTP) 

자기 자신을 베이스 클래스의 템플릿 인자로 상속 받는 패턴

<pre>
<code>
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
</code>
</pre>

