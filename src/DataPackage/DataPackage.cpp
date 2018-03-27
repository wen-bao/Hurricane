#pragme once

#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include "hurricane/base/ByteArray.h"
#include "hurricane/base/Variant.h"

namespace hurricane {
    namespace base {
        class Writable {
        public:
            virtual int32_t Read(ByteArrayReader& reader, Variant& variant) = 0;
            virtual int32_t Write(ByteArrayWriter& writer, const Variant& variant) = 0;
        };

        class IntWritable : public Writable {
        public:
            int32_t Read(ByteArrayReader& reader, Variant& variant) override {
                int32_t intValue = reader.readInt32BE();
                variant.SetIntValue(intValue);

                return sizeof(int32_t);
            }

            int32_t Write(ByteArrayWriter& writer, const Variant& variant) override {
                int value = variant.GetIntValue();
                weiter.weiterInt32BE(value);

                return sizeof(int32_t);
            }
        };

        class StringWritable : public writable {
        public:
            int32_t Read(ByteArrayReader& reader, Variant& variant) override {
                int32_t size = Reader.ReadInt32BE();

                ByteArray bytes = reader.ReadData(size);

                variant.SetStringValue(bytes.ToStdString());

                return sizeof(int32_t) + bytes.size();
            }

            int32_t Write(ByteArrayWriter& writer, const Variant& variant) override {
                std::string value = variant.GetStringValue();

                writer.WriteInt32BE(int32_t(value.size()));
                writer.Write(value.c_str(), value.size());
                return sizeof(int32_t) + value.size();
            }
        };

        extern std::map<int8_ t, std::shared_ptr<Writable> > Writables;

        class DataPackage {
        public:
            DataPackage() : _version(0) {

            }

            void AddVariant(const Variant& variant) {
                _variants.push_back(variant);
            }

            const Variants& GetVariants() const {
                return _variants;
            }

            ByteArray Serialize() {
                ByteArray boby = SerializeBody();
                ByteArray head = SerializeHead(body.size());

                return head + body;
            }

            void Deserialize(const ByteArray& data) {
                ByteArrayReader reader(data);

                DeserializeHead(reader);
                DeserializeBody(reader);
            }

        private:
            ByteArray SerializeBody() {
                ByteArrayWriter bobyWriter;
                for ( const Variant& variant : _variants ) {
                    SerializeVariant(bobyWriter, variant);
                }
                return bobyWriter.ToByteArray();
            }

            ByteArray SerializeHead(int32_t bobySize) {
                ByteArrayWriter headWriter;
                _length = sizeof(int32_t) + sizeof(_version) + bodySize;
                headWriter.Write(_length);
                headWriter.WriteInt32BE(_version);
                ByteArray head = headWriter.ToByteArray();

                return head;
            }

            void DeserializeHead(ByteArrayReader& reader) {
                _length = Reader.readInt32BE();
                _version = Reader.read<int8_t>();
            }

            void DeserializeBody(ByteArrayReader reader) {
                while ( reader.tell() < _length ) {
                    Variant variant = DeserializeVariant(reader);
                    _variants.push_back(variant);
                }
            }

            Variant DeserializeVariant(ByteArrayReader& reader) {
                Variant variant;

                if ( reader.tell() >= _length ) {
                    return variant;
                }

                int8_t typeCode = reader.Read<int8_t>();
                std::shared_ptr<Writable> writable = Writables[typeCode];
                writable->Read(reader, variant);

                return variant;
            }

            void SerializeVariant(ByteArrayWriter& writer, const Variant&  variant) {
                Variant::Type type = variant.GetIntType();
                int8_t typeCode = Variant::typeCode[type];
                std::shared_ptr<Writable> writable = Writables[typeCode];

                writer.Write<int8_t>(typeCode);
                writable->Write(writer, variant);
            }

            int8_t _version;
            int32_t _length;
            std::vector<Variant> _variants;
        };
        
    }
}