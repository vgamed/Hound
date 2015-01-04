#ifndef __HOUND_BLOB_DATA_H__
#define __HOUND_BLOB_DATA_H__

#include <string.h>
#include <vector>

class Blob
{
public:

};

class BlobData
{
public:
	BlobData(size_t size);
	~BlobData(void);

	size_t getLength(void) const { return m_length; }
	void* getBlob(void) { return m_blob; }
	bool isAtEnd(void) { return (m_curPos>=m_endPos); }
	void seek(size_t offset);
	bool evaluate(size_t size) const;

	friend BlobData& operator<< (BlobData &blob, bool value);
	friend BlobData& operator<< (BlobData &blob, int value);
	friend BlobData& operator<< (BlobData &blob, unsigned int value);
	friend BlobData& operator<< (BlobData &blob, long value);
	friend BlobData& operator<< (BlobData &blob, unsigned long value);
	friend BlobData& operator<< (BlobData &blob, float value);
	friend BlobData& operator<< (BlobData &blob, double value);
	friend BlobData& operator<< (BlobData &blob, const std::string &value);

	friend BlobData& operator>> (BlobData &blob, bool &value);
	friend BlobData& operator>> (BlobData &blob, int &value);
	friend BlobData& operator>> (BlobData &blob, unsigned int &value);
	friend BlobData& operator>> (BlobData &blob, long &value);
	friend BlobData& operator>> (BlobData &blob, unsigned long &value);
	friend BlobData& operator>> (BlobData &blob, float &value);
	friend BlobData& operator>> (BlobData &blob, double &value);
	friend BlobData& operator>> (BlobData &blob, std::string &value);

private:
	void pushData(const void* buf, size_t size);
	void popData(void* buf, size_t size);

	void *m_blob;
	char *m_curPos;
	char *m_endPos;
	size_t m_length;
};

BlobData& operator<< (BlobData &blob, bool value);
BlobData& operator<< (BlobData &blob, int value);
BlobData& operator<< (BlobData &blob, unsigned int value);
BlobData& operator<< (BlobData &blob, long value);
BlobData& operator<< (BlobData &blob, unsigned long value);
BlobData& operator<< (BlobData &blob, float value);
BlobData& operator<< (BlobData &blob, double value);
BlobData& operator<< (BlobData &blob, const std::string &value);

BlobData& operator>> (BlobData &blob, bool &value);
BlobData& operator>> (BlobData &blob, int &value);
BlobData& operator>> (BlobData &blob, unsigned int &value);
BlobData& operator>> (BlobData &blob, long &value);
BlobData& operator>> (BlobData &blob, unsigned long &value);
BlobData& operator>> (BlobData &blob, float &value);
BlobData& operator>> (BlobData &blob, double &value);
BlobData& operator>> (BlobData &blob, std::string &value);

#endif //__HOUND_BLOB_DATA_H__