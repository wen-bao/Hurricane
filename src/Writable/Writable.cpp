class Writable {
public:
    virtual int32_t Read(ByteArrayReader& reader, Variant& variant) = 0;
    virtual int32_t Write(ByteArrayWriter& writer, const Variant& variant) = 0;
};