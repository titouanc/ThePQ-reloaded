#ifndef DEFINE_BANK_HEADER
#define DEFINE_BANK_HEADER

#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "loadPath.hpp"

typedef std::string(*PathBuilder)(std::string const &);

/*! Lazy resource manager. Load objects from files on demand, and keep them 
 *  for all program life; therefore allowing multiple modules to share them.
 * @param T type of loaded objects (should define T::loadFromFile(std::string const &))
 * @param F a path builder function (map resource name => path on disk)
 */
template <typename T, PathBuilder F>
class Bank : public std::unordered_map<std::string, T> {
	private:
		/*! load specified object from file */
		T const & load(std::string const & name){
			(*this)[name] = T();
			(*this)[name].loadFromFile(F(name));
			return (*this)[name];
		}
	public:
		typedef typename std::unordered_map<std::string, T>::iterator iterator;
		/*! If specified resource is not in memory, load it first; then return it */
		T const & get(std::string const & name){
			iterator it = this->find(name);
			if (it != this->end())
				return it->second;
			else 
				return load(name);
		}
};

/*! GUI fonts */
extern Bank<sf::Font, fontPath> FONTS;

/*! GUI sounds */
extern Bank<sf::SoundBuffer, soundPath> SOUNDS;

/*! GUI textures */
extern Bank<sf::Texture, texturePath> TEXTURES;

#endif
