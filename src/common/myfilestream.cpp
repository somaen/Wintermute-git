/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include <fstream>
#include "common/myfilestream.h"


#define FORBIDDEN_SYMBOL_ALLOW_ALL

namespace Common {

class MyFileReadStream : public SeekableReadStream {
public:
	bool isOpen;
	std::fstream *_stream;
	MyFileReadStream(const char *filename) {
		_stream = new std::fstream(filename, std::fstream::in);
		if (_stream->fail()) {
			isOpen = false;
			delete _stream; // Primarily done to make sure we crash if this isn't handled.
			_stream = NULL;
		} else {
			isOpen = true;
		}
	}
	
	~MyFileReadStream() {
		if (_stream)
			_stream->close();
		delete _stream;
	}
	
	/**
	 * Returns true if a read failed because the stream end has been reached.
	 * This flag is cleared by clearErr().
	 * For a SeekableReadStream, it is also cleared by a successful seek.
	 *
	 * @note The semantics of any implementation of this method are
	 * supposed to match those of ISO C feof(). In particular, in a stream
	 * with N bytes, reading exactly N bytes from the start should *not*
	 * set eos; only reading *beyond* the available data should set it.
	 */
	virtual bool eos() const {
		return _stream->eof();
	}
	
	/**
	 * Read data from the stream. Subclasses must implement this
	 * method; all other read methods are implemented using it.
	 *
	 * @note The semantics of any implementation of this method are
	 * supposed to match those of ISO C fread(), in particular where
	 * it concerns setting error and end of file/stream flags.
	 *
	 * @param dataPtr	pointer to a buffer into which the data is read
	 * @param dataSize	number of bytes to be read
	 * @return the number of bytes which were actually read.
	 */
	virtual uint32 read(void *dataPtr, uint32 dataSize) {
		_stream->read((char*)dataPtr, dataSize);
		return _stream->gcount();
	}
	/**
	 * Obtains the current value of the stream position indicator of the
	 * stream.
	 *
	 * @return the current position indicator, or -1 if an error occurred.
	 */
	virtual int32 pos() const {
		return _stream->tellg();
	}
	
	/**
	 * Obtains the total size of the stream, measured in bytes.
	 * If this value is unknown or can not be computed, -1 is returned.
	 *
	 * @return the size of the stream, or -1 if an error occurred
	 */
	virtual int32 size() const {
		int32 curPos = pos();
		_stream->seekg(0, std::ios::end);
		int32 curSize = pos();
		_stream->seekg(curPos, std::ios::end);
		return curSize;
	}
	
	/**
	 * Sets the stream position indicator for the stream. The new position,
	 * measured in bytes, is obtained by adding offset bytes to the position
	 * specified by whence. If whence is set to SEEK_SET, SEEK_CUR, or
	 * SEEK_END, the offset is relative to the start of the file, the current
	 * position indicator, or end-of-file, respectively. A successful call
	 * to the seek() method clears the end-of-file indicator for the stream.
	 *
	 * @note The semantics of any implementation of this method are
	 * supposed to match those of ISO C fseek().
	 *
	 * @param offset	the relative offset in bytes
	 * @param whence	the seek reference: SEEK_SET, SEEK_CUR, or SEEK_END
	 * @return true on success, false in case of a failure
	 */
	virtual bool seek(int32 offset, int whence = SEEK_SET) {
		if (whence == SEEK_SET) {
			_stream->seekg(offset, std::ios::beg);
		} else if (whence == SEEK_CUR) {
			_stream->seekg(offset, std::ios::cur);
		} else {
			_stream->seekg(offset, std::ios::end);
		}
		
		if (_stream->fail())
			return false;
		if (_stream->bad())
			return false;
		return true;
	}
};

SeekableReadStream *createFileStream(const char *filename) {
	MyFileReadStream *stream = new MyFileReadStream(filename);
	if (stream->isOpen) {
		return stream;
	} else {
		delete stream;
		return NULL;
	}
}

}	// End of namespace Common
