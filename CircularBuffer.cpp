#include "CircularBuffer.h"

/* ============================================================
    函数名称 :    ReadPointeratPosition
    功能描述 :    获取环形缓冲区指定位置的指针
    返回值   :    读取的指针
    参数     :    length  输出这个指定位置到缓冲区结尾的长度
    用法     :
   ============================================================*/
char* CircularBuffer::ReadPointeratPosition(qint64 pos, qint64& length)
{
    if (pos >= 0)
    {
        pos += begin;
        for (int i = 0; i < buffers.size(); i++)
        {
            length = (i == endBuffer ? end : buffers[i].size());
            if (length > pos)
            {
                length -= pos;
                return buffers[i].data() + pos;
            }
            pos -= length;
        }
    }
    length = 0;
    return 0;
}

/* ============================================================
    函数名称 :    free
    功能描述 :    释放空间
    返回值   :    Void
    参数     :    bytes  多少字节的内存
    用法     :
   ============================================================*/
void CircularBuffer::free(qint64 bytes)
{
    Q_ASSERT(bytes <= buffersize);
    while (bytes > 0)
    {
        const qint64 blockSize = buffers.first().size() - begin;
        if (endBuffer == 0 || blockSize > bytes)
        {
            if (buffersize <= bytes)
            {
                if (buffers.first().size() <= basicBlockSize)
                {
                    buffersize = 0;
                    begin = end = 0;
                }
                else clear();
            }
            else
            {
                Q_ASSERT(bytes < MaxByteArraySize);
                begin += int(bytes);
                buffersize -= bytes;
            }
            return;
        }
        buffersize -= blockSize;
        bytes -= blockSize;
        buffers.removeFirst();
        --endBuffer;
        begin = 0;
    }

}
/* ============================================================
    函数名称 :    Reserve
    功能描述 :    申请空间，从尾开始
    返回值   :    新空间的指针
    参数     :    bytes  多少字节的内存
    用法     :
   ============================================================*/
char* CircularBuffer::Reserve(qint64 bytes)
{
    if (bytes <= 0 || bytes >= MaxByteArraySize)
        return 0;
    if (buffers.isEmpty())
    {
        buffers.append(QByteArray());
        buffers.first().resize(qMax(basicBlockSize, int(bytes)));
    }
    else
    {
        const qint64 newSize = bytes + end;
        //若超过最后一个buffer所含数据的大小，则最后一个buffer需要从新分配
        if (newSize > buffers.last().size())
        {
            //满足以下条件时，将最后一个buffer的容积缩小到其当前所含数据的大小
            //再开辟一个新buffer，并将该buffer数据的结尾位置end设置为0
            if (newSize > buffers.last().capacity() && (end >= basicBlockSize || newSize >= MaxByteArraySize))
            {
                buffers.last().resize(end);
                buffers.append(QByteArray());
                ++endBuffer;
                end = 0;
            }
            //将最后一个buffer进行扩容
            buffers.last().resize(qMax(basicBlockSize, end + int(bytes)));
        }
    }
    char* writePtr = buffers.last().data() + end;
    buffersize += bytes;
    Q_ASSERT(bytes < MaxByteArraySize);
    end += int(bytes);
    return writePtr;
}

/* ============================================================
    函数名称 :    ReserveFront
    功能描述 :    申请空间，从头开始
    返回值   :    新空间的指针
    参数     :    bytes  多少字节的内存
    用法     :
   ============================================================*/
char* CircularBuffer::ReserveFront(qint64 bytes)
{
    if (bytes <= 0 || bytes >= MaxByteArraySize)
        return 0;

    if (begin < bytes)
    {
        if (buffers.isEmpty())
            buffers.append(QByteArray());
        else
        {
            buffers.first().remove(0, begin);
            if (endBuffer == 0)
                end -= begin;
        }

        begin = qMax(basicBlockSize, int(bytes));
        if (buffersize == 0)
        {
            end = begin;
        }
        else
        {
            buffers.prepend(QByteArray());
            ++endBuffer;
        }
        buffers.first().resize(begin);
    }
    begin -= int(bytes);
    buffersize += bytes;
    return buffers.first().data() + begin;
}



/* ============================================================
    函数名称 :    chop
    功能描述 :   从尾释放空间，需配合reserve使用
    返回值   :    Void
    参数     :    length  释放空间的长度
    用法     :
   ============================================================*/
void CircularBuffer::chop(qint64 length)
{
    Q_ASSERT(length <= buffersize);
    while (length > 0)
    {
        if (endBuffer == 0 || end > length)
        {
            if (buffersize <= length)
            {
                if (buffers.first().size() <= basicBlockSize)
                {
                    buffersize = 0;
                    begin = end = 0;
                }
                else clear();
            }
            else
            {
                Q_ASSERT(length < MaxByteArraySize);
                end -= int(length);
                buffersize -= length;
            }
            return;
        }
        //直接把最后一个砍掉？
        buffersize -= end;
        length -= end;
        buffers.removeLast();
        --endBuffer;
        end = buffers.last().size();
    }
}

/* ============================================================
    函数名称 :    clear
    功能描述 :    清空缓存区
    返回值   :    Void
    参数     :     NULL
    用法     :
   ============================================================*/
