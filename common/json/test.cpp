#include "json.hpp"
#include "Document.hpp"
#include <lighttest.hpp>
#include <cstring>
extern "C" {
	#include <unistd.h>
}

void iThrow(void)
{
	/*Geek joke, please laugh if it doesn't come naturally*/
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
	ASSERT(ISINT(&i));
ENDTEST()

TEST(json_float)
	JSON::Float f(3.14);
	ASSERT(f.value() == 3.14);
	ASSERT(f.dumps() == "3.14");
	ASSERT(f.isAtom());
	ASSERT(f.isNumber());
	ASSERT(f.type() == JSON::Float_t);
	ASSERT(ISFLOAT(&f));
ENDTEST()

TEST(json_bool_true)
	JSON::Bool b(true);
	ASSERT(b.value() == true);
	ASSERT(b.dumps() == "true");
	ASSERT(b.isAtom());
	ASSERT(! b.isNumber());
	ASSERT(ISBOOL(&b));
ENDTEST()

TEST(json_bool_false)
	JSON::Bool b(false);
	ASSERT(b.value() == false);
	ASSERT(b.dumps() == "false");
	ASSERT(b.isAtom());
	ASSERT(! b.isNumber());
	ASSERT(ISBOOL(&b));
ENDTEST()

TEST(json_str)
	JSON::String s("Je suis un canape qui chante");
	ASSERT(s.value() == "Je suis un canape qui chante");
	ASSERT(s.dumps() == "\"Je suis un canape qui chante\"");
	ASSERT(s.isAtom());
	ASSERT(! s.isNumber());
	ASSERT(s.type() == JSON::String_t);
	ASSERT(ISSTR(&s));
ENDTEST()

TEST(json_str_escape)
	JSON::String s("A\n\tb\nc");
	ASSERT(s.value() == "A\n\tb\nc");
	ASSERT(s.dumps() == "\"A\\n\\tb\\nc\"");
ENDTEST()

TEST(json_list)
	JSON::List l;
	ASSERT(l.len() == 0);
	ASSERT(l.isSequence());
	ASSERT(l.type() == JSON::List_t);
	ASSERT(ISLIST(&l));
ENDTEST()

TEST(json_list_append)
	JSON::List l;
	for (int i=0; i<10; i++)
		l.append(i);
	ASSERT(l.len() == 10);
	for (int i=0; i<10; i++){
		ASSERT(INT(l[i]).value() == i);
	}
ENDTEST()

TEST(json_list_key_error)
	JSON::List l;
	ASSERT_THROWS(JSON::KeyError, l[0]);
	ASSERT_THROWS(JSON::KeyError, l[10]);

	l.append(1);
	ASSERT(ISINT(l[0]));
	ASSERT_THROWS(JSON::KeyError, l[10]);
ENDTEST()

TEST(json_list_mixed)
	JSON::List l;
	for (int i=0; i<10; i++){
		if (i%2)
			l.append(3	);
		else
			l.append(12.45);
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
	l.append(2);
	l.append(42.125);
	ASSERT(l.dumps() == "[2, 42.125]");

	JSON::List *lptr = (JSON::List *) l.clone();
	ASSERT(lptr != NULL);
	ASSERT(lptr->dumps() == "[2, 42.125]");
	delete lptr;
ENDTEST()

TEST(json_dict)
	JSON::Dict d;
	ASSERT(d.isSequence());
	ASSERT(d.type() == JSON::Dict_t);
	ASSERT(ISDICT(&d));
ENDTEST()

TEST(json_dict_keys)
	JSON::Dict d;
	d.set("key", "value");
	ASSERT(d.dumps() == "{\"key\": \"value\"}");
	ASSERT(d.hasKey("key"));
	ASSERT(! d.hasKey("1337"));
	ASSERT(ISSTR(d.get("key")));
ENDTEST()

TEST(json_dict_repr)
	JSON::Dict d;
	d.set("key1", "value1");
	d.set("key2", "value2");

	const char *repr1 = "{\"key1\": \"value1\", \"key2\": \"value2\"}";
	const char *repr2 = "{\"key2\": \"value2\", \"key1\": \"value1\"}";
	ASSERT(d.dumps() == repr1 || d.dumps() == repr2)

	JSON::Value *copy = d.clone();
	ASSERT(copy != NULL);
	ASSERT(d.dumps() == repr1 || d.dumps() == repr2);

	delete copy;
ENDTEST()

TEST(json_parse_error)
	ASSERT_THROWS(JSON::ParseError, JSON::parse("lalala")); // Unexpected character
	ASSERT_THROWS(JSON::ParseError, JSON::parse("\"string")); // Unfinished string
	ASSERT_THROWS(JSON::ParseError, JSON::parse("[1, 2, 3")); //Unfinished list
	ASSERT_THROWS(JSON::ParseError, JSON::parse("{\"key\": \"val\"")); //Unfinished dict
	ASSERT_THROWS(JSON::ParseError, JSON::parse("[1 2]")); // Missing ','
	ASSERT_THROWS(JSON::ParseError, JSON::parse("{\"haha\"}")); //Missing ':'
	ASSERT_THROWS(JSON::ParseError, JSON::parse("{\"haha\":}")); //Missing val
	ASSERT_THROWS(JSON::ParseError, JSON::parse("{42}")); //Key is not str
	ASSERT_THROWS(JSON::ParseError, JSON::parse("{\"key1\": val1}")); //val not parsable
	ASSERT_THROWS(JSON::ParseError, JSON::parse("{\"key1\": 1 \"key2\": 2}")); //Missing ', '
ENDTEST()

TEST(json_parse_nothing)
	ASSERT(JSON::parse("") == NULL);
	ASSERT(JSON::parse("\n") == NULL);
	ASSERT(JSON::parse("\r") == NULL);
	ASSERT(JSON::parse("\t") == NULL);
	ASSERT(JSON::parse(" ") == NULL);
	ASSERT(JSON::parse("\n\r\t ") == NULL);
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
	const char *repr = "42.4567";
	JSON::Value *f = JSON::parse(repr);
	ASSERT(f != NULL);
	ASSERT(f->type() == JSON::Float_t);
	ASSERT(FLOAT(f).value() == 42.4567);
	ASSERT(f->dumps() == repr);
	delete f;
ENDTEST()

TEST(json_parse_bool_true)
	const char *repr = "true";
	JSON::Value *b = JSON::parse(repr);
	ASSERT(b != NULL);
	ASSERT(b->type() == JSON::Boolean_t);
	ASSERT(BOOL(b).value() == true);
	ASSERT(b->dumps() == repr);
	delete b;
ENDTEST()

TEST(json_parse_bool_false)
	const char *repr = "false";
	JSON::Value *b = JSON::parse(repr);
	ASSERT(b != NULL);
	ASSERT(b->type() == JSON::Boolean_t);
	ASSERT(BOOL(b).value() == false);
	ASSERT(b->dumps() == repr);
	delete b;
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

TEST(json_parse_str_escape)
	const char *repr = "\"Ligne 1\\\"\\n\\tTabulation ligne 2\\nLigne 3\"";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(res != NULL);
	ASSERT(ISSTR(res));
	ASSERT(STR(res).value() == "Ligne 1\"\n\tTabulation ligne 2\nLigne 3");
	ASSERT(res->dumps() == repr);
	delete res;
ENDTEST()

TEST(json_parse_str_escape_hex)
	const char *repr = "\"Une chaine\\x20de caracteres\\xfa\"";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(res != NULL);
	ASSERT(ISSTR(res));
	ASSERT(STR(res).value() == "Une chaine de caracteres\xfa");
	delete res;
ENDTEST()

TEST(json_parse_list)
	const char *repr = "[1, 3.14, \"Archimede\"]";
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

TEST(json_parse_empty_list)
	JSON::Value *res = JSON::parse("[]");
	ASSERT(res != NULL);
	ASSERT(ISLIST(res));
	ASSERT(LIST(res).len() == 0);
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

TEST(json_parse_empty_dict)
	JSON::Value *res = JSON::parse("{}");
	ASSERT(res != NULL);
	ASSERT(ISDICT(res));
	delete res;
ENDTEST()

TEST(json_parse_empty_list_in_dict)
	JSON::Value *res = JSON::parse("{\"key\": []}");
	ASSERT(res != NULL);
	ASSERT(ISDICT(res));
	ASSERT(ISLIST(DICT(res).get("key")));
	ASSERT(LIST(DICT(res).get("key")).len() == 0);
	delete res;
ENDTEST()

TEST(json_parse_empty_dict_in_list)
	JSON::Value *res = JSON::parse("[{}]");
	ASSERT(res != NULL);
	ASSERT(ISLIST(res));
	ASSERT(ISDICT(LIST(res)[0]));
	ASSERT(DICT(LIST(res)[0]).len() == 0);
	delete res;
ENDTEST()

TEST(json_parse_dict_many_keys)
	const char *repr = "{\"name\": \"Titou\", \"age\": 23, \"male\": true}";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(res != NULL);
	ASSERT(res->type() == JSON::Dict_t);

	JSON::Dict & d = DICT(res);
	ASSERT(d.hasKey("name"));
	ASSERT(d.get("name")->type() == JSON::String_t);
	ASSERT(d.hasKey("age"));
	ASSERT(d.get("age")->type() == JSON::Integer_t);
	ASSERT(d.hasKey("male"));
	ASSERT(ISBOOL(d.get("male")));
	delete res;
ENDTEST()

TEST(json_parse_list_in_dict)
	const char *repr = "{\"players\": [{\"name\":\"Titou\", \"level\": 52},"
		               " {\"name\":\"Homere\", \"level\": 66}]}";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(res != NULL);
	ASSERT(res->type() == JSON::Dict_t);

	JSON::Dict & d = DICT(res);
	ASSERT(d.hasKey("players"));
	ASSERT(ISLIST(d.get("players")));
	ASSERT(LIST(d.get("players")).len() == 2);
	ASSERT(ISDICT(LIST(d.get("players"))[0]));

	delete res;
ENDTEST()

TEST(json_load)
	JSON::Value *val = JSON::load("fixtures/a.json");
	ASSERT(val != NULL && ISDICT(val));
	JSON::Dict const & dict = DICT(val);
	for (JSON::Dict::const_iterator it=dict.begin(); it!=dict.end(); it++){
		ASSERT(ISLIST(it->second));
		ASSERT(LIST(it->second).len() == 3);
	}
	delete val;
ENDTEST()

TEST(json_save)
	JSON::Value *val = JSON::load("fixtures/a.json");
	ASSERT(val != NULL);
	val->save("__test__.json");
	
	JSON::Value *copy = JSON::load("__test__.json");
	unlink("__test__.json");
	ASSERT(copy != NULL && ISDICT(copy));

	JSON::Dict const & dict = DICT(copy);
	for (JSON::Dict::const_iterator it=dict.begin(); it!=dict.end(); it++){
		ASSERT(ISLIST(it->second));
		ASSERT(LIST(it->second).len() == 3);
	}

	delete val;
	delete copy;
ENDTEST()

TEST(json_dict_steal)
	JSON::Dict d;
	d.set("a", "b");
	d.set("c", "d");
	ASSERT(d.hasKey("a"))
	ASSERT(d.hasKey("c"));

	JSON::Dict e;
	e.setPtr("a", d.steal("a"));
	ASSERT(e.hasKey("a"))
	ASSERT(! d.hasKey("a"));
	ASSERT(d.hasKey("c"));
ENDTEST()

TEST(json_list_steal)
	JSON::List l;
	l.append("a");
	l.append("b");
	ASSERT(l.dumps() == "[\"a\", \"b\"]");

	JSON::List m;
	m.appendPtr(l.steal(0));
	ASSERT(l.dumps() == "[\"b\"]");
	ASSERT(m.dumps() == "[\"a\"]");
ENDTEST()

TEST(json_dict_replace_key)
	JSON::Dict d;
	d.set("key", 1);
	ASSERT(ISINT(d.get("key")) && INT(d.get("key")).value() == 1);
	d.set("key", "val");
	ASSERT(ISSTR(d.get("key")) && STR(d.get("key")).value() == "val");
ENDTEST()

TEST(json_macros)
	JSON::Dict d;
	d.set("key", "val");
	ASSERT(ISSTR(d.get("key")));
	ASSERT(! ISINT(d.get("unknow")));
ENDTEST()

TEST(conversion_operators)
	const char *repr = "{\"str\": \"chaine\", \"int\": 42,"
	                   " \"float\": 3.14, \"bool\": true}";
	JSON::Value *res = JSON::parse(repr);
	ASSERT(ISDICT(res));

	JSON::Dict const & dict = DICT(res);

	ASSERT(ISINT(dict.get("int")));
	ASSERT(INT(dict.get("int")) == 42);

	ASSERT(ISFLOAT(dict.get("float")));
	ASSERT(FLOAT(dict.get("float")) == 3.14);

	ASSERT(ISBOOL(dict.get("bool")));
	ASSERT(BOOL(dict.get("bool")));

	ASSERT(ISSTR(dict.get("str")));
	ASSERT(strcmp(STR(dict.get("str")), "chaine") == 0);

	std::string const & chaine = STR(dict.get("str"));
	ASSERT(chaine == "chaine");

	delete res;
ENDTEST()

TEST(list_assign)
	JSON::Value *parsed = JSON::parse("[1, 2, 3]");
	ASSERT(ISLIST(parsed));
	JSON::List l1 = LIST(parsed);
	JSON::List l2(LIST(parsed));

	ASSERT(LIST(parsed).len() == l1.len());
	ASSERT(LIST(parsed).len() == l2.len());
	for (size_t i=0; i<l1.len(); i++){
		ASSERT(LIST(parsed)[i] != l1[i]); /* Different pointers */
		ASSERT(LIST(parsed)[i] != l2[i]);

		ASSERT(ISINT(l1[i])); /* But same val */
		ASSERT(INT(l1[i]).value() == INT(l2[i]).value());
	}

	delete parsed;
ENDTEST()

TEST(dict_assign)
	JSON::Value *parsed = JSON::load("fixtures/a.json");
	ASSERT(ISDICT(parsed));
	JSON::Dict d1 = DICT(parsed);
	JSON::Dict d2(DICT(parsed));

	for (JSON::Dict::iterator it=d1.begin(); it!=d1.end(); it++){
		ASSERT(d2.hasKey(it->first));
		ASSERT(DICT(parsed).hasKey(it->first));
	}

	delete parsed;
ENDTEST()

TEST(dict_with_bool)
	/* 16/02/2014: JSON::Bool are sent as integer */
	JSON::Dict d;
	d.set("key", JSON::Bool(true));
	ASSERT(ISBOOL(d.get("key")));
	ASSERT(d.dumps() == "{\"key\": true}");
ENDTEST()

TEST(stealmerge)
	JSON::Dict d1, d2;
	d1.set("a", 1);
	d1.set("b", 2);
	d2.set("c", 3);

	ASSERT(d1.hasKey("a"));
	ASSERT(d1.hasKey("b"));
	ASSERT(d2.hasKey("c"));

	d2.stealMerge(d1);
	ASSERT(d2.hasKey("a"));
	ASSERT(d2.hasKey("b"));
	ASSERT(d2.hasKey("c"));

	ASSERT(! d1.hasKey("a"));
	ASSERT(! d1.hasKey("b"));
	ASSERT(d1.len() == 0);
ENDTEST()

TEST(dict_shortcut)
	JSON::Dict d = {
		{"a", JSON::String("a")},
		{"b", JSON::Integer(0xB16B00B5)},
		{"c", JSON::Dict()},
		{"haha", JSON::List()}
	};

	ASSERT(ISSTR(d.get("a")));
	ASSERT(ISINT(d.get("b")));
	ASSERT(ISDICT(d.get("c")));
	ASSERT(ISLIST(d.get("haha")))
	ASSERT(d.len() == 4);
ENDTEST()

TEST(ioerror)
	JSON::Value *res = JSON::load("/dev/null");
	ASSERT(res == NULL);

	ASSERT_THROWS(JSON::IOError, JSON::load("/fichierinexistant"));

	JSON::Dict d = {{"key", JSON::String("val")}};
	ASSERT_THROWS(JSON::IOError, d.save("/dossierinexistant/ici/inexistant"));
ENDTEST()

TEST(document)
	JSON::Document<JSON::Dict> doc;
	doc.with("fixtures/a.json", [](JSON::Dict & d){
		cout << d << endl;
	});
ENDTEST()

int main(int argc, const char **argv)
{
	TestFunc testSuite[] = {
		ADDTEST(lighttest),
		ADDTEST(json_int),
		ADDTEST(json_float),
		ADDTEST(json_bool_true),
		ADDTEST(json_bool_false),
		ADDTEST(json_str),
		ADDTEST(json_str_escape),
		ADDTEST(json_list),
		ADDTEST(json_list_append),
		ADDTEST(json_list_key_error),
		ADDTEST(json_list_mixed),
		ADDTEST(json_list_repr),
		ADDTEST(json_dict),
		ADDTEST(json_dict_keys),
		ADDTEST(json_dict_repr),
		ADDTEST(json_parse_error),
		ADDTEST(json_parse_nothing),
		ADDTEST(json_parse_int),
		ADDTEST(json_parse_float),
		ADDTEST(json_parse_bool_true),
		ADDTEST(json_parse_bool_false),
		ADDTEST(json_parse_str),
		ADDTEST(json_parse_str_escape),
		ADDTEST(json_parse_str_escape_hex),
		ADDTEST(json_parse_list),
		ADDTEST(json_parse_empty_list),
		ADDTEST(json_parse_dict),
		ADDTEST(json_parse_empty_dict),
		ADDTEST(json_parse_empty_list_in_dict),
		ADDTEST(json_parse_empty_dict_in_list),
		ADDTEST(json_parse_dict_many_keys),
		ADDTEST(json_parse_list_in_dict),
		ADDTEST(json_load),
		ADDTEST(json_save),
		ADDTEST(json_dict_steal),
		ADDTEST(json_list_steal),
		ADDTEST(json_dict_replace_key),
		ADDTEST(json_macros),
		ADDTEST(conversion_operators),
		ADDTEST(list_assign),
		ADDTEST(dict_assign),
		ADDTEST(dict_with_bool),
		ADDTEST(stealmerge),
		ADDTEST(dict_shortcut),
		ADDTEST(ioerror),
		ADDTEST(document)
	};

	return RUN(testSuite);
}
