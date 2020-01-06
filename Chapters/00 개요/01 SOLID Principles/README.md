# 1. SOLID Design Patterns

## 1.1 Single Responsibility Principle(SRP) : 단일 책임 원칙

**Bad Example**

<pre><code>
struct Journal
{
    string title;
    vector<string> entries;

    explicit Journal(const string& title) : title(title){}
    void Journal::add(const string& entry)
    {
        static int count = 1;
        entries.push_back(boost::lexical_cast<string>(count ++) + ": " + entry);
    }

    void Journal::save(const string& filename)
    {
        ofstream ofs(filename);
        for (auto& s : entries)
        {
//            ofs<< s << endl;
        }
    }
}
</pre></code>

이러한 방식은 문제가 있다.
저널의 책임은 항목들은 기입/관리 하는 것이지 디스크에 쓰는 것이 아니다.
만약 디스크에 파일을 쓰는 기능을 데이터의 기입/관리를 담당하는 클래스가 함께 책임지도록 한다면 데이터 저장 방식이 바뀔때마다 클래스들을 일일이 수정해야한다.

> 작은 수정을 여러 클래스에 걸쳐서 해야한다면 아키텍쳐에 문제가 있다는 것이다. **CODE SMELL**

<pre><code>
struct PersistenceManager
{
    static void save(const Journal& j, const string& filename)
    {
        ofstream ofs(filename);
        for (auto& s: j.entries)
            ofs << s << endl;
    }
};
</pre></code>

즉, 각 클래스는 단 한가지의 책임을 부여 받아 수정할 이유가 단 한가지여야 한다.

SRP를 위배하는 안티패턴의 극단적 예로 전지 전능의 객체가 있다.

## 1.2 Open-Closed Principle (OCP) : 열림-닫힘 원칙

다음과 같은 개별 상품에 대한 색상과 크기정보를 가지는 클래스를 가정하자.

<pre><code>
enum class Color {Red, Green, Blue};
enum class Size {Small, Medium, Large};

struct  Product
{
    string name;
    Color color;
    Size size;
};
</pre></code>

이제 주어진 제품 집합을 조건에 따라 필터링하는 기능을 만들고 싶다고 하자

struct  ProductFilter
{
    typedef vector<Product *> Items;
};

이제 색상을 기준으로 삼는 필터를 만들려고 한다 하자

<pre></code>
struct  ProductFilter
{
    typedef vector<Product *> Items;
    ProductFilter::Items ProductFilter::by_color(Items items, Color color)
    {
        Items result;
        for (auto& i : items)
        {
            if (i->color == color)
            {
                result.push_back(i);
            }
        }
        return result;
    };
};
</pre></code>

나쁘지 않은 코드다.

하지만 크기를 기준으로 한 필터링 기능도 구현해달라는 요구사항이 들어온다면 어떨까?
<pre><code>
ProductFilter::Items ProductFilter::by_size(Items items, Size size)
{
    Items result;
    for (auto& i : items)
    {
        if (i->size == size)
        {
            result.push_back(i);
        }
    }
    return result;
}
</pre></code>

다음과 같이 짤 수 있지만, 왠지 반복되는 느낌이 들지 않는가?

그렇다면 범용적으로 임의의 조건을 지정받는 필터함수를 만들어보자
<pre><code>
ProductFilter::Items ProductFilter::by_color_and_size(Items items, Color color, Size size)
{
    Items result;
    for (auto& i : items)
    {
        if (i->size == size && i->color == color)
        {
            result.push_back(i);
        }
    }
    return result;
}
</pre></code>

이런식으로 코드를 짜면 필터링 요구사항이 증가할때마다 기존 코드를 수정해야한다.

이 경우 필터링 절차를 두개의 부분으로 나누어야한다 (SRP 원칙적용)

첫 번째는 "필터", 다른하나는 "명세" 이다

명세는 다음과 같이 쉽게 정의 가능하다.

<pre><code>
template <typename T> struct Specification
{
    virtual bool is_satisfied(T* item) = 0;
};
</pre></code>

Specification<T> 에 기반하여 필터링을 수행할 방법이 필요하다.

<pre><code>
template  <typename T> struct Filter
{
    virtual vector<T*> filter(
        vector<T*> items,
        Specification<T>& spec) = 0;
    )
};
</pre></code>

동일하게 filter라는 라는 함수를 만든다.

개선된 필터의 구현은 다음과 같이 할 수 있다.

