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
}

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
