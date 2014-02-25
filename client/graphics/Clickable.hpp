#include <functional>

template <typename T> class Clickable {
public:
	typedef std::function<void(T*)> Callback;
	Clickable(const Callback& callback, T* target) : _callback(callback), _target(target) {}
	void triggerAction(){ _callback(_target); }
private:
	Callback _callback;
	T* _target;
};