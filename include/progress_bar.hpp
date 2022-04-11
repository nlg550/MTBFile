/*************************************************************************
	Copyright (C) 2022 Instituto Superior Tecnico

	This file is part of the MTB library, which is licensed under the
	terms contained in the LICENSE file.
**************************************************************************/

#ifndef _MTB_PROGRESS_BAR_HPP_
#define _MTB_PROGRESS_BAR_HPP_

#include <iostream>
#include <string>

class ProgressBar
{
	private:
		int _width;
		float _progress;
		std::string _header;

		void print_bar()
		{
			int pos = std::ceil(_progress * _width);
			std::string bar(_width, empty_symbol);
			bar.replace(0, pos, pos, bar_symbol);
			if (pos < _width) bar[pos] = bar_tip;

			stream << _header << "\t" << prefix;
			stream << bar;
			stream << suffix << " " << int(_progress * 100) << "%\r";
			stream.flush();
		}

	public:
		char prefix = '[';
		char suffix = ']';
		char bar_symbol = '=';
		char bar_tip = '>';
		char empty_symbol = ' ';
		std::ostream &stream = std::cerr;

		ProgressBar(int width) : _width(width), _progress(0), _header("") {}
		virtual ~ProgressBar() = default;

		void init(std::string head)
		{
			_header = head;
			this->set(0);
			print_bar();
		}

		void set(float n)
		{
			_progress = n;
			print_bar();
		}


		void update(float n)
		{
			this->set(_progress + n);
			print_bar();
		}

		template<typename T>
		void update(T n, T max)
		{
			this->set(_progress + (float) n / max);
			print_bar();
		}

		void finish(std::string msg = "")
		{
			this->set(1.0);
			print_bar();
			stream << std::endl << msg << std::endl;
		}

};




#endif /* _MTB_PROGRESS_BAR_HPP_ */
