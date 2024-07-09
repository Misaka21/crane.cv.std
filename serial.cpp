//
// Created by misaka21 on 24-7-9.
//

#include "serial.h"


void Serial::send(uint16_t data1, uint16_t data2) {
	std::array<uint8_t, 4> data = {
			static_cast<uint8_t>((data1 >> 8) & 0xFF),
			static_cast<uint8_t>(data1 & 0xFF),
			static_cast<uint8_t>((data2 >> 8) & 0xFF),
			static_cast<uint8_t>(data2 & 0xFF)
	};
	boost::asio::write(serial, boost::asio::buffer(data));
}

uint8_t Serial::receive() {
	boost::asio::streambuf buf;
	boost::asio::read_until(serial, buf, uint8_t(0x67)); // 读取直到帧尾0x67
	std::istream is(&buf);
	uint8_t byte;
	bool frame_started = false;
	std::vector<uint8_t> read_buf;

	// 从缓冲区中提取字节
	while (is >> byte) {
		if (byte == 0x66) {
			frame_started = true; // 开始记录数据
			continue;
		}
		if (frame_started) {
			if (byte == 0x67) break; // 停止记录
			read_buf.push_back(byte);
		}
	}

	if (read_buf.size() == 1) {
		// 如果接收到的有效数据恰好为一个字节，返回这个字节
		return read_buf[0];
	} else {
		// 如果数据不足或过多，返回0
		return 0;
	}
}