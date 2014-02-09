Du presque test unitaire en un seul header plein de macros magiques.

#Utilisation
`#include <lightest.h>`

## Ecrire un test
Afin de gérer **automagiquement** les exceptions, le comptage des tests et des assertions, il ne faut pas définir de fonction soi-même, mais utiliser les macros `TEST` et `ENDTEST`.

	TEST(nom_du_test)
		ASSERT(true);
		ASSERT(not false);
		ASSERT_THROWS(const char *, fonctionQuiThrow());
	ENDTEST()

## Exécuter un test
	
	int main(int argc, const char **argv)
	{
		TestFunc tests[] = {
			ADDTEST(nom_du_test)
		};
		return RUN(tests);
	}

## Limitations
Afin de gagner en simplicité d'écriture, les macros sont parfois transformées en plusieurs instructions/blocs. Ceci a plusieurs conséquences:

* Ne pas faire d'assertion avec des effets de bord . Ex: `ASSERT(printf("%d\n", 3) == 1)` est interdit *(mais compilera !)*
* Toujours mettre des accolades autour des assertions dans des blocs if/while/for. Ex: `if (condition){ASSERT(expression 1);} else {ASSERT(expression 2);}`