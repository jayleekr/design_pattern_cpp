# 1. SOLID Design Patterns

## 1.1 Single Responsibility Principle, SRP (단일 책임 원칙)

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
            ofs<< s << endl;
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