<pre><code>
struct BetterFilter : Filter<Product>
{
    vector<Product*> filter(vector<Product*> items, Specification<Product>& spec) override
    {
        vector<Product*> result;
        for (auto& p : items)
        {
            if (spec.is_satisfied(p))
            {
                result.push_back(p);
                return result;
            }
        }
    }
};

struct ColorSpecification : Specification<Product>
{
    Color color;

    explicit ColorSpecification(const Color color): color(color){}
    bool is_satisfied(Product* item) override{
        return item->color == color;
    }
};
</pre></code>


이러한 명세를 활용하면 주어진 제품 목록을 다음과 같이 필터링 할 수 있다.


<pre><code>
Product apple{"Apple", Color::Green, Size::Small};
Product tree{"Tree", Color::Green, Size::Large};
Product house{"House", Color::Blue, Size::Large};

vector<Product*> all{&apple, &tree, &house};

BetterFilter bf;
ColorSpecification green(Color::Green);

auto green_things = bf.filter(all, green);
for (auto& x : green_things)
    ...
</pre></code>

그렇다면 크기와 색상을 동시에 필터링 하는 조건은 어떻게 만들수 있을까?
<pre><code>
template <typename T> struct AndSpecification : Specification<T>
{
    Specification<T>& first;
    Specification<T>& second;

    AndSpecification(Specification<T>& first, Specification<T>& second): first(first), second(second)
    {
    }

    bool is_satisfied(T* item) override
    {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};
</pre></code>

위와 같이 Specification을 상속 받아 연결 한다면 쉽게 해결 가능하다.

사용예제는 다음과 같다.


<pre><code>
SizeSpecification large(Size::Large);
ColorSpecification green(Color::Green);

AndSpecification<Product> green_and_large{large,green};
auto big_green_things = bf.filter(all, green_and_large);

for (auto& x : big_green_things)
    cout << x->name << "is large and green" << endl;

</pre></code>

위와같은 코드는 단지 두개의 필터를 이용할 뿐인데 적지않은 코드를 작성해야하는 단점이있다.

하지만 C++의 강력한 overloading을 이용하면 훨씬 단순해진다.
<pre><code>
template <typename T> struct Specification
{
    virtual bool is_satisfied(T* item) = 0;

    AndSpecification<T> operator &&(Specification&& other)
    {
        return AndSpecification<T>(*this, other);
    }
};

auto green_and_big = ColorSpecification(Color::Green) && SizeSpecification(Size::Large)
</pre></code>

OCP에서는 기본적으로 기존에 작성하고 테스트했던 코드에 손을 대서는 안된다는 룰이 있다.

위에 예제는 그러한 원칙을 구현하고 있다.

## 1.3 Liskov Substitution Principle (LSP) : 리스코프 치환 원칙

이 원칙은 어떤 자식 객체에 접근할 때 그 부모 객체의 인터페이스로 접근하더라도 아무런 문제가 없어야 한다는 것을 의미한다.

즉 자식 객체를 그 부모 객체와 동등하게 취급할 수 있어야 한다.

먼저 LSP가 준수되지 않는 경우가 어떤 경우인지 알아보자.

<pre><code>
class Rectangle
{
protected:
    int width, height;

public:
    Rectangle(const int width, const int height) : width{width}, height{height} {
    }

    int get_width() const {return width;}
    virtual void set_width(const int width) { this->width = width;}
    
    int get_height() const {return height;}
    virtual void set_height(const int height) { this->height = height;}

    int area() const {return width * height;}
};
</pre></code>

이제 직사각형의 특별한 경우인 정사각형을 만들어보자


<pre><code>
class Square : public Rectangle
{
public:
    Square(int size): Rectangle(size, size){}
    void set_width(const int width) override{
        this->width = this->height = width;
    }
     
    void set_height(const int height) override{
        this->height = this->width = height;
    }
};
</pre></code>

언뜻 보기에는 문제 없어보이는 코드이지만, 이 객체를 부모인 Rectangle 객체로서 접근하면 의도치 않은 상황이 생긴다.

<pre><code>
void process(Rectangle& r)
{
    int w = r.get_width();
    r.set_height(10);
};
</pre></code>

이러한 경우를 막을 수 있는 방법은 무엇일까?

여러가지 방법이 있지만 서브클래스 자체를 만들지 않는것이 좋다고 생각한다.

<pre><code>
struct RectangleFactory
{
    static Rectangle create_rectangle(int w, int h);
    static Rectangle create_square(int size);
}

bool Rectangle::is_square() const
{
    return width == height;
}
</pre></code>

## Interface Segregation Principle(ISP) : 인터페이스 분리 원칙

복합 기능 프린터기를 만들기로 했다고 하자. 이프린터는 다음과 같은 기능을 한다.

<pre><code>
struct MyFavoritePrinter : IMachine
{
    void print(vector<Document*> docs) override;
    void fax(vector<Document*> docs) override;
    void scan(vector<Document*> docs) override;
};

struct  IMachine
{
    virtual void print(vector<Document*> docs);
    virtual void fax(vector<Document*> docs);
    virtual void scan(vector<Document*> docs);
};
</pre></code>

이제 프린터의 구현을 다른 곳들에 맡기려한다.
하청 업체는 여러곳이 될수 있고, 각기 제품 라인업에 따른 기능조합을 달리 할 수 있다고 하자.

위와 같이 인터페이스를 만들게 되면 어떤 문제가 발생 할까?

어떤 업체는 프린트하는 기능만 넣고 싶을 수 있다.
하지만 이 인터페이스는 모든기능을 구현하도록 강제한다.

여기서 ISP 원칙적용이 필요하다.
인터페이스 분리 원칙이 의미하는 바는 필요에 따라 구현할 대상을 선별할 수 있도록 인터페이스를 별개로 두어야 한다는 것이다.
<pre><code>
struct IPrinter
{
    virtual void print(vector<Document*> docs) = 0;
};
struct IScanner
{
    virtual void scan(vector<Document*> docs) = 0;
};

struct IMachine: IPrinter, IScanner
{
};
</pre></code>

위와 같이 코드를 짜면 복합기의 코드까지 구성이 가능하다.

<pre><code>
struct  Machine : IMachine
{
    IPrinter& printer;
    IScanner& scanner;

    Machine(IPrinter& printer, IScanner& scanner): printer{printer}, scanner{scanner}
    {
    }

    void print(vector<Document*> docs) override{
        printer.print(docs);
    }
    
    void scan(vector<Document*> docs) override{
        scanner.scan(docs);
    }
};
</pre></code>

**다시한번 아이디어를 정리하자면, 한 덩어리의 복잡한 인터페이스를 목적에 따라 구분하여 나눔으로써, 인터페이스 모든 항목에 대한 구현을 강제하지 않고 실제 필요한 인터페이스만 구현할 수 있도록 하는 것이다.**

만약 어떤 애플리케이션의 플러그인 모듈을 개발할 때 뭐가 뭔지 알 수 없는 혼란스러운 함수들을 빈껍데기 또는 null리턴으로 구현하고 있다면, ISP원칙을 위반 한 것이라고 볼 수 있다.

## 1.5 Dependency Inversion Principle(DIP) : 의존성 역전 원칙

상위 모듈이 하위모듈에 종속성을 가져서는 안된다.
양쪽 모두 추상화에 의존해야한다.

추상화가 세부사항에 의존해서는 안된다. 세부 사항이 추상화에 의존해야한다.

아래는 방법에 대한 코드이다. (자세한 내용은 잘 이해가 안간다 200106)


<pre><code>
struct ILogger
{
    virtual ~ILogger() {}
    virtual void Log(const string& s) = 0;
}
class Reporting
{
    ILogger& logger;
public:
    Reporting(const ILogger& logger) : logger{logger} {
    }

    void prepare_report()
    {
        logger.log_info("Preparing the report");
    }
};

struct  ConsoleLogger : ILogger
{
    ConsoleLogger() {}
    void Log(const string& s) override
    {
//      cout << "LOG : " << s.c_str() << endl;
    }
};


struct  Engine
{
    float volume = 5;
    int horse_power = 400;

    friend ostream& operator << (ostream& os, const Engine& obj){
        return os << "volume: " << obj.volume 
        << " horse_poser: "<< obj.horse_power;
    }
};


struct  Car
{
    unique_ptr<Engine> engine;
    shared_ptr<ILogger> logger;

    Car(unique_ptr<Engine> engine, const shared_ptr<ILogger>& logger):
        engine{move(engine)}, logger{logger}
    {
        logger->Log("making a car");
    }

    friend ostream& operator<< (ostream&os, const Car& obj)
    {
        return os<< "car with engine" << *obj.engine;
    }
};

auto injector = di::make_injector(
    di::bind<ILogger>().to<ConsoleLogger>()
);

auto car = injector.create<shared_ptr<Car>>()
</pre></code>