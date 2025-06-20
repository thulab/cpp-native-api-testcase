#include "Session.h"
#include <cstdint>

using namespace std;
Session *session;

int main() {
    session = new Session("127.0.0.1", 6667, "root", "root");
    session->open(false);
    session->createDatabase("root.test_db1");

    // 创建对齐时间序列
    vector<std::string> measurements;
    measurements.push_back("s1");
    measurements.push_back("s2");
    measurements.push_back("s3");
    measurements.push_back("s4");
    measurements.push_back("s5");
    measurements.push_back("s6");
    vector<TSDataType::TSDataType> dataTypes;
    dataTypes.push_back(TSDataType::BOOLEAN);
    dataTypes.push_back(TSDataType::INT32);
    dataTypes.push_back(TSDataType::INT64);
    dataTypes.push_back(TSDataType::FLOAT);
    dataTypes.push_back(TSDataType::DOUBLE);
    dataTypes.push_back(TSDataType::TEXT);
    vector<TSEncoding::TSEncoding> encodings;
    encodings.push_back(TSEncoding::PLAIN);
    encodings.push_back(TSEncoding::PLAIN);
    encodings.push_back(TSEncoding::PLAIN);
    encodings.push_back(TSEncoding::PLAIN);
    encodings.push_back(TSEncoding::PLAIN);
    encodings.push_back(TSEncoding::PLAIN);
    vector<CompressionType::CompressionType> compressors;
    compressors.push_back(CompressionType::LZ4);
    compressors.push_back(CompressionType::LZ4);
    compressors.push_back(CompressionType::LZ4);
    compressors.push_back(CompressionType::LZ4);
    compressors.push_back(CompressionType::LZ4);
    compressors.push_back(CompressionType::LZ4);
    // session->createAlignedTimeseries("root.test_db1.d1", measurements, dataTypes, encodings, compressors);

    // 创建非对齐时间序列
    vector<string> paths;
    paths.emplace_back("root.test_db1.d1.s1");
    paths.emplace_back("root.test_db1.d1.s2");
    paths.emplace_back("root.test_db1.d1.s3");
    paths.emplace_back("root.test_db1.d1.s4");
    paths.emplace_back("root.test_db1.d1.s5");
    paths.emplace_back("root.test_db1.d1.s6");
    vector<TSDataType::TSDataType> tsDataTypes;
    tsDataTypes.push_back(TSDataType::BOOLEAN);
    tsDataTypes.push_back(TSDataType::INT32);
    tsDataTypes.push_back(TSDataType::INT64);
    tsDataTypes.push_back(TSDataType::FLOAT);
    tsDataTypes.push_back(TSDataType::DOUBLE);
    tsDataTypes.push_back(TSDataType::TEXT);
    vector<TSEncoding::TSEncoding> tsEncodings;
    tsEncodings.push_back(TSEncoding::PLAIN);
    tsEncodings.push_back(TSEncoding::PLAIN);
    tsEncodings.push_back(TSEncoding::PLAIN);
    tsEncodings.push_back(TSEncoding::PLAIN);
    tsEncodings.push_back(TSEncoding::PLAIN);
    tsEncodings.push_back(TSEncoding::PLAIN);
    vector<CompressionType::CompressionType> compressionTypes;
    compressionTypes.push_back(CompressionType::LZ4);
    compressionTypes.push_back(CompressionType::LZ4);
    compressionTypes.push_back(CompressionType::LZ4);
    compressionTypes.push_back(CompressionType::LZ4);
    compressionTypes.push_back(CompressionType::LZ4);
    compressionTypes.push_back(CompressionType::LZ4);
    session->createMultiTimeseries(paths, dataTypes, encodings, compressors, nullptr, nullptr, nullptr, nullptr);

    // 构建tablet
    pair<string, TSDataType::TSDataType> pairA("s1", TSDataType::BOOLEAN);
    pair<string, TSDataType::TSDataType> pairB("s2", TSDataType::INT32);
    pair<string, TSDataType::TSDataType> pairC("s3", TSDataType::INT64);
    pair<string, TSDataType::TSDataType> pairD("s4", TSDataType::FLOAT);
    pair<string, TSDataType::TSDataType> pairE("s5", TSDataType::DOUBLE);
    pair<string, TSDataType::TSDataType> pairF("s6", TSDataType::TEXT);
    vector<pair<string, TSDataType::TSDataType>> schemas;
    schemas.push_back(pairA);
    schemas.push_back(pairB);
    schemas.push_back(pairC);
    schemas.push_back(pairD);
    schemas.push_back(pairE);
    schemas.push_back(pairF);
    Tablet tablet("root.test_db1.d1", schemas, 30);

    // 写入数据
    int64_t time = 0;
    for (int row = 0; row < 10; row++) {
        int rowIndex = tablet.rowSize++;
        tablet.timestamps[rowIndex] = time++;
        tablet.addValue("s1", rowIndex, true);
        tablet.addValue("s2", rowIndex, row);
        tablet.addValue("s3", rowIndex, int64_t(row));
        tablet.addValue("s4", rowIndex, 111.1F);
        tablet.addValue("s5", rowIndex, 111.1);
        tablet.addValue("s6", rowIndex, "text");
        // 标记空值
        if (row % 2 == 0) {
            tablet.bitMaps[0].mark(row);
            tablet.bitMaps[1].mark(row);
            tablet.bitMaps[2].mark(row);
            tablet.bitMaps[3].mark(row);
            tablet.bitMaps[4].mark(row);
            tablet.bitMaps[5].mark(row);
        }
    }

    // session->insertAlignedTablet(tablet, true);
    session->insertTablet(tablet, true);

    session->close();
}