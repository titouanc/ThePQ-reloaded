#ifndef DEFINE_BANK_HEADER
#define DEFINE_BANK_HEADER

#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "loadPath.hpp"

template <typename T>
class Bank : public std::unordered_map<std::string, T> {
	protected:
		virtual std::string getPath(std::string const & name){return name;}
		T const & load(std::string const & name){
			(*this)[name] = T();
			(*this)[name].loadFromFile(getPath(name));
			return (*this)[name];
		}
	public:
		typedef typename std::unordered_map<std::string, T>::iterator iterator;
		T const & get(std::string const & name){
			iterator it = this->find(name);
			if (it != this->end())
				return it->second;
			else 
				return load(name);
		}
};

class FontBank : public Bank<sf::Font> {
	protected:
		std::string getPath(std::string const & name){return fontPath(name);}
};

class SoundBank : public Bank<sf::SoundBuffer> {
	protected:
		std::string getPath(std::string const & name){return soundPath(name);}
};

extern FontBank FONTS;
extern SoundBank SOUNDS;

#endif
