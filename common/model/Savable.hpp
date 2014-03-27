#ifndef __SAVABLE_HPP
#define __SAVABLE_HPP

#include <string>
#include <json/Document.hpp>

template<typename T>
class Savable
{
protected:
	int _ID;
	bool _loaded;
	const std::string FILE_PATH;
	const std::string ID_PATH;

	virtual bool doSave() = 0;
	virtual bool doLoad() = 0;
public:
	Savable(std::string file_path, std::string id_path, int ID = -1) : 
		_ID(ID), _loaded(false), FILE_PATH(file_path), ID_PATH(id_path)
	{}

	Savable(const Savable & s) : 
		_ID(s._ID), _loaded(s._loaded), FILE_PATH(s.FILE_PATH), ID_PATH(s.ID_PATH)
	{}

	void save(std::function<void(T&)> lambda)
	{
		if (_ID == -1)
		{
			try 
			{
				JSON::Value * json = JSON::load(ID_PATH);
				JSON::Integer const & cpt = INT(json);
				_ID = (int)cpt;
				delete json;
			}
			catch (JSON::IOError & e)
			{
				return;
			}
		}
		JSON::Document<T> toSave;
		try {
			toSave.with(FILE_PATH, lambda);
		}
		catch(JSON::IOError & e)
		{

		}
	}

	void load()
	{
		if (_loaded == false && _ID != -1)
		{
			try
			{
				JSON::Value * json = JSON::load(FILE_PATH);
				JSON::Dict const & dict = DICT(json);
				*this = (T)dict;
				delete json;
			}
			catch (JSON::IOError & e)
			{
				return;
			}
		}
	}
};

#endif // __SAVABLE_HPP