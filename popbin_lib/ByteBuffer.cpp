#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(byte1* buffer, uint4 size) {
	mPosition = 0;
	mByteContainer = buffer;
	mSize = size;
}

void ByteBuffer::ReadCharArray(char* buf, uint4 size) {
	if (size + mPosition >= mSize) return;

	memcpy(buf, mByteContainer + mPosition, size);
}
