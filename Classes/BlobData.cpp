#include "BlobData.h"

BlobData::BlobData(size_t size)
	: m_blob(nullptr)
	, m_curPos(nullptr)
	, m_endPos(nullptr)
	, m_length(0)
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

BlobData::BlobData(const void *buf, size_t size)
	: m_blob(nullptr)
	, m_curPos(nullptr)
	, m_endPos(nullptr)
	, m_length(0)
{
	setBlob(buf, size);
}

BlobData::~BlobData(void)
{
	if (m_blob != nullptr)
	{
		free(m_blob);
	}
}

void BlobData::setBlob(const void *buf, size_t size)
{
	if (m_blob != nullptr)
		free(m_blob);

	m_blob = malloc(size);
	memcpy(m_blob, buf, size);
	m_length = size;
	m_curPos = (char*)m_blob;
	m_endPos = m_curPos + m_length;
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
	if ((buf!=nullptr) && (m_blob!=nullptr) && (size!=0) && evaluate(size))
	{
		memcpy(m_curPos, buf, size);
		m_curPos += size;
	}
}

void BlobData::popData(void* buf, size_t size)
{
	if ((buf!=nullptr) && (m_blob!=nullptr) && (size!=0) && evaluate(size))
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
	blob << value.size();
	blob.pushData(value.c_str(), value.size());
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
	size_t size;
	blob >> size;

	char * buf = new char[size+1];
	blob.popData(buf, size);

	buf[size] = '\0';
	value = buf;

	return blob;
}
