#include "BlobData.h"

BlobData::BlobData(size_t size)
{
	m_blob = malloc((size_t)size);
	if (m_blob == nullptr)
	{
		//CCASSERT(...);
	}
	m_length = size;
	m_curPos = (char*)m_blob;
	m_endPos = m_curPos + m_length;
}

BlobData::~BlobData(void)
{
	if (m_blob != nullptr)
	{
		free(m_blob);
	}
}

void BlobData::seek(size_t offset)
{ 
	size_t new_off = ((char*)m_blob+offset) > m_endPos ? m_length : offset;
	m_curPos = (char*)m_blob + new_off; 
}

bool BlobData::evaluate(size_t size) const
{
	return (((char*)m_blob+size) <= m_endPos);
}

void BlobData::pushData(const void* buf, size_t size)
{
	if ((m_blob != nullptr) && evaluate(size))
	{
		memcpy(m_curPos, buf, size);
		m_curPos += size;
	}
}

void BlobData::popData(void* buf, size_t size)
{
	if ((buf!=nullptr) && (m_blob!=nullptr) && evaluate(size))
	{
		memcpy(buf, m_curPos, size);
		m_curPos += size;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Output streaming operators
///////////////////////////////////////////////////////////////////////////////////////
BlobData& operator<< (BlobData &blob, bool value)
{
	blob.pushData(&value, sizeof(value));
	return blob;
}

BlobData& operator<< (BlobData &blob, int value)
{
	blob.pushData(&value, sizeof(value));
	return blob;
}

BlobData& operator<< (BlobData &blob, unsigned int value)
{
	blob.pushData(&value, sizeof(value));
	return blob;
}

BlobData& operator<< (BlobData &blob, long value)
{
	blob.pushData(&value, sizeof(value));
	return blob;
}

BlobData& operator<< (BlobData &blob, unsigned long value)
{
	blob.pushData(&value, sizeof(value));
	return blob;
}

BlobData& operator<< (BlobData &blob, float value)
{
	blob.pushData(&value, sizeof(value));
	return blob;
}

BlobData& operator<< (BlobData &blob, double value)
{
	blob.pushData(&value, sizeof(value));
	return blob;
}

BlobData& operator<< (BlobData &blob, const std::string &value)
{
	return blob;
}

///////////////////////////////////////////////////////////////////////////////////////
// Input streaming operators
///////////////////////////////////////////////////////////////////////////////////////
BlobData& operator>> (BlobData &blob, bool &value)
{
	blob.popData(&value, sizeof(value));
	return blob;
}

BlobData& operator>> (BlobData &blob, int &value)
{
	blob.popData(&value, sizeof(value));
	return blob;
}

BlobData& operator>> (BlobData &blob, unsigned int &value)
{
	blob.popData(&value, sizeof(value));
	return blob;
}

BlobData& operator>> (BlobData &blob, long &value)
{
	blob.popData(&value, sizeof(value));
	return blob;
}

BlobData& operator>> (BlobData &blob, unsigned long &value)
{
	blob.popData(&value, sizeof(value));
	return blob;
}

BlobData& operator>> (BlobData &blob, float &value)
{
	blob.popData(&value, sizeof(value));
	return blob;
}

BlobData& operator>> (BlobData &blob, std::string &value)
{
	return blob;
}
