#include <ostream> // ostream
#include <memory> //unique_ptr, shared_ptr
using namespace::std;

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
        cout << "LOG : " << s.c_str() << endl;
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