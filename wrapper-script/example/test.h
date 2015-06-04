#ifndef TEST_H
#define TEST_H
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

class RObject {
public:
    RObject(char* name = 0) {
        if (!name) {
            robjName = (char*)malloc(255);
            sprintf(robjName, "Robert");
        }
    }
    //QML char* robjectName
    char* get_robjectName() const { return robjName; }
    void set_robjectName(const char* foo) {
        fprintf(stderr, "You can't rename a Robject, especially not to %s!\n", foo);
    }
protected:
    virtual void robjectName_changed() {};
private:
    char* robjName;
};

class Testable : public RObject {
public:
    //Reexpose robjectName because QmlTestable won't inherit QmlRObject
    //QML char* robjectName
    Testable() : alpha(5), beta(false), gamma(3.14158265357) {;}
public:
    int get_alpha() { return alpha; }
    void set_alpha(const int arg) { if(alpha==arg) return; alpha = arg; alpha_changed(); }
    bool get_beta() { return beta; }
    void set_beta(const bool arg) { if(beta==arg) return; beta = arg; beta_changed(); }
    double get_gamma() { return gamma; }
    void set_gamma(const double arg) { if(gamma==arg) return; gamma = arg; gamma_changed(); }
protected:
    virtual void alpha_changed() {};
    virtual void beta_changed() {};
    virtual void gamma_changed() {};
private:
    //Location of QML tags doesn't actually matter
    //QML int alpha
    int alpha;
    //QML bool beta
    bool beta;
    //QML double gamma
    double gamma;
};
#endif
