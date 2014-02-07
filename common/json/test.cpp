#include "json.h"
#include "lighttest.h"

void iThrow(void)
{
	throw "Hello, I'm an exception :)S";
}

TEST(lighttest)
	ASSERT(true);
	ASSERT(not false);
	ASSERT_THROWS(const char *, iThrow());
ENDTEST()

TEST(json_int)
	JSON::Integer i(3);
	ASSERT(i.value() == 3);
	ASSERT(i.dumps() == "3");
	ASSERT(i.isAtom());
	ASSERT(i.isNumber());
	ASSERT(i.type() == JSON::Integer_t);
ENDTEST()

TEST(json_float)
	JSON::Float f(3.14);
	ASSERT(f.value() == 3.14);
	ASSERT(f.dumps() == "3.140000");
	ASSERT(f.isAtom());
	ASSERT(f.isNumber());
	ASSERT(f.type() == JSON::Float_t);
ENDTEST()

TEST(json_str)
	JSON::String s("Je suis un canape qui chante");
	ASSERT(s.value() == "Je suis un canape qui chante");
	ASSERT(s.dumps() == "\"Je suis un canape qui chante\"");
	ASSERT(s.isAtom());
	ASSERT(! s.isNumber());
	ASSERT(s.type() == JSON::String_t);
ENDTEST()

TEST(json_list)
	JSON::List l;
	ASSERT(l.len() == 0);
	ASSERT(l.isSequence());
	ASSERT(l.type() == JSON::List_t);
ENDTEST()

TEST(json_list_append)
	JSON::List l;
	for (int i=0; i<10; i++)
		l.append(JSON::Integer(i));
	ASSERT(l.len() == 10);
	for (int i=0; i<10; i++){
		ASSERT(INT(l[i]).value() == i);
	}
ENDTEST()

TEST(json_list_mixed)
	JSON::List l;
	for (int i=0; i<10; i++){
		if (i%2)
			l.append(JSON::Integer(3));
		else
			l.append(JSON::Float(12.45));
	}
	ASSERT(l.len() == 10);
	for (int i=0; i<10; i++){
		ASSERT(l[i]->isNumber());
		switch (l[i]->type()){
			case JSON::Integer_t:
				ASSERT(i%2);
				ASSERT(INT(l[i]).value() == 3);
				break;
			case JSON::Float_t:
				ASSERT(i%2 == 0);
				ASSERT(FLOAT(l[i]).value() == 12.45);
				break;
			default: break;
		}
	}
ENDTEST()

TEST(json_list_repr)
	JSON::List l;
	ASSERT(l.dumps() == "[]");
	l.append(JSON::Integer(2));
	l.append(JSON::Float(42.125));
	ASSERT(l.dumps() == "[2, 42.125000]");

	JSON::List *lptr = (JSON::List *) l.clone();
	ASSERT(lptr->dumps() == "[2, 42.125000]");
	delete lptr;
ENDTEST()

TEST(json_dict)
	JSON::Dict d;
	ASSERT(d.isSequence());
	ASSERT(d.type() == JSON::Dict_t);
ENDTEST()

TEST(json_dict_keys)
	JSON::Dict d;
	d.set("key", JSON::String("value"));
	ASSERT(d.dumps() == "{\"key\": \"value\"}");
	ASSERT(d.hasKey("key"));
	ASSERT(! d.hasKey("1337"));
	ASSERT(d.get("key")->type() == JSON::String_t);
	ASSERT_THROWS(JSON::KeyError, d.get("1337"));
ENDTEST()

TEST(json_parse_int)
	const char *repr = "42";
	JSON::Value *i = JSON::parse(repr);
	ASSERT(i != NULL);
	ASSERT(i->type() == JSON::Integer_t);
	ASSERT(INT(i).value() == 42);
	ASSERT(i->dumps() == repr);
	delete i;
ENDTEST()

TEST(json_parse_float)
	const char *repr = "42.456700";
	JSON::Value *f = JSON::parse(repr);
	ASSERT(f != NULL);
	ASSERT(f->type() == JSON::Float_t);
	ASSERT(FLOAT(f).value() == 42.4567);
	ASSERT(f->dumps() == repr);
	delete f;
ENDTEST()

TEST(json_parse_str)
	const char *repr = "\"Naim Qachri\"";
	JSON::Value *s = JSON::parse(repr);
	ASSERT(s != NULL);
	ASSERT(s->type() == JSON::String_t);
	ASSERT(STR(s).value() == "Naim Qachri");
	ASSERT(s->dumps() == repr);
	delete s;
ENDTEST()

TEST(json_parse_list)
	const char *repr = "[1, 3.140000, \"Archimede\"]";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(res != NULL);
	ASSERT(res->type() == JSON::List_t);

	JSON::List & l = LIST(res);
	ASSERT(l.len() == 3);
	ASSERT(l[0]->type() == JSON::Integer_t);
	ASSERT(l[1]->type() == JSON::Float_t);
	ASSERT(l[2]->type() == JSON::String_t);
	ASSERT(l.dumps() == repr);
	delete res;
ENDTEST()

TEST(json_parse_dict)
	const char *repr = "{\"group\": 3}";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(res != NULL);
	ASSERT(res->type() == JSON::Dict_t);

	JSON::Dict & d = DICT(res);
	ASSERT(d.hasKey("group"));
	ASSERT(d.get("group")->type() == JSON::Integer_t);
	ASSERT(d.dumps() == repr);
	delete res;
ENDTEST()

TEST(json_parse_dict_many_keys)
	const char *repr = "{\"name\": \"Titou\", \"age\": 23}";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(res != NULL);
	ASSERT(res->type() == JSON::Dict_t);

	JSON::Dict & d = DICT(res);
	ASSERT(d.hasKey("name"));
	ASSERT(d.get("name")->type() == JSON::String_t);
	ASSERT(d.hasKey("age"));
	ASSERT(d.get("age")->type() == JSON::Integer_t);
	delete res;
ENDTEST()

int main(int argc, const char **argv)
{
	TestFunc testSuite[] = {
		ADDTEST(lighttest),
		ADDTEST(json_int),
		ADDTEST(json_float),
		ADDTEST(json_str),
		ADDTEST(json_list),
		ADDTEST(json_list_append),
		ADDTEST(json_list_mixed),
		ADDTEST(json_list_repr),
		ADDTEST(json_dict),
		ADDTEST(json_dict_keys),
		ADDTEST(json_parse_int),
		ADDTEST(json_parse_float),
		ADDTEST(json_parse_str),
		ADDTEST(json_parse_list),
		ADDTEST(json_parse_dict),
		ADDTEST(json_parse_dict_many_keys)
	};

	return RUN(testSuite);
}
