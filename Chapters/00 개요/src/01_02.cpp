#include <String> //string
#include <vector> //vector
using namespace std;

enum class Color {Red, Green, Blue};
enum class Size {Small, Medium, Large};

struct  Product
{
    string name;
    Color color;
    Size size;
};

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
};

template <typename T> struct Specification
{
    virtual bool is_satisfied(T* item) = 0;
};


template  <typename T> struct Filter
{
    virtual vector<T*> filter(
        vector<T*> items,
        Specification<T>& spec) = 0;
    )
};

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

struct SizeSpecification : Specification<Product>
{
    Size size;
    explicit SizeSpecification(const Size size): size(size) {}
    bool is_satisfied(Product* item) override{
        return item->size == size;
    }
};


Product apple{"Apple", Color::Green, Size::Small};
Product tree{"Tree", Color::Green, Size::Large};
Product house{"House", Color::Blue, Size::Large};

vector<Product*> all{&apple, &tree, &house};

BetterFilter bf;
ColorSpecification green(Color::Green);

auto green_things = bf.filter(all, green);
for (auto& x : green_things)
    cout << x->name << " is green" <<endl;

template <typename T> struct AndSpecification : Specification<T>
{
    Specification<T>& first;
    Specification<T>& second;

    AndSpecification(Specification<T>& first, Specification<T>& second): first{first}, second{second}
    {
    }

    bool is_satisfied(T* item) override
    {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};

SizeSpecification large(Size::Large);
ColorSpecification green(Color::Green);

AndSpecification<Product> green_and_large{large,green};
auto big_green_things = bf.filter(all, green_and_large);

for (auto& x : big_green_things)
    cout << x->name << "is large and green" << endl;


template <typename T> struct Specification
{
    virtual bool is_satisfied(T* item) = 0;

    AndSpecification<T> operator &&(Specification&& other)
    {
        return AndSpecification<T>(*this, other);
    }
};
