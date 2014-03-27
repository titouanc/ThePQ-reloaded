#ifndef __SAVABLE_HPP
#define __SAVABLE_HPP

class Savable
{
protected:
	int _ID;
	bool _loaded;

	virtual bool doSave() = 0;
	virtual bool doLoad() = 0;
public:
	Savable(int ID = -1) : _ID(ID), _loaded(false)
	{}

	Savable(const Savable & other) : _ID(other._ID), _loaded(other._loaded)
	{}

	void save()
	{
		if (_ID != -1)
		{
			doSave();
		}
	}

	void load()
	{
		if (_ID != -1)
		{
			_loaded = doLoad();
		}
	}
};

#endif // __SAVABLE_HPP