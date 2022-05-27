#pragma once
#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_
#include<QByteArray>
#include<QList>
#include<QFile>

#ifndef QCIRCULARBUFFER_CHUNKSIZE
#define QCIRCULARBUFFER_CHUNKSIZE 1024
#endif

enum
{
    //1G-1byte
    MaxAllocSize = (1 << (std::numeric_limits<int>::digits - 1)) - 1
};
enum
{
    //1G-1-16bytes
    MaxByteArraySize = MaxAllocSize - sizeof(QtPrivate::qRemovePointerLike_t<QByteArray::DataPointer>::parameter_type)
    
};
class CircularBuffer :public QByteArray
{

public:
    //默认分配QCIRCULARBUFFER_CHUNKSIZE大小的buffer
    CircularBuffer(int growth = QCIRCULARBUFFER_CHUNKSIZE) :begin(0), end(0), endBuffer(0), basicBlockSize(growth), buffersize(0) {}
    ~CircularBuffer() { }
    //signals:
    //    void DataChanged(QByteArray newData);
    bool isEmpty() { return buffersize == 0; }//判断是否为空
    //bool isFull() const;
    //获取环形缓冲区指定位置的指针，length：输出这个指定位置到缓冲区结尾的长度
    char* ReadPointeratPosition(qint64 pos, qint64& length);
    //申请空间：从尾开始，返回新空间的指针
    char* Reserve(qint64 bytes);
    //申请空间：从头开始，返回新空间的指针
    char* ReserveFront(qint64 bytes);
    //缩短空间
    void Truncate(qint64 pos) { if (pos < buffersize)chop(buffersize - pos); }
    //从头读取一个字符并转换为int返回
    int getchar()
    {
        if (isEmpty())
            return -1;
        char c = *readPointer();
        free(1);
        return int(uchar(c));
    }
    //在缓冲区尾部添加字符
    void putChar(char c) { char* ptr = Reserve(1); *ptr = c; }
    //在缓冲区头部添加字符
    void ungetChar(char c)
    {
        if (begin > 0)
        {
            --begin;
            buffers.first()[begin] = c;
            ++buffersize;
        }
        else
        {
            char* ptr = ReserveFront(1);
            *ptr = c;
        }
    }
    //读取buffers中的第一个buffer
    QByteArray Read();
    //从指定位置pos拷贝maxlength长度的数据到data中，返回实际截取的长度
    qint64 Peek(char* data, qint64 maxlength, qint64 pos = 0);
    //扩展最后一个buffer
    void append(const char* data, qint64 size);
    //在最后添加一个新buffer
    void append(const QByteArray& qba);
    //从头释放length长度的空间，需要配合reserve使用
    qint64 skip(qint64 length)
    {
        qint64 bytesToskip = qMin(length, buffersize);
        free(bytesToskip);
        return bytesToskip;
    }
    //从尾释放length长度空间，需要配合reserve使用
    void chop(qint64 length);
    //读取一行，包括该行的结束标志'\n'
    qint64 Readline(char* data, qint64 maxlength);
    //能否读一行
    bool canReadline() { return Indexof('\n', buffersize) >= 0; }
    void clear();//清空缓冲区
    //void Print(CircularBuffer* cb);
    //int Increase(CircularBuffer* cb, int p) { return (p + 1) & (2 * cb->capacity - 1); }//start and end pointers incrementation is done module 2*size
   // void Wirte(CircularBuffer* cb, const QByteArray bytes);
    //读取maxlength长度数据到data中，若buffer中数据<maxlength就读取全部，返回长度
    qint64 Read(char* data, qint64 maxlength);

private:
    qint64 buffersize;//buffers数据总大小
    int begin, end;//表示第一个buffer起始位置和最后一个buffer的大小
    int endBuffer;//大小为buffers.size()-1,若为0即只剩一个
    int basicBlockSize;//初始分配空间大小
    QVector<QByteArray> buffers;
    //获取下一个数据块大小，若只剩一个buffer，返回最后一个所含数据大小，否则第一个
    qint64 nextDataBlockSize() { return (endBuffer == 0 ? end : buffers.first().size()) - begin; }
    //获取缓冲区第一个有效数据的指针
    char* readPointer() { return buffersize == 0 ? Q_NULLPTR : (buffers.first().data() + begin); }
    //搜寻内部内容
    qint64 Indexof(char c, qint64 maxlength, qint64 pos = 0);
    //释放空间
    void free(qint64 bytes);
}; 

#endif // !CIRCULARBUFFER_H_

