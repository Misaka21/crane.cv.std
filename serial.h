//
// Created by misaka21 on 24-7-9.
//

#ifndef CRANE_CV_STD_SERIAL_H
#define CRANE_CV_STD_SERIAL_H

#include <boost/asio.hpp>
#include <iostream>
#include <array>
class Serial {
public:
	Serial(const std::string& port, unsigned int baud_rate)
			: io(), serial(io, port) {
		serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	}

	uint8_t receive();
	void send(uint16_t data1, uint16_t data2);
private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
};


#endif //CRANE_CV_STD_SERIAL_H
