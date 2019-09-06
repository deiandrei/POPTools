#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include "defines.h"

class ByteBuffer {
	public:
		ByteBuffer(byte1* buffer, uint4 size);
		~ByteBuffer() { }
		
		template<typename T>
		T Read() {
			if (sizeof(T) + mPosition >= mSize) return 0;

			T temp;
			memcpy(&temp, mByteContainer + mPosition, sizeof(T));
			mPosition += sizeof(T);
			return temp;
		}

		void ReadCharArray(char* buf, uint4 size);

		uint4& Position() { return mPosition; }
		uint4 Size() { return mSize; }

	private:
		byte1* mByteContainer;
		uint4 mPosition, mSize;

};

#endif
