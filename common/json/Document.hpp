#ifndef DEFINE_JSON_DOCUMENT_HEADER
#define DEFINE_JSON_DOCUMENT_HEADER

#include <functional>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>

namespace JSON {
	/*
	 * An object of this class provide a thread-safe method to modify existing
	 * files, and map them to C++ objects transparently. Any object that define
	 * at least the following methods:
	 *   - operator JSON::Type()
	 *   - contructor(JSON::Type)       (Where Type is any of Value subclasses)
	 * can be loaded, accessed, modified and saved easily like this:
	 *
	 * JSON::Document<Class, JSON::Type> objDocument;
	 * objDocument.with("filename", [](Class & obj){
	 *	 //modify obj
	 * });
	 */
	template <typename T, typename JSON_T=JSON::Dict>
	class Document {
		private:
			/* Open and lock filename with given flags */
			int _open(std::string const & filename, int flags=O_RDWR){
	        	int fd = open(filename.c_str(), flags, 0644);
				if (fd < 0){
					std::string error = strerror(errno);
					error += " in ";
					error += filename;
					throw IOError(error.c_str());
				}

				/* Acquire an exclusive lock on this file */
				int lock = flock(fd, LOCK_EX);
				if (lock != 0){
					std::string error("Couldn't lock file ");
					error += filename;
					close(fd);
					throw IOError(error.c_str());
				}

				return fd;
			}

			/* Read JSON from file, and map to C++ obj */
			T _readVal(int fd){
				JSON_T referenceObj;
				JSON::Value *parsed = readFD(fd);
				if (referenceObj.type() != parsed->type())
					throw TypeError("Could not load JSON");

				T res(*((JSON_T*) parsed));
				delete parsed;
				return T(res);
			}

			/* Map C++ obj to JSON obj and write to file */
			void _writeVal(int fd, T const & val){
				JSON_T res(val);
				/* Erase file before writing */
				if (ftruncate(fd, 0) != 0)
					throw JSON::IOError("Unable to erase file");
				/* An rewind to first character of file */
				lseek(fd, 0, SEEK_SET);
				writeFD(fd, res);
			}

			/* Unlock and close file */
			void _close(int fd){
				flock(fd, LOCK_UN);
				close(fd);
			}
		public:
			/* Showtime here ! */
			void with(std::string const & filename, std::function<void(T&)> lambda){
				int fd = -1;
				try {
					T obj;

					/* If file doesn't exist, create it */
					try {
						fd = _open(filename);
						obj = _readVal(fd);
					} catch (IOError & err){
						fd = _open(filename, O_WRONLY|O_CREAT|O_TRUNC);
					}

					lambda(obj);
					_writeVal(fd, obj);
					_close(fd);
				} catch (...){
					if (fd >= 0)
						_close(fd);
					throw;
				}
			}
	};
}

#endif
