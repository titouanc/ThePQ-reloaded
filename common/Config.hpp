#ifndef DEFINE_CONFIG_HEADER
#define DEFINE_CONFIG_HEADER

#include <json/json.hpp>

class Config {
    /*!
     * @class Config
     * @brief Helper class to wrap load/save of configuration files
     */

    private:
        /*! Location of config file on disk */
        std::string _filename;
    public:
        /*! Create a config object with associated filename */
        Config(std::string const & filename) : _filename(filename){}

        /*! Attempt to load from disk */
        bool load(void){
            JSON::Value *dict = NULL;
            bool res = false;
            try {
                dict = JSON::load(_filename);
                if (ISDICT(dict)){
                    fromDict(DICT(dict));
                    res = true;
                }
            } catch (JSON::Error const & err) {}
            if (dict)
                delete dict;
            return res;
        }

        /*! Save config to disk */
        void save(void) const {
            JSON::Dict res = *this;
            res.save(_filename);
        }

        /*! Return config filename */
        std::string const & filename(void) const {
            return _filename;
        }

        /* HOOKS (subclass defined) */

        /*! get config values from a dict */
        virtual void fromDict(JSON::Dict const &) = 0;
        /*! get config values as a dict */
        virtual operator JSON::Dict() const = 0;
};

#endif
