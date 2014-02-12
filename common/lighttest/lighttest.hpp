#ifndef DEFINE_LIGHTEST_HEADER
#define DEFINE_LIGHTEST_HEADER

/*
 * INFO-F202 - 2013-2014 - Projet C++
 * Titouan CHRISTOPHE <tichrist@ulb.ac.be>
 * #000382957 BA2 Sciences Informatiques
 */

#include <iostream>
#include <typeinfo>
using namespace std;

#define ADDTEST(t) {t, #t}
struct TestFunc {
    bool(*func)(void);
    const char *name;
};

static size_t __assert_count = 0;

#define RUN(t) runTestSuite(t, sizeof(t)/sizeof(TestFunc))
static inline int runTestSuite(TestFunc suite[], size_t n_tests)
{
    size_t ok=0, fails=0;
    cout << "\033[33m=============== Starting test suite ===============\033[0m"
         << endl;
    for (size_t i=0; i<n_tests; i++){
        cout << "\033[1m===== Testing " << suite[i].name << " =====\033[0m\n";
        bool pass = false;
        try {pass = suite[i].func();}
        catch (const char * err){
            cout << "Exception occured: " << err << endl;
        }
        if (pass){
            ok++;
            cout << "=====      OK      =====\n";
        }
        else {
            fails++;
            cout << "\033[1m\033[31m=====     FAIL     =====\033[0m\n";
        }
    }
    int color = (fails == 0) ? 2 : 1;
    cout << "\033[3" << color
         << "m===================================================\033[0m" 
         << endl;
    cout << "Total: " << (ok+fails) << " tests; " << __assert_count
         << " assertions; \033[32m" << ok << " OK\033[0m";
    if (fails) cout << ", \033[31m" << fails << " FAIL\033[0m";
    cout << endl;
    return (int) fails;
}

#define TEST(name) bool name (void) { try {
#define ENDTEST() } catch (std::exception & e){\
                    cout << "\033[31m !!! A wild exception appears: "\
                         << typeid(e).name() << ": " << e.what()\
                         << "\033[0m" << endl;\
                    return false;\
                  } return true; }

#define ASSERT(expr) __assert_count++;\
                     if (not (expr)){\
                        cout << "Assertion failed (" << __FILE__ << ":"\
                             << __LINE__ << ") " << #expr << endl;\
                        return false;}

#define HAS_THROWED false
#define ASSERT_THROWS(exc, expr) try{expr; ASSERT(HAS_THROWED);}\
                                 catch(exc e){ASSERT(true)}

#endif
