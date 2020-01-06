
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

## 1.2 Maxin Inheritance : 첨가상속

클래스를 정의할 때 자기 자신을 템플릿 인자로 사용하는 법

<pre><code>
template <typename T> struct Mixin : T
{
    ...
}
</pre></code>

## 1.3 속성

get/set method 를 가지는 클래스의 내부 변수를 그 클래스의 속성이라 함
<pre><code>
class Person
{
    int age;
public:
    int get_age() const {return age;}
    void set_age(int value) {age = value;}
}
</pre></code>
