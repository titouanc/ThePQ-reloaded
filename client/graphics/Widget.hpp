#ifndef WIDGET_HPP
#define WIDGET_HPP

namespace GUI{
	class Widget {
	public:
		Widget(): _x(0), _y(0), _w(0), _h(0), _hidden(false){}
		Widget(int x, int y, int w, int h, bool hidden): _x(x), _y(y), _w(w), _h(h), _hidden(hidden){}
		virtual void setPosition(int x, int y){
			_x = x; 
			_y = y;
		}
		virtual void setWidth(int w) 	{ _w = w; }
		virtual void setHeight(int h) 	{ _h = h; }
		virtual int getWidth() 			{ return _w; }
		virtual int getHeight()			{ return _h; }
		virtual void hide() 			{ _hidden = true; }
		virtual void unhide() 			{ _hidden = false; }
		virtual bool isHidden()			{ return _hidden; }
	protected:
		int _x;
		int _y;
		int _w;
		int _h;
		bool _hidden;
	};
}
#endif