void CircularBuffer::clear()
{
    if (buffers.isEmpty()) return;
    //为什么要这样分两步处理？
    buffers.erase(buffers.begin() + 1, buffers.end());
    buffers.first().clear();

    begin = end = 0;
    endBuffer = 0;
    buffersize = 0;
}

/* ============================================================
    函数名称 :    Indexof
    功能描述 :    搜寻内部内容
    返回值   :    qint64
    参数     :     char c  搜寻字符，maxlength  最大长度，pos  位置
    用法     :
   ============================================================*/

qint64 CircularBuffer::Indexof(char c, qint64 maxlength, qint64 pos)
{
    if (maxlength <= 0 || pos < 0)
        return -1;

    qint64 index = -(pos + begin);
    for (int i = 0; i < buffers.size(); i++)
    {
        qint64 nextBlockIndex = qMin(index + (i == endBuffer ? end : buffers[i].size()), maxlength);

        if (nextBlockIndex > 0)
        {
            const char* ptr = buffers[i].data();
            if (index < 0)
            {
                ptr -= index;
                index = 0;
            }
            const char* findPtr = reinterpret_cast<const char*>(memchr(ptr, c, nextBlockIndex - index));
            if (findPtr) return qint64(findPtr - ptr) + index + pos;
            if (nextBlockIndex == maxlength) return -1;
        }
        index = nextBlockIndex;
    }
    return -1;
}
/* ============================================================
    函数名称 :    Read
    功能描述 :    读取maxlength长度数据到data中，若buffer<maxlength就读取全部
    返回值   :    qint64 长度
    参数     :     char* data  数据，maxlength  最大长度
    用法     :
   ============================================================*/

qint64 CircularBuffer::Read(char* data, qint64 maxlength)
{
    const qint64 bytesToRead = qMin(buffersize, maxlength);
    qint64 readSoFar = 0;
    while (readSoFar < bytesToRead)
    {
        const qint64 bytesToReadFromThisBlock = qMin(bytesToRead - readSoFar, nextDataBlockSize());
        if (data)
            memcpy(data + readSoFar, readPointer(), bytesToReadFromThisBlock);
        readSoFar += bytesToReadFromThisBlock;
        free(bytesToReadFromThisBlock);

    }
    return readSoFar;
}
/* ============================================================
    函数名称 :    Read
    功能描述 :    读取buffers中的第一个buffer
    返回值   :    QByteArray 数据
    参数     :     NULL
    用法     :
   ============================================================*/
QByteArray CircularBuffer::Read()
{
    if (buffersize == 0)
        return QByteArray();

    QByteArray qba(buffers.takeFirst());

    //避免调整大小时不必要的内存分配，使QByteArray更高效
    qba.reserve(0);
    if (endBuffer == 0)
    {
        qba.resize(end);
        end = 0;
    }
    else
    {
        --endBuffer;
    }
    qba.remove(0, begin);
    begin = 0;
    buffersize -= qba.size();
    return qba;
}
/* ============================================================
    函数名称 :    Peek
    功能描述 :    从指定位置pos拷贝maxlength长度的数据到data中
    返回值   :    qint64 实际截取的长度
    参数     :     data 复制到的地方 maxlength 长度 pos 位置
    用法     :
   ============================================================*/
qint64 CircularBuffer::Peek(char* data, qint64 maxlength, qint64 pos)
{
    qint64 readSoFar = 0;
    if (pos >= 0)
    {
        pos += begin;
        for (int i = 0; readSoFar < maxlength && i < buffers.size(); i++)
        {
            qint64 blockLength = (i == endBuffer ? end : buffers[i].size());

            if (pos < blockLength)
            {
                blockLength = qMin(blockLength - pos, maxlength - readSoFar);
                memcpy(data + readSoFar, buffers[i].data() + pos, blockLength);
                readSoFar += blockLength;
                pos = 0;
            }
            else pos -= blockLength;
        }
    }
    return readSoFar;
}
/* ============================================================
    函数名称 :    append
    功能描述 :    扩展最后一个buffer
    返回值   :    Void
    参数     :     data 数据 size 大小
    用法     :
   ============================================================*/
void CircularBuffer::append(const char* data, qint64 size)
{
    char* writePointer = Reserve(size);
    if (size == 1)
        *writePointer = *data;
    else if (size)
        ::memcpy(writePointer, data, size);
}
/* ============================================================
    函数名称 :    append
    功能描述 :    在最后添加一个新buffer
    返回值   :    Void
    参数     :     qba 新的buffer
    用法     :
   ============================================================*/
void CircularBuffer::append(const QByteArray& qba)
{
    if (end == 0)
    {
        if (buffers.isEmpty())
            buffers.append(qba);
        else
            buffers.last() = qba;
    }
    else
    {
        buffers.last().resize(end);
        buffers.append(qba);
        ++endBuffer;
    }
    end = qba.size();
    buffersize += end;
}
/* ============================================================
    函数名称 :   Readline
    功能描述 :    读取一行，包括该行的结束标志'\n'
    返回值   :    Void
    参数     :     data 数据 maxlength 最大长度
    用法     :
   ============================================================*/
qint64 CircularBuffer::Readline(char* data, qint64 maxlength)
{
    if (!data || --maxlength < 0)
        return -1;

    qint64 i = Indexof('\n', maxlength);
    i = Read(data, i >= 0 ? (i + 1) : maxlength);

    data[i] = '\0';
    return i;
}
