class A{
public:
    A(int);
public:
    int a;
};

A::A(int a):
    a(a){

}

class B : public A{
public:
    B(void);
};

B::B():
    A(1)
{

}
